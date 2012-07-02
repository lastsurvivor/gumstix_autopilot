/*
 ============================================================================
 Name        : tiC2000.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : TIC2000 Communication Implementation 
			    This file covers communication stuff
				-of TIC2000
 ============================================================================
 */

#ifndef TI_C2000_H
#define TI_C2000_H

/*Package types used in communication between Gumstix and TIC2000 */
//Transmitted Package Types
#define TI_SENSOR_DATA 0x01
#define TI_PID_DATA 0x02
#define TI_IBS_DATA 0x03
#define TI_PULSE_DATA 0x04
#define TI_RATE_DATA 0x05

#define TI_SET_PID_ROLL_GAIN 0x10
#define TI_SET_PID_PITCH_GAIN 0x11
#define TI_SET_PID_YAW_GAIN 0x12
#define TI_REMOTE_ESC_INIT 0x20
#define TI_REMOTE_ALLOW_FLIGHT 0x21
#define TI_REMOTE_EMERGENCY_STOP 0x22

float transmissionGain = 1e5;

#endif
