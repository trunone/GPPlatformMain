/*
 *   Task.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include "Status.h"
#include "Task.h"
#include "Vector.h"
#include <vector>
#define Def_AnglePrecision (5*M_PI/180.0)
#define Def_MinTurnAngle (20*M_PI/180.0)
#include "TCoordinate.h"
#pragma package(smart_init)
 
using namespace Robot;


Task* Task::m_UniqueInstance = new Task();

Task::Task()
{


    	FlagTaskFinish      = false;

    	FlagSetInitialData  = false;



    	GotoRoomStep  = 0;

    	ActiveState   = etIdle;

    	DoorState     = false;

    	Past_RoomCnt  = -1;

    	TouchCnt      = 0;



    	PastScanLineData = new int[24];



    	//Room3StartPos = aVector(195,245);

}

Task::~Task()
{
}

void Task::Initialize()
{
	FlagTaskFinish      = false;

  	FlagSetInitialData  = false;

  	GotoRoomStep  = 0;

	ActiveState   = etIdle;

  	DoorState     = false;

   	Past_RoomCnt  = -1;

   	TouchCnt      = 0;


}

void Task::Process()
{		
	while(StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Enable == false && StrategyStatus::Room.Cnt < 5 )

       		StrategyStatus::tsRoomInfo.Cnt++;

    	if( StrategyStatus::FlagRoomRenew == true)

    	{

        	ActiveState  = etIdle;

        	GotoRoomStep = 0;

		StrategyStatus::tsRoomInfo. SKSRoomState = etSKSMoving;

        	FlagSetInitialData = false;

        	Past_RoomCnt = StrategyStatus::Room.Cnt;

        	StrategyStatus::FlagRoomRenew = false;

    	}



    	//---------------- by yao 2012/08/28

    	int FB_EN = (StrategyStatus::FindBallEn % (int)pow(10, StrategyStatus::Room.Cnt+1))/ (int)pow(10, StrategyStatus::Room.Cnt);



    	if( StrategyStatus::Room.Cnt == StrategyStatus::LivRM )

    	{

        	if(!FB_EN) StrategyStatus::Room.Cnt++;



        	switch( GotoRoomStep )

        	{

        		case 0: //

            			ActiveState =  etAStar;

            			if( !FlagSetInitialData )

                			SetAStar( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door );

        		break;

       			case 1:

            			ActiveState =  etTurnToAngle;

            			GoalAngle = (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center -

                         			StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door).Angle();

        		break;

        		case 2:

           			WaitCatchball();

        		break;

        		default:

            			ActiveState = etIdle;

            			StrategyStatus::Room.Cnt++;

            			GotoRoomStep = 0;

            			StrategyStatus::Room.SKSRoomState = etSKSMoving;

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

        		case 0:

            			ActiveState =  etAStar;

            			//if( StrategyStatus::Room.Cnt == StrategyStatus::Lib &&

            			//    (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door - Info->LocInfo->Position).Length() < 150 )

            			//    StrategyStatus::FlagAvoidEnable = false; //Ãö³¬Á×»Ù

            			if( !FlagSetInitialData )

                			SetAStar( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door );

       			break;

        		case 1:

            			ActiveState =  etTurnToAngle;

            			GoalAngle = (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center -

                         			StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door).Angle();

        		break;

        		case 2:

           			 if( FB_EN ) WaitCatchball();

            			else{

                			GotoRoomStep++;

                			StrategyStatus::Room.SKSRoomState = etSKSMoving;

                			FlagSetInitialData = false;

            			}

       			break;

        		case 3:

            			ActiveState =  etAStar;

            			if( StrategyStatus::Room.Cnt == StrategyStatus::Lib && 
					(Info->EscapePosition - Info->LocInfo->Position).Length() < 150 )

                			Info->StraInfo->FlagAvoidEnable = false;

            			if( !FlagSetInitialData )

               				 SetAStar( Info->EscapePosition );

       			break;

        		default:

            			ActiveState = etIdle;

            			StrategyStatus::Room.Cnt++;

            			GotoRoomStep = 0;

            			StrategyStatus::Room.SKSRoomState = etSKSMoving;

            			FlagSetInitialData = false;

        		break;

        	}

    	}

    	else if( StrategyStatus::Room.Cnt == 4 )

    	{

        switch( GotoRoomStep )

        {

        case 0:

            ActiveState =  etAStar;

            if( !FlagSetInitialData )

                SetAStar( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door );

        break;

        case 1:

            ActiveState =  etTurnToAngle;

            GoalAngle = (StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center -

                         StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Door).Angle();

        break;

        case 2: 

            ActiveState = etTouchButton;

            StrategyStatus::FlagAvoidEnable = false;

            //GoalPos = StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Center;

        break;

        case 3:

            ActiveState = etBackward;

            StrategyStatus::FlagAvoidEnable = false;

        break;

        default:

            ActiveState = etIdle;

            StrategyStatus::Room.Cnt++;

            GotoRoomStep = 0;

            StrategyStatus::Room.SKSRoomState = etSKSMoving;

            FlagSetInitialData = false;

        break;

        }

    }

    else                                      

    {

        ActiveState =  etAStar;

        if( !FlagSetInitialData ) SetAStar( Info->EndPosition );

        if( (Info->EndPosition - Info->LocInfo->Position).Length() < 50 )

            StrategyStatus::FlagAvoidEnable = false; //Ãö³¬Á×»Ù

        this->FlagTaskFinish = false;

    }
	if( this->FlagTaskFinish )

    {

        GotoRoomStep++; FlagTaskFinish = false; FlagSetInitialData = false;

    }

    else

        ActiveFunction();



    	//---

    	return Caption;

	}

//---------------------------------------------------------------------------

void Task::ActiveFunction()

{

    switch( ActiveState )

    {

    //----------------------

    case etAStar :

            if(StrategyStatus::AStarPath.Status != etAchieve )

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

    //----------------------

    /*case etWaitDoorOpen:

        FlagTaskFinish = WaitDoorOpen();

    break; //*/

    //----------------------

    default:

    break;

    }

}

//---------------------------------------------------------------------------

void Task::WaitCatchball()

{

    ActiveState = etIdle;

    if( StrategyStatus::Room.SKSRoomState == etCatchFinish )

    { //*/

        //StrategyStatus::Room.Cnt++;

        //GotoRoomStep = 0;

        GotoRoomStep++;

        StrategyStatus::Room.SKSRoomState = etSKSMoving;

        FlagSetInitialData = false;

    }

    else

    {

        StrategyStatus::Room.SKSRoomState = etSKSCatchBall;

    }  //*/

}

//---------------------------------------------------------------------------

void Task::SetAStar( TCoordinate  Goal )

{

    FlagSetInitialData = true;

    StartPos = Info->LocInfo->Position;

    GoalPos  = Goal;



    StrategyStatus::AStarPath.Status = etMotion;

}

//---------------------------------------------------------------------------

bool Task::MotionToPosition( Vector3D  Goal )

{

    if( (Goal - Info->LocInfo->Position).Length()  > 10 )

    {

        StrategyStatus::Goal1 = (Goal - Info->LocInfo->Position) >>  Info->LocInfo->Handle;

        return false;

    }

    else{ return true; }

}

//---------------------------------------------------------------------------

bool Task::TurnToAngle( float GoalAngle )

{

    StrategyStatus::Goal1 = aVector(0,0);

    float AngleError = NormalizeAngle(GoalAngle - Info->LocInfo->Handle);



    if( fabs(AngleError) > Def_AnglePrecision )

    {

        //-------CheckJun

        if( fabs(AngleError) > Def_MinTurnAngle )

            StrategyStatus::Direction = NormalizeAngle(GoalAngle - Info->LocInfo->Handle);

            //¥¿³W€Æ

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

bool Task::SpecialMove( int Forward )

{

    Info->LocInfo->FlagEvaluatuonEnable = false;

    //-------CheckJun

    if( Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[34] &&

        Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[36] )



    {

        StrategyStatus::Goal1 = aVector(Info->HdwInfo->LaserInfo.ScanArray[35] - Forward,0);

    }

    else if( Info->HdwInfo->LaserInfo.ScanArray[36] > Info->HdwInfo->LaserInfo.ScanArray[34] )

    //36>34

    {

        Info->StraInfo->Goal1 = aVector(Info->HdwInfo->LaserInfo.ScanArray[35] - Forward,0) << -Def_AnglePrecision;

    }

    else if(Info->HdwInfo->LaserInfo.ScanArray[36] < Info->HdwInfo->LaserInfo.ScanArray[34])

    //34>36

    {

        Info->StraInfo->Goal1 = aVector(Info->HdwInfo->LaserInfo.ScanArray[35] - Forward,0) << Def_AnglePrecision;

    }



    if( fabs(Info->HdwInfo->LaserInfo.ScanArray[35] - Past_LaserData ) > 50)



            DoorState = !DoorState;

    Past_LaserData = Info->HdwInfo->LaserInfo.ScanArray[35];



    return ( Info->HdwInfo->LaserInfo.ScanArray[35] - Forward <= 0 )?true:false;



}

//---------------------------------------------------------------------------

bool Task::SpecialTurn()

{

    Info->LocInfo->FlagEvaluatuonEnable = false;

    if( Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[34] &&

        Info->HdwInfo->LaserInfo.ScanArray[35] <= Info->HdwInfo->LaserInfo.ScanArray[36] )



    {

        return true;

    }

    else if( Info->HdwInfo->LaserInfo.ScanArray[36] > Info->HdwInfo->LaserInfo.ScanArray[34] )

    {

        StrategyStatus::Direction = -M_PI_4/2.0;        //36>34

    }

    else if(Info->HdwInfo->LaserInfo.ScanArray[36] < Info->HdwInfo->LaserInfo.ScanArray[34])

    {

        StrategyStatus::Direction = M_PI_4/2.0; //34>36

    }

    return false;

}

//---------------------------------------------------------------------------

bool Task::TouchButton()

{

    if( TouchCnt < 20 )

    {

        StrategyStatus::Goal1 = aVector(100,0);

        StrategyStatus::FixSpeed = 70;

        TouchCnt++;

        return false;

    }

    else        //( TouchCnt >= 20 )

    {

        TouchCnt = 0;

        return true;

    }

}

//---------------------------------------------------------------------------

bool Task::Backward()

{

    if( TouchCnt < 30 )

    {

        StrategyStatus::FlagForward = false; 

        StrategyStatus::Goal1 = aVector(-100,0);

        StrategyStatus::FixSpeed = 50;

        TouchCnt++;

        return false;

    }

    else        //(TouchCnt >= 30)

    {

        TouchCnt = 0;

        return true;

    }



}
