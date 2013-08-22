#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "Odometer.h"
#include "LocationStatus.h"

#define WHEEL_RADIUS 5.125
#define ROBOT_RADIUS 17.0
#define PULSE_PER_TURN 4096.0
#define GEAR_RATIO 2.85

#define ANGLE_1 7*(M_PI/6)
#define ANGLE_2 5*(M_PI/6)
#define ANGLE_3 M_PI/2

using namespace Robot;

Odometer* Odometer::m_UniqueInstance = new Odometer();

Odometer::Odometer():
    mAngle1Sin(sin(ANGLE_1)),
    mAngle2Sin(-sin(ANGLE_2)),
    mAngle3Sin(sin(ANGLE_3)),
    mAngle1Cos(cos(ANGLE_1)),
    mAngle2Cos(-cos(ANGLE_2)),
    mAngle3Cos(cos(ANGLE_3)),
    mDistConst((2*WHEEL_RADIUS*M_PI)/(PULSE_PER_TURN*GEAR_RATIO))
{
}
Odometer::~Odometer()
{
}
void Odometer::Initialize()
{
    mPastMotorPulse[0] = 0;
    mPastMotorPulse[1] = 0;
    mPastMotorPulse[2] = 0;
}

void Odometer::Process()
{
    GetWheelDistance(0);
    GetWheelDistance(1);
    GetWheelDistance(2);
	double tmp[3];

	tmp[0] = fabs(mWheelDist[0]) / mWheelDist[0];
	tmp[1] = fabs(mWheelDist[1]) / mWheelDist[1];
	tmp[2] = fabs(mWheelDist[2]) / mWheelDist[2];

	if( (tmp[0] == tmp[1]) && (tmp[1] == tmp[2])){
			LocationStatus::FB_Movement.Direction /= ROBOT_RADIUS;
	}else{
			LocationStatus::FB_Movement.Position.y =
				(-2.0/3.0)*( mAngle1Sin*mWheelDist[0] + mAngle2Sin*mWheelDist[1] + mAngle3Sin*mWheelDist[2]);
			LocationStatus::FB_Movement.Position.x =
				(-2.0/3.0)*( mAngle1Cos*mWheelDist[0] + mAngle2Cos*mWheelDist[1] + mAngle3Cos*mWheelDist[2]);
			LocationStatus::FB_Movement.Direction =
				(1.0/3.0)*( mWheelDist[0] + mWheelDist[1] + mWheelDist[2] );
	}	
}

void Odometer::GetWheelDistance(short wheel_num) {
    if(mPastMotorPulse[wheel_num] == 0)
        mPastMotorPulse[wheel_num] = LocationStatus::MotorPulse[wheel_num];
    double pulse_diff = LocationStatus::MotorPulse[wheel_num] - mPastMotorPulse[wheel_num];
    mPastMotorPulse[wheel_num] = LocationStatus::MotorPulse[wheel_num];
    mWheelDist[wheel_num] = pulse_diff*mDistConst;
}

