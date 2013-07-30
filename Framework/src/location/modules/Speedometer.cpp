#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "Speedometer.h"

#define WHEEL_RADIUS 5
#define ROBOT_RADIUS 17
#define PULSE_PER_TURN 4096

using namespace Robot;

Speedometer* Speedometer::m_UniqueInstance = new Speedometer();

Speedometer::Speedometer()
{
}
Speedometer::~Speedometer()
{
}
void Speedometer::Initialize()
{
    mPastMotorPulse[0] = 0;
    mPastMotorPulse[1] = 0;
    mPastMotorPulse[2] = 0;
}

void Speedometer::Process()
{
	long pulse_diff[3];
    double motor_dist[3];
	double FI=LocationStatus::FI;
	double angle1 = 7*(M_PI/6)+FI;
	double angle2 = 5*(M_PI/6)+FI;
	double angle3 = M_PI/2+FI;

///////////////////////////////////////////////////////////////////第一顆馬達

    if(mPastMotorPulse[0] == 0) mPastMotorPulse[0] = LocationStatus::MotorPulse[0];
	pulse_diff[0] = LocationStatus::MotorPulse[0] - mPastMotorPulse[0];
	mPastMotorPulse[0] = LocationStatus::MotorPulse[0];
	motor_dist[0] = (pulse_diff[0]/PULSE_PER_TURN)*2*WHEEL_RADIUS*M_PI;

///////////////////////////////////////////////////////////////////第二顆馬達

    if(mPastMotorPulse[1] == 0) mPastMotorPulse[1] = LocationStatus::MotorPulse[1];
	pulse_diff[1] = LocationStatus::MotorPulse[1] - mPastMotorPulse[1];
	mPastMotorPulse[1] = LocationStatus::MotorPulse[1];
	motor_dist[1] = (pulse_diff[1]/PULSE_PER_TURN)*2*WHEEL_RADIUS*M_PI;

///////////////////////////////////////////////////////////////////第三顆馬達

    if(mPastMotorPulse[2] == 0) mPastMotorPulse[2] = LocationStatus::MotorPulse[2];
	pulse_diff[2] = LocationStatus::MotorPulse[2] - mPastMotorPulse[2];
	mPastMotorPulse[2] = LocationStatus::MotorPulse[2];
	motor_dist[2] = (pulse_diff[2]/PULSE_PER_TURN)*2*WHEEL_RADIUS*M_PI;

///////////////////////////////////////////////////////////////////計算

	LocationStatus::FB_Movement.Position.x =
        (2/3)*( sin(angle1)*motor_dist[0] - sin(angle2)*motor_dist[1] + sin(angle3)*motor_dist[2]);
	LocationStatus::FB_Movement.Position.y =
        ((-2)/3)*( cos(angle1)*motor_dist[0] - cos(angle2)*motor_dist[1] + cos(angle3)*motor_dist[2]);
	LocationStatus::FB_Movement.Direction =
        (1/3)*( motor_dist[0] + motor_dist[1] + motor_dist[2] );
    
    LocationStatus::FB_Movement.Direction /= ROBOT_RADIUS;
}

