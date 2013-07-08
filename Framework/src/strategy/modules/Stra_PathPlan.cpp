#include <stdio.h>
#include "Status.h"
#include "Stra_PathPlan.h"
#include "TCoordinate.h"
#include "StrategyStatus.h"
using namespace Robot;

Stra_PathPlan* Stra_PathPlan::m_UniqueInstance = new Stra_PathPlan();

//Need modifed to the xml system 
/*/Stra_PathPlan::Stra_PathPlan()
:TCommonUnit("./Strategy/StraConfig/Stra_PathPlan.txt", 1 )
{

    this->Caption = "Stra_PathPlan";

    this->ParameterPath = "./Strategy/StraConfig/Stra_PathPlan.txt";

    this->ParameterReset();

    this->P1_Dis = 0;         //¥ØŒÐ¶ZÂ÷

    this->P1_CutAng = 0;      //¥ØŒÐš€«×

    this->P2_Dis = 0;

    this->P2_CutAng = 0;      //¥ØŒÐ2š€«×  š®ÀYš€«×

}//*/

//----------------------------------------------------------------------------
/*
string Stra_PathPlan::ParameterReset(void)
{
    string str_ = this->Caption +" ParameterReset";

    this->DetourConst = this->Parameter[0];

    this->bNewParameter = false;

      return str_;

}
*/
//----------------------------------------------------------------------------

void Stra_PathPlan::Initialize(void)
{
//    string str_ = this->Caption +" Initial";

    this->P1_Dis = 0;         //¥ØŒÐ¶ZÂ÷

    this->P1_CutAng = 0;      //¥ØŒÐš€«×

    this->P2_Dis = 0;

    this->P2_CutAng = 0;      //¥ØŒÐ2š€«×  š®ÀYš€«×


//    return str_;

}

//----------------------------------------------------------------------------

void Stra_PathPlan::Process( void )
{
//    if( this->bNewParameter ) this->ParameterReset();

    //-----------------------------------------------------------------------

    #ifndef Def_OMNIDIRECTION_SYSTEM

    if(StrategyStatus::FlagForward )                                             //9.5

    {

        if( (StrategyStatus::Goal1.Length() *fabs(StrategyStatus::Goal1.Angle()) > 10.5 ) &&

            (fabs(StrategyStatus::Goal1.Angle()) > M_PI / 6 ) )//three wheel omnidircetion system

        {

		StrategyStatus::Direction = StrategyStatus::Goal1.Angle();

            	StrategyStatus::Goal1 =TCoordinate::aVector(0,0);

        }

    }

    #endif
    //------------------------------------------------------------------------
    if( StrategyStatus::Goal1.Length() != 0 && StrategyStatus::Goal2.Length() != 0 )

    {

        this->P1_Dis    = StrategyStatus::Goal1.Length();

        this->P1_CutAng = StrategyStatus::Goal1.Angle();



        this->P2_Dis    = StrategyStatus::Goal2.Length();

        this->P2_CutAng = StrategyStatus::Goal2.Angle();


        PathPlan();


        StrategyStatus::GoalVector.AssignLength(this->P1_Dis);

        StrategyStatus::GoalVector.AssignAngle(this->P1_CutAng);

    }

    else

    {

        StrategyStatus::GoalVector = StrategyStatus::Goal1;

    }



    StrategyStatus::MotionDistance =  StrategyStatus::GoalVector.Length();

    StrategyStatus::MotionAngle    =  StrategyStatus::GoalVector.Angle();


}

//----------------------------------------------------------------------------

double Stra_PathPlan::Trajectory( void )
{

    double dAlpha = NormalizeAngle( this->P1_CutAng - this->P2_CutAng);

    //-----­YP2šìP1ªºš€«×¬°¥¿¡A«hš®ÀYšìP1ªºš€«×¶·ŠV¥¿€èŠV­×¥¿¡A€Ï€§

    short Sgn_Alpha = (dAlpha >= 0) ? 1 : -1;
    //---------Â¶Šæ­yžñ¬°±µªñP2

    if( StrategyStatus::FlagDetour ){

        dAlpha = fabs( dAlpha );

        return this->P1_CutAng + Sgn_Alpha * GetMin( dAlpha ,atan2( this->DetourConst , this->P1_Dis) );

    //---------Â¶Šæ­yžñ¬°»·Â÷P2

    }else{

        dAlpha = M_PI - fabs( dAlpha );

        return this->P1_CutAng - Sgn_Alpha * GetMin( dAlpha ,atan2( this->DetourConst , this->P1_Dis) );
    }

}

//----------------------------------------------------------------------------

void Stra_PathPlan::PathPlan( void )
{
    double TmpAngle = Trajectory();

    this->P1_Dis = this->P1_Dis*((fabs(sin( TmpAngle - this->P1_CutAng))+1)/fabs(cos( TmpAngle - this->P1_CutAng )));//·s¥ØŒÐ¶ZÂ÷

    this->P1_CutAng = TmpAngle;

}

    //static TInfo* Info;
