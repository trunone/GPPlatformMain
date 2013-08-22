//=============================================
//File name: Stra_Avoid
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/09
//
//Function :Robot avoidance strategy
//===============================================
#ifndef STRA_AVOIDH
#define STRA_AVOIDH
#include "TCoordinate.h"
#include "StrategyModule.h"
#include "StrategyStatus.h"
#include <stdio.h>
#include <math.h>
#include <tinyxml.h>
namespace Robot {
class Stra_Avoid : public StrategyModule
{
public:

    static Stra_Avoid*  GetInstance() {
        return m_UniqueInstance;
    }

    ~Stra_Avoid();

    void Initialize(void);

    void Process(void);

    TCoordinate CorrectV;

    int LoadXMLSettings(TiXmlElement* element);  //load xml

private:

    static Stra_Avoid* m_UniqueInstance;

    Stra_Avoid();

    TCoordinate ScanLineAvoidFunction( TCoordinate OrigTarget );

	TCoordinate NewAvoidFunction( TCoordinate OrigTarget);

    TCoordinate LeftForce, RightForce;

	vector<long> AvoidLaserData;

    int AvoidScanLineNum;

    double ScanStartAngle;

    double ScanScale;

    int SafeDistance;

    double SafeArc_D;

    double SafeArc_A;

    double AvoidConfig1;

    double AvoidForce;

    int FixDirect;

    int Lock_Avoid;

    TCoordinate *Stone;

};
}

#endif
