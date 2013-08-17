//=============================================
//File name: AstarTool
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/10
//
//Function :AStar alogrithm
//==============================================
#ifndef AstarH
#define AstarH
#include "StrategyModule.h"
#include "TCoordinate.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <tinyxml.h>

#define Def_Open 1
#define Def_Closed 2
using namespace std;
namespace Robot{

	typedef struct
	{
    		TCoordinate Pos;
    		short Weight;
	}tsNeighbor;

	typedef struct
	{
    		vector<tsNeighbor> Neighbor;
    		short Status;
    		int Weight;
    		TCoordinate Father;
    		long F,G,H;
	}tsNode;

	class Nodelist{
		public:
			int x,y;
			short Weight;
			Nodelist(int x, int y, short Weight){
				this->x = x;
				this->y = y;
				this->Weight = Weight; 
			}
			inline bool operator < (const Nodelist &rhs) const {return Weight < rhs.Weight;}
		};

	class NodelistFinder{
		private:
			int Weight;
		public:
			NodelistFinder(const int n):Weight(n){}
			bool operator ()(const vector<Nodelist>::value_type & value){
				return value.Weight > this->Weight;
			}
	};

	
	class AstarTool
	{	
	public:
		static AstarTool* GetInstance() { return m_UniqueInstance; }		
		    		
		AstarTool();

	    void  Main( TCoordinate Start , TCoordinate Goal );
	    void  AdjustPath( void );
	    void  CleanList( void );

	    void OneStepInitial( TCoordinate Start , TCoordinate Goal );
	    void OneStepExe();
	    unsigned char CurrentStatus;
	    unsigned char ObstacleThreshold;

	    vector <TCoordinate> SmoothPath;

		int LoadXMLSettings (TiXmlElement* element);

	    vector < vector<tsNode> > Map;
	    vector <TCoordinate> Path;
	private:

 
		static AstarTool* m_UniqueInstance;
    		
		void SearchNeighbor( TCoordinate Current );
	    void SearchNeighbor_8Connect( TCoordinate Current );
	    unsigned int CheckPath_Same( unsigned int PathNum );
	    unsigned int CheckPath_Diff( unsigned int PathNum );

	    vector<Nodelist> OpenList;
	    vector<TCoordinate> ClosedList;

	    TCoordinate StartNode,GoalNode;

	    int NodeResolution;


	};
}

#endif
