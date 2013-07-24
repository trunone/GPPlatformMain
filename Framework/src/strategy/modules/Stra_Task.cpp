/*
 *   Stra_Task.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "Stra_Task.h"

// for use M_PI
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Robot;
using namespace std;

Stra_Task* Stra_Task::m_UniqueInstance = new Stra_Task();

<<<<<<< HEAD
=======
Stra_Task* Stra_Task::m_UniqueInstance = new Stra_Task();

>>>>>>> 2db1a3b5fcb82021d9f615b816cd29cb494acc0b
Stra_Task::Stra_Task()
{

}

Stra_Task::~Stra_Task()
{

}

//---------------------------------------------------------------------------
void Stra_Task::Initialize(void)
{
   // string str_ = this->Caption +" Initial";

    FlagTaskFinish      = false;
    FlagSetInitialData  = false;
    GotoRoomStep  = 0;
    ActiveState   = etIdle;
    DoorState     = false;
    Past_RoomCnt  = -1;
    TouchCnt      = 0;
    PastScanLineData = new int[24];
   // return str_;
}
//---------------------------------------------------------------------------
void Stra_Task::Process(void)
{
    //if( this->bNewParameter ) this->ParameterReset();
    //---


    while( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Enable == false && StrategyStatus::Room.Cnt < 5 )
        StrategyStatus::Room.Cnt++;

    if( StrategyStatus::FlagRoomRenew == true)
    {
        ActiveState  = etIdle;
        GotoRoomStep = 0;
        StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        FlagSetInitialData = false;
        Past_RoomCnt = StrategyStatus::Room.Cnt;
        StrategyStatus::FlagRoomRenew = false;
    }

    //---------------- by yao 2012/08/28
    //int FB_EN = (Info->StraInfo->FindBallEn % (int)pow(10, Info->StraInfo->Room.Cnt+1))/ (int)pow(10, Info->StraInfo->Room.Cnt);

    if( StrategyStatus::Room.Cnt == StrategyStatus::LivRM )  //客廳
    {
        //if(StrategyStatus::ThiefRoom != StrategyStatus::LivRM) StrategyStatus::Room.Cnt++;

        switch( GotoRoomStep )
        {
        case 0: // 到房間門口
            ActiveState =  etAStar;
            if( !FlagSetInitialData )
               SetAStar( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door );
        break;
        case 1: // 車頭面向房間
            ActiveState =  etTurnToAngle;
            GoalAngle = (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center -
                         StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door).Angle();
        break;
        case 2:
	    ActiveState = etMakeSoundMove;
	    MakeSound();
	    WaitCatchball();  
        break;
        default:
            ActiveState = etIdle;
            StrategyStatus::Room.Cnt++;
            GotoRoomStep = 0;
            StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
            FlagSetInitialData = false;
        break;
        }
    }
    else if( StrategyStatus::Room.Cnt == StrategyStatus::DinRM ||
        StrategyStatus::Room.Cnt == StrategyStatus::Lib ||
        StrategyStatus::Room.Cnt == StrategyStatus::BedRM )
    {
        switch( GotoRoomStep )
        {
        case 0: // 到房間門口
            ActiveState =  etAStar;
            if( !FlagSetInitialData )
                SetAStar( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door );
        break;
        case 1: // 車頭面向房間
            ActiveState =  etTurnToAngle;
            GoalAngle = (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center -
                         StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door).Angle();
        break;
        case 2:
            //if( Info->StraInfo->Room.Cnt ==2 && Info->StraInfo->ThiefRoom ==2 ) 
		
		ActiveState = etMakeSoundMove;
		MakeSound();
	        WaitCatchball();
            //else{
                //GotoRoomStep++;
                //StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
                //FlagSetInitialData = false;
            //}
        break;
        case 3:
            ActiveState =  etAStar;
            if( StrategyStatus::Room.Cnt == StrategyStatus::Lib && (StrategyStatus::EscapePosition - 			LocationStatus::Position).Length() < 150 )
                StrategyStatus::FlagAvoidEnable = false; //關閉避障
            if( !FlagSetInitialData )
                SetAStar( StrategyStatus::EscapePosition );
        break;
        default:
            ActiveState = etIdle;
            StrategyStatus::Room.Cnt++;
            GotoRoomStep = 0;
            StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
            FlagSetInitialData = false;
        break;
        }
    }
    else if(StrategyStatus::Room.Cnt == 4 )  //充電區
    {
        switch( GotoRoomStep )
        {
        case 0: // 到房間門口
            ActiveState =  etAStar;
            if( !FlagSetInitialData )
                    SetAStar( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door );
        break;
        case 1: // 車頭面向房間
            ActiveState =  etTurnToAngle;
            GoalAngle = (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center -
                         StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door).Angle();
        break;
        case 2: // 碰擊充電開關 by yao
            ActiveState = etTouchButton;
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
        break;
        case 3:
            ActiveState = etBackward; // 倒車動作
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
        break;
        default:
            ActiveState = etIdle;
            StrategyStatus::Room.Cnt++;
            GotoRoomStep = 0;
            StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
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
        this->FlagTaskFinish = false;
    }

    //--------------------------
    if( this->FlagTaskFinish )
    {
        GotoRoomStep++;
	FlagTaskFinish = false;
	FlagSetInitialData = false;
    }
    else
        ActiveFunction();

    //---
   // return Caption;
    printf("Task done");
}
//---------------------------------------------------------------------------
void Stra_Task::ActiveFunction()
{
    switch( ActiveState )
    {
    //----------------------
    case etAStar :
            if(StrategyStatus::AStarPath.Status != StrategyStatus::etAchieve )
            {
                StrategyStatus::AStarPath.StartPos = StartPos;
                StrategyStatus::AStarPath.GoalPos  = GoalPos;
            }
            else{ FlagTaskFinish = true; }
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
    case etSpecialMove:
        FlagTaskFinish = SpecialMove( SpecialDistance );
    break;
    //----------------------
    case etSpecialTurn:
        FlagTaskFinish = SpecialTurn();
    break;
    //----------------------
    case etTouchButton:
        FlagTaskFinish = TouchButton();
    break;
    case etBackward:
        FlagTaskFinish = Backward();
    break;
    case etMakeSoundMove:
	FlagTaskFinish = MakeSoundMove();
    break;
    //----------------------
    //----------------------
    default:
    break;
    }
}
//---------------------------------------------------------------------------
void Stra_Task::WaitCatchball()
{
    ActiveState = etIdle;
    if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etCatchFinish )
    { //*/
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
    else{ return true; }
}
//---------------------------------------------------------------------------
bool Stra_Task::TurnToAngle( float GoalAngle )
{
    StrategyStatus::Goal1 = aVector(0,0);
    float AngleError = NormalizeAngle(GoalAngle - LocationStatus::Handle);

    if( fabs(AngleError) > Def_AnglePrecision )
    {
        //-------CheckJun
        if( fabs(AngleError) > Def_MinTurnAngle )
            StrategyStatus::Direction = NormalizeAngle(GoalAngle - LocationStatus::Handle);
        else
        {
            StrategyStatus::Direction = ( AngleError > 0 ) ? Def_MinTurnAngle : -Def_MinTurnAngle;
        }
        return false;
    }
    else
    {
        StrategyStatus::Direction = 0;
        return true;
    }
}
//---------------------------------------------------------------------------
bool Stra_Task::SpecialMove( int Forward )
{/*
    Info->LocInfo->FlagEvaluatuonEnable = false;
    //-------CheckJun
    if( Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[34] &&
        Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[36] )
    {
        Info->StraInfo->Goal1 = aVector(Info->HdwInfo->LaserInfo.ScanArray[35] - Forward,0);
    }
    else if( Info->HdwInfo->LaserInfo.ScanArray[36] > Info->HdwInfo->LaserInfo.ScanArray[34] )
    {
        Info->StraInfo->Goal1 = aVector(Info->HdwInfo->LaserInfo.ScanArray[35] - Forward,0) << -Def_AnglePrecision;
    }
    else if(Info->HdwInfo->LaserInfo.ScanArray[36] < Info->HdwInfo->LaserInfo.ScanArray[34])
    {
        Info->StraInfo->Goal1 = aVector(Info->HdwInfo->LaserInfo.ScanArray[35] - Forward,0) << Def_AnglePrecision;
    }

    if( fabs(Info->HdwInfo->LaserInfo.ScanArray[35] - Past_LaserData ) > 50)
            DoorState = !DoorState;
    Past_LaserData = Info->HdwInfo->LaserInfo.ScanArray[35];

    return ( Info->HdwInfo->LaserInfo.ScanArray[35] - Forward <= 0 )?true:false;
*/
}
//---------------------------------------------------------------------------
bool Stra_Task::SpecialTurn()
{/*
    Info->LocInfo->FlagEvaluatuonEnable = false;
    if( Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[34] &&
        Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[36] )
    {
        return true;
    }
    else if( Info->HdwInfo->LaserInfo.ScanArray[36] > Info->HdwInfo->LaserInfo.ScanArray[34] )
    {
        Info->StraInfo->Direction = -M_PI_4/2.0;
    }
    else if(Info->HdwInfo->LaserInfo.ScanArray[36] < Info->HdwInfo->LaserInfo.ScanArray[34])
    {
        Info->StraInfo->Direction = M_PI_4/2.0;
    }
    return false;
*/
}
//---------------------------------------------------------------------------
bool Stra_Task::TouchButton()
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
bool Stra_Task::Backward()
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
bool Stra_Task::MakeSoundMove()
{
	StrategyStatus::FlagForward = true;
	StrategyStatus::Goal1 =  aVector(100,0);
	StrategyStatus::FixSpeed = 25;
}
//---------------------------------------------------------------------------
void Stra_Task::MakeSound()
{
	if( StrategyStatus::Room.Cnt == StrategyStatus::LivRM )
	{
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound )
		{
        		GotoRoomStep++;
        		StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        		FlagSetInitialData = false;
    		}
    		else
    		{
        		StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound;
    		}
	}	
	else if( StrategyStatus::Room.Cnt == StrategyStatus::DinRM )
	{
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound )
		{
        		GotoRoomStep++;
        		StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        		FlagSetInitialData = false;
    		}
    		else
    		{
        		StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound;
    		}
	}
	else if( StrategyStatus::Room.Cnt == StrategyStatus::Lib )
	{
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound )
		{
        		GotoRoomStep++;
        		StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        		FlagSetInitialData = false;
    		}
    		else
    		{
        		StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound;
    		}
	}
	else if( StrategyStatus::Room.Cnt == StrategyStatus::BedRM )
	{
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound )
		{
        		GotoRoomStep++;
        		StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        		FlagSetInitialData = false;
    		}
    		else
    		{
        		StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSMakeSound;
    		}
	}		
}


