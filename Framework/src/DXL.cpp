#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include "DXL.h"
#include "dynamixel.h"
#include "dxl_hal.h"

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define Moving_Speed_L    	32
#define Moving_Speed_H		33

using namespace Robot;

int  DXL::initialize()
{
	int deviceIndex = 0;
	float baudrate;	
	baudrate = 115200;
	if( dxl_hal_open(deviceIndex, baudrate) == 0 )
		return 0;

	return 1;

}
void DXL::Degree(int deg)
{
	printf("Which degrees do you want ? (0~90)\n");
	if(deg>90)
	{
		deg=90;
	}else if(deg<0)
	{
		deg=0;
	}
	deg=(deg*1023/300)+512;
	dxl_write_word( 1, P_GOAL_POSITION_L ,deg);
}
void DXL::EndlessTurn(int i)
{
	int left = 0;
	int right  = 0;	
	printf("Mode of catching ball ? (0~2)\n");
	switch(i)
	{
		case 0 : left=0; right=0; break;	//1~1023=>CCW 1024~2047=>CW
			
		case 1 : left=1023; right=2047; break;

		case 2 : left=1536; right=512; break;  
	}
	dxl_write_word( 2, Moving_Speed_L ,left);
	dxl_write_word( 3, Moving_Speed_L ,right);
	}
	
}
void DXL::dxl_terminate(void)
{
	dxl_hal_close();
}
