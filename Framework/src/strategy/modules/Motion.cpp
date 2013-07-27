/*
 *   Motion.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include "Motion.h"

#define PI 3.1415926

using namespace Robot;
using namespace std;

Motion* Motion::m_UniqueInstance = new Motion();

Motion::Motion()
{
}

Motion::~Motion()
{
}

void Motion::Initialize()
{
}

void Motion::Process()
{
	double FI = StrategyStatus::FI;
	double w = StrategyStatus::w*240;
	double robot_radius = 1;
    double x = StrategyStatus::x;
    double y = StrategyStatus::y;
	double angle1 = (PI/6)+FI;
	double angle2 = 5*(PI/6)+FI;
	double angle3 = 3*(PI/2)+FI;
	StrategyStatus::MotorSpeed[0] = -sin(angle1)*(x)+cos(angle1)*(y)-robot_radius*w;
	StrategyStatus::MotorSpeed[1] = -sin(angle2)*(x)+cos(angle2)*(y)-robot_radius*w;
	StrategyStatus::MotorSpeed[2] = -sin(angle3)*(x)+cos(angle3)*(y)-robot_radius*w;
}
