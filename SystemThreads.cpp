#include "SystemThreads.h"
#include "SharedMemory.h"
#include "SystemConfig.h"
#include "ch6dm_linux.h"					// IMU Operations
#include "adcAccess.h"						// ADC Operations
#include "tiC2000.h"						// Microcontroller Communication


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
		printf("ADC Thread couldnt be opened\n");
		return NULL;
	}
	else{
		printf("ADC Thread has opened ADCport\n");
	

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
	int numBytez;						//Number of Bytes read from serialInterface
	float roll, pitch, yaw;				//Internal Storage
	struct termios options;				//Define Serial Port Options
	fd = open_serial(serial1Device);	//Open Serial Port
	printf("Serial1 Thread has opened port\n");

	
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
			numBytez = read(fd, &byte, 1);
		}
		numBytez = read(fd, &byte, 1);
		if ( byte != 'n')
			goto search_header;
		numBytez = read(fd, &byte, 1);
		if ( byte != 'p')
			goto search_header;

		//Valid Package Start here
		if ( SERIAL1_DEBUG)
			printf("Valid Package Start Detected \n");

		numBytez = read(fd, &byte, 1);
		if ( byte != SENSOR_DATA){
			continue;
			/*Interpret ChannelData Here.
			 * v1.0 For now we Interpret Sensor Data Package
			 * v2.0 We will interpret more package types
			 */
		}

		numBytez = read(fd, &len, 1);
		numBytez = read(fd, &flagByte1, 1);
		numBytez = read(fd, &flagByte2, 1);
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
			if ( -1 != ( byteNum = read(fd, &dataBuffer[readBytes], requiredBytes - readBytes) )){
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
	bool SERIAL2_DEBUG = false;
	SharedMemory *mem;
	mem = (SharedMemory*) (param);		// Get Shared Memory Instance of the System

	struct termios options;

	//Open Serial
	int fd;
	fd = open_serial(serial2Device);
		
	if (fd < 0) {
		perror("open");
		printf("Serial2 Thread couldnt open port\n");
		
	}
	else{
	printf("Serial2 Thread has opened port\n");
	
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
	unsigned int numBytes;
	char dataBuffer[32];
	unsigned char flagByte1,flagByte2;
	unsigned short motorDuty[4];
 	float roll,pitch, yaw;
	int i;
	float U[4];

	while(1){
		search_header:
		while( byte != 's'){
			read(fd, &byte, 1);
		}
		numBytes = read(fd, &byte, 1);
		if ( byte != 'u')
			goto search_header;

		//Valid Package Start here{
		/*
		if ( DEBUG)
			printf("Valid Package Start Detected \n");
		*/

		numBytes = read(fd, &byte, 1);
		numBytes = read(fd, &len, 1);

		/*
		if ( DEBUG) {
		printf("Type: %d\n", (unsigned int)byte);
		printf("Len: %d\n", (unsigned int)len);
		}*/
		
		if ( byte != TI_SENSOR_DATA && byte != TI_PID_DATA ){
			if ( SERIAL2_DEBUG ){
				 printf("Warning there is a package type which is not supported!\n");
			 }
		 	goto search_header;
		 }
		 			

		/*Read all data to dataBuffer */
		int byteNum, readBytes = 0, requiredBytes = len;
		while ( readBytes < requiredBytes){
			if ( -1 != ( byteNum = read(fd, &dataBuffer[readBytes], requiredBytes - readBytes) )){
				readBytes += byteNum;
			}

		}

		/*Interpret packages individually*/
		if ( byte == TI_SENSOR_DATA){
			/*SULink only gets IMU values for now*/
	        roll = char_to_float( &dataBuffer[0]);
	        pitch = char_to_float( &dataBuffer[4]);
	        yaw = char_to_float( &dataBuffer[8]);
	        //printf("roll:%.2f pitch:%.2f yaw: %.2f \n", yaw, pitch, roll);
		mem->setRoll(roll);
		mem->setPitch(pitch);
		mem->setYaw(yaw);
		}
		else if ( byte == TI_PID_DATA){
			/*
			for(i = 0; i < len; i++)
				printf("[%x] ", dataBuffer[i]);
			*/

			motorDuty[0] = char_to_short( &dataBuffer[0]);
			motorDuty[1] = char_to_short( &dataBuffer[2]);
			motorDuty[2] = char_to_short( &dataBuffer[4]);
			motorDuty[3] = char_to_short( &dataBuffer[6]);
			U[0] = char_to_float( &dataBuffer[8]);
			U[1] = char_to_float( &dataBuffer[12]);
			U[2] = char_to_float( &dataBuffer[16]);
			U[3] = char_to_float( &dataBuffer[20]);
			mem->MotorDuty[0] = motorDuty[0];
			mem->MotorDuty[1] = motorDuty[1];
			mem->MotorDuty[2] = motorDuty[2];
			mem->MotorDuty[3] = motorDuty[3];
			mem->U[0] = U[0];
			mem->U[1] = U[1];
			mem->U[2] = U[2];
			mem->U[3] = U[3];
			//printf("MotorDuties: %d %d %d %d \n", motorDuty[0], motorDuty[1], motorDuty[2], motorDuty[3]);
			//printf("Virtual   U: %f %f %f %f \n", U[0], U[1], U[2], U[3]);
		}
		else{
			//Other package types not supported for now
			continue;
		}

		//Flush Serial Buffer or we will process all packages by order...
		tcflush(fd, TCIOFLUSH );
		usleep(serialSleepPeriod);
		//system("clear");


	}

	}
}

/*******************************************************************************
* Function Name  : loggerThreadRun
* Input          : 
* Output         : Logs system status & Shared Memory to Blackbox
* Return         : None
* Description    : 
*******************************************************************************/
void *loggerThreadRun(void *param)	
{
	SharedMemory *mem;
	mem = (SharedMemory*) (param);		// Get Shared Memory Instance of the System

	
	static char FlightLogFileString[127];
	static char folderName[127]; 
	char timeStr[100];
	char todayDate[100]; 
	
	/* TO BE IMPLEMENTED 
	//Check for BlackBox folder's folder size
	//If foldersize is greater than 10mb, dont start the autopilot and warn 
	//User to backup the Blackbox folder.
	*/
	
	//Create Log directories if NOT existing........
	strcpy(folderName, logPathRoot);
	DIR *dir = opendir(folderName);
	if ( dir == NULL ){
		mkdir(folderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);	//Create logRoot directory
		strcat(folderName, flightLogPath);							
		mkdir(folderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);	//Create flightLogRoot directory
		strcpy(folderName, logPathRoot);
		strcat(folderName, algLogPath);							
		mkdir(folderName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);	//Create algorithmLogRoot directory
	}

	char *datePtr;
	strcpy(FlightLogFileString, logPathRoot);			//Copy LogPathRoot 
	strcat(FlightLogFileString, flightLogPath);			//Append FlightLogPath
	//datePtr = getDateString();							//Append date string to FlightLogFileString
	//strReplace(datePtr, " ", "_" , &todayDate[0]);		//Replace whitespaces with _
	//strcat(FlightLogFileString, &todayDate[0]);			//Append today's hour::min
	getTimeString(&timeStr[0]);
	strcat(FlightLogFileString, timeStr);
	strcat(FlightLogFileString, ".txt");				//Append .txt extension to FlightLogFileString
	
	FILE *logFile;
	while( 1 ){
	
	 	logFile = fopen(FlightLogFileString, "a+");
		//Get timeString
		datePtr = getDateString();
	
		//Append shared memory to logfile
		fprintf(logFile, "ROLL: %.2f PITCH: %.2f YAW:%.3f SONAR1:%.2f ", mem->getRoll(), mem->getPitch(), mem->getYaw(), mem->getSonar(SONAR1));
		fprintf(logFile, "U1: %.2f U2: %.2f U3: %.2f U4: %.2f ", mem->getU1(), mem->getU2(), mem->getU3(), mem->getU4());
		fprintf(logFile, "Duty1: %d Duty2: %d Duty3: %d Duty4: %d ", mem->getDuty1(), mem->getDuty2(), mem->getDuty3(), mem->getDuty4());
		
		fprintf(logFile, " TIME:%s\n", datePtr);
		//Append systemStatus to logfile
		
		//write necessary variables....
		
		fclose(logFile);
		usleep(loggerSleepPeriod * 1000);	
	}
	
}

/*******************************************************************************
* Function Name  : sysStatusTXThreadRun
* Input          : 
* Output         : Transmits system status to remote computer (Slow Comm)
* Return         : None
* Description    : Transmits system status with an interval of sysStatusSleepPeriod 
*				   defined in SystemConfig.h
*******************************************************************************/
void *sysStatusTXThreadRun(void *param)
{
		char target_ip[100];
    	float position[6] = {};
    	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);/* socket identifier */
    	int opt=1;        								/* option is to be on/TRUE or off/FALSE */
    	struct sockaddr_in gcAddr; 
    	struct sockaddr_in locAddr;
    	//struct sockaddr_in fromAddr;
    	uint8_t buf[BUFFER_LENGTH];
    	ssize_t recsize;
    	socklen_t fromlen;
    	int bytes_sent;
    	mavlink_message_t msg;
    	uint16_t len;
    	int i = 0;
    	//int success = 0;
    	unsigned int temp = 0;
    	
    	strcpy(target_ip, baseStationIP);			//Ground Station's IP Address defined in SystemConfig
    	
    	memset(&locAddr, 0, sizeof(locAddr));
    	locAddr.sin_family = AF_INET;
    	locAddr.sin_addr.s_addr = INADDR_ANY;
    	locAddr.sin_port = htons(connectTXPort1);   // DEFINED IN SYSTEM CONFIG
    	
    	
     	/* Bind the socket to port connectTXPort1 - necessary to receive packets from qgroundcontrol */ 
    	if (-1 == bind(sock,(struct sockaddr *)&locAddr, sizeof(struct sockaddr)))
        {
    		perror("error bind failed: sysStatusTXThreadRun");
    		close(sock);
    		exit(EXIT_FAILURE);
        }    	 	
    	
    	/* Attempt to make it non blocking */
    	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
        {
    		fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
    		close(sock);
    		exit(EXIT_FAILURE);
        }
        
        memset(&gcAddr, 0, sizeof(gcAddr));
    	gcAddr.sin_family = AF_INET;
    	gcAddr.sin_addr.s_addr = inet_addr(target_ip);
    	gcAddr.sin_port = htons(connectRXPort1);    	
    	
    	while(1 == 1){
    		/*Send Heartbeat */
    		mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_MANUAL_ARMED, 0, MAV_STATE_ACTIVE);
    		len = mavlink_msg_to_send_buffer(buf, &msg);
    		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
    		/* Send Status */
    		mavlink_msg_sys_status_pack(1,  /* system_id ID of this system */
					  MAV_COMP_ID_ALL,  /* component_id ID of this component */
					  &msg, /*The MAVLink channel this message was sent over */
					  0,    /*onboard_control_sensors_present */
					  0,    /*onboard_control_sensors_enabled */
					  0,    /*onboard_control_sensors_health */
					  500,  /*load Maximum usage in percent of the mainloop time */
					  11000,/*Battery voltage, in millivolts */ 
					  -1,   /*Battery current, in 10*milliamperes */
					  100,  /*Remaining battery energy: (0%: 0, 100%: 100) */
					  0,    /*Communication drops in percent, (0%: 0, 100%: 10'000) */
					  0,    /*Communication errors (UART, I2C, SPI, CAN), dropped packets on all linksr */
					  0,    /*Autopilot-specific errors */
					  0,    /*Autopilot-specific errors */
					  0,    /*Autopilot-specific errors */
					  0);   /*Autopilot-specific errors */   
    		len = mavlink_msg_to_send_buffer(buf, &msg);
    		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof (struct sockaddr_in));
			/* Send global position */
			mavlink_msg_global_position_int_pack(255, 200, &msg, 0, 408931000, 293750000, 550000,550000, 0,0,0,0);
			len = mavlink_msg_to_send_buffer(buf, &msg);
			bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));    							  
     		/* Receive from GroundStation */
    		memset(buf, 0, BUFFER_LENGTH);
    		recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
    		if (recsize > 0)
          	{
    			// Something received - print out all bytes and parse packet
    			mavlink_message_t msg;
    			mavlink_status_t status;
    			printf("Bytes Received: %d\nDatagram: ", (int)recsize);
    			for (i = 0; i < recsize; ++i){
    				temp = buf[i];
    				printf("%02x ", (unsigned char)temp);
    				if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
    				{
    					// Packet received
    					printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
						switch(msg.msgid)
						{
								case MAVLINK_MSG_ID_HEARTBEAT:
								{
							  // E.g. read GCS heartbeat and go into
						                      // comm lost mode if timer times out
								}
								  break;
							case MAVLINK_MSG_ID_COMMAND_LONG:
							  // EXECUTE ACTION
							  break;
							case MAVLINK_MSG_ID_PING:
								printf("\nPing request from SuGroundControl! %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
							break;
							default:
							//Do nothing
							  break;
						}
    				}
    			}
    			printf("\n");
    		}
    		memset(buf, 0, BUFFER_LENGTH);
    		usleep(sysStatusSleepPeriod * 1000); // Sleep one second    				
					   		
    	}
}
void *sensorTXThreadRun(void *param)
{
		SharedMemory *mem;
		mem = (SharedMemory*) (param);		// Get Shared Memory Instance of the System

		char target_ip[100];
    	float position[6] = {};
    	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);/* socket identifier */
    	int opt=1;        								/* option is to be on/TRUE or off/FALSE */
    	struct sockaddr_in gcAddr; 
    	struct sockaddr_in locAddr;
    	//struct sockaddr_in fromAddr;
    	uint8_t buf[BUFFER_LENGTH];
    	ssize_t recsize;
    	socklen_t fromlen;
    	int bytes_sent;
    	mavlink_message_t msg;
    	uint16_t len;
    	int i = 0;
    	//int success = 0;
    	unsigned int temp = 0;
    	
    	strcpy(target_ip, baseStationIP);			//Ground Station's IP Address defined in SystemConfig
    	
    	memset(&locAddr, 0, sizeof(locAddr));
    	locAddr.sin_family = AF_INET;
    	locAddr.sin_addr.s_addr = INADDR_ANY;
    	locAddr.sin_port = htons(connectTXPort2);   // DEFINED IN SYSTEM CONFIG
    	
    	
     	/* Bind the socket to port connectTXPort2 - necessary to receive packets from qgroundcontrol */ 
    	if (-1 == bind(sock,(struct sockaddr *)&locAddr, sizeof(struct sockaddr)))
        {
    		perror("error bind failed: sensorTXThreadRun");
    		close(sock);
    		exit(EXIT_FAILURE);
        }    	 	
    	
    	/* Attempt to make it non blocking */
    	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
        {
    		fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
    		close(sock);
    		exit(EXIT_FAILURE);
        }
        
        memset(&gcAddr, 0, sizeof(gcAddr));
    	gcAddr.sin_family = AF_INET;
    	gcAddr.sin_addr.s_addr = inet_addr(target_ip);
    	gcAddr.sin_port = htons(connectRXPort2);    	
    	
    	while(1 == 1){
    		uint64_t currentTime = microsSinceEpoch();
			/* Send global position */
			mavlink_msg_global_position_int_pack(1, 200, &msg, 0, 408931000, 293750000, 550000,550000, 0,0,0,0);    	
    		/* Send Local Position 
    		mavlink_msg_local_position_ned_pack(1, MAV_COMP_ID_IMU, &msg, currentTime, position[0], position[1], position[2], position[3], position[4], position[5]);
    		len = mavlink_msg_to_send_buffer(buf, &msg);
    		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));*/
     
    		/* Send attitude */
    		//Convert roll,pitch,yaw to radian
    		float rollRad = (mem->getRoll() * PI ) / 180;
    		float pitchRad = (mem->getPitch() * PI ) / 180;
    		float yawRad = (mem->getYaw() * PI ) / 180;
    		mavlink_msg_attitude_pack(1, MAV_COMP_ID_IMU, &msg, microsSinceEpoch(), rollRad, pitchRad, yawRad, 0, 0, 0);
    		len = mavlink_msg_to_send_buffer(buf, &msg);
    		bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));    		  							  
     		/* Receive from GroundStation */
    		memset(buf, 0, BUFFER_LENGTH);
    		recsize = recvfrom(sock, (void *)buf, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
    		if (recsize > 0)
          	{
    			// Something received - print out all bytes and parse packet
    			mavlink_message_t msg;
    			mavlink_status_t status;
    			printf("Bytes Received: %d\nDatagram: ", (int)recsize);
    			for (i = 0; i < recsize; ++i){
    				temp = buf[i];
    				printf("%02x ", (unsigned char)temp);
    				if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
    				{
    					// Packet received
    					printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
						switch(msg.msgid)
						{
								case MAVLINK_MSG_ID_HEARTBEAT:
								{
							  // E.g. read GCS heartbeat and go into
						                      // comm lost mode if timer times out
								}
								  break;
							case MAVLINK_MSG_ID_COMMAND_LONG:
							  // EXECUTE ACTION
							  break;
							case MAVLINK_MSG_ID_PING:
								printf("\nPing request from SuGroundControl! %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
							break;
							default:
							//Do nothing
							  break;
						}
    				}
    			}
    			printf("\n");
    		}
    		memset(buf, 0, BUFFER_LENGTH);
    		usleep(sensorTXSleepPeriod * 1000); // Sleep one second    				
					   		
    	}
}
