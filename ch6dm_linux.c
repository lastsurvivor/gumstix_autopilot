#include "SystemThreads.h"

/*
 ============================================================================
 Name        : ch6dm_linux.c
 Author      : Taygun Kekec
 Version     : 1.0
 Copyright   : GNU
 Description : CH6DM IMU Open Source Serial Interface

 ============================================================================
 */

#define SET_ACTIVE_CHANNELS         0x80
#define SET_SILENT_MODE             0x81
#define SET_BROADCAST_MODE          0x82
#define SENSOR_DATA 				0xB7
#define COMMAND_COMPLETE 			0xB0
#define COMMAND_FAILED 				0xB1
#define SET_BROADCAST_MODE			0x82
#define SCALE_FACTOR 0.0109863

#define DEBUG 0

float roll, pitch, yaw;
unsigned int rollTrans, pitchTrans, yawTrans;

int open_serial(char *serialDevice)
{
	int fd; /* File descriptor for the port */
	fd = open(serialDevice, O_RDWR | O_NDELAY);
	if (fd == -1){
	/*
	* Could not open the port.
	*/
	perror("open_port: Unable to open /dev/ttyS0 - ");
	}
	else
	fcntl(fd, F_SETFL, 0);
	return (fd);
}

int close_serial(char *serialDevice)
{
	int fd; /* File descriptor for the port */
	fd = open(serialDevice, O_RDWR | O_NDELAY);
	if (fd == -1){
	/*
	* Could not open the port.
	*/
	perror("open_port: Unable to open /dev/ttyS0 - ");
	}
	else
	fcntl(fd, F_SETFL, 0);
	return (fd);
}

void sendPacket(int fd, char command, char pack_len, char* data)  {
	int i = 0;
	static unsigned char header[5] = { 's', 'n', 'p'};
	header[3] = command;	// Write PT
	header[4] = pack_len;	// Write N
	int checksum = 0;

	for (i = 0; i < 5; i++) {
		//Assume no error now. Later modify this lane as well.
		write(fd,&header[i],1);
		checksum+=header[i];
	}

	/*Write all data to dataBuffer */
	int byteNum, writtenBytes = 0, requiredBytes = pack_len;
	while ( writtenBytes < requiredBytes){
		if ( -1 != ( byteNum = write(fd, &data[writtenBytes], requiredBytes - writtenBytes) )){
			writtenBytes += byteNum;
		}
	}
	//Add checksum
	for (i = 0; i < pack_len; i++) {
		checksum+=data[i];
	}

	//Assume no error now. Later modify this lane as well.
	//Write Checksum on channel
	// HERE I HAVE AN ERROR PROBABLY.
	unsigned char firstChkByte = (char)checksum;
	unsigned char secondChkByte = checksum >> 8;
	write(fd, &firstChkByte,1);
	write(fd, &secondChkByte, 1);

}

int sendCommandWaitACK(int fd, unsigned char command)
{
	int offset = 0;
	unsigned char dataBuffer[50];
	tcflush(fd, TCIOFLUSH );
	sendPacket(fd, command, 0, NULL);
	//Wait for ACK
	usleep(20000);
	read(fd, &dataBuffer, 50);
	//Advance to 'p'
	while( dataBuffer[offset++] != 'p');
	tcflush(fd, TCIOFLUSH );
	//Get Package Type
	if ( dataBuffer[offset] == COMMAND_COMPLETE )
		return 0;
	if ( dataBuffer[offset] == COMMAND_FAILED )
		return -1;
	return -2;
}

int sendDataCommandWaitACK(int fd, unsigned char command, char len, unsigned char *data)
{
	int offset = 0;
	unsigned char dataBuffer[50];
	tcflush(fd, TCIOFLUSH );
	sendPacket(fd, command, len, data);
	//Wait for ACK
	usleep(20000);
	read(fd, &dataBuffer, 50);
	//Advance to 'p'
	while( dataBuffer[offset++] != 'p');
	tcflush(fd, TCIOFLUSH );
	//Get Package Type
	if ( dataBuffer[offset] == COMMAND_COMPLETE ){
		return 0;
	}
	if ( dataBuffer[offset] == COMMAND_FAILED )
		return -1;
	return -1;
}

int imuTestmain(int argc, char *argv[]) {
	struct termios options;

	//Open Serial
	int fd;
	if ( argc < 2 ){
		fd = open_serial("/dev/ttyS0");
	}
	else
		fd = open_serial(argv[1]);
		
	//Configure Serial
	tcgetattr(fd, &options);
	/* Set the new options for the port... */
	/* Set the baud rates to 115200 */
	cfsetispeed(&options, 115200);
	/* Enable the receiver and set local mode... */
	options.c_cflag |= (CLOCAL | CREAD);
	/* Select 8 data bits */
	options.c_cflag |= CS8;
	/* No parity (8N1) configuration */
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	tcsetattr(fd, TCSAFLUSH, &options);

	unsigned char byte = 'o';
	unsigned char len;
	unsigned char dataBuffer[18];
	unsigned char flagByte1,flagByte2;

	//Remote Init of the Sensor

	/*
	if ( -1 == sendCommandWaitACK(fd, SET_SILENT_MODE) )
		perror("SET_SILENT_MODE failed.\n");
	else
		printf("SET_SILENT_MODE succesfully set.\n");

	char flagz[2];
	flagz[0] = flagz[1] = 0xFE;
	if ( -1 == sendDataCommandWaitACK(fd, SET_ACTIVE_CHANNELS, 2, &flagz[0]) )
		perror("SET_ACTIVE_CHANNELS failed.\n");
	else
		printf("SET_ACTIVE_CHANNELS succesfully set.\n");

	if ( -1 == sendCommandWaitACK(fd, SET_BROADCAST_MODE) )
		perror("SET_BROADCAST_MODE failed.\n");
	else
		printf("SET_BROADCAST_MODE succesfully set.\n");
	*/

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

		//Valid Package Start here{
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

		if ( DEBUG) {
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
		usleep(20000);
		system("clear");


	}


	//close_serial("serialDevice");
	return EXIT_SUCCESS;

}

