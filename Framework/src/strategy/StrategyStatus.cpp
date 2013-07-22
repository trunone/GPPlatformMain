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
//--------------------------------------------------------------------------------------xml
int StrategyStatus::loadxml (TiXmlElement* element,int mod){
if(mode==1){
	if(element != NULL) {
		TiXmlElement* child;
		child = element->FirstChildElement("Rotate");
		if(child != NULL){
		child->Attribute("w", &StrategyStatus::w);
	}
	child = element->FirstChildElement("Vector");
	if(child != NULL){
		child->Attribute("x", &StrategyStatus::x);
		child->Attribute("y", &StrategyStatus::y);
	}
}else if(mode==2){
	if(element != NULL){	
		TiXmlElement* modelchild;
		modelchild=element->FirstChildElement("RootHandle");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::RootHandle.x);
			modelchild->Attribute("y", &StrategyStatus::RootHandle.y);						
		}
		modelchild=element->FirstChildElement("StartPos");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::StartPosition.x);
			modelchild->Attribute("y", &StrategyStatus::StartPosition.y);						
		}
		modelchild=element->FirstChildElement("EndPos");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::EndPosition.x);
			modelchild->Attribute("y", &StrategyStatus::EndPosition.y);						
		}
		modelchild=element->FirstChildElement("FindBallEn");
		if(modelchild != NULL){
			modelchild->Attribute("FindBallEn", &StrategyStatus::FindBallEn);
		}
		modelchild=element->FirstChildElement("LivRMDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LivRMDoor.x);
			modelchild->Attribute("y", &StrategyStatus::LivRMDoor.y);						
		}
		modelchild=element->FirstChildElement("LivRMCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LivRMCen.x);
			modelchild->Attribute("y", &StrategyStatus::LivRMCen.y);						
		}
		modelchild=element->FirstChildElement("DinRMDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::DinRMDoor.x);
			modelchild->Attribute("y", &StrategyStatus::DinRMDoor.y);						
		}
		modelchild=element->FirstChildElement("DinRMCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::DinRMCen.x);
			modelchild->Attribute("y", &StrategyStatus::DinRMCen.y);						
		}
		modelchild=element->FirstChildElement("LibDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LibDoor.x);
			modelchild->Attribute("y", &StrategyStatus::LibDoor.y);						
		}
		modelchild=element->FirstChildElement("LibCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LibCen.x);
			modelchild->Attribute("y", &StrategyStatus::LibCen.y);						
		}
		modelchild=element->FirstChildElement("BedRMDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::BedRMDoor.x);
			modelchild->Attribute("y", &StrategyStatus::BedRMDoor.y);						
		}
		modelchild=element->FirstChildElement("BedRMCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::BedRMCen.x);
			modelchild->Attribute("y", &StrategyStatus::BedRMCen.y);						
		}
		modelchild=element->FirstChildElement("ChrgDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::ChrgDoor.x);
			modelchild->Attribute("y", &StrategyStatus::ChrgDoor.y);						
		}
		modelchild=element->FirstChildElement("ChrgCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::ChrgCen.x);
			modelchild->Attribute("y", &StrategyStatus::ChrgCen.y);						
		}
	}
}
	return 0;
}
//----------------------------------------------------------------------------------------------
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
