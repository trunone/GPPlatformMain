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
	StrategyStatus::Motor1Speed=-sin(angle1)*(x)+cos(angle1)*(y)-robot_radius*w;
	StrategyStatus::Motor2Speed=-sin(angle2)*(x)+cos(angle2)*(y)-robot_radius*w;
	StrategyStatus::Motor3Speed=-sin(angle3)*(x)+cos(angle3)*(y)-robot_radius*w;
}
