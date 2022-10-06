#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>
int nanosleep(const struct timespec *req, struct timespec *rem);
#include <errno.h>  
#include "ch8.h"
#include "sound.h"
#include <sys/time.h>
#include <pthread.h>

int update_timers();
int msleep();
void *timer_update_callback();
int timediff_ms(struct timeval *end, struct timeval *start);

#endif