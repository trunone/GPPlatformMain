#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "Speedometer.h"

#define PI 3.1415926

using namespace Robot;

Speedometer::Speedometer()
{
}
Speedometer::~Speedometer()
{
}

void Speedometer::Process()
{
	short device_id;
	long *pPositionIs;
	double position_now1,position_now2,position_now3;
	double position_last1,position_last2,position_last3;
	double encorder1,encorder2,encorder3;
	double Motor1Distance,Motor2Distance,Motor3Distance;
	double FI=0;
	double angle1 = 7*(PI/6)+FI;
	double angle2 = 5*(PI/6)+FI;
	double angle3 = PI/2+FI;
	double Motor_x,Motor_y,Motor_sita;

///////////////////////////////////////////////////////////////////第一顆馬達

	position_now1 = GetPositionIs( 0, pPositionIs);
	encorder1 = position_now1 - position_last1;
	position_last1 = position_now1;
	
	Motor1Distance = encorder1/4096*10*PI;

///////////////////////////////////////////////////////////////////第二顆馬達

	position_now2 = GetPositionIs( 1, pPositionIs);
	encorder2 = position_now2 - position_last2;
	position_last2 = position_now2;
	
	Motor2Distance = encorder2/4096*10*PI;

///////////////////////////////////////////////////////////////////第三顆馬達

	position_now3 = GetPositionIs( 2, pPositionIs);
	encorder3 = position_now3 - position_last3;
	position_last3 = position_now3;
	
	Motor3Distance = encorder3/4096*10*PI;

///////////////////////////////////////////////////////////////////計算
	Motor_x = 2/3 * ( sin(angle1)*Motor1Distance - sin(angle2)*Motor2Distance + sin(angle3)*Motor3Distance);
	Motor_y = (-2)/3 * ( cos(angle1)*Motor1Distance - cos(angle2)*Motor2Distance + cos(angle3)*Motor3Distance);
	Motor_sita = (1/3)*( Motor1Distance + Motor2Distance + Motor3Distance );

}

