#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include "consumer.h"
#include "shared.h"

using namespace std;

int main(int argc, char *argv[]) {
    int nthreads;
    string logFileName = "prodcon.log";
    int id = 0;
    int parentID = 0;

    // Check for valid command line usage
    if (argc < 2) {
        cerr << "Usage: prodcon nthreads <id>" << endl;
        return 1;
    }

    nthreads = stoi(argv[1]);

    // Change output log file name
    if (argc == 3) {
        id = stoi(argv[2]);
        logFileName =  "prodcon." + to_string(id) + ".log";
    }


    // Setting buffer size, output log file in shared data
    sizeBufferMax = 2*nthreads;
    cout << "Max Buffer Size: " << sizeBufferMax << endl;
    logFile = fopen(logFileName.c_str(), "w");

    startTime = high_resolution_clock::now();

    // Creating the consumer threads
    pthread_t consumerThreads[nthreads];
    int * thread_ids = new int[nthreads];
    for (int i = 0; i < nthreads; i++) {
        thread_ids[i] = i+1;
        if (pthread_create(&consumerThreads[i], NULL, consumer_work, (void*)&thread_ids[i])) {
            // handle thread creation error
            cerr << "Error creating thread " << i + 1 << endl;
            return 1;
        }
    }

    string transaction;
    int arg;

    while(getline(cin, transaction)) {
        istringstream iss(transaction);
        char commandType;
        int value;
        if (iss >> commandType >> value) {

            // Transaction command
            if (commandType == 'T') {
                logInfo(parentID, "Work", transaction);
                pthread_mutex_lock(&bufferLock);
                while (buffer.size() == sizeBufferMax) {
                    pthread_cond_wait(&notFull, &bufferLock);
                }
                arg = stoi(transaction.substr(1));
                cout << "Adding " << transaction << " to Buffer" << endl;
                buffer.push(transaction);
                cout << "Current Buffer Transactions:" << endl;
                queue<string> bufferCopy = buffer;

                // Remove transaction command from queue as thread gets the command
                while (!bufferCopy.empty()) {
                    string transaction = bufferCopy.front();
                    cout << transaction << endl;
                    bufferCopy.pop();
                }

                pthread_cond_signal(&notEmpty);

                pthread_mutex_unlock(&bufferLock);


            } else if (commandType == 'S') {
                logInfo(parentID, "Sleep", transaction);
                arg = stoi(transaction.substr(1));
                cout << "Sleeping for " << arg << endl;
                Sleep(arg);
            }
        } else {
            cerr << "Failed to parse line: " << transaction << endl;
        }
    }
    logInfo(parentID, "End", "");
    inputFinished = true;

    for (pthread_t thread : consumerThreads) {
        pthread_join(thread, NULL);
    }

    outputSummary();

    return 0;
}
