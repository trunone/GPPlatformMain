/*
 *   StrategyStatus.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "StrategyStatus.h"

using namespace Robot;

long StrategyStatus::MotorSpeed[3];
double StrategyStatus::x(0);
double StrategyStatus::y(0);
double StrategyStatus::FI(0);
double StrategyStatus::w(0);

TCoordinate StrategyStatus::StartPosition;
TCoordinate StrategyStatus::EscapePosition;
TCoordinate StrategyStatus::EndPosition;

TCoordinate StrategyStatus::RobotPos;
TCoordinate StrategyStatus::RootHandle;

TCoordinate StrategyStatus::LivRMDoor;
TCoordinate StrategyStatus::LivRMCen;
TCoordinate StrategyStatus::DinRMDoor;
TCoordinate StrategyStatus::DinRMCen;
TCoordinate StrategyStatus::LibDoor;
TCoordinate StrategyStatus::LibCen;
TCoordinate StrategyStatus::BedRMDoor;
TCoordinate StrategyStatus::BedRMCen;
TCoordinate StrategyStatus::ChrgDoor;
TCoordinate StrategyStatus::ChrgCen;

bool StrategyStatus::FlagRoomRenew;
bool StrategyStatus::FlagMember;
bool StrategyStatus::FlagGrandPa;
bool StrategyStatus::FlagGrandMa;
bool StrategyStatus::FlagFather;
bool StrategyStatus::FlagMother;
bool StrategyStatus::FlagSon;
bool StrategyStatus::Flagdaughter;
int StrategyStatus::CurrentBallState;

int StrategyStatus::GotoRoomStep;


int StrategyStatus::RoomSort;
int StrategyStatus::FindBallEn;
//----------SIM_FLAG---------------------------------------------------
bool StrategyStatus::SimulatorFlag;
//----------Test int----------------------------------------------------

int StrategyStatus::FamilyMember;

//---------- Strategy --------------------------------------------------
TCoordinate StrategyStatus::Goal1;                   // Target1 vector
TCoordinate StrategyStatus::Goal2;                   // Target2 vector
double StrategyStatus::Direction;                    // Target direction
bool StrategyStatus::FlagDetour;
bool StrategyStatus::FlagForward;
StrategyStatus::tsAStarPath StrategyStatus::AStarPath;

//----------------------------------------------------------------------
//----------- Path Plan ------------------------------------------------
TCoordinate StrategyStatus::GoalVector;

//----------------------------------------------------------------------
//----------- Avoidance ------------------------------------------------
bool StrategyStatus::FlagAvoidEnable;
TCoordinate StrategyStatus::CorrectionVector;
//----------------------------------------------------------------------
//----------- Velocity Control -----------------------------------------
float StrategyStatus::FixSpeed;                                 // Speed Power 1~100 %
double StrategyStatus::MotionDistance;
double StrategyStatus::MotionAngle;
TCoordinate StrategyStatus::PathMotion;
float StrategyStatus::PathRotation;

//----------------------------------------------------------------------
bool StrategyStatus::PathR_Priority;

//----------- Ball Data -----------------------------------------

StrategyStatus::tsStraBallInfo StrategyStatus::RadBallInfo;

//tsObjectiveInfo RedBall;

int StrategyStatus::AX12_Angle;
int StrategyStatus::CatchBallMode;
unsigned char StrategyStatus::FindBallCnt;
bool StrategyStatus::FlagRecognize;

//---------- Laser Average Data-----------------
int StrategyStatus::LaserAverageData[3];

//---------- ŽM²yµŠ²€šBÆJ-----------------

int StrategyStatus::FindBallState;

//---------- ŽM²y­«·sÅxÂI-----------------

bool StrategyStatus::FindLocResample;
TCoordinate StrategyStatus::RseLocPos;


