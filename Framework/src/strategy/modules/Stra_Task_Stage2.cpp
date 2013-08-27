/*
 *   Stra_Task_Stage2.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "Stra_Task_Stage2.h"
#include "VisionStatus.h"
#include "StrategyStatus.h"
#include "LinuxActionScript.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "DXL.h"

using namespace Robot;
using namespace std;

Stra_Task_Stage2* Stra_Task_Stage2::m_UniqueInstance = new Stra_Task_Stage2();

Stra_Task_Stage2::Stra_Task_Stage2()
{

}

Stra_Task_Stage2::~Stra_Task_Stage2()
{

}
//---------------------------------------------------------------------------xml
int Stra_Task_Stage2::LoadXMLSettings (TiXmlElement* element) {
    if(element != NULL) {
        TiXmlElement *child = element->FirstChildElement();
         for(int i = 0; child != NULL; i++) {
            string tmp;
            child->Attribute("x", &Members[i].FrontPosition.x);
            child->Attribute("y", &Members[i].FrontPosition.y);
            child->Attribute("Member_x", &Members[i].MemberPosition.x);
            child->Attribute("Member_y", &Members[i].MemberPosition.y);
            child->QueryStringAttribute("room", &tmp);
            if(!strcmp("DinRM", tmp.c_str()))
                Members[i].Room = etDinRM;
            if(!strcmp("Lib", tmp.c_str()))
                Members[i].Room = etLib;
            if(!strcmp("BedRM", tmp.c_str()))
                Members[i].Room = etBedRM;
            child = child->NextSiblingElement();
        }
   }
    return 0;
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::Initialize(void)
{
    FlagTaskFinish      = false;
    FlagSetInitialData  = false;
    GotoRoomStep  = 0;
    Room.SortList[0] = etLivRM;
    Room.Cnt = 0;
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::Process(void)
{
    TCoordinate LivTmp = aVector(StrategyStatus::LivRMDoor.x, 0);
    TCoordinate DinTmp = aVector(0, StrategyStatus::DinRMDoor.y);
    TCoordinate BedTmp = aVector(0, StrategyStatus::BedRMDoor.y);
    TCoordinate LibTmp = aVector(0, StrategyStatus::LibDoor.y);

    if( StrategyStatus::FlagRoomRenew == true )
    {
        GotoRoomStep = 0;
        FlagSetInitialData = false;
        StrategyStatus::FlagRoomRenew = false;
    }
    if(Room.SortList[Room.Cnt] == etLivRM){//客廳
        switch( GotoRoomStep )
        {
        case 0://到房間門口
            ActiveState = etAStar;
            if( !FlagSetInitialData )
                SetAStar( StrategyStatus::LivRMDoor );
            break;
        case 1:
        	ActiveState = etTurnToAngle;
            GoalAngle = (StrategyStatus::LibCen - StrategyStatus::LivRMDoor).Angle();
            break;
        case 2:
            ActiveState = etIdle;
            if(!FlagSetInitialData){
                VisionStatus::doordet_enable = true;
                FlagSetInitialData = true;
            }else{
                if(!VisionStatus::doordet_enable)
                    FlagTaskFinish = true;
            }
            break;
        case 3:
            ActiveState = etIdle;
            if(!ReadBillBoard())
                FlagTaskFinish = true;
            break;
        default:
            ActiveState = etIdle;
            Room.Cnt++; 
            GotoRoomStep = 0;
            FlagSetInitialData = false;
            break;
        }
    }
    else if(Room.SortList[Room.Cnt] == etDinRM || Room.SortList[Room.Cnt] == etBedRM ||Room.SortList[Room.Cnt] == etLib )
    {  
        switch( GotoRoomStep )
        {
        case 0://到房間門口
            ActiveState = etAStar;
			if( !FlagSetInitialData ) {
            	if(Room.SortList[Room.Cnt] == etDinRM )
					SetAStar( StrategyStatus::DinRMDoor );
				else if( Room.SortList[Room.Cnt] == etBedRM )
					SetAStar( StrategyStatus::BedRMDoor );
				else if( Room.SortList[Room.Cnt] == etLib )
					SetAStar( StrategyStatus::LibDoor );
            }
            break;
		case 1:
            ActiveState = etIdle;
			MakeSound();
            FlagTaskFinish = true;
			break;
        case 2:
            ActiveState = etIdle;
            if(LinuxActionScript::GetPlayable() == 0)
                FlagTaskFinish = false;
            else
                FlagTaskFinish = true;
            break;
        case 3:// 車頭面向房間 
        	ActiveState = etTurnToAngle;
            if( Room.SortList[Room.Cnt] == etDinRM )
                GoalAngle = (StrategyStatus::DinRMCen -StrategyStatus::DinRMDoor).Angle();
            else if( Room.SortList[Room.Cnt] == etBedRM)
                GoalAngle = (StrategyStatus::BedRMCen - StrategyStatus::BedRMDoor).Angle();
            else if( Room.SortList[Room.Cnt] == etLib)
            		GoalAngle = (StrategyStatus::LibCen -StrategyStatus::LibDoor).Angle();
            break;
        case 4:
            if(Room.SortList[Room.Cnt] == etDinRM) {
                if(Members[0].Room == etDinRM) {
                    ActiveState = etAStar;
                    if( !FlagSetInitialData )
                        SetAStar(Members[0].FrontPosition);
                }
            }
            else if(Room.SortList[Room.Cnt] == etBedRM) {
                if(Members[0].Room == etBedRM) {
                    ActiveState = etAStar;
                    if( !FlagSetInitialData )
                        SetAStar(Members[0].FrontPosition);
                }
            }
            else if(Room.SortList[Room.Cnt] == etLib) {
                if(Members[0].Room == etLib) {
                    ActiveState = etAStar;
                    if( !FlagSetInitialData )
                        SetAStar(Members[0].FrontPosition);
                }
            }
            break;
        case 5:
            if(Room.SortList[Room.Cnt] == etDinRM) {
                if(Members[0].Room == etDinRM) {
                    ActiveState = etTurnToAngle;
                    GoalAngle = ( Members[0].MemberPosition - Members[0].FrontPosition ).Angle();
                    //if(StrategyStatus::FlagThief == true)
                    VisionStatus::sendimg_enable = true;
                    ThiefEvent();
                }
            }
            else if(Room.SortList[Room.Cnt] == etBedRM) {
                if(Members[0].Room == etBedRM) {
                    ActiveState = etTurnToAngle;
                    GoalAngle = ( Members[0].MemberPosition - Members[0].FrontPosition ).Angle();
                    //if(StrategyStatus::FlagThief == true)
                    VisionStatus::sendimg_enable = true;
                    ThiefEvent();
                }
            }
            else if(Room.SortList[Room.Cnt] == etLib) {
                if(Members[0].Room == etLib) {
                    ActiveState = etTurnToAngle;
                    GoalAngle = ( Members[0].MemberPosition - Members[0].FrontPosition ).Angle();
                    //if(StrategyStatus::FlagThief == true)
                    VisionStatus::sendimg_enable = true;
                    ThiefEvent();
                }
            }
            break;
        case 6:
            ActiveState = etIdle;
            if(LinuxActionScript::GetPlayable() == 0)
                FlagTaskFinish = false;
            else
                FlagTaskFinish = true;
            break;
        case 7:
            ActiveState = etTurnToAngle;
            if(Room.SortList[Room.Cnt] == etDinRM)
                GoalAngle = (StrategyStatus::DinRMDoor - DinTmp ).Angle();
            else if(Room.SortList[Room.Cnt] == etBedRM)
                GoalAngle = (StrategyStatus::BedRMDoor - BedTmp).Angle();
            else if(Room.SortList[Room.Cnt] == etLib);
            GoalAngle = (StrategyStatus::LibDoor - LibTmp).Angle();
            break;

   //     case 3:
   // 		if( StrategyStatus::FlagEatBall == 1 ){
   // 			StrategyStatus::CatchBallMode = DXL::THROW;
   // 		}
   // 		else	FlagTaskFinish = true;
   //         break;
    //    case 4:
    //    	ActiveState = etAStar;
	//		if( !FlagSetInitialData )
    //        	if( VisionStatus::Flagdoor_red == true )
	//				SetAStar( StrategyStatus::DinRMAStarPos );
	//			else if( VisionStatus::Flagdoor_green == true )
	//				SetAStar( StrategyStatus::BedRMAStarPos );
	//			else if( VisionStatus::Flagdoor_blue == true )
	//				SetAStar( StrategyStatus::LibAStarPos );
	//		if( StrategyStatus::FlagThief == true ){
	//			ThiefEvent();
	//		}
	//		break;
	//	case 5:
	//		ActiveState = etTurnToAngle;
    //        if( VisionStatus::Flagdoor_red == true )
    //            GoalAngle = ( aVector(265,555) - LocationStatus::Position ).Angle();
    //        else if( VisionStatus::Flagdoor_green == true )
    //            GoalAngle = ( aVector(565,325) - LocationStatus::Position ).Angle();
    //        else if( VisionStatus::Flagdoor_blue == true )
    //        	GoalAngle = ( aVector(345,175) - LocationStatus::Position ).Angle();
	//		if( StrategyStatus::FlagThief == true ){
	//			ThiefEvent();
	//		}
	//		break;
	//	case 6:
	//		ActiveState = etTurnToAngle;
    //        if( VisionStatus::Flagdoor_red == true )
    //            GoalAngle = ( aVector(35,325) - LocationStatus::Position ).Angle();
    //        else if( VisionStatus::Flagdoor_green == true )
    //            GoalAngle = ( aVector(345,565) - LocationStatus::Position ).Angle();
    //        else if( VisionStatus::Flagdoor_blue == true )
    //        	GoalAngle = ( aVector(565,35) - LocationStatus::Position ).Angle();
	//		if( StrategyStatus::FlagThief == true ){
	//			ThiefEvent();
	//		}
	//		break;*/
        default:
            ActiveState = etIdle;
            GotoRoomStep = 0;
            FlagSetInitialData = false;
            Room.Cnt++;
            break;
        }
    }
   else   //待機區
    {
        ActiveState =  etAStar;
        if( !FlagSetInitialData )
            SetAStar( StrategyStatus::EndPosition );
        if( (StrategyStatus::EndPosition - LocationStatus::Position).Length() < 50 )
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
    } 
    
	if( FlagTaskFinish == true && Room.Cnt != 4)
    {
        GotoRoomStep++;
        FlagTaskFinish = false;
        FlagSetInitialData = false;
    }
    else
        ActiveFunction();
}

void Stra_Task_Stage2::ActiveFunction()
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
    default:
        break;
    }
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::WaitCatchball()
{
    if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etCatchFinish )
    {   //*/
        //Info->StraInfo->Room.Cnt++;
        //GotoRoomStep = 0;
        GotoRoomStep++;
        StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        FlagSetInitialData = false;
    }
    else
    {
        StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSCatchBall;
    }
}
//---------------------------------------------------------------------------
int Stra_Task_Stage2::ReadBillBoard()
{
	if( VisionStatus::Flagdoor_green == true){
        if(StrategyStatus::EndPosition.x <= 300){
            Room.SortList[1] = etBedRM;   
            Room.SortList[2] = etLib;    
            Room.SortList[3] = etDinRM;    
        }else{
            Room.SortList[1] = etBedRM;   
            Room.SortList[2] = etDinRM;    
            Room.SortList[3] = etLib;    
        }
        return 0; 
    }else if( VisionStatus::Flagdoor_red == true){
        Room.SortList[1] = etDinRM;   
        Room.SortList[2] = etBedRM;    
        Room.SortList[3] = etLib;    
        return 0; 	
	}else if( VisionStatus::Flagdoor_blue == true){
        Room.SortList[1] = etLib;   
        Room.SortList[2] = etBedRM;    
        Room.SortList[3] = etDinRM;    
        return 0; 
    }else{
        return 1; 
    }
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::ThiefEvent()
{
	LinuxActionScript::PlayMP3("../../../Data/mp3/Detect_Intruduer.mp3");
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::SetAStar( TCoordinate  Goal )
{
    FlagSetInitialData = true;
    StartPos = LocationStatus::Position;

    GoalPos  = Goal;

    StrategyStatus::AStarPath.Status = StrategyStatus::etMotion;
}
//---------------------------------------------------------------------------
bool Stra_Task_Stage2::MotionToPosition( TCoordinate  Goal )
{
    if( (Goal - LocationStatus::Position).Length()  > 10 )
    {
        StrategyStatus::Goal1 = (Goal - LocationStatus::Position) >>  LocationStatus::Handle;
        return false;
    }
    else {
        return true;
    }
}
//---------------------------------------------------------------------------
bool Stra_Task_Stage2::TurnToAngle( float GoalAngle )
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
//---------------------------------------------------------------------------
void Stra_Task_Stage2::MakeSound()
{
	if(Room.SortList[Room.Cnt] == etDinRM ){
        if(Room.Cnt == 1){
            LinuxActionScript::PlayMP3("../../../Data/mp3/Enter_SuspiciousRoom.mp3");
        }else{
            LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Restaurant.mp3");
        }
	}
	else if( Room.SortList[Room.Cnt] == etLib ){
        if(Room.Cnt == 1){
            LinuxActionScript::PlayMP3("../../../Data/mp3/Enter_SuspiciousRoom.mp3");
        }else{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Studyroom.mp3");
        }
	}
	else if( Room.SortList[Room.Cnt] == etBedRM ){
        if(Room.Cnt == 1){
            LinuxActionScript::PlayMP3("../../../Data/mp3/Enter_SuspiciousRoom.mp3");
        }else{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Badroom.mp3");
	    }
    }
}

