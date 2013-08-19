#include "Stra_PathPlan.h"
using namespace Robot;
using namespace std;

Stra_PathPlan* Stra_PathPlan::m_UniqueInstance = new Stra_PathPlan();

Stra_PathPlan::Stra_PathPlan()
{

}

Stra_PathPlan::~Stra_PathPlan()
{

}
//------------------------------------------------------------------------------xml
int Stra_PathPlan::LoadXMLSettings (TiXmlElement* element){
	if(element != NULL){
		element->Attribute("DetourConst", &DetourConst);						
	}
	return 0;
}
//------------------------------------------------------------------------------
void Stra_PathPlan::Initialize(void)
{
    P1_Dis = 0;         

    P1_CutAng = 0;      

    P2_Dis = 0;

    P2_CutAng = 0;
}
//----------------------------------------------------------------------------

void Stra_PathPlan::Process( void )
{
    #ifndef Def_OMNIDIRECTION_SYSTEM

    if(StrategyStatus::FlagForward )                                             //9.5

    {
        if( (StrategyStatus::Goal1.Length() *fabs(StrategyStatus::Goal1.Angle()) > 10.5 ) &&

            (fabs(StrategyStatus::Goal1.Angle()) > M_PI / 6 ) )//three wheel omnidircetion system

        {

			StrategyStatus::Direction = StrategyStatus::Goal1.Angle();

            StrategyStatus::Goal1 =aVector(0,0);
			
        }

    }

    #endif
    //------------------------------------------------------------------------
    if( StrategyStatus::Goal1.Length() != 0 && StrategyStatus::Goal2.Length() != 0 ) //two points
    {

        P1_Dis    = StrategyStatus::Goal1.Length();

        P1_CutAng = StrategyStatus::Goal1.Angle();

        P2_Dis    = StrategyStatus::Goal2.Length();

        P2_CutAng = StrategyStatus::Goal2.Angle();


        PathPlan();


        StrategyStatus::GoalVector.AssignLength(this->P1_Dis);

        StrategyStatus::GoalVector.AssignAngle(this->P1_CutAng);

    }

    else{

		StrategyStatus::GoalVector = StrategyStatus::Goal1;
		printf("goal vector %f %f\n",StrategyStatus::Goal1.x,StrategyStatus::Goal1.y);

    }
	
    StrategyStatus::MotionDistance =  StrategyStatus::GoalVector.Length();	

    StrategyStatus::MotionAngle    =  StrategyStatus::GoalVector.Angle();

}

//----------------------------------------------------------------------------
double Stra_PathPlan::Trajectory( void )
{

    double dAlpha = NormalizeAngle( P1_CutAng - P2_CutAng);


    short Sgn_Alpha = (dAlpha >= 0) ? 1 : -1;


    if( StrategyStatus::FlagDetour ){

        dAlpha = fabs( dAlpha );

        return P1_CutAng + Sgn_Alpha * GetMin( dAlpha ,atan2( DetourConst , P1_Dis) );


    }else{

        dAlpha = M_PI - fabs( dAlpha );

        return P1_CutAng - Sgn_Alpha * GetMin( dAlpha ,atan2( DetourConst , P1_Dis) );
    }

}
//----------------------------------------------------------------------------
void Stra_PathPlan::PathPlan( void )
{
    double TmpAngle = Trajectory();

    P1_Dis = P1_Dis*((fabs(sin( TmpAngle - P1_CutAng))+1)/fabs(cos( TmpAngle - P1_CutAng )));

    P1_CutAng = TmpAngle;

}
//-----------------------------------------------------------------------------
