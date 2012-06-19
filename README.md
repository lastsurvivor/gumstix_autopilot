gumstix_autopilot
=================
Project Author : Taygun Kekec


GNU LESSER GENERAL PUBLIC LICENSE

                    GNU GENERAL PUBLIC LICENSE
                       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 
 
An autopilot application for Gumstix for usage in MAVs.


Current Code Functionality :
   - Monitors CH6DM IMU
   - Monitors Overo ADC
   - Added Half-Duplex Communication with TIC2000
   - Transmits local position, orientation using MAVLINK protocol
   - Logs system data to /blackbox/

To build:
    ./createMakeFile.sh [nameofyourexecutable] > makefile
    make
    
to run:
   ./[nameofyourexecutable]