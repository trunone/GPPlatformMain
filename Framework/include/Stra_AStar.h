//=============================================
//File name: Stra_AStar
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/09
//
//Function :AStar Strategy
//===============================================
#ifndef Star_AStarH
#define Star_AStarH
#include "StrategyModule.h"
#include "StrategyStatus.h"
#include "LocationStatus.h"
#include "AstarTool.h"
#include "TCoordinate.h"
#include <tinyxml.h>
namespace Robot
{
	class Stra_AStar  : public StrategyModule
	{
	public:
		static Stra_AStar*  GetInstance() {return m_UniqueInstance;}
		
		~Stra_AStar();

		void Initialize(void);                 

		void Process(void);  

		int LoadXMLSettings(TiXmlElement* element);

		void Behavior_AstarPath( void );

	private:

        static Stra_AStar* m_UniqueInstance; 
		
		Stra_AStar();


		TCoordinate StartPos, GoalPos;

		bool CloseState;

		int PathErrRange;

		int AchieveErrRange;

		TCoordinate TargetVector,NextVector;

		TCoordinate VirtualPos;

	};
}

#endif
