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
#define TI_SONAR_DATA 0x06
#define TI_PILOT_DATA 0x07

#define TI_SET_PID_ROLL_GAIN 0x10
#define TI_SET_PID_PITCH_GAIN 0x11
#define TI_SET_PID_YAW_GAIN 0x12
#define TI_SET_SONAR_GAIN 0x13
#define TI_REMOTE_ESC_INIT 0x20
#define TI_REMOTE_ALLOW_FLIGHT 0x21
#define TI_REMOTE_FORBID_FLIGHT 0x22
#define TI_REMOTE_IMU_BIAS_SET 0x23
#define TI_REMOTE_MOTOR12_GAIN_SET 0x24
#define TI_REMOTE_MOTOR34_GAIN_SET 0x25
#define TI_REMOTE_COMPASS_INIT 0x26

#define TI_REMOTE_COMPFILTER_SET_GAMMA 0x40
#define TI_REMOTE_COMPFILTER_SET_BETA  0x41

float transmissionGain = 1e5;

#endif
