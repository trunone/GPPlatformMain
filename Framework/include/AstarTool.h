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
//===============================================
#ifndef AstarH
#define AstarH
#include "StrategyModule.h"
#include "BinaryHeapTool.h"
#include "TCoordinate.h"
#include <vector>
#define Def_Open 1
#define Def_Closed 2
#include <tinyxml.h>
namespace Robot{

	typedef struct
	{
    		TCoordinate Pos;
    		unsigned char Weight;
	}tsNeighbor;

	typedef struct
	{
    		vector<tsNeighbor> Neighbor;
    		unsigned char Status;
    		unsigned char Weight;
    		TCoordinate Father;
    		short F,G,H;
	}tsNode;
	
	class AstarTool
	{	
	public:
		static AstarTool* GetInstance() { return m_UniqueInstance; }		
		    		
		AstarTool();

    	void  Main( TCoordinate Start , TCoordinate Goal );
    	void  AdjustPath( void );
    	void  CleanList( void );

    	void  AssignMap( vector< vector<tsNode> > &Map,
            int W, int H, int Resolution );

    	void OneStepInitial( TCoordinate Start , TCoordinate Goal );
    	void OneStepExe();
    	unsigned char CurrentStatus;
    	unsigned char ObstacleThreshold;

    	vector< vector<tsNode> > Map;
    	vector<TCoordinate> Path;

    	vector<TCoordinate> SmoothPath;

		int LoadXMLSettings (TiXmlElement* element);   // load xml
		int LoadXMLSettings_GridMap (TiXmlElement* element);
		static MapX=61,MapY=61;
		static vector< vector<int> > GridMapVector(MapY,< vector<int>(MapX));
	private:


		static AstarTool* m_UniqueInstance;
    		
		void SearchNeighbor( TCoordinate Current );
    	void SearchNeighbor_8Connect( TCoordinate Current );
    	unsigned int CheckPath_Same( unsigned int PathNum );
    	unsigned int CheckPath_Diff( unsigned int PathNum );

    	Heap::BinaryHeap<int> OpenList;
    	vector<TCoordinate> ClosedList;

    	TCoordinate StartNode,GoalNode;

    	int NodeResolution;
    	int MapWidth, MapHeight;

	};
}

#endif
