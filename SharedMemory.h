/*
 ============================================================================
 Name        : SharedMemory.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : (Future)SharedMemory Implementation 
				Application will access all memory from this class
 ============================================================================
 */
#ifndef GSTIX_SYS_SHARED_H
#define GSTIX_SYS_SHARED_H

#include "SystemConfig.h"

static float imuRoll;
static float imuPitch;
static float imuYaw;

/* Sonar Sensor Based Variables */
#define sonarGain 0.00381875; 			
static double dt = 0.06; 						/* dt of LowPassFilter for Sonar Sensor */
static double RC = 0.06; 						/* time constant of LowPassFilter of Sonar Sensor */
static double adcValues_old[MAX_OVERO_CHANNEL];	/* array containing time t-1 adc values */
static double adcValues[MAX_OVERO_CHANNEL];		// array containing time t   adc values */
#endif
