/*
 *   Stra_Task.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "Stra_Task.h"
#include "VisionStatus.h"
#include "LinuxActionScript.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define MEMBERS 3

using namespace Robot;
using namespace std;

Stra_Task* Stra_Task::m_UniqueInstance = new Stra_Task();

Stra_Task::Stra_Task()
{

}

Stra_Task::~Stra_Task()
{

}
//---------------------------------------------------------------------------xml
int Stra_Task::LoadXMLSettings (TiXmlElement* element) {
    if(element != NULL) {
		TiXmlElement *childelement = element->FirstChildElement();
		TiXmlElement *child = childelement->FirstChildElement();
		for(int i = 0;child != NULL;i++) {
			if(!strcmp("LivRM", child->Value()))
				Room.SortList[i] = etLivRM;
		    if(!strcmp("DinRM", child->Value()))
				Room.SortList[i] = etDinRM;
			if(!strcmp("Lib", child->Value()))
				Room.SortList[i] = etLib;
			if(!strcmp("BedRM", child->Value()))
				Room.SortList[i] = etBedRM;
			child=child->NextSiblingElement();
		}
		for(int i = 0; childelement != NULL; i++) {
			string tmp;
            childelement->Attribute("x", &Members[i].FrontPosition.x);
            childelement->Attribute("y", &Members[i].FrontPosition.y);
			childelement->Attribute("Member_x", &Members[i].MemberPosition.x);
            childelement->Attribute("Member_y", &Members[i].MemberPosition.y);
            childelement->QueryStringAttribute("room", &tmp);
			if(!strcmp("LivRM", tmp.c_str()))
				Members[i].Room = etLivRM;
		    if(!strcmp("DinRM", tmp.c_str()))
				Members[i].Room = etDinRM;
			if(!strcmp("Lib", tmp.c_str()))
				Members[i].Room = etLib;
			if(!strcmp("BedRM", tmp.c_str()))
				Members[i].Room = etBedRM;
			childelement = childelement->NextSiblingElement();
        }
		return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------
void Stra_Task::Initialize(void)
{
    FlagTaskFinish      = false;
    StrategyStatus::FlagMember	= false;
    StrategyStatus::FlagGrandPa	= false;
    StrategyStatus::FlagGrandMa	= false;
    StrategyStatus::FlagFather	= false;
    StrategyStatus::FlagMother	= false;
    StrategyStatus::FlagSon	= false;
    StrategyStatus::Flagdaughter	= false;
    FlagSetInitialData  = false;
    GotoRoomStep  = 0;
    ActiveState   = etIdle;
    DoorState     = false;
    Past_RoomCnt  = -1;
    TouchCnt      = 0;
    PastScanLineData = new int[24];
    Room.SKSRoomState = etSKSMoving;    
    MemberIndex = 0;
}
//---------------------------------------------------------------------------
void Stra_Task::Process(void)
{
    TCoordinate LivTmp = aVector(StrategyStatus::LivRMDoor.x, 0);
    TCoordinate DinTmp = aVector(0, StrategyStatus::DinRMDoor.y);
    TCoordinate BedTmp = aVector(0, StrategyStatus::BedRMDoor.y);
    TCoordinate LibTmp = aVector(0, StrategyStatus::LibDoor.y);
    //------face det part---------
    TCoordinate Livfacefront = aVector(StrategyStatus::LivRMDoor.x, StrategyStatus::LivRMDoor.y);
    TCoordinate Livface = aVector(StrategyStatus::LivRMDoor.x, 0);
    TCoordinate Dinfacefront = aVector(70, StrategyStatus::DinRMDoor.y);
    TCoordinate Dinface = aVector(0, StrategyStatus::DinRMDoor.y);
    TCoordinate Bedfacefront = aVector(StrategyStatus::BedRMDoor.x + 60, StrategyStatus::BedRMDoor.y);
    TCoordinate Bedface = aVector(400, 500);
    TCoordinate Libfacefront = aVector(StrategyStatus::LibDoor.x - 50, StrategyStatus::LibDoor.y);
    TCoordinate Libface = aVector(400, 160);
    //------------------------------
    if( StrategyStatus::FlagRoomRenew == true )
    {
        ActiveState  = etIdle;
        GotoRoomStep = 0;
        Room.SKSRoomState = etSKSMoving;
        FlagSetInitialData = false;
        Past_RoomCnt = Room.SortList[Room.Cnt];
        StrategyStatus::FlagRoomRenew = false;
    }
    //---------------- by yao 2012/08/28-----------------------------------------------
    if( Room.SortList[Room.Cnt] == etLivRM)  //客廳
    {
        switch( GotoRoomStep )
        {
		break;
        case 0://到房間門口
            ActiveState = etAStar;
            if( !FlagSetInitialData )
            	SetAStar( StrategyStatus::LivRMDoor );
        break;
        case 1://車頭面向房間
            ActiveState =  etTurnToAngle;
            GoalAngle = ( StrategyStatus::LivRMCen - StrategyStatus::LivRMDoor ).Angle();
            break;
        case 2:
            ActiveState = etAStar;
            if( !FlagSetInitialData )
            	SetAStar( Livfacefront );
            break;
        case 3:
            ActiveState = etTurnToAngle;
            GoalAngle = (Livfacefront - Livface).Angle();
        case 4:
	    	if(StrategyStatus::FlagMember == true){
	        	EncounterPeople();
            }else{
                GotoRoomStep++;
            }
            break;
        case 5:
            if(LinuxActionScript::GetPlayable() == 0)
                FlagTaskFinish = false;
            else
                FlagTaskFinish = true;
            break;
        case 6: 
            ActiveState = etTurnToAngle;
            GoalAngle = (StrategyStatus::LivRMDoor - LivTmp).Angle();
            break;
        default:
            ActiveState = etIdle;
            Room.Cnt++; 
            GotoRoomStep = 0;
            Room.SKSRoomState = etSKSMoving;
            FlagSetInitialData = false;
        break;
        }
    }
    else if( Room.SortList[Room.Cnt] == etDinRM || 
			Room.SortList[Room.Cnt] == etLib || 
			Room.SortList[Room.Cnt] == etBedRM )
    {
        
        switch( GotoRoomStep )
        {
        	case 0://到房間門口
            	ActiveState = etAStar;
            	if( !FlagSetInitialData )
               		if(Room.SortList[Room.Cnt] == etDinRM)
                    	SetAStar( StrategyStatus::DinRMDoor );
                	else if(Room.SortList[Room.Cnt] == etBedRM)
                    	SetAStar( StrategyStatus::BedRMDoor );
               		else if(Room.SortList[Room.Cnt] == etLib)
                    	SetAStar( StrategyStatus::LibDoor );
            break;
            case 1:
            	MakeSound();
                break;
            case 2:
                if(LinuxActionScript::GetPlayable() == 0)
                    FlagTaskFinish = false;
                else
                    FlagTaskFinish = true;
                break;
            case 3:
                ActiveState = etTurnToAngle;
                if(Room.SortList[Room.Cnt] == etDinRM)
                	GoalAngle = (StrategyStatus::DinRMCen -StrategyStatus::DinRMDoor).Angle();
            	else if(Room.SortList[Room.Cnt] == etBedRM)
                    GoalAngle = (StrategyStatus::BedRMCen - StrategyStatus::BedRMDoor).Angle();
            	else if(Room.SortList[Room.Cnt] == etLib)
            		GoalAngle = (StrategyStatus::LibCen -StrategyStatus::LibDoor).Angle();
                break;
			case 4:
				if(Room.SortList[Room.Cnt] == etDinRM){
					for(;MemberIndex<MEMBERS-1;MemberIndex++) {
						if(Members[MemberIndex].Room == etDinRM) {
							ActiveState = etAStar;
            				if( !FlagSetInitialData )			
								SetAStar(Members[MemberIndex].FrontPosition);
						}
					}
				}
				else if(Room.SortList[Room.Cnt] == etBedRM){
					for(;MemberIndex<MEMBERS-1;MemberIndex++) {
						if(Members[MemberIndex].Room == etBedRM) {
							ActiveState = etAStar;
            				if( !FlagSetInitialData )
								SetAStar(Members[MemberIndex].FrontPosition);
						}
					}
				}
				else if(Room.SortList[Room.Cnt] == etLib){
					for(;MemberIndex<MEMBERS-1;MemberIndex++) {
						if(Members[MemberIndex].Room == etLib) {
						ActiveState = etAStar;
            				if( !FlagSetInitialData )		
								SetAStar(Members[MemberIndex].FrontPosition);
						}
					}
				}	
			break;
			case 5:
				if(Room.SortList[Room.Cnt] == etDinRM){
					for(;MemberIndex<MEMBERS-1;MemberIndex++) {
						if(Members[MemberIndex].Room == etDinRM) {
							ActiveState = etTurnToAngle;
							GoalAngle = ( Members[MemberIndex].MemberPosition - Members[MemberIndex].FrontPosition ).Angle();
            if(StrategyStatus::FlagMember == true) EncounterPeople();

						}
					}
				}
				else if(Room.SortList[Room.Cnt] == etBedRM){
					for(;MemberIndex<MEMBERS-1;MemberIndex++) {
						if(Members[MemberIndex].Room == etBedRM) {
							ActiveState = etTurnToAngle;
							GoalAngle = ( Members[MemberIndex].MemberPosition - Members[MemberIndex].FrontPosition ).Angle();
                                        if(StrategyStatus::FlagMember == true) EncounterPeople();

						}
					}
				}
				else if(Room.SortList[Room.Cnt] == etLib){
					for(int i=0;MemberIndex<MEMBERS-1;i++) {
						if(Members[i].Room == etLib) {
							ActiveState = etTurnToAngle;
							GoalAngle = ( Members[MemberIndex].MemberPosition - Members[MemberIndex].FrontPosition ).Angle();
            if(StrategyStatus::FlagMember == true) EncounterPeople();

						}
					}
				}
			break;
			case 6:
               if(LinuxActionScript::GetPlayable() == 0)
               FlagTaskFinish = false;
           else
               FlagTaskFinish = true;
           break;			break;
            case 7:
                ActiveState = etTurnToAngle;
                if(Room.SortList[Room.Cnt] == etDinRM)
                	GoalAngle = (StrategyStatus::DinRMDoor - DinTmp ).Angle();
            	else if(Room.SortList[Room.Cnt] == etBedRM)
            		GoalAngle = (StrategyStatus::BedRMDoor - BedTmp).Angle();
                else if(Room.SortList[Room.Cnt] == etLib);
            		GoalAngle = (StrategyStatus::LibDoor - LibTmp).Angle();
            break;
            default:
                ActiveState = etIdle;
                Room.Cnt++;
                GotoRoomStep = 0;
                Room.SKSRoomState = etSKSMoving;
                FlagSetInitialData = false;
            break;
        }
    }
    else if(Room.Cnt == 4 )  //充電區
    { 
        switch( GotoRoomStep )
        {
        case 0: // 到房間門口
			ActiveState =  etAStar;
            if( !FlagSetInitialData )
            {
                SetAStar( StrategyStatus::ChrgDoor );
            }
            break;
       case 1: // 車頭面向房間
            ActiveState =  etTurnToAngle;
            GoalAngle = (StrategyStatus::ChrgCen -StrategyStatus::ChrgDoor).Angle();
        break;
       case 2: // 碰擊充電開關 by yao
            ActiveState = etTouchButton;
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
        break;
        default:
            ActiveState = etIdle;
            Room.Cnt++;
            GotoRoomStep = 0;
            Room.SKSRoomState = etSKSMoving;
            FlagSetInitialData = false;
        break;
        }
    }
    else  //待機區
    {
        ActiveState =  etAStar;
        if( !FlagSetInitialData ) SetAStar( StrategyStatus::EndPosition );
        if( (StrategyStatus::EndPosition - LocationStatus::Position).Length() < 50 )
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
        FlagTaskFinish = false;
    }
    //-----------------------------------------------------------------------
    
	if( FlagTaskFinish == true )
    {
        GotoRoomStep++;
        FlagTaskFinish = false;
        FlagSetInitialData = false;
    }
    else
        ActiveFunction();
}
//---------------------------------------------------------------------------
void Stra_Task::ActiveFunction()
{
    switch( ActiveState )
    {
	case etAStar :
        if( StrategyStatus::AStarPath.Status != StrategyStatus::etAchieve )
        {
            StrategyStatus::AStarPath.StartPos = StartPos;
            StrategyStatus::AStarPath.GoalPos  = GoalPos;
        }
        else {
            FlagTaskFinish = true;
        }
        break;
        //----------------------
    case etTurnToAngle :
        FlagTaskFinish = TurnToAngle( GoalAngle );
        break;
        //----------------------
    case etMotionToPos :
        FlagTaskFinish = MotionToPosition( GoalPos );
        break;
        //----------------------
    case etTouchButton:
        FlagTaskFinish = TouchButton();
        break;
    default:
        break;
    }
}
//---------------------------------------------------------------------------
void Stra_Task::WaitCatchball()
{
    ActiveState = etIdle;
    if( Room.SKSRoomState == etCatchFinish )
    {
        GotoRoomStep++;
        Room.SKSRoomState = etSKSMoving;
        FlagSetInitialData = false;
    }
    else
    {
        Room.SKSRoomState = etSKSCatchBall;
    }
}
//---------------------------------------------------------------------------
void Stra_Task::SetAStar( TCoordinate  Goal )
{
    FlagSetInitialData = true;
    StartPos = LocationStatus::Position;

    GoalPos  = Goal;

    StrategyStatus::AStarPath.Status = StrategyStatus::etMotion;
}
//---------------------------------------------------------------------------
bool Stra_Task::MotionToPosition( TCoordinate  Goal )
{
    if( (Goal - LocationStatus::Position).Length()  > 10 )
    {
        StrategyStatus::Goal1 = (Goal - LocationStatus::Position) >>  LocationStatus::Handle;
        return false;
    }
    else{
        return true;
    }
}
//---------------------------------------------------------------------------
bool Stra_Task::TurnToAngle( float GoalAngle )
{
    StrategyStatus::Goal1 = aVector(0,0);
    float AngleError = NormalizeAngle(GoalAngle - LocationStatus::Handle);

    if( fabs(AngleError) > Def_AnglePrecision )
    {
        //-------CheckJun
        if( fabs(AngleError) > Def_MinTurnAngle ){
            StrategyStatus::w = NormalizeAngle(GoalAngle - LocationStatus::Handle);
            StrategyStatus::w *= -1;
        }
        else
        {
            StrategyStatus::w = ( AngleError > 0 ) ? Def_MinTurnAngle : -Def_MinTurnAngle;
            StrategyStatus::w *= -1;
        }
        return false;
    }
    else
    {
        StrategyStatus::w = 0;
        return true;
    }
}
bool Stra_Task::TouchButton()
{
    if( TouchCnt < 40)
    {
        StrategyStatus::Goal1 = aVector(120,0);
        StrategyStatus::FixSpeed = 70;
        TouchCnt++;
        return false;
    }
    else
    {
        TouchCnt = 0;
        return true;
    }
}
//---------------------------------------------------------------------------
void Stra_Task::MakeSound()
{   
	if( Room.SortList[Room.Cnt] == etDinRM )
	{
	    LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Restaurant.mp3");
        FlagTaskFinish = true;
    }	
	else if( Room.SortList[Room.Cnt] == etLib )
	{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Studyroom.mp3");
        FlagTaskFinish = true;
	}
	else if( Room.SortList[Room.Cnt] == etBedRM )
	{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Badroom.mp3");
        FlagTaskFinish = true;
	}		
}
//---------------------------------------------------------------------------
void Stra_Task::EncounterPeople()
{
    if(StrategyStatus::FamilyMember == VisionStatus::etGrandPa){
        LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Grandpa.mp3");
       // FlagTaskFinish = true;
    }else if(StrategyStatus::FamilyMember == VisionStatus::etGrandMa){
        LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Grandma.mp3");
       // FlagTaskFinish = true;
    }else if(StrategyStatus::FamilyMember == VisionStatus::etFather){
        LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Dad.mp3");
       // FlagTaskFinish = true;
    }else if(StrategyStatus::FamilyMember == VisionStatus::etMother){
        LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Mom.mp3");
       // FlagTaskFinish = true;
    }else if(StrategyStatus::FamilyMember == VisionStatus::etSon){
        LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Bro.mp3");
       // FlagTaskFinish = true;
    }else if(StrategyStatus::FamilyMember == VisionStatus::etdaughter){
        LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Sis.mp3");
       // FlagTaskFinish = true;
    }
}

