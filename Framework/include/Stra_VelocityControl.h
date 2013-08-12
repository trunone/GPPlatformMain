//=============================================
//File name: Stra_VelocityControl
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/05
//
//Function :Use S-Function to decide the robot velocity
//          x-axis parameter:dismax, dismin
//          y-axis parameter:speedmax, speedmin
//===============================================

#ifndef Stra_VelocityControlH
#define Stra_VelocityControlH
#include "StrategyModule.h"
#include <stdio.h>
#include <tinyxml.h>
namespace Robot{
	class Stra_VelocityControl : public StrategyModule
	{

	public:
		
    	~Stra_VelocityControl();

		static Stra_VelocityControl*  GetInstance() {return m_UniqueInstance;}
		//static string  ParameterReset(void);

        void Initialize(void);

        void Process(void);

		int LoadXMLSettings(TiXmlElement* element);  //LoadXMLSettings

	private:

		static Stra_VelocityControl* m_UniqueInstance; 

		Stra_VelocityControl();

		void VelocityTransform( double, double, double );

   		double S_Function( const double& , const double& , const double& , const double&, double );

   		double Raise_Function( double V, double Cmd );

   		static double GoalDistance;

   		static double GoalAngle;

   		static double HandleAngle;

		static double DistanceMax;

   		static double DistanceMin;

   		static double SpeedMax;

        static double SpeedMin;

		static double ThetaMax;

   		static double ThetaMin;

        static double OmegaMax;

        static double OmegaMin;

        static double SpeedCmd;

   	 	static double SpeedTmp;

	};
}
#endif
