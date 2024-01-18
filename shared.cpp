#include "shared.h"

pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

pthread_mutex_t logFileLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bufferLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t outputStatsLock = PTHREAD_MUTEX_INITIALIZER;

int sizeBufferMax;
FILE* logFile;
queue<string> buffer;

outputStats stats = {0, 0, 0, 0, 0};

time_point< system_clock, duration<double>> startTime;

map<int, int> tasksCompleted;

bool inputFinished = false;

void logInfo(int id, string event, string transaction) {
    double duration = (double)duration_cast<microseconds>(high_resolution_clock::now() - startTime).count() / (double)1000000;

    char q_out[] = "    ";
    if (event == "Receive" || event == "Work") {
        pthread_mutex_lock(&bufferLock); 
        int sizeBuffer = buffer.size();
        pthread_mutex_unlock(&bufferLock);
        sprintf(q_out, "Q=%2u", sizeBuffer);
    }

    char event_out[] = "          ";
    sprintf(event_out, "%s", event.c_str());
    char n_out[] = "    ";
    if (transaction != "") {
        int arg = stoi(transaction.substr(1));
        sprintf(n_out, "%4u", arg);
    }
    pthread_mutex_lock(&logFileLock);
    fprintf(logFile, FORMAT_OUTPUT, duration, id, q_out, event_out, n_out);

    pthread_mutex_unlock(&logFileLock);

    pthread_mutex_lock(&outputStatsLock);
    if (event == "Ask") {
        stats.ask++;
    } else if (event == "Work") {
        stats.work++;
    } else if (event == "Receive") {
        stats.receive++;
    } else if (event == "Complete") {
        stats.complete++;
    } else if (event == "Sleep") {
        stats.sleep++;
    }
    pthread_mutex_unlock(&outputStatsLock);
}

void outputSummary() {
    // Output task manager stats
    pthread_mutex_lock(&logFileLock);
    fprintf(logFile, "Summary:\n");
    fprintf(logFile, "    %-12s %5d\n", "Work", stats.work);
    fprintf(logFile, "    %-12s %5d\n", "Ask", stats.ask);
    fprintf(logFile, "    %-12s %5d\n", "Receive", stats.receive);
    fprintf(logFile, "    %-12s %5d\n", "Complete", stats.complete);
    fprintf(logFile, "    %-12s %5d\n", "Sleep", stats.sleep);
    
    // Output thread stats
    for (auto thread: tasksCompleted) {
        fprintf(logFile, "    Thread %-3d %7d\n", thread.first, thread.second);
    }

    // Output transaction time
    double duration = (double)duration_cast<microseconds>(high_resolution_clock::now() - startTime).count() / (double)1000000;
    double durationPerSeconds = (double) stats.complete / duration;

    /* Place the variable declaration before using it in fprintf */
    fprintf(logFile, "Transactions per second: %-5.2f", durationPerSeconds);
    pthread_mutex_unlock(&logFileLock);
}
