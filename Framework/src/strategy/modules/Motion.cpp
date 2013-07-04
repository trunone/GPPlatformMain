/*
 *   Motion.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include "Motion.h"
#include "Vector.h"

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
	double w = StrategyStatus::w;
	Vector3D vector = StrategyStatus::vector;
	double robot_radius = 1;
	double angle1 = (PI/6)+FI;
	double angle2 = 5*(PI/6)+FI;
	double angle3 = 3*(PI/2)+FI;
	StrategyStatus::Motor1Speed=-sin(angle1)*(vector.X)+cos(angle1)*(vector.Y)+robot_radius*w;
	StrategyStatus::Motor1Speed=-sin(angle2)*(vector.X)+cos(angle2)*(vector.Y)+robot_radius*w;
	StrategyStatus::Motor1Speed=-sin(angle3)*(vector.X)+cos(angle3)*(vector.Y)+robot_radius*w;
}
