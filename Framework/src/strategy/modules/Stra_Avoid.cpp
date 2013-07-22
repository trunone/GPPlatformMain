#include "Stra_Avoid.h"
#define Def_SafeRadiusStart 15

using namespace Robot;

Stra_Avoid* Stra_Avoid::m_UniqueInstance = new Stra_Avoid();
/*
Stra_Avoid::Stra_Avoid()
:TCommonUnit("./Strategy/StraConfig/Stra_Avoid.txt",9 )
{
    this->Caption = "Stra_Avoid";
    this->ParameterPath = "./Strategy/StraConfig/Stra_Avoid.txt";
    this->ParameterReset();
    Lock_Avoid = 0;
    Stone    = new TCoordinate[ this->ScanLineNum ];
    ScanLine = Info->HdwInfo->LaserInfo.ScanArray;
}
*/
/*
string Stra_Avoid::ParameterReset(void)
{
    string str_ = this->Caption +" ParameterReset";
    this->ScanStartAngle= this->Parameter[0] * M_PI / 180.0;
    this->ScanLineNum   = this->Parameter[1];
    this->ScanScale     = this->Parameter[2] * M_PI / 180.0;
    this->SafeDistance  = this->Parameter[3];
    this->SafeArc_D     = this->Parameter[4];
    this->SafeArc_A     = this->Parameter[5]* M_PI / 180.0;
    this->AvoidConfig1  = this->Parameter[6];
    this->AvoidForce    = this->Parameter[7] * M_PI / 180.0;
    this->FixDirect     = this->Parameter[8];
    this->bNewParameter = false;
    return str_;
}
*/
//------------------------------------------------------------------------------xml
int Stra_Avoid::loadxml (TiXmlElement* element){
	if(element != NULL){
		element->Attribute("StartAngle_du", &Stra_Avoid::ScanStartAngle);
		element->Attribute("ScanLineSize", &Stra_Avoid::ScanLineNum);
		element->Attribute("ScanLineScale_du", &Stra_Avoid::ScanScale);
		element->Attribute("SafeRange", &Stra_Avoid::SafeDistance);
		element->Attribute("SafeArc_D", &Stra_Avoid::SafeArc_D);
		element->Attribute("SafeArc_A", &Stra_Avoid::SafeArc_A);
		element->Attribute("AvoidConfig1", &Stra_Avoid::AvoidConfig1);
		element->Attribute("AvoidForce_du", &Stra_Avoid::AvoidForce);
		element->Attribute("FixDirect", &Stra_Avoid::FixDirect);						
	}
	return 0;
}

//---------------------------------------------------------------------------
void Stra_Avoid::Initialize(void)
{
/*
    string str_ = this->Caption +" Initial";
    Lock_Avoid = 0;
    return str_;
*/
}
void Stra_Avoid::Process(void)
{
    //if( this->bNewParameter ) this->ParameterReset();

    if( StrategyStatus::FlagAvoidEnable )
    {
        this->ScanLine = NULL ;//Info->HdwInfo->LaserInfo.ScanArray;

        StrategyStatus::CorrectionVector = this->ScanLineAvoidFunction( StrategyStatus::GoalVector );

        StrategyStatus::MotionDistance = StrategyStatus::CorrectionVector.Length();

        StrategyStatus::MotionAngle    = StrategyStatus::CorrectionVector.Angle();

    }
}

//*---------------------------------------------------------------------------

//---------- [€ušã] Á×»ÙšçŒÆ

//---------------------------------------------------------------------------

TCoordinate Stra_Avoid::ScanLineAvoidFunction( TCoordinate Goal )

{

    LeftForce  = TCoordinate::aVector(0,0);

    RightForce = TCoordinate::aVector(0,0);

    

    double StoneDistance = 0;

    double TmpCutAngle;

    TCoordinate Orien(0,0);



    float LeftCutAngle,RightCutAngle;

    float TmpLen;

    for( int i =0; i< this->ScanLineNum ;i++ )
    {
        this->Stone[i] = TCoordinate::aVector(1,0) << (i*this->ScanScale + this->ScanStartAngle);

        //---- ŽM§äšãŠ³ŠM¯Ù©Êªº±œŽyœu

        //---- §PÂ_¶ZÂ÷

        if( (  this->ScanLine[i] < this->SafeDistance )&& ( this->ScanLine[i] > 0 ) )

        {

            //---- §PÂ_»P¥ØŒÐ€èŠV§šš€ªº©·ªø

            TmpCutAngle = NormalizeAngle( this->Stone[i].Angle() - Goal.Angle() );

            TmpLen      = this->SafeDistance - this->ScanLine[i];

            if( ( fabs( TmpCutAngle ) * ScanLine[i] ) <  (SafeArc_A +0.1) * (SafeArc_D + 10) )

            {

                if( i < ScanLineNum/2 )

                    if( RightForce.Length() < TmpLen ){RightForce= Stone[i]*TmpLen; RightCutAngle= TmpCutAngle; }



                if( i > ScanLineNum/2 )

                    if( LeftForce.Length() < TmpLen ){ LeftForce = Stone[i]*TmpLen; LeftCutAngle = TmpCutAngle; }



            }

            if( ( fabs( TmpCutAngle ) * this->ScanLine[i] ) <  this->SafeArc_A * this->SafeArc_D  )

            {

                this->Stone[i].AssignLength( this->SafeDistance - this->ScanLine[i] );

            }

            else

            {

             
                this->Stone[i] = TCoordinate::aVector(0,0);

            }

        }

        else{ this->Stone[i] = TCoordinate::aVector(0,0); }



        Orien = Orien + this->Stone[i];

        //--------- ¬ö¿ý³Ì€j«Â¯Ù¶ZÂ÷

        StoneDistance = ( ( this->Stone[i].Length() > StoneDistance ) ? this->Stone[i].Length(): StoneDistance );





    }



    if( Goal.Length() < (this->SafeDistance - StoneDistance) )   //¥ØŒÐÂIŠb»ÙÃªª««e€è

        Orien = TCoordinate::aVector( 0 ,0 );

    else

        Orien =( Orien.UnitVector() ) * StoneDistance;


    float ForceRate = ( RightForce.Length() == 0 || LeftForce.Length() == 0 )? 0 : LeftForce.Length()/RightForce.Length();

    ForceRate = ForceRate > 1 ? 1/ForceRate : ForceRate;

    if( ForceRate > 0.4 )

    {

        float Left_d  = sin( fabs(LeftCutAngle)  )*(SafeDistance - LeftForce.Length());

        float Right_d = sin( fabs(RightCutAngle) )*(SafeDistance - RightForce.Length());

        if( Left_d + Right_d > 50 )

        {
          
          // int Config = 0.6;

        }

    }



    TCoordinate break_vel =  Goal.UnitVector() ;   //break_vel:¥ØŒÐÂIªº³æŠìŠV¶q



    if( StoneDistance > SafeDistance - Def_SafeRadiusStart )

    {

        break_vel = -1*Goal;

        Lock_Avoid = 0;

    }

    else

    {

        float value_cross1 = ( break_vel >> M_PI_2 ).cross( Orien );      //¥~¿n

        float avoid_value   = 0; /*Config * value_cross1*( Goal.Length()/this->SafeDistance )*/ // need to get the laser information 07/09  ming-hua 



        float value_cross2 =   break_vel.cross( Orien );                   //§PÂ_¥ž€OŠb¥ªÃäÁÙ¬O¥kÃä (¥ªÃä¬°¥¿¡B¥kÃä¬°­t)



        if( value_cross2 > 0 )

        {

            break_vel = ( Goal.UnitVector() >> AvoidForce ) * avoid_value;  // ¥k°{

            Lock_Avoid  = -1;

        }

        else

        {

            break_vel = ( Goal.UnitVector() << AvoidForce ) * avoid_value;  // ¥ª°{

            Lock_Avoid  = 1;

        }



    }


    return (Goal + break_vel);      // °{Á×€èŠV

}

