#include "SystemThreads.h"
#include "SharedMemory.h"
#include "SystemConfig.h"
#include "ch6dm_linux.h"					// IMU Operations
#include "adcAccess.h"						// ADC Operations


/*******************************************************************************
* Function Name  : adcThreadRun
* Input          : SharedMemory Pointer
* Output         : Updates Shared Memory Variables (adcValues ...)
* Return         : None
* Description    : Reads adcDevice1 with an interval of adcSleepPeriod 
*				   defined in SystemConfig.h
*******************************************************************************/
void *adcThreadRun(void *param)
{
	int argc = 1;
	char **argv = NULL;
	
	SharedMemory *mem;
	mem = (SharedMemory*) (param);		// Get Shared Memory Instance of the System
	
	int fd, i, ch, avg, machine_read, opt;

	fd = open(adcDevice, O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) {
		perror("open");
		exit(1);
	}
	
	ch = -1;
	avg = 0;
	machine_read = 0;
	
	if (optind == argc) {
		while( 1 == 1 ) {
			for (i = MIN_OVERO_CHANNEL; i <= MAX_OVERO_CHANNEL; i++){
				read_channel(fd, i, avg, machine_read, mem);
			}
			usleep(adcSleepPeriod);
		}
	}

	close(fd);
	
					
}


/*******************************************************************************
* Function Name  : serial1ThreadRun
* Input          : SharedMemory Pointer
* Output         : Updates Shared Memory Variables (imuRoll,imuPitch,imuYaw)
* Return         : None
* Description    : Reads serialDevice1 with an interval of serialSleepPeriod 
*				   defined in SystemConfig.h
*******************************************************************************/

void *serial1ThreadRun(void *param)
{
	SharedMemory *mem;
	mem = (SharedMemory*) (param);		// Get Shared Memory Instance of the System

	int fd;
	float roll, pitch, yaw;				//Internal Storage
	struct termios options;				//Define Serial Port Options
	fd = open_serial(serial1Device);	//Open Serial Port

	
	tcgetattr(fd, &options);			/*Configure Serial Port for Usage */
	cfsetispeed(&options, 115200);		/* Set the baud rates to 115200 */
	options.c_cflag |= (CLOCAL | CREAD);/* Enable the receiver and set local mode... */
	options.c_cflag |= CS8;				/* Select 8 data bits */
	
	options.c_cflag &= ~PARENB;			/* No parity (8N1) configuration */
	options.c_cflag &= ~CSTOPB;			/* No parity (8N1) configuration */
	options.c_cflag &= ~CSIZE;			/* No parity (8N1) configuration */
	options.c_cflag |= CS8;				/* No parity (8N1) configuration */
	tcsetattr(fd, TCSAFLUSH, &options); /* Apply changes immediately */

	unsigned char byte = 'o';
	unsigned char len;
	unsigned char dataBuffer[18];
	unsigned char flagByte1,flagByte2;

	while(1){
		search_header:
		while( byte != 's'){
			read(fd, &byte, 1);
		}
		read(fd, &byte, 1);
		if ( byte != 'n')
			goto search_header;
		read(fd, &byte, 1);
		if ( byte != 'p')
			goto search_header;

		//Valid Package Start here
		if ( SERIAL1_DEBUG)
			printf("Valid Package Start Detected \n");

		read(fd, &byte, 1);
		if ( byte != SENSOR_DATA){
			continue;
			/*Interpret ChannelData Here.
			 * v1.0 For now we Interpret Sensor Data Package
			 * v2.0 We will interpret more package types
			 */
		}

		read(fd, &len, 1);
		read(fd, &flagByte1, 1);
		read(fd, &flagByte2, 1);
		/*Interpret ChannelData Here.
		 * v1.0 For now we get raw, pitch, roll with default settings
		 * v2.0 determine active channels and interpret all data
		 */

		if (SERIAL1_DEBUG) {
			printf("Type: %d\n", (unsigned int)byte);
			printf("Len: %d\n", (unsigned int)len);
			printf("Flag1: %d\n", (unsigned int)flagByte1);
			printf("Flag2: %d\n", (unsigned int)flagByte2);
		}

		/*Read all data to dataBuffer */
		int byteNum, readBytes = 0, requiredBytes = 6;
		while ( readBytes < requiredBytes){
			if ( -1 != ( byteNum = read(fd, &dataBuffer, requiredBytes - readBytes) )){
				readBytes += byteNum;
			}

		}

        int act_channels = (flagByte1 << 8) | (flagByte2);
        int i = 0;
        int m_recentData[3];

        // Yaw angle
        if ((act_channels & 0x8000) != 0)
        {
            m_recentData[0] = (short)((dataBuffer[i] << 8) | dataBuffer[i + 1]);
            yaw = (float)m_recentData[0] * (.0109863);
            i += 2;
        }
        // Pitch angle
        if ((act_channels & 0x4000) != 0)
        {
        	m_recentData[1] = (short)((dataBuffer[i] << 8) | dataBuffer[i + 1]);
            pitch = (float)m_recentData[1] * (.0109863);
            i += 2;
        }

        // Roll angle
        if ((act_channels & 0x2000) != 0)
        {
            m_recentData[2] = (short)((dataBuffer[i] << 8) | dataBuffer[i + 1]);
            roll = (float)m_recentData[2] * (.0109863);
            i += 2;
        }

		//Flush Serial Buffer or we will process all packages by order...
		tcflush(fd, TCIOFLUSH );

		/***************SHARED MEMORY ACCESS ****************/
		/* UPDATE SHARED MEMORY - ONLY DO THIS WITH CAUTION */
		//printf("yaw: %.2f pitch:%.2f roll:%.2f \n", yaw, pitch, roll);
		mem->setRoll(roll);
		mem->setPitch(pitch);
		mem->setYaw(yaw);
		/* UPDATE SHARED MEMORY - ONLY DO THIS WITH CAUTION */
		/***************SHARED MEMORY ACCESS ****************/

		usleep(serialSleepPeriod * 1000);		// first arg is in ms so...
		
		//system("clear");						//UNCOMMENT AFTER


	}

}
void *serial2ThreadRun(void *param)
{
}
void *loggerThreadRun(void *param)	
{
}
void *sysStatusTXThreadRun(void *param)
{
}
void *sensorTXThreadRun(void *param)
{
}
