/*
 *   Motion.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include "Motion.h"

#define ANGLE_1 M_PI/6
#define ANGLE_2 5*(M_PI/6)
#define ANGLE_3 3*(M_PI/2)

#define ROBOT_RADIUS 1.0

using namespace Robot;
using namespace std;

Motion* Motion::m_UniqueInstance = new Motion();

Motion::Motion():
    mAngle1Sin(-sin(ANGLE_1)),
    mAngle2Sin(-sin(ANGLE_2)),
    mAngle3Sin(-sin(ANGLE_3)),
    mAngle1Cos(cos(ANGLE_1)),
    mAngle2Cos(cos(ANGLE_2)),
    mAngle3Cos(cos(ANGLE_3))
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
    double w = StrategyStatus::w*240;
    StrategyStatus::MotorSpeed[0] = mAngle1Sin*StrategyStatus::x+mAngle1Cos*StrategyStatus::y-ROBOT_RADIUS*w;
    StrategyStatus::MotorSpeed[1] = mAngle2Sin*StrategyStatus::x+mAngle2Cos*StrategyStatus::y-ROBOT_RADIUS*w;
    StrategyStatus::MotorSpeed[2] = mAngle3Sin*StrategyStatus::x+mAngle3Cos*StrategyStatus::y-ROBOT_RADIUS*w;
}

