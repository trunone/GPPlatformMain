//==============================================================================
// File Name: Type of Coordinate
//
// Author   : ICLAB MSL 2009 TEAM
//            Ho ChengYao, Teng HangChih, Yu ChiaJun, Chen ChiaYang
// E-mail   : bgho740325@hotmail.com (Ho ChengYao)
//
// Modifier :
// Phone    :
// E-mail   :
//
// Update   : 20090225
// Function : This Model is a basic Coodinate type
//
//==============================================================================
//---------------------------------------------------------------------------

#ifndef TCoordinateH
#define TCoordinateH
class TCoordinate
{

private:

	inline void CartesianPolar(void);				//直角座標 轉 極座標
	inline void PolarCartesian(void);				//極座標 轉 直角座標
	
	double AngleValue;						        //向量極座標角度
	double LengthValue;						        //向量長度

public:

	double x;
	double y;
//------------基礎 成員函式----------
	//建構函式
	TCoordinate(double xValue,double yValue);       //(X,Y)
	TCoordinate(double sita);					    //(向量角度)
	TCoordinate();							        //建構 函式
	//解構函式
	~TCoordinate();							        //解構 函式
	
//------------運算子重載--------------
    bool operator == (TCoordinate op2);	            //向量是否相等
	TCoordinate operator + (TCoordinate op2);       //兩向量相加
	TCoordinate operator - (TCoordinate op2);       //兩向量相減

	TCoordinate operator << (double ds);	        //向量左旋(角度加)
	TCoordinate operator >> (double ds);	        //向量右旋(角度減)

//---------- -成員 函式--------------
	double	Angle();						        //向量的角度
	double	Length();						        //向量的長度

	TCoordinate UnitVector();					    //向量的單位向量
	double  dot(TCoordinate op);				    //向量內積
	double  cross(TCoordinate op);	                //兩向外積得Z方向存量

	void	AssignXY(double xValue,double yVlaue);  //更改卡式座標
	void	AssignAngle(double Vlaue);				//更改極座標角度
	void	AssignLength(double Vlaue);				//更改極座標長度
	
	//-----------------------------------------------------------------
	static TCoordinate  aVector(double xValue , double yValue);	    //產生一個向量
	friend TCoordinate  operator*(TCoordinate op , double scale);	//向量乘N倍
	friend TCoordinate  operator*(double scale , TCoordinate op);	//向量乘N倍

};

double NormalizeAngle(double Angle);			//正規劃角度 -M_PI < Angle < M_PI

//---------------------------------------------------------------------------
#endif
