#include "Stra_AStar.h"
using namespace Robot;

Stra_AStar* Stra_AStar::m_UniqueInstance = new Stra_AStar();
/*
Star_AStar::Star_AStar(void)
:TCommonUnit("./Strategy/StraConfig/Stra_AStar.txt", 2 )
{

	this->Caption   = "Stra_AStar";
        this->ParameterPath = "./Strategy/StraConfig/Stra_AStar.txt";
   	this->ParameterReset();
	this->AStar = this->Info->pVirtualAStar;
        this->GoalPos  = aVector(-999, -999);
        this->StartPos = aVector(-999, -999);
        this->CloseState = false;
}
*/
/*
void Star_AStar::ParameterReset(void)
{
    string str_ = this->Caption +" ParameterReset";
    this->PathErrRange		= this->Parameter[0];
    this->AchieveErrRange	= this->Parameter[1];
    this->bNewParameter = false;
    return str_;
}
*/
//-----------------------------------------------------------------
void Stra_AStar::Initialize(void)
{
	//string str_ = this->Caption + " Initial";

   	GoalPos = TCoordinate::aVector(-999, -999);

    	StartPos= TCoordinate::aVector(-999, -999);

    	CloseState = false;
}

//-----------------------------------------------------------------
void Stra_AStar::Process(void)
{

 //   if( this->bNewParameter ) this->ParameterReset();

	if( StrategyStatus::AStarPath.GoalPos  == TCoordinate::aVector(-999, -999) ||

        StrategyStatus::AStarPath.StartPos == TCoordinate:: aVector(-999, -999) ){ return ;}

    //----- ­YŠ³·sªº¥ØŒÐÂI ©Î °_ÂI «h­«·s­pºâžô®|
	if( !(GoalPos  == StrategyStatus::AStarPath.GoalPos) && !(StartPos == StrategyStatus::AStarPath.StartPos) )
	{
        	StartPos = StrategyStatus::AStarPath.StartPos;

        	GoalPos  = StrategyStatus::AStarPath.GoalPos;

        	AstarTool::GetInstance()->CleanList();

        	AstarTool::GetInstance()->Main( StartPos , GoalPos );

        	AstarTool::GetInstance()->AdjustPath();

		StrategyStatus::AStarPath.PCnt = 0;

        	//----- šÌ·Ó­pºâ¥Xªºžô®|²Ÿ°Ê

        	Behavior_AstarPath();
    	}else{

        	//----- šÌ·Ó­pºâ¥Xªºžô®|²Ÿ°Ê

        	Behavior_AstarPath();
    	}
}
//-----------------------------------------------------------------
void Stra_AStar::Behavior_AstarPath( void )
{
	int Length = 0;
	int Size = AstarTool::GetInstance()->SmoothPath.size();

	TCoordinate TmpGoal_V = AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt ] - LocationStatus::Position;

        if( StrategyStatus::AStarPath.PCnt < Size ){
		Length = TmpGoal_V.Length();
		if (StrategyStatus::AStarPath.PCnt == Size - 1){
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
                        			TargetVector = AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt ] -
                               			AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt-1 ];
		                        	NextVector = AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt+1 ] -
                               			AstarTool::GetInstance()->SmoothPath[ StrategyStatus::AStarPath.PCnt ];
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
	StrategyStatus::AStarPath.Status =StrategyStatus::etAchieve;
    }
}
   // static TInfo* Info;
