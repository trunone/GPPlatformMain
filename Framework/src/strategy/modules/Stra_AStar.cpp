#include "Stra_AStar.h"
using namespace Robot;
using namespace std;

Stra_AStar* Stra_AStar::m_UniqueInstance = new Stra_AStar();

Stra_AStar::Stra_AStar()
{

}

Stra_AStar::~Stra_AStar()
{

}

//-----------------------------------------------------------------
int Stra_AStar::LoadXMLSettings(TiXmlElement* element){
	if(element != NULL){				
		element->Attribute("PathErrRange", &PathErrRange);
		element->Attribute("AchieveErrRange", &AchieveErrRange);
	}
}

void Stra_AStar::Initialize(void)
{
	
   	GoalPos = aVector(-999, -999);

    StartPos= aVector(-999, -999);

    CloseState = false;

}

//-----------------------------------------------------------------
void Stra_AStar::Process(void)
{	
	//printf("enter astar module\n");
    if( StrategyStatus::AStarPath.GoalPos  == aVector(-999, -999) ||
        StrategyStatus::AStarPath.StartPos ==  aVector(-999, -999) ){ return ;}
	if( !(GoalPos  == StrategyStatus::AStarPath.GoalPos) && !(StartPos == StrategyStatus::AStarPath.StartPos) )
	{
			
        	StartPos = StrategyStatus::AStarPath.StartPos;

        	GoalPos  = StrategyStatus::AStarPath.GoalPos;
			
        	AstarTool::GetInstance()->CleanList();
			//printf("clean done\n");
        	AstarTool::GetInstance()->Main( StartPos , GoalPos );
			//printf("main done\n");
        	AstarTool::GetInstance()->AdjustPath();
			//printf("adj done\n");
			StrategyStatus::AStarPath.PCnt = 0;

        	Behavior_AstarPath();
			
			//printf("beh\n");
			
    	}else{
        	Behavior_AstarPath();

    	}
		
}
//-----------------------------------------------------------------
void Stra_AStar::Behavior_AstarPath( void )
{
	int Length = 0;
	int Size = AstarTool::GetInstance()->SmoothPath.size();
	//printf("AStarPath.PCnt %d Size  %d\n",StrategyStatus::AStarPath.PCnt,Size);
	TCoordinate TmpGoal_V = AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt ] - LocationStatus::Position;
    if( StrategyStatus::AStarPath.PCnt < Size ){
		Length = TmpGoal_V.Length();
        if(StrategyStatus::AStarPath.PCnt == Size - 1){
			StrategyStatus::Goal1 = TmpGoal_V >> LocationStatus::Handle;
			if( Length < AchieveErrRange ){
				StrategyStatus::AStarPath.PCnt++;
				StrategyStatus::AStarPath.Status = StrategyStatus::etAchieve;
			}
		}else{
            if( StrategyStatus::AStarPath.PCnt > 0 ){
				StrategyStatus::Goal1 = TmpGoal_V >> LocationStatus::Handle;
				if( Length < PathErrRange ){
                    if( !CloseState ){
                        TargetVector = AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt ] - AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt-1 ];
                        NextVector = AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt+1 ] - AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt ];
                        VirtualPos = LocationStatus::Position +(( TmpGoal_V.UnitVector() ) * (Length+10));
                        CloseState = true;
                    }
                    if( Length < PathErrRange  ) StrategyStatus::FixSpeed = 60;
                        StrategyStatus::Goal1 =(VirtualPos - LocationStatus::Position) >> LocationStatus::Handle;
                        if( NextVector.cross(TargetVector) * NextVector.cross( TmpGoal_V ) < 0 || Length < 10 ){
                            StrategyStatus::AStarPath.PCnt++;
                            CloseState = false;
                        }

				}
            }else{
				StrategyStatus::Goal1 = TmpGoal_V >> LocationStatus::Handle;
                if( Length < PathErrRange ){ StrategyStatus::AStarPath.PCnt++; }
            }

		}
		StrategyStatus::AStarPath.Status = StrategyStatus::etMotion;
    }else{
        StrategyStatus::AStarPath.Status = StrategyStatus::etAchieve;
    }

}
   
