//---------------------------------------------------------------------------

#ifndef CommonUnitH
#define CommonUnitH

#include <string>
#include <fstream>
#include <vector>
using namespace std;

#include <math.h>
#include "InfoUnit.h"

#define Def_AngularToRadian 0.017453293		
#define Def_RadianToAngular 57.295779513		

class TCommonUnit
{
public:
    TCommonUnit();
    TCommonUnit(string FilePath,int ParameterNum);

    virtual string  ParameterReset(void);           
    virtual string  Initial(void);                  
    virtual string  Main(void);                     

    string ParameterFileRenew();                        

    inline double GetMin(const double &dX_,const double &dY_){ return (dX_>=dY_)?dY_:dX_; }

    string Caption;
    string State;		//篈
    string ParameterPath;		//把计隔畖

    static TInfo* Info;
    bool enable;
    //======== 弄郎盡ノ =======

    bool bNewParameter;         //把计穝篨夹
    int ParameterNumber;        //把计计ヘ
    float *Parameter;           //把计﹃
    ifstream *FPtr_Parameter;   //郎夹
    //==========================

    //TRunningFrequency rfTime;
    //TRunningFrequency rfFreq;

    int Test_rf;
};
//---------------------------------------------------------------------------
#endif
