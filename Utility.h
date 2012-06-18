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
#include <dirent.h>
#include <sys/stat.h>		// Linux Directory Functions
#include <stdio.h>			// sprintf functions

#define PI 3.14159265

/* Console text styles for usage of textcolor() function */
#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

/* Console color enumerations for usage of textcolor() function */

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

float getCpuLoad();								//Returns cpuload
char* getDateString();							//Returns date string
void getTimeString(char *timeStr);				//Returns time string
uint64_t microsSinceEpoch();					//Returns number of microseconds passed from Unixtime

void textcolor(int attr, int fg, int bg);		//Change Text Color of Console 


float char_to_float(const char* cs);
short char_to_short(const char* cs);
int char_to_int(const char* cs);
#endif
