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

float getCpuLoad();
char* getDateString();
uint64_t microsSinceEpoch();
#endif
