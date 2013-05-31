#include <stdio.h>
#include <string.h>
#include "cmd_process.h"

using namespace Robot;

void Help()
{
	printf( "\n" );
	printf( " exit : Exits the program\n" );
	printf( " scan : Outputs the current status of all Dynamixels\n" );
	printf( " id [ID] : Go to [ID]\n" );
	printf( " d : Dumps the current control table of CM-730 and all Dynamixels\n" );
	printf( " reset : Defaults the value of current Dynamixel\n" );
	printf( " reset all : Defaults the value of all Dynamixels\n" );
	printf( " wr [ADDR] [VALUE] : Writes value [VALUE] to address [ADDR] of current Dynamixel\n" );
	printf( " on/off : Turns torque on/off of current Dynamixel\n" );
	printf( " on/off all : Turns torque on/off of all Dynamixels)\n" );
	printf( "\n       Copyright ROBOTIS CO.,LTD.\n\n" );
}

