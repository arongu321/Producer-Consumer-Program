#include "consumer.h"

void* consumer_work(void * args) {
    int thread_id = *(int*) args;
    while (true) {
        logInfo(thread_id, "Ask", "");
        pthread_mutex_lock(&bufferLock);
        while (buffer.empty()) {
            if (inputFinished) {
                pthread_mutex_unlock(&bufferLock);
                pthread_exit(NULL);
            }
            pthread_cond_wait(&notEmpty, &bufferLock);
        }
        string transaction = buffer.front();
        buffer.pop();

        pthread_cond_signal(&notFull);

        cout << "Thread " << thread_id << " doing " << transaction << endl;

        pthread_mutex_unlock(&bufferLock);

        logInfo(thread_id, "Receive", transaction);

        int arg = stoi(transaction.substr(1));
        Trans(arg);
        cout << "Thread " << thread_id << " done " << transaction << endl;
        tasksCompleted[thread_id]++;
        logInfo(thread_id, "Complete", transaction);
    }

}