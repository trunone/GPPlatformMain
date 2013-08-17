#include "StrategyStatus.h"
#include "Stra_VelocityControl.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Robot;
using namespace std;


Stra_VelocityControl* Stra_VelocityControl::m_UniqueInstance = new Stra_VelocityControl();

Stra_VelocityControl::Stra_VelocityControl()
{

}

Stra_VelocityControl::~Stra_VelocityControl()
{

}
//----------------------------------------------------------------------------xml
int Stra_VelocityControl::LoadXMLSettings (TiXmlElement* element){
	if(element != NULL){
		element->Attribute("dis_max", &DistanceMax);
		element->Attribute("dis_min", &DistanceMin);
		element->Attribute("spd_max", &SpeedMax);
		element->Attribute("spd_min", &SpeedMin);
		element->Attribute("sita_max", &ThetaMax);
		element->Attribute("sita_min", &ThetaMin);
		element->Attribute("w_max", &OmegaMax);
		element->Attribute("w_min", &OmegaMin);
	}
}
//----------------------------------------------------------------------------
void Stra_VelocityControl::Initialize(void)
{

}

void Stra_VelocityControl::Process(void)
{
    #ifndef Def_OMNIDIRECTION_SYSTEM

    if( StrategyStatus::MotionDistance != 0 )
    {
        if( StrategyStatus::FlagForward )
        {
			StrategyStatus::Direction = StrategyStatus::MotionAngle;
		}
        else
        {
            if( StrategyStatus::MotionAngle > M_PI_2)
            {

                StrategyStatus::Direction = StrategyStatus::MotionAngle - M_PI ;

            }

            else if( StrategyStatus::MotionAngle < -M_PI_2) 

            {

                StrategyStatus::Direction = StrategyStatus::MotionAngle + M_PI ;

            }

            else

            {

                StrategyStatus::Direction = StrategyStatus::MotionAngle;
				//printf("%f\n",StrategyStatus::Direction);

            }
        }

    }

    #endif

    //------------------------------------------------------------------------
	
    VelocityTransform( StrategyStatus::MotionDistance, StrategyStatus::MotionAngle, StrategyStatus::Direction );
	
}


//-----------------------------------------------------------------------------

void Stra_VelocityControl::VelocityTransform( double dTargetDis, double dTargetCutAng, double Theta )
{

    TCoordinate Vector( dTargetCutAng );

	//printf("%f\n",Vector.x);
	//printf("%f\n",Vector.y);

    double Speed = 0;

    if( dTargetDis == 0)
		
        Speed = 0;

    else if( dTargetDis > this->DistanceMax )

        Speed = SpeedMax;

    else if( dTargetDis < this->DistanceMin )

        Speed = SpeedMin;

    else

        Speed = S_Function( this->SpeedMax   , this->SpeedMin,

                            this->DistanceMax, this->DistanceMin, dTargetDis);

    if( Speed != 0 && StrategyStatus::FixSpeed != 0 )
		
        Speed = (StrategyStatus::FixSpeed/100.0)*this->SpeedMax ;

    /*if( Speed > 20 ){

        SpeedCmd = Raise_Function( Speed ,Speed - SpeedCmd );

        this->SpeedTmp++

    }else{

        SpeedCmd = Speed;

    }*/
	
	StrategyStatus::x = -Speed * Vector.y;

	StrategyStatus::y = Speed * Vector.x;

	//printf("%f\n",StrategyStatus::x);

	//printf("%f\n",StrategyStatus::y);

    //StrategyStatus::PathMotion = Speed * Vector;

    //---------------------------------------------------

    double Omega;

    if( Theta == 0 )

        Omega = 0;

    else if( fabs(Theta) > this->ThetaMax )

        Omega = OmegaMax;

    else if( fabs(Theta) < this->ThetaMin )

        Omega = OmegaMin;

    else

        Omega = S_Function( this->OmegaMax, this->OmegaMin,

                            this->ThetaMax, this->ThetaMin, fabs(Theta) );



    

    StrategyStatus::PathRotation = (Theta < 0) ? -Omega : Omega;
	
	//StrategyStatus::w = StrategyStatus::PathRotation;

}

//---------------------------------------------------------------------------

//---------- [tools] S-Function

//---------------------------------------------------------------------------

double Stra_VelocityControl::S_Function(const double &TargetMax, const double &TargetMin,

                                        const double &CurrentMax,const double &CurrentMin,

                                        double Data)

{

    return (TargetMax-TargetMin)*((cos(M_PI*(((Data-CurrentMin)/(CurrentMax-CurrentMin))-1))+1)/2 )+TargetMin;

}

//---------------------------------------------------------------------------

//---------- [tools] Raise-Function

//---------------------------------------------------------------------------

double Stra_VelocityControl::Raise_Function( double V, double Cmd )

{

    return V * (1-exp((-1*(10/V))*((V+10/V)-Cmd)));

}
