/*
 ============================================================================
 Name        : SystemConfig.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : SystemConfig Class Implementation 
			    This file covers all System Parameters
				Please do not remove comments when you change parameters
				User can change thread periods, logpaths ...
 ============================================================================
 */

#ifndef GSTIX_SYS_CONFIG_H
#define GSTIX_SYS_CONFIG_H

virtual class SystemConfig{
public:
	/* System-Wide Configuration */
	static const char *logPathRoot   = "./blackbox/";	// Root directory of system logpath. Default is "./blackbox"
	static const char *algLogPath    = "algorithm/";	// Algorithms' logging directory. Default is "algorithm/"
	static const char *flightLogPath = "flight/";		// Flight logging directory. Default is "flight/"
	static const char *serial1device = "/dev/ttyS0";	// First serial port device address
	static const char *serial2device = "/dev/ttyS2";	// Second serial port device address
	/* Thread Configuration 
	 *
	 * All threads on system has a period of sleep. 
	 * Sleeping a lot results in delayed data input, sleeping very less results 
	 * in more CPU time allocated to threads.
	 */
	static const int adcSleepPeriod = 50;			// Gumstix ADC Serial read period in milliseconds. Default is 50
	static const int serialSleepPeriod = 20;		// Gumstix Serial port read period in milliseconds. Default is 20

};
#endif