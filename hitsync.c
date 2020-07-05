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
#include <string.h>
#include <ctype.h>

int displayTime = 0;
int mode = 0;
int hourset = 0;
int minset = 0;

int main(int argc, char **argv) {
    
    char timeSetBuf[4];

    if (argc > 2) {
        goto USAGE;
    }

    if (argc == 2) {
        if (strlen(argv[1]) != 4) goto USAGE;
        strncpy(timeSetBuf, argv[1], 2);
        if (!checkStringDigit(timeSetBuf)) goto USAGE;
        hourset = atoi(timeSetBuf);

        strncpy(timeSetBuf, argv[1]+2, 2);
        if (!checkStringDigit(timeSetBuf)) goto USAGE;
        minset = atoi(timeSetBuf);
        
        if (hourset < 0 || hourset > 23 || minset < 0 || minset > 59) goto USAGE;
        mode = 1;
    }

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
    printf("\rtime synced.                                                          \n");
    return 0;

USAGE:
    usage();
    return -1;
}

int checkStringDigit(const char *checkstr) {
    while (*checkstr) {
        if (!isdigit(*checkstr++)) return 0;
    }
    return 1;
}

void syncTime() {
    struct timeval currentTime;
    int hour, min, sec;
    setvbuf(stdout, NULL, _IOLBF, 0);
    tm snapTime;
    getSnapTime(&snapTime);
    const struct timeval tv = {mktime(&snapTime), 0};
    settimeofday(&tv, 0);
}

void getSnapTime(tm *snapTime) {
    struct timeval currentTime;
    gettimeofday(&currentTime, 0); // should check for success
    struct tm *localTime = localtime(&currentTime.tv_sec);
    memcpy(snapTime, localTime, sizeof(struct tm));

    if (mode == 0) {
        if (snapTime->tm_sec >= 30) snapTime->tm_min++;
    } else {
        snapTime->tm_hour = hourset;
        snapTime->tm_min = minset;
    }
    snapTime->tm_sec = 0;
}

void *updateTimeDisplay() {
    tm snapTime;
    while (displayTime) {
        usleep(500);
        struct timeval currentTime;
        gettimeofday(&currentTime, 0); // should check for success
        struct tm *localTime = localtime(&currentTime.tv_sec);
        getSnapTime(&snapTime);
        printf("\rCurrent Time: %02d:%02d:%02d, Press enter sync to %02d:%02d:00.", localTime->tm_hour,
            localTime->tm_min, localTime->tm_sec, snapTime.tm_hour, snapTime.tm_min);
        fflush(stdout);
    }
    pthread_exit(NULL);
}

void signalHandle(int sig) {
    displayTime = 0;
    usleep(1000);
    printf("\n");
    fflush(stdout);
    exit(1);
}

void usage() {
    printf("usage: hitsync [HHMM]\n");
}