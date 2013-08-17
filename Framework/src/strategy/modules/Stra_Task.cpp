/*
 *   Stra_Task.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "Stra_Task.h"
#include "VisionStatus.h"
#include "LinuxActionScript.h"
// for use M_PI
#define _USE_MATH_DEFINES
#include <math.h>

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
int Stra_Task::LoadXMLSettings (TiXmlElement* element){
	if(element != NULL){
		element->Attribute("LivRM", &LivRM);
		element->Attribute("DinRM", &DinRM);
		element->Attribute("Lib", &Lib);
		element->Attribute("BedRM", &BedRM);						
	}
	return 0;
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
	for(int i=0;i<5;i++)
		StrategyStatus::Room.Info[i].Enable = true;
	GotoRoomStep = 0;
	StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
}
//---------------------------------------------------------------------------
void Stra_Task::Process(void)
{
	while( StrategyStatus::Room.Info[StrategyStatus::Room.Cnt].Enable == false && StrategyStatus::Room.Cnt < 5 ){
		StrategyStatus::Room.Cnt++;	
	}
	if( StrategyStatus::FlagRoomRenew == true )
    { 
        ActiveState  = etIdle;
        GotoRoomStep = 0;
        StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
        FlagSetInitialData = false;
        Past_RoomCnt = StrategyStatus::Room.Cnt;
        StrategyStatus::FlagRoomRenew = false;
    }
    //---------------- by yao 2012/08/28-----------------------------------------------
    //printf("room cnt %d\n",StrategyStatus::Room.Cnt);;
	if( StrategyStatus::Room.Cnt == LivRM)  //客廳
    {
        switch( GotoRoomStep )
        {
        case 0://到房間門口
            ActiveState = etAStar;
            if( !FlagSetInitialData )
            	SetAStar( StrategyStatus::LivRMDoor );
        break;
        /*case 1://車頭面向房間
            ActiveState =  etTurnToAngle;
            GoalAngle = (StrategyStatus::LivRMCen - StrategyStatus::LivRMDoor).Angle();
        break;*//*
        case 2:
	    //WaitCatchball();
	    if(StrategyStatus::FlagMember = true)
	        EncounterPeople();
        break;*/
        default:
            ActiveState = etIdle;
            StrategyStatus::Room.Cnt ++; 
            GotoRoomStep = 0;
            StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
            FlagSetInitialData = false;			
        break;
        }
    }

    else if( StrategyStatus::Room.Cnt == DinRM || 
			StrategyStatus::Room.Cnt == Lib || 
			StrategyStatus::Room.Cnt == BedRM )
    {
        switch( GotoRoomStep )
        {
        case 0://到房間門口
            ActiveState =  etAStar;
            if( !FlagSetInitialData )
				if(StrategyStatus::Room.Cnt == DinRM)                			
					SetAStar( StrategyStatus::DinRMDoor );
				else if(StrategyStatus::Room.Cnt == BedRM)
					SetAStar( StrategyStatus::BedRMDoor );
				else if(StrategyStatus::Room.Cnt == Lib)
					SetAStar( StrategyStatus::LibDoor );
				
        break;
        /*case 1: // 車頭面向房間
            ActiveState =  etTurnToAngle;
			if(StrategyStatus::Room.Cnt == StrategyStatus::etDinRM)
            	GoalAngle = (StrategyStatus::DinRMCen - StrategyStatus::DinRMDoor).Angle();
			else if(StrategyStatus::Room.Cnt == StrategyStatus::etBedRM)
				GoalAngle = (StrategyStatus::BedRMCen - StrategyStatus::BedRMDoor).Angle();
			else if(StrategyStatus::Room.Cnt == StrategyStatus::etLib);
				GoalAngle = (StrategyStatus::LibCen - StrategyStatus::LibDoor).Angle();			
        break;*/
        /*case 2:
			ActiveState = etMakeSoundMove;
			MakeSound();
	        //WaitCatchball();
            //else{
                //GotoRoomStep++;
                //StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
                //FlagSetInitialData = false;
            //}
        break;
		case 3:
	    	if(StrategyStatus::FlagMember = true)
				EncounterPeople();
		break;
        case 4:
            ActiveState =  etAStar;
            if( StrategyStatus::Room.Cnt == StrategyStatus::Lib && (StrategyStatus::EscapePosition-LocationStatus::Position).Length() < 150 )
                StrategyStatus::FlagAvoidEnable = false; //關閉避障
            if( !FlagSetInitialData )
                SetAStar( StrategyStatus::EscapePosition );
        break;*/
        default:
            ActiveState = etIdle;
            StrategyStatus::Room.Cnt++;
            GotoRoomStep = 0;
            StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
            FlagSetInitialData = false;
        break;
        }
    }
    /*else if(StrategyStatus::Room.Cnt == 4 )  //充電區
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
    }*/
    /*else  //待機區
    {
        ActiveState =  etAStar;
        if( !FlagSetInitialData ) SetAStar( StrategyStatus::EndPosition );
        if( (StrategyStatus::EndPosition - LocationStatus::Position).Length() < 50 )
            StrategyStatus::FlagAvoidEnable = false; //關閉避障
        this->FlagTaskFinish = false;
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
void Stra_Task::ActiveFunction()
{
	switch( ActiveState )
    {
    //----------------------
    case etAStar :
			//printf("enter etAstar\n");
            if( StrategyStatus::AStarPath.Status != StrategyStatus::etAchieve )
            {
            	//printf("enter etAStar\n");
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
	if( StrategyStatus::Room.Cnt == DinRM )
	{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Restaurant.mp3");
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etCatchFinish )
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
	else if( StrategyStatus::Room.Cnt == Lib )
	{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Studyroom.mp3");
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etCatchFinish )
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
	else if( StrategyStatus::Room.Cnt == BedRM )
	{
		LinuxActionScript::PlayMP3("../../../Data/mp3/Patrol_Badroom.mp3");
		if( StrategyStatus::Room.SKSRoomState == StrategyStatus::etCatchFinish )
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
//---------------------------------------------------------------------------
void Stra_Task::EncounterPeople()
{
	if(StrategyStatus::FamilyMember == VisionStatus::etGrandPa)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Grandpa.mp3");
	else if(StrategyStatus::FamilyMember == VisionStatus::etGrandMa)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Grandma.mp3");
	else if(StrategyStatus::FamilyMember == VisionStatus::etFather)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Dad.mp3");
	else if(StrategyStatus::FamilyMember == VisionStatus::etMother)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Mom.mp3");
	else if(StrategyStatus::FamilyMember == VisionStatus::etSon)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Bro.mp3");
	else if(StrategyStatus::FamilyMember == VisionStatus::etdaughter)	LinuxActionScript::PlayMP3("../../../Data/mp3/Hi_Sis.mp3");
}

