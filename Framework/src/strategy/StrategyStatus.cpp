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

void Initial();

bool StrategyStatus::FlagRoomRenew;

//tsRoomInfo StrategyStatus::Room;

int StrategyStatus::RoomSort;		

int StrategyStatus::FindBallEn;		

int StrategyStatus::LivRM;
int StrategyStatus::DinRM; 
int StrategyStatus::Lib;
int StrategyStatus::BedRM;


//tsAStarPath StrategyStatus::AStarPath;

//---------- Strategy -----------------------------------
TCoordinate StrategyStatus::Goal1;                   // Target1 vector

TCoordinate StrategyStatus::Goal2;                   // Target2 vector

double StrategyStatus::Direction;                    // Target direction

bool StrategyStatus::FlagDetour;                     //true: Ÿaªñ¥ØŒÐÂIP2 , flase: »·Â÷¥ØŒÐÂIp2

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

//----------- Ball Data -----------------------------------------

//tsStraBallInfo StrategyStatus::RadBallInfo;

//tsObjectiveInfo RedBall;

unsigned char StrategyStatus::FindBallCnt;

bool StrategyStatus::FlagRecognize;
//---------- Laser Average Data-----------------

int StrategyStatus::LaserAverageData[3];       //¹p®g¥ª ¥k «e ¥­§¡Data

//---------- ŽM²yµŠ²€šBÆJ-----------------
int StrategyStatus::FindBallState;



//---------- ŽM²y­«·sÅxÂI-----------------

bool StrategyStatus::FindLocResample;

TCoordinate StrategyStatus::RseLocPos;        //·sªº©wŠìÂI	
double StrategyStatus::x(0);
double StrategyStatus::y(0);
