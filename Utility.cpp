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
