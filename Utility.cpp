#include "Utility.h"

/*******************************************************************************
* Function Name  : getCpuLoad
* Input          : -
* Output         : -
* Return         : Current CPULoad, between [0,1]
* Description    : The function asks Operating System about current CPULoad
*				   and returns it to the user.
*******************************************************************************/
float getCpuLoad()
{

}

/*******************************************************************************
* Function Name  : getDateString
* Input          : -
* Output         : -
* Return         : Current Date, format : hh:mm:ss_dd:mm:yy
* Description    : The function returns user current date
*				   ex.   14:52:05_28:02:12  which means 14.52  28 February 2012
*******************************************************************************/
char* getDateString()
{
	time_t rawtime;
	time ( &rawtime );
	return ctime(&rawtime);
}

/*******************************************************************************
* Function Name  : getTimeString
* Input          : -
* Output         : -
* Return         : Current Date, format : hh:mm
* Description    : The function returns current time
*				   ex.   14:52
*******************************************************************************/
void getTimeString(char *timeStr)
{
	struct tm * timeinfo;
	time_t rawtime;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (timeStr,20,"%I:%M%p",timeinfo);

}

/*******************************************************************************
* Function Name  : microsSinceEpoch
* Input          : -
* Output         : -
* Return         : Very obvious...
* Description    : Returns in int64 format
*******************************************************************************/
uint64_t microsSinceEpoch()
{
 
	struct timeval tv;
 
	uint64_t micros = 0;
 
	gettimeofday(&tv, NULL);  
	micros =  ((uint64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
 
	return micros;
}

void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}


/*******************************************************************************
* Function Name  : char_to_float
* Input          : -
* Output         : -
* Return         : Very obvious...
* Description    : Combines 4 chars into a float
*******************************************************************************/
float char_to_float( char* cs)
{
    return *(float*)cs;
}

/*******************************************************************************
* Function Name  : char_to_short
* Input          : -
* Output         : -
* Return         : Very obvious...
* Description    : Combines 4 chars into a 16bit integer
*******************************************************************************/
short char_to_short( char* cs)
{
    return *(short*)cs;
}

/*******************************************************************************
* Function Name  : char_to_int
* Input          : -
* Output         : -
* Return         : Very obvious...
* Description    : Combines 4 chars into a 32bit integer
*******************************************************************************/
int char_to_int( char* cs)
{
    return *(int*)cs;
}
