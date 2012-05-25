/*
 ============================================================================
 Name        : Gumstix_autopilot.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : 
			   This file covers entry point of Autopilot application
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>			//Utility Library
#include <pthread.h>		//Thread Library
#include "SystemStatus.h"
#include "SystemConfig.h"
#include "SharedMemory.h"
#include "SystemThreads.h"
#include "Utility.h"

/*System Thread Declarations */
pthread_t adcThread;				// Responsible for copying ADC input to SharedMemory
pthread_t serial1Thread;			// Responsible for copying Serial1 input to SharedMemory
pthread_t serial2Thread;			// Responsible for copying Serial2 input to SharedMemory
pthread_t loggerThread;			
pthread_t sysStatusTXThread;		// Responsible for transmitting SystemStatus to remote
pthread_t sensorTXThread;			// Responsible for transmitting SharedMemory to remote

/* System Function Declarations */
void init_threads();				// Initializes and starts threads
void main_loop();
void checkSensorFailure();			// Periodically checks sensorFailures
void checkSystemFailure();			// Periodicaly checks systemFailures

int main(int argc, char *argv[])
{
	init_threads();
	printf(MSG_SYS_START);
	return 0;
}

void init_threads()
{
	int  iret1, iret2, iret3, iret4, iret5, iret6;
	char *parameter = "Reserved for future usage";
	/* Create independent threads each of which will execute function */
	iret1 = pthread_create( &adcThread, NULL, adcThreadRun, (void*) parameter);
	iret2 = pthread_create( &serial1Thread, NULL, serial1ThreadRun, (void*) parameter);
	/* Serial2 is unused for now */
	//iret3 = pthread_create( &serial2Thread, NULL, serial2ThreadRun, (void*) parameter);	
	iret4 = pthread_create( &loggerThread, NULL, loggerThreadRun, (void*) parameter);
	iret5 = pthread_create( &sysStatusTXThread, NULL, sysStatusTXThreadRun, (void*) parameter);
	iret6 = pthread_create( &sensorTXThread, NULL, sensorTXThreadRun, (void*) parameter);
}



void main_loop()
{
}
