/* 
 ============================================================================
 Name        : adcAccess.h
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : ADC Read Implementation 
			    This file covers definitions, functions and test program to read twl4030-madc ADCs on a Gumstix Overo.
 ============================================================================
 */

#ifndef _TWL4030_MADC_H
#define _TWL4030_MADC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "SharedMemory.h"

/*******************************************************************************
*	Various Definitions for I/O
*******************************************************************************/

/* 
 Copied twl4030-madc.h from kernel src include/linux/i2c 
 and needed a couple of userland typedefs for kernel types. 
*/
typedef uint8_t u8;
typedef uint16_t u16;

struct twl4030_madc_conversion_method {
	u8 sel;
	u8 avg;
	u8 rbase;
	u8 ctrl;
};

#define TWL4030_MADC_MAX_CHANNELS 16

struct twl4030_madc_request {
	u16 channels;
	u16 do_avg;
	u16 method;
	u16 type;
	int active;
	int result_pending;
	int rbuf[TWL4030_MADC_MAX_CHANNELS];
	void (*func_cb)(int len, int channels, int *buf);
};

enum conversion_methods {
	TWL4030_MADC_RT,
	TWL4030_MADC_SW1,
	TWL4030_MADC_SW2,
	TWL4030_MADC_NUM_METHODS
};

enum sample_type {
	TWL4030_MADC_WAIT,
	TWL4030_MADC_IRQ_ONESHOT,
	TWL4030_MADC_IRQ_REARM
};

#define TWL4030_MADC_CTRL1		0x00
#define TWL4030_MADC_CTRL2		0x01

#define TWL4030_MADC_RTSELECT_LSB	0x02
#define TWL4030_MADC_SW1SELECT_LSB	0x06
#define TWL4030_MADC_SW2SELECT_LSB	0x0A

#define TWL4030_MADC_RTAVERAGE_LSB	0x04
#define TWL4030_MADC_SW1AVERAGE_LSB	0x08
#define TWL4030_MADC_SW2AVERAGE_LSB	0x0C

#define TWL4030_MADC_CTRL_SW1		0x12
#define TWL4030_MADC_CTRL_SW2		0x13

#define TWL4030_MADC_RTCH0_LSB		0x17
#define TWL4030_MADC_GPCH0_LSB		0x37

#define TWL4030_MADC_MADCON		(1<<0)	/* MADC power on */
#define TWL4030_MADC_BUSY		(1<<0)	/* MADC busy */
#define TWL4030_MADC_EOC_SW		(1<<1)	/* MADC conversion completion */
#define TWL4030_MADC_SW_START		(1<<5)  /* MADC SWx start conversion */

#define	TWL4030_MADC_ADCIN0		(1<<0)
#define	TWL4030_MADC_ADCIN1		(1<<1)
#define	TWL4030_MADC_ADCIN2		(1<<2)
#define	TWL4030_MADC_ADCIN3		(1<<3)
#define	TWL4030_MADC_ADCIN4		(1<<4)
#define	TWL4030_MADC_ADCIN5		(1<<5)
#define	TWL4030_MADC_ADCIN6		(1<<6)
#define	TWL4030_MADC_ADCIN7		(1<<7)
#define	TWL4030_MADC_ADCIN8		(1<<8)
#define	TWL4030_MADC_ADCIN9		(1<<9)
#define	TWL4030_MADC_ADCIN10		(1<<10)
#define	TWL4030_MADC_ADCIN11		(1<<11)
#define	TWL4030_MADC_ADCIN12		(1<<12)
#define	TWL4030_MADC_ADCIN13		(1<<13)
#define	TWL4030_MADC_ADCIN14		(1<<14)
#define	TWL4030_MADC_ADCIN15		(1<<15)

/* Fixed channels */
#define TWL4030_MADC_BTEMP		TWL4030_MADC_ADCIN1
#define TWL4030_MADC_VBUS		TWL4030_MADC_ADCIN8
#define TWL4030_MADC_VBKB		TWL4030_MADC_ADCIN9
#define	TWL4030_MADC_ICHG		TWL4030_MADC_ADCIN10
#define TWL4030_MADC_VCHG		TWL4030_MADC_ADCIN11
#define	TWL4030_MADC_VBAT		TWL4030_MADC_ADCIN12

/* BCI related - XXX To be moved elsewhere */
#define TWL4030_BCI_BCICTL1		0x23
#define	TWL4030_BCI_MESBAT		(1<<1)
#define	TWL4030_BCI_TYPEN		(1<<4)
#define	TWL4030_BCI_ITHEN		(1<<3)

/* USB related - XXX To be moved elsewhere */
#define TWL4030_USB_CARKIT_ANA_CTRL		0xBB
#define TWL4030_USB_SEL_MADC_MCPC		(1<<3)

#define TWL4030_MADC_IOC_MAGIC '`'
#define TWL4030_MADC_IOCX_ADC_RAW_READ		_IO(TWL4030_MADC_IOC_MAGIC, 0)

struct twl4030_madc_user_parms {
	int channel;
	int average;
	int status;
	u16 result;
};

int twl4030_madc_conversion(struct twl4030_madc_request *conv);

/*******************************************************************************
*	Various Definitions for I/O
*******************************************************************************/

/*******************************************************************************
/*   Function Declarations
*******************************************************************************/
double sonarLowPassFilter(double *yi_1, double *xi, double *dt, double *RC);
int read_channel(int fd, int ch, int avg, int machine_read);
int adcTestMain(int argc, char **argv);



#endif
