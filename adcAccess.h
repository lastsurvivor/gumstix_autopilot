/*
 ============================================================================
 Name        : adcAccess.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : ADC Read Implementation 
			    This file covers functions and test program to read twl4030-madc ADCs on a Gumstix Overo.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>

/* 
 Copied twl4030-madc.h from kernel src include/linux/i2c 
 and needed a couple of userland typedefs for kernel types. 
*/
typedef uint8_t u8;
typedef uint16_t u16;
 
#include "twl4030-madc.h"

/* only query the exposed Overo ADCs when no channel is specified */
#define MIN_OVERO_CHANNEL 2
#define MAX_OVERO_CHANNEL 7

/* Our MAXSonar Gain */
#define sonarGain 0.00381875; 

//Sonar based variables
double dt = 0.06, RC = 0.06; //Currently unknown values

// Array containing previous measurements
double adcValues_old[MAX_OVERO_CHANNEL];
// Array containing current measurements
double adcValues[MAX_OVERO_CHANNEL];

double sonarLowPassFilter(double *yi_1, double *xi, double *dt, double *RC);
int read_channel(int fd, int ch, int avg, int machine_read);
int adcTestMain(int argc, char **argv);
