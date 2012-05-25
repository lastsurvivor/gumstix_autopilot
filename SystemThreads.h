/*
 ============================================================================
 Name        : SystemThreads.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : SystemThreads Class Implementation 
			    This file covers Thread Run functions which will run forever.
 ============================================================================
 */

#ifndef GSTIX_SYS_THREADS_H
#define GSTIX_SYS_THREADS_H

#include "twl4030-madc.h"		/* Adc Read functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>				/* String function definitions */
#include <unistd.h>				/* UNIX standard function definitions */
#include <fcntl.h>				/* File control definitions */
#include <errno.h>				/* Error number definitions */
#include <termios.h>			/* POSIX terminal control definitions */

/*
	Each function is bound to a thread which will run until autopilot is terminated.
*/

void *adcThreadRun(void *param);
void *serial1ThreadRun(void *param);
void *serial2ThreadRun(void *param);
void *loggerThreadRun(void *param);			
void *sysStatusTXThreadRun(void *param);
void *sensorTXThreadRun(void *param);

#endif
