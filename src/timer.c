#include "timer.h"

#define TIMER_DELAY (1000 / 60)

int timediff_ms(struct timeval *end, struct timeval *start) {
    int diff = (end->tv_sec - start->tv_sec) * 1000 + 
		(end->tv_usec - start->tv_usec) / 1000;
    return diff;
}
  
int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void *timer_update_callback() {
    while(1) {
        msleep(TIMER_DELAY);
        update_timers();
    }
    return 0;
}

int update_timers() {
    // Decrease timers if necessary
    if (m.delay_timer > 0) {
        m.delay_timer--;
    }

    if (m.sound_timer > 0) {
        m.should_beep = 1;
        m.sound_timer--;
    } else {
        m.should_beep = 0;
    }

    return 0;
}
