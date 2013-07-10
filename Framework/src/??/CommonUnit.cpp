//---------------------------------------------------------------------------


//#pragma hdrstop

#include "CommonUnit.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

class TInfo *(TCommonUnit::Info) = NULL;

TCommonUnit::TCommonUnit()
{
    this->Caption = "NULL" ;
    this->State   = "NULL" ;    //���D�M���A����"NULL"
}
//---------------------------------------------------------------------------
TCommonUnit::TCommonUnit(string FilePath,int ParameterNum)
{
    this->Caption = "NULL";
    this->State   = "NULL";
    this->ParameterPath = FilePath;     //
    //========== ���J�Ѽ� ==============

    string PatameterName;                                       //�ѼƦW�ټȦs��

    this->FPtr_Parameter  = new ifstream(FilePath.c_str());     //�}���ɮ׫���
    this->ParameterNumber = ParameterNum;                       //�x�s�ѼƼƶq
    this->Parameter       = new float[ParameterNum];
    //-------���ɮ׸��J�ѼƦ�C
    for(int Cnt=0 ;Cnt < ParameterNum ;Cnt++){

        *(this->FPtr_Parameter) >> PatameterName;               //���J�ѼƦW��(�Ȧs)
        *(this->FPtr_Parameter) >> Parameter[Cnt];              //���J�Ѽƭ�
    }
    this->FPtr_Parameter->close();                              //�����ɮ׫���
    delete this->FPtr_Parameter;                                //����O����

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

    //========== ���J�Ѽ� ==============
    string PatameterName;                                                   //�ѼƦW�ټȦs��

    this->FPtr_Parameter  = new ifstream( this->ParameterPath.c_str() );    //�}���ɮ׫���

    //-------���ɮ׸��J�ѼƦ�C
    for(int Cnt=0 ;Cnt < this->ParameterNumber ;Cnt++){
        *(this->FPtr_Parameter) >> PatameterName;               //���J�ѼƦW��(�Ȧs)
        *(this->FPtr_Parameter) >> Parameter[Cnt];              //���J�Ѽƭ�
    }
    this->FPtr_Parameter->close();                              //�����ɮ׫���
    delete this->FPtr_Parameter;                                //����O����

    this->bNewParameter = true;
    //===================================

    return this->ParameterPath;
}
