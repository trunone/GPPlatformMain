//---------------------------------------------------------------------------
#pragma hdrstop

#include <math.h>

#include "TCoordinate.h"
//==============================================================================

//--------------------轉換座標系------------------------------------------------
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

//--------------------運算子重載------------------------------------------------
bool TCoordinate::operator == (TCoordinate op2)             //向量是否相等
{
    return ( this->x == op2.x && this->y == op2.y );
}
//------------------------------------------------------------------------------
TCoordinate TCoordinate::operator + (TCoordinate op2)       //兩向量相加
{
    TCoordinate temp;
    temp.x = this->x + op2.x;
    temp.y = this->y + op2.y;
    return temp;
}
//------------------------------------------------------------------------------
TCoordinate TCoordinate::operator - (TCoordinate op2)       //兩向量相減
{
    TCoordinate temp;
    temp.x = this->x - op2.x;
    temp.y = this->y - op2.y;
    return temp;
}
//------------------------------------------------------------------------------

TCoordinate TCoordinate::operator << (double ds)	        //向量左旋(角度加)
{
    TCoordinate temp;
    //座標系逆旋
    temp.x = this->x*cos(ds) - this->y*sin(ds);
    temp.y = this->x*sin(ds) + this->y*cos(ds);
    temp.CartesianPolar();
    return temp;
}
//------------------------------------------------------------------------------
TCoordinate TCoordinate::operator >> (double ds)	        //向量右旋(角度減)
{
    TCoordinate temp;
    //座標系正旋
    temp.x =  this->x*cos(ds) + this->y*sin(ds);
    temp.y = -this->x*sin(ds) + this->y*cos(ds);
    temp.CartesianPolar();
    return temp;
}
//------------------------------------------------------------------------------

//==============================================================================
//------------------------------------------------------------------------------
double	TCoordinate::Angle()						        //向量的角度
{
    this->CartesianPolar();
    return this->AngleValue;
}
//------------------------------------------------------------------------------
double	TCoordinate::Length()						        //向量的長度
{
    this->CartesianPolar();
    return this->LengthValue;
}
//------------------------------------------------------------------------------

TCoordinate TCoordinate::UnitVector()					    //向量的單位向量
{
    TCoordinate temp = *this;
    temp.AssignLength(1.0);
    return temp;
}
//------------------------------------------------------------------------------
double TCoordinate::dot(TCoordinate op)				        //向量內積
{
    return (this->x*op.x + this->y*op.y);
}
//------------------------------------------------------------------------------
double TCoordinate::cross(TCoordinate op)	                //兩向量旋轉方向
{
    return (this->x*op.y - this->y*op.x);
}
//------------------------------------------------------------------------------

void TCoordinate::AssignXY(double xValue,double yValue)     //更改卡式座標
{
    this->x = xValue;
    this->y = yValue;
    this->CartesianPolar();
}
//------------------------------------------------------------------------------
void TCoordinate::AssignAngle(double Value)				    //更改極座標角度
{
    this->CartesianPolar();
    this->AngleValue = Value;
    this->PolarCartesian();
}
//------------------------------------------------------------------------------
void TCoordinate::AssignLength(double Value)				//更改極座標長度
{
    this->CartesianPolar();
    this->LengthValue = Value;
    this->PolarCartesian();
}
//------------------------------------------------------------------------------
TCoordinate  aVector(double xValue , double yValue)	    //產生一個向量
{
    return  TCoordinate(xValue , yValue);
}
//------------------------------------------------------------------------------
TCoordinate  operator*(TCoordinate op , double scale)	//向量乘N倍
{
    op.x *= scale;
    op.y *= scale;
    return op;
}
//------------------------------------------------------------------------------
TCoordinate  operator*(double scale , TCoordinate op)	//向量乘N倍
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
