#include "Stra_FindBall.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define Def_AnglePrecision (5*M_PI/180.0)
#define Def_MinTurnAngle (20*M_PI/180.0)
#define Def_CenterThreshold 20
#define Def_CenterThresholdHoldOn 40
#define Def_BallCnt 30
#define Def_EnterDoorSpeed 80
#define Def_EnterDoorEatAngle (15*M_PI/180.0)
#define Def_EnterDoorEatDistance 30
#define Def_TurnToApproachCondi_Angle (8*M_PI/180.0)
#define Def_ApproachBackTurnCondi_Angle (24*M_PI/180.0)
#define Def_ApproachBallSpeed 70
#define Def_ApproachToEatCondi_AX12 36
#define Def_SearchToMiddleSpeed 80
#define Def_SearchTrunSpeed (M_PI / 8.0)
#define Def_LR_SearchAngle (50*M_PI/180.0)
#define Def_NearSearchAngle (15*M_PI/180.0)
#define Def_EatBallDistance 60
#define Def_LivingRoom  0
#define Def_DiningRoom  1
#define Def_Library     2
#define Def_BedRoom     3

using namespace Robot;
using namespace std;

Stra_FindBall* Stra_FindBall::m_UniqueInstance = new Stra_FindBall();

Stra_FindBall::Stra_FindBall()
{

}

Stra_FindBall::~Stra_FindBall()
{

}

void Stra_FindBall::Initialize(void)
{

}

//----------------------------------------------------------------------------

void Stra_FindBall::Process(void )
{
    float AngleError;

    AnalyseBall();

    if(StrategyStatus::Room.SKSRoomState == StrategyStatus::etSKSCatchBall)
    {
        FlagRecognize = true;

        RoomCnt = StrategyStatus::Room.Cnt;

        RoomVector = StrategyStatus::Room.Info[RoomCnt].Center - StrategyStatus::Room.Info[RoomCnt].Door;

        CenterVector = StrategyStatus::Room.Info[RoomCnt].Center - LocationStatus::Position;

        switch( FindBallState )
        {
        case etInitial:

            StrategyStatus::AX12_Angle = 35;

            RenewRoomCorner();

            SearchBall_Initial();

            BallDirection    = 0;

            EatBallDirection = 0;

            FlagEatBall      = false;

            switch(RoomCnt)
            {
            case Def_LivingRoom:
            case Def_Library:

            case Def_DiningRoom:
            case Def_BedRoom:

                FindBallState = etSearchBall;

                break;

            }

            break;

        /*case etEnterRoom:

            StrategyStatus::Goal1 = CenterVector >> LocationStatus::Handle;

            StrategyStatus::FixSpeed = Def_EnterDoorSpeed;

            //---------------------------------------------------------------

            AX12_TakeBall();

            if( StrategyStatus::RadBallInfo.FindBallCnt > 0 &&    // ¬Ýš£²y

                    StrategyStatus::RadBallInfo.Distance < Def_EnterDoorEatDistance   &&    // ²yŠbŠY²y¶ZÂ÷€º

                    fabs(StrategyStatus::RadBallInfo.Angle ) < Def_EnterDoorEatAngle )     // ²yŠbŠY²yš€«×€º

            {
                FindBallState = etTurnToBall;

            }

            if( CenterVector.Length() < 10 )
            {
                if(StrategyStatus::RadBallInfo.FindBallCnt > 0 )

                    FindBallState = etTurnToBall;

                else

                    FindBallState = etSearchBall;
            }

            break;

            //----------------------------------------*/

        case etSearchBall:   //  §ä²y

            if( StrategyStatus::RadBallInfo.FindBallCnt > 0 )
            {
                FindBallState = etTurnToBall;

                AX12_TakeBall();

                TurnToBall();
            }
            else
            {

                SearchBall();

            }
            break;

            //----------------------------------------

        case etTurnToBall:  //  ÂàŠV²y

            AX12_TakeBall();

            TurnToBall();

            if( StrategyStatus::RadBallInfo.FindBallCnt == 0 && FlagEatBall == false)
            {
                FindBallState = etSearchBall;

                SearchStep    = etNearSearch;

                LoseBallCnt   = 0;

                SearchBall();
            }
            else if( fabs(StrategyStatus::RadBallInfo.Angle ) <  Def_TurnToApproachCondi_Angle )
            {
                FindBallState = etApproachBall;
            }

            break;

            //----------------------------------------

        case etApproachBall: //  Ÿaªñ²y

            AX12_TakeBall();

            Approach_Ball();
            if(StrategyStatus::RadBallInfo.FindBallCnt == 0 && FlagEatBall == false)
            {

                FindBallState = etSearchBall;

                SearchStep    = etNearSearch;

                LoseBallCnt   = 0;

                SearchBall();
            }
            else if( fabs(StrategyStatus::RadBallInfo.Angle ) >  Def_ApproachBackTurnCondi_Angle )
            {
                FindBallState = etTurnToBall;
            }
            else if( StrategyStatus::AX12_Angle > Def_ApproachToEatCondi_AX12 )
            {
                FindBallState = etEatBall;

                PastOdometer = aVector(0,0);

                DetermineEatDirection();

                FlagEatBall == true;
            }

            break;
            //----------------------------------------
        case etEatBall:    //  ŠY±Œ²y
            if( Eat_Ball() )
            {
                if( RoomCnt == Def_LivingRoom )
                    FindBallState = etLeaveRoom;
                else
                    FindBallState = etGotoCenter;
            }
            break;
            //----------------------------------------
        case  etGotoCenter:  //  Š^€€€ßÂI

            //---- AdjustJun  šÌ·ÓÂ¶Šæ­yžñ°Ÿ²Ÿ

            if( NormalizeAngle(CenterVector.Angle() - LocationStatus::Handle ) > (45 * M_PI /180.0) )

                StrategyStatus::Goal1 = aVector(35,0) << ( 40 * M_PI /180.0 );

            else if( NormalizeAngle(CenterVector.Angle() - LocationStatus::Handle ) < (-45 * M_PI /180.0) )

                StrategyStatus::Goal1 = aVector(35,0) << ( -40 * M_PI /180.0 );

            else
                StrategyStatus::Goal1 = CenterVector >> LocationStatus::Handle;

            StrategyStatus::FixSpeed = 70;

            if( CenterVector.Length() < 20 )
            {
                FindBallState = etInitial;

                StrategyStatus::Room.SKSRoomState = StrategyStatus::etCatchFinish;

                // Info->HdwInfo->MSLCommand.Brush_En = false;
            }

            break;

        case  etLeaveRoom:  //  ªðŠ^ªù€f

            //--- Â÷¶}©Ð¶¡¡AšÏ¥Î©T©w³t«×

            StrategyStatus::Goal1 = (StrategyStatus::Room.Info[RoomCnt].Door - LocationStatus::Position) >> LocationStatus::Handle;

            StrategyStatus::FixSpeed = Def_EnterDoorSpeed;

            // Info->HdwInfo->MSLCommand.Brush_En = false;

            if( (StrategyStatus::Room.Info[RoomCnt].Door - LocationStatus::Position).Length() < 10 )
            {
                FindBallState = etInitial;
                StrategyStatus::Room.SKSRoomState = StrategyStatus::etCatchFinish;
            }
            break;
        }

    }

    //TestFindBall2->Label4->Caption =( Info->LocInfo->FlagEvaluatuonEnable == false) ? "No Evaluatuon": "Have";

    StrategyStatus::FlagRecognize = FlagRecognize;
    //printf("FindBall done");
}

//----------------------------------------------------------------------------

void  Stra_FindBall::TurnToBall()
{
    if(StrategyStatus::RadBallInfo.Angle > 0 )

        StrategyStatus::Direction =  Def_SearchTrunSpeed;
    else
        StrategyStatus::Direction = -Def_SearchTrunSpeed;
}

//----------------------------------------------------------------------------
void  Stra_FindBall::Approach_Ball()
{

    if( StrategyStatus::RadBallInfo.Angle > 8 * Def_DuToPI )

        StrategyStatus::Goal1 = aVector(35,0) << (M_PI_4/ 3.0);

    else if( StrategyStatus::RadBallInfo.Angle < -8 * Def_DuToPI )

        StrategyStatus::Goal1 = aVector(35,0) << -(M_PI_4/ 3.0);
    else
        StrategyStatus::Goal1 = aVector(35,0);

    StrategyStatus::FixSpeed = Def_ApproachBallSpeed;
}

//----------------------------------------------------------------------------

bool  Stra_FindBall::Eat_Ball()
{

    //if( LocationStatus::FlagNewFeedback ){

    // PastOdometer = PastOdometer + TCoordinate::aVector(Share_Info->HdwInfo->NIOSFeedback.ShiftX,Share_Info->HdwInfo->NIOSFeedback.ShiftY );
    //}
    static bool Dir_Lock = false;

    int AddCornerRange = ( Dir_Lock )? 20 : 0;

    if( PastOdometer.Length() < Def_EatBallDistance )
    {
        StrategyStatus::Goal1 = aVector(35,0);

        StrategyStatus::FixSpeed = 100;

        Dir_Lock = false;

        // Info->HdwInfo->MSLCommand.Brush_En = true;

        return false;

    }
    else return true;
}

//----------------------------------------------------------------------------

void  Stra_FindBall::AX12_TakeBall()
{   /*
       if( Info->ImgInfo->BallInfo.Cy != -999 )
       {
           if( StrategyStatus::RadBallInfo.UPDown == 1)

               Info->HdwInfo->MSLCommand.AX12_Angle++ ;

           else if( StrategyStatus::RadBallInfo.UPDown == -1 )

               Info->HdwInfo->MSLCommand.AX12_Angle--;

           if( Info->HdwInfo->MSLCommand.AX12_Angle > 41 ) Info->HdwInfo->MSLCommand.AX12_Angle = 41;

           if( Info->HdwInfo->MSLCommand.AX12_Angle < 32 ) Info->HdwInfo->MSLCommand.AX12_Angle = 32;

       }
    */
}

//----------------------------------------------------------------------------

void  Stra_FindBall::SearchBall_Initial()
{

    SearchStep      = etLeftRight;

    LoseBallCnt     = 0;

    LoseBallAngle   = LocationStatus::Handle;

    SearchTrunStep  = 0;

    Lock            = false;

    SearchTimes     = 0;

}

//----------------------------------------------------------------------------

int  Stra_FindBall::DisTransfer(int AX12_CMD)
{

    int Distance;

    if( AX12_CMD > 41 ) AX12_CMD = 41;

    if( AX12_CMD < 32 ) AX12_CMD = 32;

    switch(AX12_CMD)

    {

    case 32:
        Distance = 200;
        break;

    case 33:
        Distance = 110;
        break;

    case 34:
        Distance = 74;
        break;

    case 35:
        Distance = 59;
        break;

    case 36:
        Distance = 44;
        break;

    case 37:
        Distance = 35;
        break;

    case 38:
        Distance = 29;
        break;

    case 39:
        Distance = 24;
        break;

    case 40:
        Distance = 22;
        break;

    case 41:
        Distance = 18;
        break;

    default:

        Distance = 200;

    }

    return Distance;

}

//----------------------------------------------------------------------------

int  Stra_FindBall::MiddleValue(int *Array, int Num)    //€€­È

{

    int TmpValue;

    for(int i = 0; i < Num; i++)

        for(int j = 0; j < Num-1; j++)

        {

            if(Array[j] > Array[j+1])

            {

                TmpValue = Array[j];

                Array[j] = Array[j+1];

                Array[j+1] = TmpValue;

            }

        }

    return Array[Num/2];

}

//----------------------------------------------------------------------------

void  Stra_FindBall::RenewRoomCorner()

{

    if( fabs(RoomVector.x) > fabs(RoomVector.y) )

        if( RoomVector.x > 0)

        {

            Corner[3] = aVector(StrategyStatus::Room.Info[RoomCnt].LeftBottom.x, StrategyStatus::Room.Info[RoomCnt].RightTop.y);

            Corner[2] = StrategyStatus::Room.Info[RoomCnt].RightTop;

            Corner[1] = aVector(StrategyStatus::Room.Info[RoomCnt].RightTop.x, StrategyStatus::Room.Info[RoomCnt].LeftBottom.y);

            Corner[0] = StrategyStatus::Room.Info[RoomCnt].LeftBottom;

        }

        else

        {

            Corner[1] = aVector(StrategyStatus::Room.Info[RoomCnt].LeftBottom.x, StrategyStatus::Room.Info[RoomCnt].RightTop.y);

            Corner[0] = StrategyStatus::Room.Info[RoomCnt].RightTop;

            Corner[3] = aVector(StrategyStatus::Room.Info[RoomCnt].RightTop.x, StrategyStatus::Room.Info[RoomCnt].LeftBottom.y);

            Corner[2] = StrategyStatus::Room.Info[RoomCnt].LeftBottom;

        }

    else

        if( RoomVector.y > 0)

        {

            Corner[2] = aVector(StrategyStatus::Room.Info[RoomCnt].LeftBottom.x, StrategyStatus::Room.Info[RoomCnt].RightTop.y);

            Corner[1] = StrategyStatus::Room.Info[RoomCnt].RightTop;

            Corner[0] = aVector(StrategyStatus::Room.Info[RoomCnt].RightTop.x, StrategyStatus::Room.Info[RoomCnt].LeftBottom.y);

            Corner[3] = StrategyStatus::Room.Info[RoomCnt].LeftBottom;

        }

        else

        {

            Corner[0] = aVector(StrategyStatus::Room.Info[RoomCnt].LeftBottom.x, StrategyStatus::Room.Info[RoomCnt].RightTop.y);

            Corner[3] = StrategyStatus::Room.Info[RoomCnt].RightTop;

            Corner[2] = aVector(StrategyStatus::Room.Info[RoomCnt].RightTop.x, StrategyStatus::Room.Info[RoomCnt].LeftBottom.y);

            Corner[1] = StrategyStatus::Room.Info[RoomCnt].LeftBottom;
        }
}

//----------------------------------------------------------------------------

void  Stra_FindBall::DetermineEatDirection()
{
    TCoordinate Robot_V = LocationStatus::Position - StrategyStatus::Room.Info[RoomCnt].Door;
    if( Robot_V.cross(RoomVector) > 0 )
    {
        if( fabs(NormalizeAngle(RoomVector.Angle() - LocationStatus::Handle )) > M_PI_2  )
            CornerID = 0;
        else
            CornerID = 1;
    }
    else
    {
        if( fabs(NormalizeAngle(RoomVector.Angle() - LocationStatus::Handle )) > M_PI_2  )
            CornerID = 3;
        else
            CornerID = 2;
    }

    if( NormalizeAngle((Corner[CornerID] - LocationStatus::Position).Angle() - LocationStatus::Handle) > 0 )
        EatBallDirection = 1;
    else
        EatBallDirection = -1;
}

//----------------------------------------------------------------------------

void  Stra_FindBall::AnalyseBall()
{
    /*
        if( Info->ImgInfo->BallInfo.Cy != -999 && Info->ImgInfo->BallInfo.Angle != -999 )

        {

            //----- ¬ö¿ýš€«×

            StrategyStatus::RadBallInfo.Angle = Info->ImgInfo->BallInfo.Angle * Def_DuToPI;

            BallDirection = (StrategyStatus::RadBallInfo.Angle > 0 )? 1 : -1;

            if( ( 240-Info->ImgInfo->BallInfo.Cy ) > CenterThreshold )

            {     StrategyStatus::RadBallInfo.UPDown =  1;}                //AX12©R¥OŒW¥[  ©¹€U²Ÿ°Ê(Šb€€€ßœu€U€è)

            else if(( 240-Info->ImgInfo->BallInfo.Cy ) < -CenterThreshold )

            {     StrategyStatus::RadBallInfo.UPDown = -1;}                //AX12©R¥OŽî€Ö  ©¹€W²Ÿ°Ê(Šb€€€ßœu€W€è)

            else{ StrategyStatus::RadBallInfo.UPDown =  0;}                //AX12€£°Ê      Šb€€€ßŠìžm



            //----------- Á×§KŸ_Àúªº»Ö­È 40 : 20

            CenterThreshold = StrategyStatus::RadBallInfo.UPDown == 0 ? Def_CenterThresholdHoldOn :Def_CenterThreshold;



            //------------ ·í²yŠbµe­±€€¶¡®É ------------------------------------

            if(StrategyStatus::RadBallInfo.UPDown == 0)
            {

                //-- ²yŠb³õŠaªº€èŠV

                float BallAngle = LocationStatus::Handle + StrategyStatus::RadBallInfo.Angle;

                //-- ²yªº¶ZÂ÷

                StrategyStatus::RadBallInfo.Distance = DisTransfer( Info->HdwInfo->MSLCommand.AX12_Angle );

                StrategyStatus::RadBallInfo.Position = LocationStatus::Position +

                                                      (aVector(StrategyStatus::RadBallInfo.Distance,0)<<BallAngle);

            }

            StrategyStatus::RadBallInfo.FindBallCnt = StrategyStatus::RadBallInfo.FindBallCnt >= Def_BallCnt?

                                                      Def_BallCnt : StrategyStatus::RadBallInfo.FindBallCnt+1;

        else
        {
            if( StrategyStatus::RadBallInfo.FindBallCnt < 1 )
            {
                StrategyStatus::RadBallInfo.Angle    = -999;

                StrategyStatus::RadBallInfo.Distance = -999;

                StrategyStatus::RadBallInfo.Position = aVector(-999,-999);

                StrategyStatus::RadBallInfo.UPDown   = -999;

                StrategyStatus::RadBallInfo.LeftRight= -999;

                StrategyStatus::RadBallInfo.FindBallCnt = 0;
            }

            else StrategyStatus::RadBallInfo.FindBallCnt--;
        }
    */
}

//----------------------------------------------------------------------------
void  Stra_FindBall::SearchBall()
{
    TCoordinate RoomMidPos;

    int TrunType      = ( BallDirection == 1 ) ? 1 : -1;

    int TrunCondition = ( BallDirection == 1 ) ? 0 :  1;

    switch(SearchStep)

    {

    case etLeftRight:

        StrategyStatus::AX12_Angle = 36;

        if( SearchTrunStep == 0 || SearchTrunStep == 2)

            StrategyStatus::Direction =  TrunType * Def_SearchTrunSpeed;

        else if( SearchTrunStep == 1 )

            StrategyStatus::Direction = -TrunType * Def_SearchTrunSpeed;

        if( NormalizeAngle(LocationStatus::Handle-RoomVector.Angle()) > (Def_LR_SearchAngle) && SearchTrunStep == TrunCondition )

            SearchTrunStep++;

        else if( NormalizeAngle(LocationStatus::Handle-RoomVector.Angle()) < (-1*(Def_LR_SearchAngle)) && SearchTrunStep == (1-TrunCondition) )

            SearchTrunStep++;

        else if( fabs(NormalizeAngle(LocationStatus::Handle-RoomVector.Angle())) < (5*Def_DuToPI)  && SearchTrunStep == 2)

            SearchTrunStep++;

        if( SearchTrunStep == 3 )

        {
            //SearchStep = etRoomMid;

            if( RoomCnt == Def_DiningRoom || RoomCnt == Def_BedRoom )  // yao

                SearchStep = etRoomMid;

            else

                SearchStep = etEndSearch;

            SearchTrunStep  = 0;

        }

        break;

        //---------------------------------------------------------------------------

    case etRoomMid:

        StrategyStatus::AX12_Angle = 35;

        RoomMidPos = StrategyStatus::Room.Info[RoomCnt].Center; //+ (0.75*(Corner[1] - Corner[0]));

        if( (RoomMidPos - LocationStatus::Position).Length() < 10 || Lock )
        {
            Lock = true;

            if( SearchTrunStep == 0 || SearchTrunStep == 2)

                StrategyStatus::Direction = TrunType * Def_SearchTrunSpeed;

            else if( SearchTrunStep == 1 )

            {

                FlagRecognize = false;

                StrategyStatus::Direction = TrunType * M_PI_4;

            }

            if(( TrunType * NormalizeAngle(LocationStatus::Handle - (Corner[3]-RoomMidPos).Angle()) > 0) && SearchTrunStep == TrunCondition)

                SearchTrunStep++;

            else if(( TrunType * NormalizeAngle(LocationStatus::Handle - (Corner[0]-RoomMidPos).Angle())> 0) && SearchTrunStep == 1-TrunCondition)

                SearchTrunStep++;

            else if(( fabs(NormalizeAngle(LocationStatus::Handle-RoomVector.Angle())) > 175*Def_DuToPI ) && SearchTrunStep == 2)

                SearchTrunStep++;

            if( SearchTrunStep == 3 )

            {

                SearchStep = etEndSearch;

                SearchTrunStep  = 0;

                Lock = false;

            }

        }

        else

        {

            StrategyStatus::Goal1 = (RoomMidPos - LocationStatus::Position)  >> LocationStatus::Handle;

            StrategyStatus::FixSpeed = Def_SearchToMiddleSpeed;

        }

        break; //*/

        //---------------------------------------------------------------------------

    case etNearSearch:

        StrategyStatus::AX12_Angle = 36 +LoseBallCnt;

        if( SearchTrunStep == 0 )

            StrategyStatus::Direction =  TrunType* Def_SearchTrunSpeed;

        else if( SearchTrunStep == 1 )

            StrategyStatus::Direction = -TrunType* Def_SearchTrunSpeed;

        if( NormalizeAngle(LocationStatus::Handle- LoseBallAngle) > Def_NearSearchAngle && SearchTrunStep == TrunCondition )

            SearchTrunStep++;

        else if( NormalizeAngle(LocationStatus::Handle-LoseBallAngle) < -Def_NearSearchAngle && SearchTrunStep == (1 - TrunCondition) )

            SearchTrunStep++;

        if( SearchTrunStep == 2 )
        {
            SearchTrunStep  = 0;

            LoseBallCnt++;

            if( LoseBallCnt >= 3)

            {

                LoseBallCnt = 0;

                //SearchStep = etRoomMid;

                SearchStep = etEndSearch;

            }

        }

        break;

        //---------------------------------------------------------------------------

    case etEndSearch:

        if( RoomCnt == Def_DiningRoom || RoomCnt == Def_BedRoom )

            FindBallState = etGotoCenter;

        else

            FindBallState = etLeaveRoom;

        break;

    }

}



