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
	float imuYaw;
    double adcValues_old[MAX_OVERO_CHANNEL];	/* array containing time t-1 adc values */
    double adcValues[MAX_OVERO_CHANNEL];		// array containing time t   adc values */
	
	public:
	float getRoll(){ return imuRoll; }
	float getPitch() {return imuPitch; }
	float getYaw(){ return imuYaw; }
	float getSonar(int sonarId){ return adcValues[sonarId]; }

	inline void setRoll(float roll){ imuRoll = roll; }
	inline void setPitch(float pitch) { imuPitch = pitch; }
	inline void setYaw(float yaw){ imuYaw = yaw; }
	inline float setSonar(int sonarId, float value){ adcValues[sonarId] = value; }
};

#endif
