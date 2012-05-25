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
	/*
		THERE CAN BE A MEMORY LEAK HERE
		CHECK LATER. (._.)
	*/
	time_t rawtime;
	time ( &rawtime );
	return ctime(&rawtime);
}

/*******************************************************************************
* Function Name  : getTimeString
* Input          : -
* Output         : -
* Return         : Current Date, format : hh:mm:ss:mm
* Description    : The function returns current time
*				   ex.   14:52:05:79  which means 14.52.05 and 79milliseconds
*******************************************************************************/
void getTimeString(char *timeStr)
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


#include <string.h>
#include <stdlib.h>

void strReplace(
    char * original, char * pattern, char *replacement, char *returned){
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  //find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
  {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);

    if (returned != NULL)
    {
      // copy the original string, 
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement 
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
  }
}


