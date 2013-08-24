#include <stdio.h>
#include "time.h"
#include "DXL.h"
#include "dynamixel.h"
// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define Moving_Speed_L    	32
#define Moving_Speed_H		33

#define NORMALIZE 3.41 // 1023/300

#define MAX_DEGREE 70

using namespace Robot;

DXL::~DXL()
{
}

DXL::DXL()
{
}

int  DXL::OpenDevice(int deviceIndex)
{
    if( dxl_initialize(deviceIndex, 0 ) == 0 )
        return 0;
    return 1;
}

void DXL::GoToDegree(int deg)
{
    if(deg>MAX_DEGREE)
    {
        deg=MAX_DEGREE;
    } else if(deg<0)
    {
        deg=0;
    }
    deg=(deg*NORMALIZE)+512;
    dxl_write_word( 1, P_GOAL_POSITION_L ,deg);
}

void DXL::EndlessTurn(int mode)
{
	clock_t StartTime;
	switch(mode){
	case STOP:
        dxl_write_word( 2, Moving_Speed_L,0);
        dxl_write_word( 3, Moving_Speed_L,0);
        break;			

	case CATCH:
        dxl_write_word( 2, Moving_Speed_L,1536);
        dxl_write_word( 3, Moving_Speed_L,512);
        break;

	case THROW: 
		dxl_write_word( 2, Moving_Speed_L,0);
		dxl_write_word( 3, Moving_Speed_L,0);
		while((clock()-StartTime)<1*CLOCKS_PER_SEC);
		dxl_write_word( 2, Moving_Speed_L,400);
		dxl_write_word( 3, Moving_Speed_L,1424);
		StartTime=clock();    					
		while((clock()-StartTime)<2.7*CLOCKS_PER_SEC);
		dxl_write_word( 2, Moving_Speed_L,0);
		dxl_write_word( 3, Moving_Speed_L,0);
		StartTime=clock();
		while((clock()-StartTime)<1*CLOCKS_PER_SEC);
		dxl_write_word( 2, Moving_Speed_L,1700);
		dxl_write_word( 3, Moving_Speed_L,1536);
		StartTime=clock();
		while((clock()-StartTime)<2*CLOCKS_PER_SEC);
		dxl_write_word( 2, Moving_Speed_L,0);
		dxl_write_word( 3, Moving_Speed_L,0);
		break;
	}
}

void DXL::CloseDevice(void)
{
    dxl_terminate();
}
