//
//  hitsync.c
//  
//
//  Created by Razor Ren on 2018/11/3.
//

#include "hitsync.h"
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

int displayTime = 0;

int main(int argc, char **argv) {
    
    pthread_t timeDisplayThread;
    displayTime = 1;
    signal(SIGINT, signalHandle);
    int rc = pthread_create(&timeDisplayThread, NULL, updateTimeDisplay, NULL);
    char ch = getchar();
    while (ch != '\n') {
        ch = getchar();
    }
    syncTime();
    displayTime = 0;
    usleep(1000);
    printf("\r0 second synced.                            \n");
    return 0;
}

void syncTime() {
    struct timeval currentTime;
    int hour, min, sec;
    setvbuf(stdout, NULL, _IOLBF, 0);
    int success = gettimeofday(&currentTime, 0); // should check for success
    struct tm *localTime = localtime(&currentTime.tv_sec);
    if (localTime)
    {
        if (localTime->tm_sec >= 30) localTime->tm_min++;
        localTime->tm_sec = 0;
        const struct timeval tv = {mktime(localTime), 0};
        success = settimeofday(&tv, 0);
    }
}


void *updateTimeDisplay() {
    printf("\e[?25l");
    while (displayTime) {
        usleep(500);
        struct timeval currentTime;
        int success = gettimeofday(&currentTime, 0); // should check for success
        struct tm *localTime = localtime(&currentTime.tv_sec);
        if (localTime)
        {
            
            printf("\r%02d:%02d:%02d, Press enter to sync 0 second.", localTime->tm_hour,
                   localTime->tm_min, localTime->tm_sec);
        }
    }
    printf("\e[?25h");
    pthread_exit(NULL);
}

void signalHandle(int sig) {
    displayTime = 0;
    usleep(1000);
    printf("\e[?25h");
    printf("\n");
    exit(1);
}
