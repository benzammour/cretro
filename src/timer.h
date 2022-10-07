#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include "cpu.h"
#include "sound.h"
#include <sys/time.h>
#include <pthread.h>

int nanosleep(const struct timespec *req, struct timespec *rem);
int msleep(long msec);
void *timer_update_callback(void*);
long timediff_ms(struct timeval *end, struct timeval *start);

#endif