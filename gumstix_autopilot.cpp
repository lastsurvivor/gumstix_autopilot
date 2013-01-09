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
pthread_t cameraThread;			// Responsible for transmitting SharedMemory to remote

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
	
	if ( argc > 1 ){
	    if ( strcmp( argv[1], "-d" ) == 0 ){
			memory.SERIAL2_DEBUG = 1;
			memory.showMonitorGUI = 0;
		}
}else{
			memory.SERIAL2_DEBUG = 0;
			memory.showMonitorGUI = 1;	}
	
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
	int  iret1, iret2, iret3, iret4, iret5, iret6, iret7;
	/* Create independent threads each of which will execute function */
	iret1 = pthread_create( &adcThread, NULL, adcThreadRun, (void*) &memory);
	//iret2 = pthread_create( &serial1Thread, NULL, serial1ThreadRun, (void*) &memory);
	/* Serial2 is TI Comm for now */
	iret3 = pthread_create( &serial2Thread, NULL, serial2ThreadRun, (void*) &memory);	
	iret4 = pthread_create( &loggerThread, NULL, loggerThreadRun, (void*) &memory);
	iret5 = pthread_create( &sysStatusTXThread, NULL, sysStatusTXThreadRun, (void*) &memory);
	iret6 = pthread_create( &sensorTXThread, NULL, sensorTXThreadRun, (void*) &memory);
	iret7 = pthread_create( &cameraThread, NULL, cameraThreadRun, (void*) &memory);
}

void init_sharedMemory()
{
	//Intialize imuData
	memory.setRoll(0);
	memory.setPitch(0);
	memory.setYaw(0);
	memory.U[1] = 0;
	memory.U[2] = 0;
	memory.U[3] = 0;
	memory.U[4] = 0;
	memory.isCameraRunning = 0;
	memory.isCameraRecording = 1;
	memory.showCameraGUI = 1;
	memory.GUICloseRequest = 0;
}

void main_loop()
{
	//Main System Loop
	for(;;){
		usleep(25000);
		if ( memory.showMonitorGUI )
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
printf(" SharedMemory\t\t\t\t\t\t"); printf(" System State\n"); 
textcolor(BRIGHT, WHITE, BLACK);
//printf(" ****************************************************************************\n");
textcolor(BRIGHT, BLUE, BLACK);
printf(" Roll:   %10.3f %3.2f \t",memory.getRoll(), memory.getRoll() * (180/M_PI)  ); printf(" RollRate:  %10.3f \t", memory.imuRollRate );

memory.hoverMode == 1 ? textcolor(BRIGHT, BLUE, BLACK) : textcolor(BRIGHT, YELLOW, BLACK);     //Set color
printf(" HoverMode:      %s \n",memory.hoverMode == 1 ? "ON" : "OFF"); 
textcolor(BRIGHT, BLUE, BLACK);

printf(" Pitch:  %10.3f %3.2f \t",memory.getPitch(), memory.getPitch() * ( 180/M_PI )  ); printf(" PitchRate: %10.3f \t", memory.imuPitchRate );

memory.flightAllow == 1 ? textcolor(BRIGHT, GREEN, BLACK) : textcolor(BRIGHT, RED, BLACK);     //Set color
printf(" FlightAllow:    %s \n",memory.flightAllow == 1 ? "ON" : "OFF"); 
textcolor(BRIGHT, BLUE, BLACK);

printf(" Yaw:    %10.3f %3.2f \t",memory.getYaw() , memory.getYaw() * (180 / M_PI )  ); printf(" YawRate:   %10.3f \t",memory.imuYawRate * (180 / M_PI )  );

memory.RFflightAllow == 1 ? textcolor(BRIGHT, GREEN, BLACK) : textcolor(BRIGHT, RED, BLACK);     //Set color
printf(" RFFlightAllow:  %s \n",memory.RFflightAllow == 1 ? "ON" : "OFF"); 
textcolor(BRIGHT, BLUE, BLACK);

printf(" Sonar:  %10.3f \t\t",memory.getSonar1()); printf(" RawSonar:  %10.3f \t",memory.getRawSonar());

memory.isCameraRunning == 1 ? textcolor(BRIGHT, GREEN, BLACK) : textcolor(BRIGHT, RED, BLACK); //Set color
printf(" Camera:  	 %s \n",memory.isCameraRunning == 1 ? "ON" : "OFF"); 	
textcolor(BRIGHT, BLUE, BLACK);

printf(" MedianFiltered:  %9.3f",memory.getMedianFiltered()); 


memory.isCameraRecording == 1 ? textcolor(BRIGHT, GREEN, BLACK) : textcolor(BRIGHT, RED, BLACK); //Set color
printf(" \t\t\t\t Recording:  	 %s \n",memory.isCameraRecording == 1 ? "ON" : "OFF"); 	
textcolor(BRIGHT, BLUE, BLACK);
printf(" MedianDiff:  %9.3f\t\t",memory.getMedianDiff());
printf(" MedianInt:  %9.3f\t\t\n",memory.getMedianInt()); 

printf(" Des_Altitude :  %9.3f\n",memory.desiredAltitude );
printf(" U1:      %10.3f\t",memory.U[0] ); printf(" MDuty1:  %5d ",memory.MotorDuty[0]  ); printf(" ChDuty1:  %6d \n",memory.PulseDuty[0]  );
printf(" U2:      %10.3f\t",memory.U[1] ); printf(" MDuty2:  %5d ",memory.MotorDuty[1] ); printf(" ChDuty2:  %6d \n",memory.PulseDuty[1]  );
printf(" U3:      %10.3f\t",memory.U[2]   );printf(" MDuty3:  %5d ",memory.MotorDuty[2]   );printf(" ChDuty3:  %6d \n",memory.PulseDuty[2]  );
printf(" U4:      %10.3f\t",memory.U[3]); printf(" MDuty4:  %5d ",memory.MotorDuty[3]);  printf(" ChDuty4:       - \n" );

textcolor(BRIGHT, WHITE, BLACK);
printf(" ==============================================================================\n");
}
