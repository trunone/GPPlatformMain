//=============================================
//File name: Stra_Avoid
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/09
//
//Function :Robot avoidance strategy
//===============================================
#ifndef STRA_AVOIDH
#define STRA_AVOIDH
#include "TCoordinate.h"
#include "StrategyModule.h"
#include "StrategyStatus.h"
#include <stdio.h>
#include <math.h>
namespace Robot{
	class Stra_Avoid : public StrategyModule
	{
	public:

		static Stra_Avoid*  GetInstance() {return m_UniqueInstance;}
    		
		~Stra_Avoid();   

   	 	//void ParameterReset(void);

    		void Initialize(void);                  

    	 	void Process(void);                     

    		TCoordinate CorrectV;

	private:

		static Stra_Avoid* m_UniqueInstance;
	    	
		Stra_Avoid();

    		TCoordinate ScanLineAvoidFunction( TCoordinate OrigTarget );

    		TCoordinate LeftForce, RightForce;

    		int *ScanLine;

    		float ScanStartAngle;

    		int ScanLineNum;

    		float ScanScale;

    		int SafeDistance;

    		float SafeArc_D;

    		float SafeArc_A;

    		float AvoidConfig1;

    		float AvoidForce;

    		short FixDirect;

    		int Lock_Avoid;

    		TCoordinate *Stone;

	};
}

#endif
