#include <math.h>
#include "AstarTool.h"
using namespace Robot;

AstarTool* AstarTool::m_UniqueInstance = new AstarTool();

AstarTool::AstarTool()
{
    StartNode = TCoordinate::aVector(-1,-1);
    GoalNode  = TCoordinate::aVector(-1,-1);

    AstarTool::NodeResolution = 10;

    CurrentStatus = 0;
    ObstacleThreshold = 200;
}
//---------------------------------------------------------------------------
void AstarTool::CleanList( void )
{
    Path.clear();
    ClosedList.clear();
    while( (OpenList.minnum()).x != -1 && (OpenList.minnum()).y != -1 ){ OpenList.extract_min(); }
}
//---------------------------------------------------------------------------xml
int AstarTool::loadxml (TiXmlElement* element){
	if(element != NULL){	
		TiXmlElement* modelchild;
		modelchild=element->FirstChildElement("MapGrid_Config");
		if(modelchild != NULL){
			modelchild->Attribute("length",&MapHeight);
			modelchild->Attribute("Width", &MapWidth);
			modelchild->Attribute("NodeResolution", &NodeResolution);
		}
		
	}
}
//---------------------------------------------------------------------------
void AstarTool::AssignMap( vector< vector<tsNode> > &Map,
                           int W, int H, int Resolution )
{
    Map = Map;
    //MapWidth = W;
    //MapHeight= H;
    //NodeResolution= Resolution;
}
//---------------------------------------------------------------------------
void AstarTool::Main( TCoordinate Start , TCoordinate Goal )
{
    CurrentStatus += (CurrentStatus == 255 ? 6 : 5);

    TCoordinate Front,Father;
    //---- record the start and goal -----
    StartNode.x = (int)Start.x/NodeResolution;
    StartNode.y = (int)Start.y/NodeResolution;
    GoalNode.x  = (int)Goal.x /NodeResolution;
    GoalNode.y  = (int)Goal.y /NodeResolution;

    //---- initial the list information
    Map[StartNode.x][StartNode.y].Father = StartNode;
    Map[StartNode.x][StartNode.y].G = 0;
    Map[StartNode.x][StartNode.y].H = NodeResolution*(( GoalNode - StartNode ).Length());
    Map[StartNode.x][StartNode.y].F = Map[StartNode.x][StartNode.y].G + Map[StartNode.x][StartNode.y].H;

    OpenList.insert( Map[StartNode.x][StartNode.y].F ,StartNode.x, StartNode.y );

    //---- execute the A Star
    Front.x = OpenList.minnum().x;
    Front.y = OpenList.minnum().y;
    //--------------------------------------------------------------------------
    //-------------- Stop Condition --------------------------------------------
    //--------------------------------------------------------------------------
    while( (Front.x != -1        || Front.y != -1) &&
           (Front.x != GoalNode.x || Front.y != GoalNode.y) )
    {
        OpenList.extract_min();

        Map[Front.x][Front.y].Status = CurrentStatus-Def_Closed;

        ClosedList.push_back( Front );
        SearchNeighbor_8Connect( Front );

        Front.x = OpenList.minnum().x;
        Front.y = OpenList.minnum().y;

    }
    //--------------------------------------------------------------------------
    //-------------- Return Path Info ------------------------------------------
    //--------------------------------------------------------------------------
    if( Front.x == GoalNode.x && Front.y == GoalNode.y )
    {
        Path.insert( Path.begin(), Goal );
        Father = GoalNode;
        while( StartNode.x != Father.x || StartNode.y != Father.y   )
        {
            Father = Map[ Father.x ][ Father.y ].Father;
            if(StartNode.x == Father.x && StartNode.y == Father.y)
                Path.insert( Path.begin(), Start );
            else
                Path.insert( Path.begin(), (Father*NodeResolution)+ TCoordinate::aVector(NodeResolution/2,NodeResolution/2) );
        }
    }
}
//---------------------------------------------------------------------------
void AstarTool::SearchNeighbor_8Connect( TCoordinate Current )
{
    TCoordinate TmpPos;
    int TmpWeight;

    for( int i = -1; i < 2; i++ )
    {
        for( int j = -1; j < 2; j++ )
        {
            if( i==0 && j==0 ) continue;

            TmpPos    = Current + TCoordinate::aVector( i, j );
            if( TmpPos.x < 0 || TmpPos.y < 0 || TmpPos.x >= MapWidth ||TmpPos.y >= MapHeight ) continue;

            TmpWeight = Map[TmpPos.x][TmpPos.y].Weight;

            if( TmpWeight < ObstacleThreshold  )
            {
                if( i*j==0 ){       // if neighbor is diagonal, then G is 14.
                    TmpWeight += NodeResolution;
                }else{
                    if( Map[TmpPos.x][Current.y].Weight > ObstacleThreshold ||
                        Map[Current.x][TmpPos.y].Weight > ObstacleThreshold ) continue;
                    TmpWeight += 1.4*NodeResolution;
                }
                
                if( Map[TmpPos.x][TmpPos.y].Status == CurrentStatus-Def_Open)
                {
                    if( Map[ TmpPos.x ][ TmpPos.y ].G > Map[Current.x][Current.y].G + TmpWeight )
                    {
                        Map[TmpPos.x][TmpPos.y].Father = Current;
                        Map[TmpPos.x][TmpPos.y].G = Map[Current.x][Current.y].G + TmpWeight;
                        Map[TmpPos.x][TmpPos.y].F = Map[TmpPos.x][TmpPos.y].G + Map[TmpPos.x][TmpPos.y].H;

                        OpenList.decrease_key( OpenList.Search(TmpPos.x,TmpPos.y) , Map[ TmpPos.x ][ TmpPos.y ].F );
                    }
                }
                else if( Map[TmpPos.x][TmpPos.y].Status != CurrentStatus-Def_Closed )
                {
                    Map[TmpPos.x][TmpPos.y].Status = CurrentStatus-Def_Open;
                    Map[TmpPos.x][TmpPos.y].Father = Current;
                    Map[TmpPos.x][TmpPos.y].G = Map[Current.x][Current.y].G + TmpWeight;
                    Map[TmpPos.x][TmpPos.y].H = NodeResolution*((GoalNode - TmpPos).Length());
                    Map[TmpPos.x][TmpPos.y].F = Map[TmpPos.x][TmpPos.y].G + Map[TmpPos.x][TmpPos.y].H;

                    OpenList.insert( Map[TmpPos.x][TmpPos.y].F ,TmpPos.x, TmpPos.y );
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void AstarTool::SearchNeighbor( TCoordinate Current )
{
    TCoordinate TmpPos;
    int TmpWeight;
    int Cnt1 = 0;
    int TmpNearSize = Map[Current.x][Current.y].Neighbor.size();

    while( Cnt1 < TmpNearSize )
    {

        TmpPos    = Map[Current.x][Current.y].Neighbor[Cnt1].Pos;
        TmpWeight = Map[Current.x][Current.y].Neighbor[Cnt1].Weight;

        if( TmpWeight < ObstacleThreshold  )
        {
            if( Map[Current.x][Current.y].Status == CurrentStatus-Def_Open)
            {
                if( Map[ TmpPos.x ][ TmpPos.y ].G > Map[Current.x][Current.y].G + TmpWeight )
                {
                    Map[TmpPos.x][TmpPos.y].Father = Current;
                    Map[TmpPos.x][TmpPos.y].G = Map[Current.x][Current.y].G + TmpWeight;
                    Map[TmpPos.x][TmpPos.y].F = Map[TmpPos.x][TmpPos.y].G + Map[TmpPos.x][TmpPos.y].H;

                    OpenList.decrease_key( OpenList.Search(TmpPos.x,TmpPos.y) , Map[ TmpPos.x ][ TmpPos.y ].F );
                }
                break;
            }
            else if( Map[Current.x][Current.y].Status != CurrentStatus-Def_Closed )
            {
                Map[TmpPos.x][TmpPos.y].Status = CurrentStatus-Def_Open;
                Map[TmpPos.x][TmpPos.y].Father = Current;
                Map[TmpPos.x][TmpPos.y].G = Map[Current.x][Current.y].G + TmpWeight;
                Map[TmpPos.x][TmpPos.y].H = NodeResolution*((GoalNode - TmpPos).Length());
                Map[TmpPos.x][TmpPos.y].F = Map[TmpPos.x][TmpPos.y].G + Map[TmpPos.x][TmpPos.y].H;

                OpenList.insert( Map[TmpPos.x][TmpPos.y].F ,TmpPos.x, TmpPos.y );
            }
        }
        Cnt1++;
    }
}
//---------------------------------------------------------------------------
void AstarTool::AdjustPath( void )
{
    unsigned int tmp1,tmp2;

    if( Path.size() > 3 )
    {
        SmoothPath.clear();
        SmoothPath.push_back(Path.front());
        for(unsigned int i=2; i< Path.size()-1 ;i++)
        {
            tmp1 = CheckPath_Same( i );
            if( tmp1 == i )
            {
                tmp2 = CheckPath_Diff(i+1);
                i = ( tmp2 - i > 1 )? tmp2 : tmp1;
            }
            else
            {
                i = tmp1;
            }
            SmoothPath.push_back( Path[i] );
        }
    }
    else
    {
        SmoothPath = Path ;
    }
}
//---------------------------------------------------------------------------
unsigned int AstarTool::CheckPath_Diff( unsigned int PathNum )
{
    if( PathNum == Path.size()-2 ) return Path.size()-1;
    TCoordinate Vector1 = Path[PathNum-1] - Path[PathNum-2];
    TCoordinate Vector2 = Path[PathNum] - Path[PathNum-1];
    TCoordinate Vector3 = Path[PathNum+1] - Path[PathNum];
    if( Vector3 == Vector2 )
        if( (Vector1.x != Vector3.x) || (Vector1.y != Vector3.y) )
            return PathNum-1;
        else
            return PathNum;
    else
        return CheckPath_Diff( PathNum+1 );
}
//---------------------------------------------------------------------------
unsigned int AstarTool::CheckPath_Same( unsigned int PathNum )
{
    if( PathNum == Path.size()-2 ) return Path.size()-1;

    TCoordinate Vector1 = Path[PathNum] - Path[PathNum-1];
    TCoordinate Vector2 = Path[PathNum+1] - Path[PathNum];
    if( Vector1 == Vector2 )
        return CheckPath_Same( PathNum+1 );
    else
        return PathNum;
}
//---------------------------------------------------------------------------
void AstarTool::OneStepInitial( TCoordinate Start , TCoordinate Goal )
{
    CurrentStatus += (CurrentStatus == 255 ? 6 : 5);

    StartNode.x = Start.x/NodeResolution;
    StartNode.y = Start.y/NodeResolution;
    GoalNode.x  = Goal.x/NodeResolution;
    GoalNode.y  = Goal.y/NodeResolution;

    Map[StartNode.x][StartNode.y].Father = StartNode;
    Map[StartNode.x][StartNode.y].G = 0;
    Map[StartNode.x][StartNode.y].H = ( GoalNode - StartNode ).Length();
    Map[StartNode.x][StartNode.y].F = Map[StartNode.x][StartNode.y].G + Map[StartNode.x][StartNode.y].H;

    OpenList.insert( Map[StartNode.x][StartNode.y].F ,StartNode.x, StartNode.y );
}
//---------------------------------------------------------------------------
void AstarTool::OneStepExe()
{
    TCoordinate Father,Front;

    Front.x = OpenList.minnum().x;
    Front.y = OpenList.minnum().y;
    //--------------------------------------------------------------------------
    //-------------- Stop Condition --------------------------------------------
    //--------------------------------------------------------------------------
    if( (Front.x != -1        || Front.y != -1) &&
        (Front.x != GoalNode.x || Front.y != GoalNode.y) )
    {
        OpenList.extract_min();

        Map[Front.x][Front.y].Status = CurrentStatus-Def_Closed;

        ClosedList.push_back( Front );
        SearchNeighbor( Front );
    }
    Front.x = OpenList.minnum().x;
    Front.y = OpenList.minnum().y;
    //--------------------------------------------------------------------------
    //-------------- Return Path Info ------------------------------------------
    //--------------------------------------------------------------------------
    if( Front.x == GoalNode.x && Front.y == GoalNode.y )
    {
        Path.insert( Path.begin(), GoalNode*NodeResolution );
        Father = GoalNode;
        while( StartNode.x != Father.x || StartNode.y != Father.y   )
        {
            Father = Map[ Father.x ][ Father.y ].Father;
            Path.insert( Path.begin(), (Father*NodeResolution)+ TCoordinate::aVector(NodeResolution/2,NodeResolution/2) );
        }
    }
}
