/*
 ============================================================================
 Name        : SystemStatus.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : SystemStatus Implementation 
			    This file covers current state of the system
				-cpu overload
				-running algorithm
				-etc.
 ============================================================================
 */

#ifndef GSTIX_SYS_STATUS_H
#define GSTIX_SYS_STATUS_H

namespace SystemStatus{
	/* System-Wide Configuration */
	static bool systemBoot;
	static bool systemFailure;	// Set to true when any system check fail
	static bool sensorFailure;		// Set to true when any sensor check fails
	static bool serialResponse;			// Set to true on serial data transmission
	static bool adcResponse;			// Set to true on adc    data transmission
	static char curAlgoName[255];		// Stores currently running algorithm on Gumstix
};
#endif
