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
void init_sharedMemory();			// Initializes shared memory and its content
void init_threads();				// Initializes and starts threads
void main_loop();
void checkSensorFailure();			// Periodically checks sensorFailures
void checkSystemFailure();			// Periodicaly checks systemFailures
void systemMonitor();				// Periodically updates console gui

SharedMemory memory;

int main(int argc, char *argv[])
{
	init_sharedMemory();
	init_threads();
	main_loop();
	printf(MSG_SYS_START);
	return 0;
}

void init_threads()
{
	int  iret1, iret2, iret3, iret4, iret5, iret6;
	/* Create independent threads each of which will execute function */
	iret1 = pthread_create( &adcThread, NULL, adcThreadRun, (void*) &memory);
	iret2 = pthread_create( &serial1Thread, NULL, serial1ThreadRun, (void*) &memory);
	/* Serial2 is unused for now */
	//iret3 = pthread_create( &serial2Thread, NULL, serial2ThreadRun, (void*) &memory);	
	iret4 = pthread_create( &loggerThread, NULL, loggerThreadRun, (void*) &memory);
	iret5 = pthread_create( &sysStatusTXThread, NULL, sysStatusTXThreadRun, (void*) &memory);
	iret6 = pthread_create( &sensorTXThread, NULL, sensorTXThreadRun, (void*) &memory);
}

void init_sharedMemory()
{
	//Intialize imuData
	memory.setRoll(0);
	memory.setPitch(0);
	memory.setYaw(0);
}

void main_loop()
{
	//Main System Loop
	for(;;){
		usleep(50000);
		systemMonitor();
	}
}

void systemMonitor()
{
int c = system("clear");
printf(" ============================================================================\n");
printf(" %s \n" ,consoleName);
printf(" \n");
printf(" SharedMemory \n");
printf(" ----------------------------------------------------------------------------\n");
printf(" Roll:    %.3f\n",memory.getRoll()  );
printf(" Pitch:   %.3f\n",memory.getPitch() );
printf(" Yaw:     %.3f\n",memory.getYaw()   );
printf(" Sonar1:  %.3f\n",memory.getSonar(SONAR1)); 
printf(" ----------------------------------------------------------------------------\n");
printf("System Time : %s", "NOT IMPLEMENTED\n");
printf(" ============================================================================\n");
}
