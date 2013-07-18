//---------------------------------------------------------------------------
#pragma hdrstop

#include <math.h>

#include "TCoordinate.h"
//==============================================================================

//--------------------�ഫ�y�Шt------------------------------------------------
inline void TCoordinate::CartesianPolar(void)
{
    if(x == 0 && y != 0){
        this->LengthValue = fabs( y );
        this->AngleValue  = ( y > 0 ) ? M_PI / 2.0 : NormalizeAngle( M_PI / 2.0 + M_PI ) ;
    }else if(x != 0 && y == 0){
        this->LengthValue = fabs( x );
        this->AngleValue  = ( x > 0 )? 0 : M_PI;
    }else if(x == 0 && y == 0){
        this->AngleValue  = 0;
        this->LengthValue = 0;
    }else{
        this->AngleValue  = atan2(this->y , this->x );
        this->LengthValue = sqrt(x*x + y*y) ;
    }
}

inline void TCoordinate::PolarCartesian(void)
{
    this->x = this->LengthValue * cos(this->AngleValue);
    this->y = this->LengthValue * sin(this->AngleValue);
}
//==============================================================================

//--------------------Construct Function----------------------------------------
TCoordinate::TCoordinate(double xValue,double yValue)
{
    this->x = xValue;
    this->y = yValue;

    CartesianPolar();
}
//------------------------------------------------------------------------------
TCoordinate::TCoordinate(double sita)
{
    this->LengthValue = 1;
    this->AngleValue = NormalizeAngle(sita);

    PolarCartesian();
}
//------------------------------------------------------------------------------
TCoordinate::TCoordinate()
{
    this->x = 0.0;
    this->y = 0.0;
    this->AngleValue = 0.0;
    this->LengthValue = 0.0;
}
//------------------------------------------------------------------------------
TCoordinate::~TCoordinate()
{
    //fuck Writing Code can't let me be WET
    //GO to DMC Go to DMC
    //fuck!fuck!fuck!fuck!fuck!fuck!fuck!fuck!fuck!fuck!fuck!fuck!fuck!
}

//==============================================================================

//--------------------�B��l����------------------------------------------------
bool TCoordinate::operator == (TCoordinate op2)             //�V�q�O�_�۵�
{
    return ( this->x == op2.x && this->y == op2.y );
}
//------------------------------------------------------------------------------
TCoordinate TCoordinate::operator + (TCoordinate op2)       //��V�q�ۥ[
{
    TCoordinate temp;
    temp.x = this->x + op2.x;
    temp.y = this->y + op2.y;
    return temp;
}
//------------------------------------------------------------------------------
TCoordinate TCoordinate::operator - (TCoordinate op2)       //��V�q�۴�
{
    TCoordinate temp;
    temp.x = this->x - op2.x;
    temp.y = this->y - op2.y;
    return temp;
}
//------------------------------------------------------------------------------

TCoordinate TCoordinate::operator << (double ds)	        //�V�q����(���ץ[)
{
    TCoordinate temp;
    //�y�Шt�f��
    temp.x = this->x*cos(ds) - this->y*sin(ds);
    temp.y = this->x*sin(ds) + this->y*cos(ds);
    temp.CartesianPolar();
    return temp;
}
//------------------------------------------------------------------------------
TCoordinate TCoordinate::operator >> (double ds)	        //�V�q�k��(���״�)
{
    TCoordinate temp;
    //�y�Шt����
    temp.x =  this->x*cos(ds) + this->y*sin(ds);
    temp.y = -this->x*sin(ds) + this->y*cos(ds);
    temp.CartesianPolar();
    return temp;
}
//------------------------------------------------------------------------------

//==============================================================================
//------------------------------------------------------------------------------
double	TCoordinate::Angle()						        //�V�q������
{
    this->CartesianPolar();
    return this->AngleValue;
}
//------------------------------------------------------------------------------
double	TCoordinate::Length()						        //�V�q������
{
    this->CartesianPolar();
    return this->LengthValue;
}
//------------------------------------------------------------------------------

TCoordinate TCoordinate::UnitVector()					    //�V�q�����V�q
{
    TCoordinate temp = *this;
    temp.AssignLength(1.0);
    return temp;
}
//------------------------------------------------------------------------------
double TCoordinate::dot(TCoordinate op)				        //�V�q���n
{
    return (this->x*op.x + this->y*op.y);
}
//------------------------------------------------------------------------------
double TCoordinate::cross(TCoordinate op)	                //��V�q�����V
{
    return (this->x*op.y - this->y*op.x);
}
//------------------------------------------------------------------------------

void TCoordinate::AssignXY(double xValue,double yValue)     //���d���y��
{
    this->x = xValue;
    this->y = yValue;
    this->CartesianPolar();
}
//------------------------------------------------------------------------------
void TCoordinate::AssignAngle(double Value)				    //��﷥�y�Ш���
{
    this->CartesianPolar();
    this->AngleValue = Value;
    this->PolarCartesian();
}
//------------------------------------------------------------------------------
void TCoordinate::AssignLength(double Value)				//��﷥�y�Ъ���
{
    this->CartesianPolar();
    this->LengthValue = Value;
    this->PolarCartesian();
}
//------------------------------------------------------------------------------
TCoordinate  aVector(double xValue , double yValue)	    //���ͤ@�ӦV�q
{
    return  TCoordinate(xValue , yValue);
}
//------------------------------------------------------------------------------
TCoordinate  operator*(TCoordinate op , double scale)	//�V�q��N��
{
    op.x *= scale;
    op.y *= scale;
    return op;
}
//------------------------------------------------------------------------------
TCoordinate  operator*(double scale , TCoordinate op)	//�V�q��N��
{
    op.x *= scale;
    op.y *= scale;
    return op;
}
//------------------------------------------------------------------------------

double NormalizeAngle(double Angle)
{
    while(Angle >   M_PI) Angle -= 2*M_PI;
    while(Angle <= -M_PI) Angle += 2*M_PI;
    return Angle ;
}
//------------------------------------------------------------------------------


#pragma package(smart_init)
