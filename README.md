gumstix_autopilot
=================

An autopilot application for Gumstix for usage in MAVs.

Current Code Functionality :
   - Monitors CH6DM IMU
   - Monitors Overo ADC
   - Transmits local position, orientation using MAVLINK protocol
   - Logs system data to /blackbox/

To build:
    ./createMakeFile.sh [nameofyourexecutable] > makefile
    make
    
to run:
   ./[nameofyourexecutable]