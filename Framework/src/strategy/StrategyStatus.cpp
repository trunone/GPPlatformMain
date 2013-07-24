/*
 *   StrategyStatus.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "StrategyStatus.h"

using namespace Robot;

long StrategyStatus::Motor1Speed(0);
long StrategyStatus::Motor2Speed(0);
long StrategyStatus::Motor3Speed(0);

double StrategyStatus::FI(0);
double StrategyStatus::w(0);
Vector3D StrategyStatus::vector;

TCoordinate StrategyStatus::StartPosition;
TCoordinate StrategyStatus::EscapePosition;
TCoordinate StrategyStatus::EndPosition;

//void Initial();

bool StrategyStatus::FlagRoomRenew;

<<<<<<< HEAD
StrategyStatus::tsRoomInfo StrategyStatus::Room;

=======
//tsRoomInfo StrategyStatus::Room;
>>>>>>> 2db1a3b5fcb82021d9f615b816cd29cb494acc0b
int StrategyStatus::RoomSort;		
int StrategyStatus::FindBallEn;		
int StrategyStatus::LivRM;
int StrategyStatus::DinRM; 
int StrategyStatus::Lib;
int StrategyStatus::BedRM;


StrategyStatus::tsAStarPath StrategyStatus::AStarPath;

//---------- Strategy --------------------------------------------------
TCoordinate StrategyStatus::Goal1;                   // Target1 vector
TCoordinate StrategyStatus::Goal2;                   // Target2 vector
double StrategyStatus::Direction;                    // Target direction
bool StrategyStatus::FlagDetour;                     // true: Ÿaªñ¥ØŒÐÂIP2 , flase: »·Â÷¥ØŒÐÂIp2
bool StrategyStatus::FlagForward;                     

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
double StrategyStatus::MotionDistance;		//²Ÿ°Ê¶ZÂ÷
double StrategyStatus::MotionAngle;		//²Ÿ°Êš€«×
TCoordinate StrategyStatus::PathMotion;
float StrategyStatus::PathRotation;

//----------------------------------------------------------------------
bool StrategyStatus::PathR_Priority;

<<<<<<< HEAD
//----------- Ball Data -----------------------------------------

StrategyStatus::tsStraBallInfo StrategyStatus::RadBallInfo;

//tsObjectiveInfo RedBall;

int StrategyStatus::AX12_Angle;

=======
//----------- Ball Data ------------------------------------------------
//tsStraBallInfo StrategyStatus::RadBallInfo;
//tsObjectiveInfo RedBall;
>>>>>>> 2db1a3b5fcb82021d9f615b816cd29cb494acc0b
unsigned char StrategyStatus::FindBallCnt;
bool StrategyStatus::FlagRecognize;

//---------- Laser Average Data-----------------
int StrategyStatus::LaserAverageData[3];       //¹p®g¥ª ¥k «e ¥­§¡Data

<<<<<<< HEAD
//---------- ŽM²yµŠ²€šBÆJ-----------------

int StrategyStatus::FindBallState;

//---------- ŽM²y­«·sÅxÂI-----------------

=======
//---------- ŽM²yµŠ²€šBÆJ ----------------------------------------------
int StrategyStatus::FindBallState;

//---------- ŽM²y­«·sÅxÂI -----------------------------------------------
>>>>>>> 2db1a3b5fcb82021d9f615b816cd29cb494acc0b
bool StrategyStatus::FindLocResample;
TCoordinate StrategyStatus::RseLocPos;        //·sªº©wŠìÂI	
	
double StrategyStatus::x(0);
double StrategyStatus::y(0);
