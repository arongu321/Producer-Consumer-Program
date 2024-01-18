#ifndef CONSUMER_H
#define CONSUMER_H

#include <pthread.h>
#include "shared.h"
#include "tands.h"

void* consumer_work(void * args);

#endif
