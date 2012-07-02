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
#include <cmath>
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
	printf("Shared Memory Initialized\n");
	init_threads();
	printf("Threads has been initialized\n");
	main_loop();
	printf(MSG_SYS_START);
	return 0;
}

void init_threads()
{
	int  iret1, iret2, iret3, iret4, iret5, iret6;
	/* Create independent threads each of which will execute function */
	iret1 = pthread_create( &adcThread, NULL, adcThreadRun, (void*) &memory);
	//iret2 = pthread_create( &serial1Thread, NULL, serial1ThreadRun, (void*) &memory);
	/* Serial2 is TI Comm for now */
	iret3 = pthread_create( &serial2Thread, NULL, serial2ThreadRun, (void*) &memory);	
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

/*Print topline */
printf(" ______________________________________________________________________________\n");
textcolor(BRIGHT, GREEN, BLACK); printf(" %s",companyName); textcolor(BRIGHT, WHITE, BLACK);
printf(" -------");
textcolor(BRIGHT, GREEN, BLACK); printf(" %s" ,consoleName); textcolor(BRIGHT, WHITE, BLACK);
printf(" --------- ");
textcolor(BRIGHT, GREEN, BLACK); printf( "%s",getDateString() ); textcolor(BRIGHT, WHITE, BLACK);
printf(" ==============================================================================\n");

textcolor(BRIGHT, MAGENTA, BLACK);
printf(" SharedMemory\n"); 
textcolor(BRIGHT, WHITE, BLACK);
//printf(" ****************************************************************************\n");
textcolor(BRIGHT, BLUE, BLACK);
printf(" Roll:    %10.5f %5.2f  ",memory.getRoll(), memory.getRoll() * (180/M_PI)  );
printf(" RollRate: %10.5f \n", memory.imuRollRate );
printf(" Pitch:   %11.5f %5.2f ",memory.getPitch(), memory.getPitch() * ( 180/M_PI )  );
printf(" PitchRate: %10.5f \n", memory.imuPitchRate );
printf(" Yaw:     %5.5f %5.2f \n",memory.getYaw() , memory.getYaw() * (180 / M_PI )  );
printf(" Sonar1:  %9.3f\n",memory.getSonar(SONAR1)); 
printf(" U1:      %7.5f\n",memory.U[0] );
printf(" U2:      %7.5f\n",memory.U[1] );
printf(" U3:      %7.5f\n",memory.U[2]   );
printf(" U4:      %7.5f\n",memory.U[3]); 
printf(" MDuty1:  %15d ",memory.MotorDuty[0]  );
printf(" ChDuty1:  %7d \n",memory.PulseDuty[0]  );
printf(" MDuty2:  %15d ",memory.MotorDuty[1] );
printf(" ChDuty2:  %7d \n",memory.PulseDuty[1]  );
printf(" MDuty3:  %15d ",memory.MotorDuty[2]   );
printf(" ChDuty3:  %7d \n",memory.PulseDuty[2]  );
printf(" MDuty4:  %15d ",memory.MotorDuty[3]); 
printf(" ChDuty4:  notSupportedYet \n" );
textcolor(BRIGHT, WHITE, BLACK);
printf(" ==============================================================================\n");
}
