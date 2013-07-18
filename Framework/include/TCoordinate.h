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

	inline void CartesianPolar(void);				//�����y�� �� ���y��
	inline void PolarCartesian(void);				//���y�� �� �����y��
	
	double AngleValue;						        //�V�q���y�Ш���
	double LengthValue;						        //�V�q����

public:

	double x;
	double y;
//------------��¦ �����禡----------
	//�غc�禡
	TCoordinate(double xValue,double yValue);       //(X,Y)
	TCoordinate(double sita);					    //(�V�q����)
	TCoordinate();							        //�غc �禡
	//�Ѻc�禡
	~TCoordinate();							        //�Ѻc �禡
	
//------------�B��l����--------------
    bool operator == (TCoordinate op2);	            //�V�q�O�_�۵�
	TCoordinate operator + (TCoordinate op2);       //��V�q�ۥ[
	TCoordinate operator - (TCoordinate op2);       //��V�q�۴�

	TCoordinate operator << (double ds);	        //�V�q����(���ץ[)
	TCoordinate operator >> (double ds);	        //�V�q�k��(���״�)

//---------- -���� �禡--------------
	double	Angle();						        //�V�q������
	double	Length();						        //�V�q������

	TCoordinate UnitVector();					    //�V�q�����V�q
	double  dot(TCoordinate op);				    //�V�q���n
	double  cross(TCoordinate op);	                //��V�~�n�oZ��V�s�q

	void	AssignXY(double xValue,double yVlaue);  //���d���y��
	void	AssignAngle(double Vlaue);				//��﷥�y�Ш���
	void	AssignLength(double Vlaue);				//��﷥�y�Ъ���
	
	//-----------------------------------------------------------------
	static TCoordinate  aVector(double xValue , double yValue);	    //���ͤ@�ӦV�q
	friend TCoordinate  operator*(TCoordinate op , double scale);	//�V�q��N��
	friend TCoordinate  operator*(double scale , TCoordinate op);	//�V�q��N��

};

double NormalizeAngle(double Angle);			//���W������ -M_PI < Angle < M_PI

//---------------------------------------------------------------------------
#endif
