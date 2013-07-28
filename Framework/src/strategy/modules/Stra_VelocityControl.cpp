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
    if( StrategyStatus::MotionDistance != 0 )
    {
        if( StrategyStatus::FlagForward )
        {//=== ¥u¯à¥H¥¿­±²Ÿ°Ê

		StrategyStatus::Direction = StrategyStatus::MotionAngle;
        }

        else

        {//=== ¯à«e«á²Ÿ°Ê

            if( StrategyStatus::MotionAngle > M_PI_2)       //-- ²Ÿ°Ê§šš€€j©ó 90«×(¥ªÂà) ,šÏ¥Î«á°h šÃ§ï¬°(¥kÂà)

            {

                StrategyStatus::Direction = StrategyStatus::MotionAngle - M_PI ;

            }

            else if( StrategyStatus::MotionAngle < -M_PI_2) //-- ²Ÿ°Ê§šš€€p©ó-90«×(¥kÂà) ,šÏ¥Î«á°h šÃ§ï¬°(¥ªÂà)

            {

                StrategyStatus::Direction = StrategyStatus::MotionAngle + M_PI ;

            }

            else

            {

                StrategyStatus::Direction = StrategyStatus::MotionAngle;

            }

        }

    }

    #endif

    //------------------------------------------------------------------------

    VelocityTransform( StrategyStatus::MotionDistance, StrategyStatus::MotionAngle, StrategyStatus::Direction );

    printf("VelocityContril done");
}


//---------- [€ušã] ³t«×»Pš€³t«×³W¹º

void Stra_VelocityControl::VelocityTransform( double dTargetDis, double dTargetCutAng, double Theta )
{

    TCoordinate Vector( dTargetCutAng );

    double Speed = 0;


    //³t²v(¯Â¶q)³W¹º

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



    StrategyStatus::PathMotion = Speed * Vector;

    //---------------------------------------------------

    //š€³t«×³W¹º

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



    //³t«×€èŠV§PÂ_

    StrategyStatus::PathRotation = (Theta < 0) ? -Omega : Omega;


}

//---------------------------------------------------------------------------

//---------- [€ušã] S-Function

//---------------------------------------------------------------------------

double Stra_VelocityControl::S_Function(const double &TargetMax, const double &TargetMin,

                                        const double &CurrentMax,const double &CurrentMin,

                                        double Data)

{

    return (TargetMax-TargetMin)*((cos(M_PI*(((Data-CurrentMin)/(CurrentMax-CurrentMin))-1))+1)/2 )+TargetMin;

}

//---------------------------------------------------------------------------

//---------- [€ušã] Raise-Function

//---------------------------------------------------------------------------

double Stra_VelocityControl::Raise_Function( double V, double Cmd )

{

    return V * (1-exp((-1*(10/V))*((V+10/V)-Cmd)));

}
   // static TInfo* Info;
