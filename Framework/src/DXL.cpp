#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include "DXL.h"

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define Moving_Speed_L    	32
#define Moving_Speed_H		33

#define NORMALIZE 3.41 // 1023/300

#define MAX_DEGREE 70

using namespace Robot;

int  DXL::initialize(int deviceIndex)
{
    float baudrate;
    baudrate = 115200;
    if( dxl_hal_open(deviceIndex, baudrate) == 0 )
        return 0;

    return 1;

}
void DXL::Degree(int deg)
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
	switch(mode){
	case 0 : dxl_write_word( 2, Moving_Speed_L,0);
		 dxl_write_word( 3, Moving_Speed_L,0);  break;	//1~1023=>CCW 1024~2047=>CW
			
	case 1 : dxl_write_word( 2, Moving_Speed_L,1536);
		 dxl_write_word( 3, Moving_Speed_L,512); break; 	//eat

	case 2 : dxl_write_word( 2, Moving_Speed_L,512);
		 dxl_write_word( 3, Moving_Speed_L,1536);
		 usleep(2200000);
		 dxl_write_word( 2, Moving_Speed_L,1700);
		 dxl_write_word( 3, Moving_Speed_L,1536); 
		 usleep(1900000);   
		 dxl_write_word( 2, Moving_Speed_L,0);
		 dxl_write_word( 3, Moving_Speed_L,0);break; 	//spit
	}
}
void DXL::dxl_terminate(void)
{
    dxl_hal_close();
}
DXL::~DXL()
{
}
DXL::DXL()
{
}
