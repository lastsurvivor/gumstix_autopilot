/*
 ============================================================================
 Name        : SystemConfig.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU 2.0
 Description : SystemConfig Implementation 
			    This file covers all System Parameters
				Please do not remove comments when you change parameters
				User can change thread periods, logpaths ...
 ============================================================================
 */

#ifndef GSTIX_SYS_CONFIG_H
#define GSTIX_SYS_CONFIG_H

/* System-Wide Configuration */
static const char *consoleName   = "Autopilot Console";	// Default Console Application name. Default is "Autopilot Console"
static const char *logPathRoot   = "./blackbox/";		// Root directory of system logpath. Default is "./blackbox"
static const char *algLogPath    = "algorithm/";		// Algorithms' logging directory. Default is "algorithm/"
static const char *flightLogPath = "flight/";			// Flight logging directory. Default is "flight/"
static const char *serial1Device = "/dev/ttyS0";		// First serial port device address
static const char *serial2Device = "/dev/ttyS2";		// Second serial port device address
static const char *adcDevice     = "/dev/twl4030-madc";	// Gumstix ADC Access Device
/* Thread Configuration 
 *
 * All threads on system has a period of sleep. 
 * Sleeping a lot results in delayed data input, sleeping very less results 
 * in more CPU time allocated to threads.
 */
static const int adcSleepPeriod = 50;			// Gumstix ADC Serial read period in milliseconds. Default is 50
static const int serialSleepPeriod = 20;		// Gumstix Serial port read period in milliseconds. Default is 20
static const int loggerSleepPeriod = 2000;		// Gumstix logger thread sleep in milliseconds. Default is 2000
static const int sysStatusSleepPeriod = 1000;	// Gumstix system state communication thread sleep in ms. Default is 1000
static const int sensorTXSleepPeriod = 100;		// Gumstix system state communication thread sleep in ms. Default is 100

/*System Notification Messages */
#define MSG_SYS_START  "AUTOPILOT SYSTEM STARTED\n"

/*Gumstix Hardware Configuration */
#define MIN_OVERO_CHANNEL 2						/*Gumstix ADC Pins Enumeration Minimum , default : 2*/
#define MAX_OVERO_CHANNEL 7						/*Gumstix ADC Pins Enumeration Maximum , default : 7*/
#define SONAR1			  3						/*Sonar sensor1 Adc Channel Connection , default : 3*/

/*Sonar Sensor Configuration */
#define sonarGain 0.00381875; 					/* Default : 0.00381875 */
static double dt = 0.06; 						/* dt of LowPassFilter for Sonar Sensor */
static double RC = 0.06; 						/* time constant of LowPassFilter of Sonar Sensor */


#endif
