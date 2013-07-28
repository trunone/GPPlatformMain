//=============================================
//File name: Stra_PathPlan
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/05
//
//Function :Path plan module, acrooding to the vector
//          which strategy assing to plan the trajectory
//===============================================
#ifndef Stra_PathPlanH

#define Stra_PathPlanH

#include "StrategyModule.h"
#include "TCoordinate.h"
#include "StrategyStatus.h"
#include <math.h>
#include <stdio.h>
#include <tinyxml.h>
//----------------------------------------------------------------------------
namespace Robot
{
	class Stra_PathPlan : public StrategyModule
	{
	public:
    		~Stra_PathPlan();   //žÑºcšçŠ¡

		static Stra_PathPlan*  GetInstance() {return m_UniqueInstance;}
//    		virtual string  ParameterReset(void);           //°ÑŒÆ­«žm
	    void  Initialize(void);                  //ªì©l€Æ
	    void  Process(void);                     //°õŠæšçŒÆ
		inline double GetMin(const double &dX_,const double &dY_){return (dX_>= dY_)?dY_:dX_;} 

		int LoadXMLSettings(TiXmlElement* element);  // load xml		

	private:
        static Stra_PathPlan* m_UniqueInstance; 

		Stra_PathPlan();      //«ØºcšçŠ¡
			
        void PathPlan( void );
		
		double Trajectory( void );

		int DetourConst;

		double P1_Dis;          //¥ØŒÐ¶ZÂ÷

		double P1_CutAng;       //¥ØŒÐš€«×
	
		double P2_Dis;          //¥ØŒÐ2¶ZÂ÷(ÂùÂIžô®|³W¹º¥Î)

		double P2_CutAng;       //¥ØŒÐ2š€«×(ÂùÂIžô®|³W¹º¥Î)

	};
}
#endif
