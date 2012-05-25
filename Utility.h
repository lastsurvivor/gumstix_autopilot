/*
 ============================================================================
 Name        : Utility.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : Utility Class Implementation 
			    The class includes utility functions such as:
				-time handling
				-date string handling
				-obtaining cpuLoad 
 ============================================================================
 */
#ifndef GSTIX_SYS_UTIL_H
#define GSTIX_SYS_UTIL_H

#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

#define PI 3.14159265

float getCpuLoad();
char* getDateString();
void getTimeString(char *timeStr);
uint64_t microsSinceEpoch();
void strReplace(
    char * original, 
    char * pattern, 
    char *replacement,
    char *returned
);
#endif
