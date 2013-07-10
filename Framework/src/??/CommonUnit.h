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
    string State;		//���A
    string ParameterPath;		//�ѼƸ��|

    static TInfo* Info;
    bool enable;
    //======== Ū�ɱM�� =======

    bool bNewParameter;         //�ѼƧ�s�X��
    int ParameterNumber;        //�ѼƼƥ�
    float *Parameter;           //�ѼƦ�C
    ifstream *FPtr_Parameter;   //�ɮ׫���
    //==========================

    //TRunningFrequency rfTime;
    //TRunningFrequency rfFreq;

    int Test_rf;
};
//---------------------------------------------------------------------------
#endif
