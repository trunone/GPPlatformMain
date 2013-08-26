/*
 *   Stra_Task_Stage2.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "Stra_Task_Stage2.h"
#include "VisionStatus.h"
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
        element->Attribute("LivRM", &LivRM);
        element->Attribute("DinRM", &DinRM);
        element->Attribute("Lib", &Lib);
        element->Attribute("BedRM", &BedRM);
    }
    return 0;
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::Initialize(void)
{
    FlagTaskFinish      = false;
    StrategyStatus::FlagMember	= false;
    FlagSetInitialData  = false;
    GotoRoomStep  = 0;
    ActiveState   = etIdle;
    DoorState     = false;
    Past_RoomCnt  = -1;
    TouchCnt      = 0;
    PastScanLineData = new int[24];
	//StrategyStatus::CatchBallMode = DXL::STOP;
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
        ActiveState  = etIdle;
        GotoRoomStep = 0;
        FlagSetInitialData = false;
        Past_RoomCnt = StrategyStatus::Room.Cnt;
        StrategyStatus::FlagRoomRenew = false;
    }
    //---------------- by yao 2012/08/28-----------------------------------------------
    if( TmpCnt == 0 )  //客廳
    {
        switch( GotoRoomStep )
        {		
        case 0://到房間門口
            ActiveState = etAStar;
            if( !FlagSetInitialData )
            	SetAStar( StrategyStatus::LivRMDoor );
            break;
		case 1:
			ReadBillBoard();
			break;
		/*case 2:
			ActiveState =  etTurnToAngle;
			GoalAngle = ( StrategyStatus::LivRMCen - StrategyStatus::LivRMDoor ).Angle();
			break;
        case 3: 
            ActiveState = etTurnToAngle;
            GoalAngle = (StrategyStatus::LivRMDoor - LivTmp).Angle();
            break;
		case 4:
			ActiveState = etAStar;
            if( !FlagSetInitialData )
            	SetAStar( aVector(305,255) );
		case 5:
			ActiveState = etTurnToAngle;
			GoalAngle = ( aVector( 585,255 )- LocationStatus::Position ).Angle();
			break;*/
		//==================================================================================================
		/*case 6:
			if( StrategyStatus::FlagEatBall == 1 ){
				if( VisionStatus::Flagdoor_red == true ){
					StrategyStatus::BluePos = aVector(VisionStatus::Blue_X,VisionStatus::Blue_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::BluePos- LocationStatus::Position ).Angle();
				}
				else if( VisionStatus::Flagdoor_green == true ){
					StrategyStatus::BluePos = aVector(VisionStatus::Blue_X,VisionStatus::Blue_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::BluePos- LocationStatus::Position ).Angle();
				}
				else if( VisionStatus::Flagdoor_blue == true ){
					StrategyStatus::RedPos = aVector(VisionStatus::Red_X,VisionStatus::Red_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::RedPos- LocationStatus::Position ).Angle();				
			}
			else	FlagTaskFinish = true;
			break;
		case 7:
			if( StrategyStatus::FlagEatBall == 1 ){
				if( VisionStatus::Flagdoor_red == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::BluePos);
				}
				else if( VisionStatus::Flagdoor_green == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::BluePos);
				}
				else if( VisionStatus::Flagdoor_blue == true )
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::RedPos);
				}
			}
			else	FlagTaskFinish = true;
			break;
		//==================================================================================================
		case 8:
			if( StrategyStatus::FlagEatBall == 1 ){
				if( VisionStatus::Flagdoor_red == true ){
					StrategyStatus::GreenPos = aVector(VisionStatus::Green_X,VisionStatus::Green_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::GreenPos- LocationStatus::Position ).Angle();
				}
				else if( VisionStatus::Flagdoor_green == true ){
					StrategyStatus::RedPos = aVector(VisionStatus::Red_X,VisionStatus::Red_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::RedPos- LocationStatus::Position ).Angle();
				}
				else if( VisionStatus::Flagdoor_blue == true ){
					StrategyStatus::GreenPos = aVector(VisionStatus::Green_X,VisionStatus::Green_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::GreenPos- LocationStatus::Position ).Angle();
				}
			}
			else	FlagTaskFinish = true;
			break;
		case 9:
			if( StrategyStatus::FlagEatBall == 1 ){
				if( VisionStatus::Flagdoor_red == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::GreenPos);
				}
				else if( VisionStatus::Flagdoor_green == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::RedPos);
				}
				else if( VisionStatus::Flagdoor_blue == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::GreenPos);
				}
			}
			else	FlagTaskFinish = true;
			break;
		//==================================================================================================
		case 10:
			if( StrategyStatus::FlagEatBall == 1 ){
				if( VisionStatus::Flagdoor_red == true ){
					StrategyStatus::RedPos = aVector(VisionStatus::Red_X,VisionStatus::Red_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::RedPos- LocationStatus::Position ).Angle();
				}
				else if( VisionStatus::Flagdoor_green == true ){
					StrategyStatus::GreenPos = aVector(VisionStatus::Green_X,VisionStatus::Green_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::GreenPos- LocationStatus::Position ).Angle();
				}
				else if( VisionStatus::Flagdoor_blue == true ){
					StrategyStatus::BluePos = aVector(VisionStatus::Blue_X,VisionStatus::Blue_Y);
					ActiveState = etTurnToAngle;
					GoalAngle = ( StrategyStatus::BluePos- LocationStatus::Position ).Angle();
				}
			}
			else	FlagTaskFinish = true;
			break;
		case 11:
			if( StrategyStatus::FlagEatBall == 1 ){
				if( VisionStatus::Flagdoor_red == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::RedPos);
				}
				else if( VisionStatus::Flagdoor_green == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::GreenPos);
				}
				else if( VisionStatus::Flagdoor_blue == true ){
					StrategyStatus::CatchBallMode = DXL::CATCH;
					ActiveState = etAStar;
					SetAStar(StrategyStatus::BluePos);
				}
			}
			else	FlagTaskFinish = true;
			break;*/
		//==================================================================================================
        default:
            ActiveState = etIdle;
            TmpCnt++; 
            GotoRoomStep = 0;
            FlagSetInitialData = false;
            break;
        }
    }
    else if( TmpCnt == 1 || TmpCnt == 2 || TmpCnt == 3 )
    {
        switch( GotoRoomStep )
        {
        case 0://到房間門口
            ActiveState = etAStar;
			if( !FlagSetInitialData )
            	if( VisionStatus::Flagdoor_red == true )
					SetAStar( StrategyStatus::DinRMDoor );
				else if( VisionStatus::Flagdoor_green == true )
					SetAStar( StrategyStatus::BedRMDoor );
				else if( VisionStatus::Flagdoor_blue == true )
					SetAStar( StrategyStatus::LibDoor );
            break;
		case 1:
			MakeSound();
			break;
        case 2:// 車頭面向房間 
        	ActiveState = etTurnToAngle;
            if( VisionStatus::Flagdoor_red == true )
                GoalAngle = (StrategyStatus::DinRMCen -StrategyStatus::DinRMDoor).Angle();
            else if( VisionStatus::Flagdoor_green == true )
                GoalAngle = (StrategyStatus::BedRMCen - StrategyStatus::BedRMDoor).Angle();
            else if( VisionStatus::Flagdoor_blue == true )
            		GoalAngle = (StrategyStatus::LibCen -StrategyStatus::LibDoor).Angle();
            break;
        /*case 3:
			if( StrategyStatus::FlagEatBall == 1 ){
				StrategyStatus::CatchBallMode = DXL::THROW;
			}
			else	FlagTaskFinish = true;
            break;
        case 4:
        	ActiveState = etAStar;
			if( !FlagSetInitialData )
            	if( VisionStatus::Flagdoor_red == true )
					SetAStar( StrategyStatus::DinRMAStarPos );
				else if( VisionStatus::Flagdoor_green == true )
					SetAStar( StrategyStatus::BedRMAStarPos );
				else if( VisionStatus::Flagdoor_blue == true )
					SetAStar( StrategyStatus::LibAStarPos );
			if( FlagThief == true ){
				ThiefEvent();
			}
			break;
		case 5:
			ActiveState = etTurnToAngle;
            if( VisionStatus::Flagdoor_red == true )
                GoalAngle = ( aVector(265,555) - LocationStatus::Position ).Angle();
            else if( VisionStatus::Flagdoor_green == true )
                GoalAngle = ( aVector(565,325) - LocationStatus::Position ).Angle();
            else if( VisionStatus::Flagdoor_blue == true )
            	GoalAngle = ( aVector(345,175) - LocationStatus::Position ).Angle();
			if( FlagThief == true ){
				ThiefEvent();
			}
			break;
		case 6:
			ActiveState = etTurnToAngle;
            if( VisionStatus::Flagdoor_red == true )
                GoalAngle = ( aVector(35,325) - LocationStatus::Position ).Angle();
            else if( VisionStatus::Flagdoor_green == true )
                GoalAngle = ( aVector(345,565) - LocationStatus::Position ).Angle();
            else if( VisionStatus::Flagdoor_blue == true )
            	GoalAngle = ( aVector(565,35) - LocationStatus::Position ).Angle();
			if( FlagThief == true ){
				ThiefEvent();
			}
			break;*/
        default:
            ActiveState = etIdle;
			//==============================================================================
			if( VisionStatus::Flagdoor_red == true && TmpCnt == 1){
				VisionStatus::Flagdoor_red = false;
				VisionStatus::Flagdoor_green = true ; 
			}
			else if( VisionStatus::Flagdoor_red == false && VisionStatus::Flagdoor_green == true ){
				VisionStatus::Flagdoor_green = false;
				VisionStatus::Flagdoor_blue = true;
			}
			//==============================================================================
			else if( VisionStatus::Flagdoor_green == true && TmpCnt == 1 ){
				VisionStatus::Flagdoor_green = false;
				VisionStatus::Flagdoor_blue = true;
			}
			else if( VisionStatus::Flagdoor_green == true == false && VisionStatus::Flagdoor_blue == true ){
				VisionStatus::Flagdoor_blue = false;
				VisionStatus::Flagdoor_red = true;
			}
			//==============================================================================
			else if( VisionStatus::Flagdoor_blue == true && TmpCnt == 1 ){
				VisionStatus::Flagdoor_blue = false;
				VisionStatus::Flagdoor_green = true;
			}
			else if( VisionStatus::Flagdoor_blue == true == false && VisionStatus::Flagdoor_green == true ){
				VisionStatus::Flagdoor_green = false;
				VisionStatus::Flagdoor_red = true;
			}
			//==============================================================================
            TmpCnt++;
            GotoRoomStep = 0;
            FlagSetInitialData = false;
            break;
        }
    }
    /*else  //待機區
    {
        ActiveState =  etAStar;
        if( !FlagSetInitialData ) SetAStar( StrategyStatus::EndPosition );
        if( (StrategyStatus::EndPosition - LocationStatus::Position).Length() < 50 )
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
        FlagTaskFinish = false;
    }*/
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
    case etTouchButton:
        FlagTaskFinish = TouchButton();
        break;
    case etBackward:
        FlagTaskFinish = Backward();
        break;
        //----------------------
    default:
        break;
    }
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::WaitCatchball()
{
    ActiveState = etIdle;
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
void Stra_Task_Stage2::ReadBillBoard()
{
	ActiveState = etIdle;
	if( VisionStatus::Flagdoor_red == true){
		GotoRoomStep++;
	}
	else if( VisionStatus::Flagdoor_blue == true){
		GotoRoomStep++;
	}
	else if( VisionStatus::Flagdoor_green == true){
		GotoRoomStep++;
	}
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::ThiefEvent()
{
	ActiveState = etIdle;
	LinuxActionScript::PlayMP3("../../../Data/mp3/Detect_Intruduer.mp3");
	FlagTaskFinish = true;
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
bool Stra_Task_Stage2::TouchButton()
{
    if( TouchCnt < 20 )
    {
        StrategyStatus::Goal1 = aVector(100,0);
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
bool Stra_Task_Stage2::Backward()
{
    if( TouchCnt < 30 )
    {
        StrategyStatus::FlagForward = false; // 開啟後退
        StrategyStatus::Goal1 =  aVector(-100,0);
        StrategyStatus::FixSpeed = 50;
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
void Stra_Task_Stage2::MakeSound()
{
	if( VisionStatus::Flagdoor_red == true && TmpCnt == 1 ){
		LinuxActionScript::PlayMP3("../../../Data/mp3/Enter_SuspiciousRoom.mp3");
		FlagTaskFinish = true;
	}
	else if( VisionStatus::Flagdoor_blue == true && TmpCnt == 1 ){
		LinuxActionScript::PlayMP3("../../../Data/mp3/Enter_SuspiciousRoom.mp3");
		FlagTaskFinish = true;
	}
	else if( VisionStatus::Flagdoor_green == true && TmpCnt == 1 ){
		LinuxActionScript::PlayMP3("../../../Data/mp3/Enter_SuspiciousRoom.mp3");
		FlagTaskFinish = true;
	}
	else if( VisionStatus::Flagdoor_red == true ){
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Restaurant.mp3");
		FlagTaskFinish = true;
	}
	else if( VisionStatus::Flagdoor_blue == true ){
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Studyroom.mp3");
		FlagTaskFinish = true;
	}
	else if( VisionStatus::Flagdoor_green == true ){
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Badroom.mp3");
		FlagTaskFinish = true;
	}
}
//---------------------------------------------------------------------------
void Stra_Task_Stage2::EncounterPeople()
{
    if(StrategyStatus::FamilyMember == VisionStatus::etGrandPa)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Grandpa.mp3");
    else if(StrategyStatus::FamilyMember == VisionStatus::etGrandMa)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Grandma.mp3");
    else if(StrategyStatus::FamilyMember == VisionStatus::etFather)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Dad.mp3");
    else if(StrategyStatus::FamilyMember == VisionStatus::etMother)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Mom.mp3");
    else if(StrategyStatus::FamilyMember == VisionStatus::etSon)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Bro.mp3");
    else if(StrategyStatus::FamilyMember == VisionStatus::etdaughter)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Sis.mp3");
}

