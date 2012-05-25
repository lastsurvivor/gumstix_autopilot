#ifndef GSTIX_SYS_IMU_CH6DM_H
#define GSTIX_SYS_IMU_CH6DM_H

/*
 ============================================================================
 Name        : ch6dm_linux.c
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : CH6DM IMU Open Source Serial Interface

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>				/* String function definitions */
#include <unistd.h>				/* UNIX standard function definitions */
#include <fcntl.h>				/* File control definitions */
#include <errno.h>				/* Error number definitions */
#include <termios.h>			/* POSIX terminal control definitions */

/* IMU'S PROTOCOL'S MESSAGE FLAGS */
#define SET_ACTIVE_CHANNELS         0x80
#define SET_SILENT_MODE             0x81
#define SET_BROADCAST_MODE          0x82
#define SENSOR_DATA 				0xB7
#define COMMAND_COMPLETE 			0xB0
#define COMMAND_FAILED 				0xB1
#define SET_BROADCAST_MODE			0x82
#define IMU_SCALE_FACTOR 0.0109863

/* WHETHER WE WANT MORE DEBUG */
#define SERIAL1_DEBUG 0

/* Function Declarations */
int open_serial(char *serialDevice);
int close_serial(char *serialDevice);
void sendPacket(int fd, char command, char pack_len, char* data);
int sendCommandWaitACK(int fd, unsigned char command);
int sendDataCommandWaitACK(int fd, unsigned char command, char len, unsigned char *data);
int imuTestmain(int argc, char *argv[]);

#endif