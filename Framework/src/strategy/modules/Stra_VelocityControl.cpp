#include "StrategyStatus.h"
#include "Stra_VelocityControl.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Robot;
using namespace std;

double Stra_VelocityControl::w1;
double Stra_VelocityControl::x1;
double Stra_VelocityControl::y1;

Stra_VelocityControl* Stra_VelocityControl::m_UniqueInstance = new Stra_VelocityControl();

Stra_VelocityControl::Stra_VelocityControl()
{

}

Stra_VelocityControl::~Stra_VelocityControl()
{

}
/*
Stra_VelocityControl::Stra_VelocityControl()
:TCommonUnit("./Strategy/StraConfig/Stra_VelocityControl.txt", 8 )
{
    this->Caption = "Stra_VelocityControl";
    this->ParameterPath = "./Strategy/StraConfig/Stra_VelocityControl.txt";
    this->ParameterReset();
    this->SpeedCmd = 0;
    this->SpeedTmp = 0;
}
*/
/*
string Stra_VelocityControl::ParameterReset(void)
{
    string str_ = this->Caption +" ParameterReset";
    this->DistanceMax =this->Parameter[0];
    this->DistanceMin =this->Parameter[1];
    this->SpeedMax    =this->Parameter[2];
    this->SpeedMin    =this->Parameter[3];
    this->ThetaMax    =this->Parameter[4];
    this->ThetaMin    =this->Parameter[5];
    this->OmegaMax    =this->Parameter[6];
    this->OmegaMin    =this->Parameter[7];
    this->bNewParameter = false;

    return str_;
}
*/
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
//--------------------------------------------------------------------------
void Stra_VelocityControl::Initialize(void)
{
  //  string str_ = this->Caption +" Initial";

}

void Stra_VelocityControl::Process(void)
{

    //if( this->bNewParameter ) this->ParameterReset();

    #ifndef Def_OMNIDIRECTION_SYSTEM

	//printf("%f\n",StrategyStatus::MotionDistance);
    if( StrategyStatus::MotionDistance != 0 )
    {
		//printf("%f\n",StrategyStatus::MotionAngle);
        //if( StrategyStatus::FlagForward )
        //{

		StrategyStatus::Direction = StrategyStatus::MotionAngle;

		//printf("%f\n",StrategyStatus::MotionAngle);
        
		//}
        //else

        //{

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

            }
			//printf("%f\n",StrategyStatus::MotionAngle);
        //}

    }

    #endif

    //------------------------------------------------------------------------

	//printf("%f\n",StrategyStatus::MotionDistance);
	//printf("%f\n",StrategyStatus::MotionAngle);
	//printf("%f\n",StrategyStatus::Direction);
	
    VelocityTransform( StrategyStatus::MotionDistance, StrategyStatus::MotionAngle, StrategyStatus::Direction );
}


//------------------------------------------

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

    } */
	Speed = 1;
	
	StrategyStatus::x = StrategyStatus::MotionDistance * Speed * Vector.x;

	StrategyStatus::y = StrategyStatus::MotionDistance * Speed * Vector.y;

	StrategyStatus::w = StrategyStatus::Direction;

	printf("%f\n",StrategyStatus::x);
	printf("%f\n",StrategyStatus::y);
	printf("%f\n",StrategyStatus::w);

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
