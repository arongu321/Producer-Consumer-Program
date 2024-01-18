#ifndef SHARED_H
#define SHARED_H

#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <chrono>
#include <map>

#define FORMAT_OUTPUT "%7.3f ID=%2u %4s %-10s %5s\n"

using namespace std;
using namespace chrono;

extern pthread_cond_t notFull;
extern pthread_cond_t notEmpty;

extern pthread_mutex_t logFileLock;
extern pthread_mutex_t bufferLock;
extern pthread_mutex_t outputStatsLock;

struct outputStats{
    int work;
    int ask;
    int receive;
    int complete;
    int sleep;
};

extern map<int, int> tasksCompleted;

extern int sizeBufferMax;

extern FILE* logFile;

extern queue<string> buffer;

extern bool inputFinished;

extern time_point<system_clock, duration< double > > startTime;

void logInfo(int id, string event, string transaction);

void outputSummary();

#endif