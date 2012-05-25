#include "SystemThreads.h"

/*
 ============================================================================
 Name        : madc.c
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : ADC Read Implementation 
			    This file covers test program to read twl4030-madc ADCs on a Gumstix Overo.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>

/* 
 Copied twl4030-madc.h from kernel src include/linux/i2c 
 and needed a couple of userland typedefs for kernel types. 
*/
typedef uint8_t u8;
typedef uint16_t u16;
 
#include "twl4030-madc.h"

/* only query the exposed Overo ADCs when no channel is specified */
#define MIN_OVERO_CHANNEL 2
#define MAX_OVERO_CHANNEL 7

/* Our MAXSonar Gain */
#define sonarGain 0.00381875; 

//Sonar based variables
double dt = 0.06, RC = 0.06; //Currently unknown values

// Array containing previous measurements
double adcValues_old[MAX_OVERO_CHANNEL];
// Array containing current measurements
double adcValues[MAX_OVERO_CHANNEL];

/*******************************************************************************
* Function Name  : sonarLowPassFilter
* Input          : 
				   yi_1 - previousOutput
				   xi   - currentMeasurement
				   dt   - samplingTime
				   RC   - timeConstant
				   
					
* Output         : yi - currentOutput
* Description    : 
* Author		 : Taygun Kekec

Performs the operation: y[i] := α * x[i] + (1-α) * y[i-1]
where a = dt / (RC + dt)
*******************************************************************************/
double sonarLowPassFilter(double *yi_1, double *xi, double *dt, double *RC)
{
	double yi;
	double a = *dt / (*RC + *dt);
	yi = *yi_1 + a * (*xi - *yi_1);
	return yi;	
}

int read_channel(int fd, int ch, int avg, int machine_read)
{
	int ret;
	struct twl4030_madc_user_parms param;
	double voltage;	
	double currentRead;

	memset(&param, 0, sizeof(param));

	param.channel = ch;
	param.average = avg;

	ret = ioctl(fd, TWL4030_MADC_IOCX_ADC_RAW_READ, &param);

	if (ret < 0) {
		perror("ioctl");
		return ret;
	}

	if (param.status == -1) {
		if (machine_read)
			printf("%d:-1:-1", ch);
		else
			printf("madc[%d]: status = -1\n", ch);
	}
	else {
		/* 10 bit ADC, reference voltage 2.5v */
		voltage = (param.result * 2.5) / 1024.0;
		currentRead = voltage / sonarGain;
		// Apply Low Pass Filter to current reading
		//adcValues[ch] = sonarLowPassFilter(&adcValues_old[ch], &currentRead, &dt, &RC);
		adcValues[ch] = currentRead;
		
		if (machine_read)
			printf("%d:%u:%0.2lf\n", ch, param.result, voltage);
		else
			printf("madc[%2d] raw = %4u  voltage = %0.2lf distance(cm):%lf\n",
				ch, param.result, voltage, adcValues[ch]);
	}

	return param.status;
}

void usage(const char *argv_0)
{
	printf("Usage: %s [options] [channel list]\n", argv_0);
	printf("Options\n");
	printf("  -a          Request the device driver average 4 readings\n");
	printf("  -m          Machine readable format ch:raw:voltage\n");
	printf("  -h          Show this help message\n\n");
	printf("channel list  A space separated list of channel numbers 0-15.\n");
	printf("              If no channel list is supplied, channels 2-7 are read.\n\n");
	printf("  Example: %s -a 2 4 6\n\n", argv_0);
}



int adcTestMain(int argc, char **argv)
{

	int fd, i, ch, avg, machine_read, opt;

	fd = open("/dev/twl4030-madc", O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	ch = -1;
	avg = 0;
	machine_read = 0;

	while ((opt = getopt(argc, argv, "amh")) != -1) {
		switch (opt) {
		case 'a':
			avg = 1;
			break;

		case 'm':
			machine_read = 1;
			break;

		case 'h':
			usage(argv[0]);
			exit(0);

		default:
			printf("Unknown arg: %s\n", optarg);
			usage(argv[0]);
			exit(1);
		}
	}	

	if (optind == argc) {
		while( 31 == 31 ) {
			for (i = MIN_OVERO_CHANNEL; i <= MAX_OVERO_CHANNEL; i++){
				read_channel(fd, i, avg, machine_read);
			}
			usleep(50000);
			system("clear");
		}
	}
	else {
		for (i = optind; i < argc; i++) {
			ch = atoi(argv[i]);

			if (ch < 0 || ch >= TWL4030_MADC_MAX_CHANNELS)
				printf("Invalid channel arg: %s\n", argv[i]);
			else
				read_channel(fd, ch, avg, machine_read); 
		}
	}

	close(fd);

	return 0;
}

