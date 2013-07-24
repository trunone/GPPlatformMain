#include "ParticleFilter.h"

using namespace Robot;
using namespace std;

ParticleFilter* ParticleFilter::m_UniqueInstance = new ParticleFilter();


void ParticleFilter::Initialize(void)
{


}


void ParticleFilter::Process( void )
{
	if(LocationStatus::FlagNewFeedback){
		LocationStatus::FB_Movement.Position =LocationStatus::FB_Movement.Position + (LocationStatus::LaserGap << LocationStatus::FB_Movement.Direction) - LocationStatus::LaserGap;
		ParticleFilterTool::GetInstance()->PredictionParticles();
		ParticleFilterTool::GetInstance()->CorrectParticles( LocationStatus::Position.x, LocationStatus::Position.y, LocationStatus::Handle, 50);

		PastMovement += LocationStatus::FB_Movement.Position.Length();
		PastDirection += fabs(LocationStatus::FB_Movement.Direction);
	}

	if(LocationStatus::FlagRenew){
		LocationStatus::FlagLaserBusy = true;		    
		/*	
		for( int i=0 ,idx3step = 0; i < Laser->ScanNum ; i++ )
		{                
			//--- 提供硬體中所需的雷射資訊
            if(!Share_Info->SimuInfo->FlagSimulator){
            		Share_Info->HdwInfo->LaserInfo.ScanArray[i] = LaserData[i]/10;
            }else{
                    Share_Info->HdwInfo->LaserInfo.ScanArray[i] = LaserData[i];
            }
                //--- 提供定位中所需的雷射資訊 (每3條取一條)
            if ( i%3 == 0){ Share_Info->LocInfo->ScanLineData[idx3step++] = Share_Info->HdwInfo->LaserInfo.ScanArray[i]; }
				           
		}*/

        LocationStatus::FlagLaserBusy = false;

        //---- 計算蒙地卡羅粒子感測修正 (當總位移量 或 總旋轉量 超過設定數值後執行)
        if (PastMovement > Def_ResamplingDis|| fabs(PastDirection) > Def_ResamplingAngle ||
        	LocationStatus::FlagLocInit || LocationStatus::FlagCoerceEvaluatuon  )
        {
        	if(LocationStatus::FlagEvaluatuonEnable || LocationStatus::FlagCoerceEvaluatuon)
            {
            	// Correction Step of PF
                ParticleFilterTool::GetInstance()->EvaluatuonParticles();
                ParticleFilterTool::GetInstance()->ResamplingParticles();

                LocationStatus::FlagLocInit = false;
                PastMovement  = 0;
                PastDirection = 0;
            }
        }
        LocationStatus::Position = ParticleFilterTool::GetInstance()->BestParticle.Position;
        LocationStatus::Handle   = ParticleFilterTool::GetInstance()->BestParticle.Direction;
        LocationStatus::Position.x -= Def_LaserGap * cos(LocationStatus::Handle);
        LocationStatus::Position.y -= Def_LaserGap * sin(LocationStatus::Handle);
	
	}
}
void ParticleFilter::SetParticleFilter(int W, int H, unsigned char *Mapdata){
	LocationStatus::LaserGap =aVector(Def_LaserGap,0);
    //-------set virtual map---------------------------------------
    tsBmpVirtualMap.Width  = W;
    tsBmpVirtualMap.Height = H;
    tsBmpVirtualMap.ImgData= Mapdata;
    //-------Localization setting------------------------
    ProbEvaluation::GetInstance()->AssignVirtualMap(&tsBmpVirtualMap);
    ProbEvaluation::GetInstance()->CameraImageScanLineDistance = LocationStatus::ScanLineData;
    //------------------------------------------------------------------------------
    InitialParticleFilter();

}
void ParticleFilter::InitialParticleFilter(){
	LocationStatus::Position = LocationStatus::SetRobotPosition;
    LocationStatus::Handle   = LocationStatus::SetRobotHandle;

    TCoordinate PosTmp = LocationStatus::SetRobotPosition + (LocationStatus::LaserGap << LocationStatus::SetRobotHandle);

    ParticleFilterTool::InitialParticles(  Def_ParticlesNum, PosTmp.x, PosTmp.y, LocationStatus::SetRobotHandle,  10 );  // PF灑點

    PastMovement = 0;
    PastDirection= 0;
    LocationStatus::FlagLocInit  = true;
}
