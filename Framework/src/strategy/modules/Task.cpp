/*
 *   Task.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "Task.h"

// for use M_PI
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Robot;


Task* Task::m_UniqueInstance = new Task();

Task::Task()
{

}

Task::~Task()
{
}

/*
Stra_Task::Stra_Task()
:TCommonUnit("./Strategy/StraConfig/Stra_Task.txt", 0 )
{
    this->Caption = "Stra_Task";

    this->ParameterPath = "./Strategy/StraConfig/Stra_Task.txt";
    //-----------------
    this->ParameterReset();
    //-----------------
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
*/
/*
string Stra_Task::ParameterReset(void)
{
    string str_ = this->Caption +" ParameterReset";
    //---

    //---
    this->bNewParameter = false;
    return str_;
}
*/
//---------------------------------------------------------------------------
void Task::Initialize(void)
{
   // string str_ = this->Caption +" Initial";

    FlagTaskFinish      = false;
    FlagSetInitialData  = false;

    GotoRoomStep  = 0;
    ActiveState   = etIdle;
    DoorState     = false;
    Past_RoomCnt  = -1;
    TouchCnt      = 0;
   // return str_;
}
//---------------------------------------------------------------------------
void Task::Process(void)
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
        if(StrategyStatus::ThiefRoom != StrategyStatus::LivRM) StrategyStatus::Room.Cnt++;

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
            if( StrategyStatus::Room.Cnt ==2 && StrategyStatus::ThiefRoom ==2) WaitCatchball();
            else{
                GotoRoomStep++;
                StrategyStatus::Room.SKSRoomState = StrategyStatus::etSKSMoving;
                FlagSetInitialData = false;
            }
        break;
        case 3:
            ActiveState =  etAStar;
            if( StrategyStatus::Room.Cnt == StrategyStatus::Lib && (StrategyStatus::EscapePosition - LocationStatus::Position).Length() < 150 )
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
    else if(StrategyStatus::Room.Cnt == 4 )  //補抓小偷(會扣分)
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
        GotoRoomStep++; FlagTaskFinish = false; FlagSetInitialData = false;
    }
    else
        ActiveFunction();

    //---
   // return Caption;
}
//---------------------------------------------------------------------------
void Task::ActiveFunction()
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
    //----------------------
    //----------------------
    default:
    break;
    }
}
//---------------------------------------------------------------------------
void Task::WaitCatchball()
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
    }  //*/
}
//---------------------------------------------------------------------------
void Task::SetAStar( TCoordinate  Goal )
{
    FlagSetInitialData = true;
    StartPos = LocationStatus::Position;
    GoalPos  = Goal;

    StrategyStatus::AStarPath.Status = StrategyStatus::etMotion;
}
//---------------------------------------------------------------------------
bool Task::MotionToPosition( TCoordinate  Goal )
{
    if( (Goal - LocationStatus::Position).Length()  > 10 )
    {
        StrategyStatus::Goal1 = (Goal - LocationStatus::Position) >>  LocationStatus::Handle;
        return false;
    }
    else{ return true; }
}
//---------------------------------------------------------------------------
bool Task::TurnToAngle( float GoalAngle )
{
    StrategyStatus::Goal1 = TCoordinate::aVector(0,0);
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
bool Task::SpecialMove( int Forward )
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
bool Task::SpecialTurn()
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
bool Task::TouchButton()
{
    if( TouchCnt < 20 )
    {
        StrategyStatus::Goal1 = TCoordinate::aVector(100,0);
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
bool Task::Backward()
{
    if( TouchCnt < 30 )
    {
        StrategyStatus::FlagForward = false; // 開啟後退
        StrategyStatus::Goal1 =  TCoordinate::aVector(-100,0);
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
/*bool __fastcall Stra_Task::WaitDoorOpen()
{
    Info->LocInfo->FlagEvaluatuonEnable = false;
    
    int TmpCnt = 0;
    if( !FlagSetInitialData )
    {
        FlagSetInitialData = true;
        for(int i=0; i<24 ; i++ )
        {
            PastScanLineData[i] = Info->LocInfo->ScanLineData[i];
        }
        return false;
    }
    else
    {
        for(int i=0; i< 24 ; i++ )
        {
            if( Info->LocInfo->ScanLineData[i] - PastScanLineData[i] > 30 )  TmpCnt++;
            
            PastScanLineData[i] = Info->LocInfo->ScanLineData[i];
        }
        return ( TmpCnt > 1 )? true: false;
    }
}  //*/
//---------------------------------------------------------------------------
/*void __fastcall Stra_Task::SpecialRoom3()
{
        switch( GotoRoomStep )
        {
        //--------------------------
        case 0:     //到門口旁偏移的特殊位置
            ActiveState =  etAStar;

            if( !FlagSetInitialData )
                SetAStar( Room3StartPos );
        break;
        //--------------------------
        case 1:     //轉向到房間門口的方向
            ActiveState = etTurnToAngle;
            GoalAngle = (Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt].Door - Room3StartPos ).Angle();

        break;
        //--------------------------
        case 2:    // 關閉修正移動到門口
            ActiveState = etMotionToPos;
            Info->LocInfo->FlagEvaluatuonEnable = false;
            GoalPos = Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt].Door;
        break;
        //--------------------------
        case 3:    // 關閉修正轉向房間
            ActiveState = etTurnToAngle;
            Info->LocInfo->FlagEvaluatuonEnable = false;
            GoalAngle = (Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt].Center -
                         Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt].Door).Angle();
        break;
        //--------------------------
        case 4:    // 等待門關起來 右打開
            ActiveState =  etWaitDoorOpen;

        break;
        //--------------------------
        default:
            WaitCatchball();
        break;
        }
}  //*/
/*---------------------------------------------------------------------------
void __fastcall Stra_Task::SpecialRoom1()
{
        switch( GotoRoomStep )
        {
        //--------------------------
        case 0:     //到前一個房間的門口
            ActiveState =  etAStar;
            if( !FlagSetInitialData )
                SetAStar( Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt-1].Door );
        break;
        //--------------------------
        case 1:     //轉向目前房間的門口
            ActiveState = etTurnToAngle;
            GoalAngle = (Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt].Center -
                         Info->StraInfo->Room.Info[Info->StraInfo->Room.Cnt].Door).Angle();
        break;
        //--------------------------
        case 2:    // 使用雷射校正角度
            ActiveState = etSpecialTurn;
            SpecialTurn();

        break;
        //--------------------------
        case 3:    // 使用雷射移動至門口
            ActiveState =  etSpecialMove;

            if( !FlagSetInitialData )
            {
                FlagSetInitialData = true;
                DoorState    = Info->HdwInfo->LaserInfo.ScanArray[35] > 300 ? true : false;
                Past_LaserData = Info->HdwInfo->LaserInfo.ScanArray[35];
            }
            SpecialDistance = ( DoorState )? 180 : 30 ;

        break;
        //--------------------------
        case 4:    // 等待門關起來 右打開
            ActiveState =  etWaitDoorOpen;

        break;
        //--------------------------
        default:
            WaitCatchball();
        break;
        }
}
//----------------------------------------------------------------------------*/


