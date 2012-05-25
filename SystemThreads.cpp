#include "SystemThreads.h"
#include "SharedMemory.h"
#include "SystemConfig.h"
#include "ch6dm_linux.h"					// IMU Operations
#include "adcAccess.h"						// ADC Operations
void *adcThreadRun(void *param)
{
	
}


/*******************************************************************************
* Function Name  : serial1ThreadRun
* Input          : None
* Output         : Updates Shared Memory Variables (imuRoll,imuPitch,imuYaw)
* Return         : None
* Description    : Reads serialDevice1 with an interval of serialSleepPeriod 
*				   defined in SystemConfig.h
*******************************************************************************/

void *serial1ThreadRun(void *param)
{
	float roll, pitch, yaw;				//Internal Storage
	struct termios options;				//Define Serial Port Options
	fd = open_serial(serial1Device);		//Open Serial Port

	/*Configure Serial Port for Usage */
	tcgetattr(fd, &options);
	/* Set the new options for the port... */
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
		if ( DEBUG)
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

		printf("yaw: %.2f pitch:%.2f roll:%.2f \n", yaw, pitch, roll);
		//Flush Serial Buffer or we will process all packages by order...
		tcflush(fd, TCIOFLUSH );

		/***************SHARED MEMORY ACCESS ****************/
		/* UPDATE SHARED MEMORY - ONLY DO THIS WITH CAUTION */
		imuRoll = roll;
		imuPitch = pitch;
		imuYaw = yaw;
		/* UPDATE SHARED MEMORY - ONLY DO THIS WITH CAUTION */
		/***************SHARED MEMORY ACCESS ****************/

		usleep(serialSleepPeriod * 1000);		// first arg is in ms so...
		system("clear");						//UNCOMMENT AFTER


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