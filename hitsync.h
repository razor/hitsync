//
//  hitsync.h
//  
//
//  Created by Razor Ren on 2018/11/3.
//

#ifndef hitsync_h
#define hitsync_h

typedef struct tm tm;

#include <stdio.h>


void *updateTimeDisplay();
void syncTime();
void signalHandle(int);
void usage();
void getSnapTime(tm *);
int checkStringDigit(const char*);

#endif /* hitsync_h */
