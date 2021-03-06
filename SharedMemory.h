/*
 ============================================================================
 Name        : SharedMemory.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : (Future)SharedMemory Implementation 
				Application will access all memory from this class
 ============================================================================
 */
#ifndef GSTIX_SYS_SHARED_H
#define GSTIX_SYS_SHARED_H

#include "SystemConfig.h"

class SharedMemory{
	private:
	float imuRoll;
	float imuPitch;
    double adcValues_old[MAX_OVERO_CHANNEL];	/* array containing time t-1 adc values */
    double adcValues[MAX_OVERO_CHANNEL];		// array containing time t   adc values */
	
	//PID Control Algorithm Variables
	public:
	float U[4];									// VirtualControl Inputs ( Acquired from TIC2000) */
	short MotorDuty[4];							// MotorDuties  		 ( Acquired from TIC2000) */
	unsigned int PulseDuty[4];							// RF-Receiver Thrust,Roll,Pitch,Yaw Duty Values  */
	float imuYaw;
	float imuYawRate;
	float imuRollRate;
	float imuPitchRate;
	
	//Sonar Data
	float sonarData;
	float sonarRate;
	
	float sonar;
	float medianFiltered;
	float medianDiff;
	float medianInt;
	float desiredAltitude;
	float rawSonar;
	
	//Pilot Data
	short hoverMode;
	short flightAllow;
	short RFflightAllow;
	
	
	//System State Block - Gumstix Rules
	public:
	char isCameraRunning;
	char isCameraRecording;
	char showCameraGUI;
	char GUICloseRequest;
	
	// System variables
	char SERIAL2_DEBUG;
	char showMonitorGUI;
	
	
	public:
	inline float getRoll(){ return imuRoll; }
	inline float getPitch() {return imuPitch; }
	inline float getYaw(){ return imuYaw; }
	inline float getSonar(int sonarId){ return adcValues[sonarId]; }
	inline float getSonar1(){ return sonar; }
//	inline float getSonar1Velocity(){ return sonarVelocity; }
	inline float getMedianFiltered(){ return medianFiltered; }
	inline float getMedianDiff(){ return medianDiff; }
	inline float getMedianInt(){ return medianInt; }
	inline float getRawSonar(){ return rawSonar;}
	inline float getU1(){ return U[0]; }
	inline float getU2(){ return U[1]; }
	inline float getU3(){ return U[2]; }
	inline float getU4(){ return U[3]; }
	inline short getDuty1(){ return MotorDuty[0]; }
	inline short getDuty2(){ return MotorDuty[1]; }
	inline short getDuty3(){ return MotorDuty[2]; }
	inline short getDuty4(){ return MotorDuty[3]; }
	inline float getHoverMode(){ return hoverMode; }
	inline float getFlightAllow(){ return flightAllow; }
	
	
	inline void setRoll(float roll){ imuRoll = roll; }
	inline void setPitch(float pitch) { imuPitch = pitch; }
	inline void setYaw(float yaw){ imuYaw = yaw; }
	inline float setSonar(int sonarId, float value){ adcValues[sonarId] = value; }
	
	
};

#endif
