//---------------------------------------------------------------------------


//#pragma hdrstop

#include "CommonUnit.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

class TInfo *(TCommonUnit::Info) = NULL;

TCommonUnit::TCommonUnit()
{
    this->Caption = "NULL" ;
    this->State   = "NULL" ;    //夹肈㎝篈常"NULL"
}
//---------------------------------------------------------------------------
TCommonUnit::TCommonUnit(string FilePath,int ParameterNum)
{
    this->Caption = "NULL";
    this->State   = "NULL";
    this->ParameterPath = FilePath;     //
    //========== 更把计 ==============

    string PatameterName;                                       //把计嘿既竟

    this->FPtr_Parameter  = new ifstream(FilePath.c_str());     //秨币郎夹
    this->ParameterNumber = ParameterNum;                       //纗把计计秖
    this->Parameter       = new float[ParameterNum];
    //-------パ郎更把计﹃
    for(int Cnt=0 ;Cnt < ParameterNum ;Cnt++){

        *(this->FPtr_Parameter) >> PatameterName;               //更把计嘿(既)
        *(this->FPtr_Parameter) >> Parameter[Cnt];              //更把计
    }
    this->FPtr_Parameter->close();                              //闽超郎夹
    delete this->FPtr_Parameter;                                //睦癘拘砰

    this->bNewParameter = true;
    //===================================
}
//---------------------------------------------------------------------------
string TCommonUnit::Main()
{
    this->Caption = "Main" ;
    this->State   = "Idle" ;

    return Caption;
}
//---------------------------------------------------------------------------
string TCommonUnit::Initial()
{
    this->Caption = "MainInit" ;
    this->State   = "MainInit" ;
    return Caption;
}
//---------------------------------------------------------------------------
string TCommonUnit::ParameterReset()
{
    this->bNewParameter = false ;
    return "NoParameter";
}
//---------------------------------------------------------------------------
string TCommonUnit::ParameterFileRenew()
{
    if( this->ParameterNumber == 0 ){ return "NoConfig"; }

    //========== 更把计 ==============
    string PatameterName;                                                   //把计嘿既竟

    this->FPtr_Parameter  = new ifstream( this->ParameterPath.c_str() );    //秨币郎夹

    //-------パ郎更把计﹃
    for(int Cnt=0 ;Cnt < this->ParameterNumber ;Cnt++){
        *(this->FPtr_Parameter) >> PatameterName;               //更把计嘿(既)
        *(this->FPtr_Parameter) >> Parameter[Cnt];              //更把计
    }
    this->FPtr_Parameter->close();                              //闽超郎夹
    delete this->FPtr_Parameter;                                //睦癘拘砰

    this->bNewParameter = true;
    //===================================

    return this->ParameterPath;
}
