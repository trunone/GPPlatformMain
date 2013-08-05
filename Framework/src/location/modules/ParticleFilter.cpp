#include "ParticleFilter.h"

#define Def_LaserGap 10

using namespace Robot;
using namespace std;

ParticleFilter* ParticleFilter::m_UniqueInstance = new ParticleFilter();

ParticleFilter::ParticleFilter() {
}

ParticleFilter::~ParticleFilter() {
}

void ParticleFilter::Initialize(void)
{
    LocationStatus::LaserGap = aVector(Def_LaserGap,0);

    //-------Localization setting------------------------
    ProbEvaluation::GetInstance()->AssignVirtualMap();
    //------------------------------------------------------------------------------
    InitialParticleFilter();

}

void ParticleFilter::Process( void )
{
	LocationStatus::FB_Movement.Position =LocationStatus::FB_Movement.Position + (LocationStatus::LaserGap << LocationStatus::FB_Movement.Direction) - LocationStatus::LaserGap;
	PredictionParticles();
    printf("%f, %f, %f\n", BestParticle.Position.x, BestParticle.Position.y, BestParticle.Direction);
	CorrectParticles( LocationStatus::Position.x, LocationStatus::Position.y, LocationStatus::Handle, 50);
    printf("Correct Done!\n");
	PastMovement += LocationStatus::FB_Movement.Position.Length();
	PastDirection += fabs(LocationStatus::FB_Movement.Direction);

    //---- 計算蒙地卡羅粒子感測修正 (當總位移量 或 總旋轉量 超過設定數值後執行)
    //if (PastMovement > Def_ResamplingDis|| fabs(PastDirection) > Def_ResamplingAngle ||
    //    LocationStatus::FlagLocInit || LocationStatus::FlagCoerceEvaluatuon  )
    if(true)
    {
        if(LocationStatus::FlagEvaluatuonEnable || LocationStatus::FlagCoerceEvaluatuon)
        {
            // Correction Step of PF
            EvaluatuonParticles();
    printf("Evaluatuon Done!\n");
            ResamplingParticles();
    printf("Resampling Done!\n");

            LocationStatus::FlagLocInit = false;
            PastMovement  = 0;
            PastDirection = 0;
        }
    }
    LocationStatus::Position = BestParticle.Position;
    LocationStatus::Handle   = BestParticle.Direction;
    LocationStatus::Position.x -= Def_LaserGap * cos(LocationStatus::Handle);
    LocationStatus::Position.y -= Def_LaserGap * sin(LocationStatus::Handle);
}

void ParticleFilter::InitialParticleFilter(){
	LocationStatus::Position = LocationStatus::SetRobotPosition;
    LocationStatus::Handle   = LocationStatus::SetRobotHandle;

    TCoordinate PosTmp = LocationStatus::SetRobotPosition + (LocationStatus::LaserGap << LocationStatus::SetRobotHandle);

    //InitialParticles( Def_ParticlesNum, PosTmp.x, PosTmp.y, LocationStatus::SetRobotHandle,  10 );  // PF灑點
    InitialParticles( Def_ParticlesNum );

    PastMovement = 0;
    PastDirection= 0;
    LocationStatus::FlagLocInit  = true;
}

string ParticleFilter::InitialParticles(int ParticlesNum)
{

    if(NULL)
        return "ParticleFilter Intial Failed" ;
    else{
        srand(time(NULL)+rand());           //selection rand model
        Particles.clear();

        tsParticle tempParticle;
        BestParticle.Probabilty = 0;
        for(int i=0 ; i<ParticlesNum ; i++ ){
            tempParticle.Position.x = (ProbEvaluation::GetInstance()->VirtualLineMap.cols)*((float)rand()/(float)RAND_MAX) ;
            tempParticle.Position.y = (ProbEvaluation::GetInstance()->VirtualLineMap.rows)*((float)rand()/(float)RAND_MAX) ;
            tempParticle.Direction  = 2.0*M_PI * ((float)rand()/(float)RAND_MAX);
            tempParticle.Probabilty = 1.0/ParticlesNum; 

            if(tempParticle.Probabilty > BestParticle.Probabilty){
                BestParticle = tempParticle;
                BestParticleNum = i;
            }

            Particles.push_back(tempParticle);
        }
        return "ParticleFilter Intial Successful" ;
    }
}
//------------------------------------------------------------------------------
string ParticleFilter::InitialParticles(int ParticlesNum,int x ,int y , float r,float range)
{
    if(NULL)
        return "ParticleFilter Intial Failed" ;
    else{
        srand(time(NULL)+rand());           //selection rand model
        Particles.clear();

        tsParticle tempParticle;
        BestParticle.Probabilty = 0;
        for(int i=0 ; i<ParticlesNum ; i++ ){
            tempParticle.Position.x = x+range * NormalRand::GetInstance()->randn(0,1);
            tempParticle.Position.y = y+range * NormalRand::GetInstance()->randn(0,1);
            tempParticle.Direction  = r+M_PI/18.0 * NormalRand::GetInstance()->randn(0,1);
            tempParticle.Probabilty = ProbEvaluation::GetInstance()->GetProbability(  tempParticle.Position.x,
                                                                                    tempParticle.Position.y,
                                                                                    tempParticle.Direction);
            if(tempParticle.Probabilty > BestParticle.Probabilty){
                BestParticle = tempParticle;
                BestParticleNum = i;
            }

            Particles.push_back(tempParticle);
        }
        BestParticle.Position.x = x;
        BestParticle.Position.y = y;
        BestParticle.Direction = r;
        return "ParticleFilter Intial Successful";
    }

}
//------------------------------------------------------------------------------
string ParticleFilter::PredictionParticles()
{
    int i=0 , i_size;

    i_size = (int)Particles.size();
    if(i_size <=0 ) return "ParticleFilter Prediction Failed" ;
    // ¨½µ{­p¸ê°T -> ¼Ë¥»¹w´ú
    BestParticle.Position  = BestParticle.Position + (LocationStatus::FB_Movement.Position<< BestParticle.Direction) ;
    BestParticle.Direction += LocationStatus::FB_Movement.Direction;
    TCoordinate prdMotion;
    double prdRotation1, prdRotation2;
    for(i=0 ; i< i_size; i++){
        //----------------------
        /*prdMotion = LocationStatus::FB_Movement.Position * ( 1 + MoveErrorRate * RandN->randn());
        double tmpNoise;
        tmpNoise = Ro2MoveErrorRate * RandN->randn();
        prdMotion.x += cos(LocationStatus::FB_Movement.Direction) * tmpNoise;
        prdMotion.y += sin(LocationStatus::FB_Movement.Direction) * tmpNoise;

        TCoordinate tmpPos;
        tmpPos = LocationStatus::FB_Movement.Position << Particles[i].Direction;
        prdRotation1 = tmpPos.Angle() * ( 1 + RotateErrorRate * RandN->randn() )
                       + ( LocationStatus::FB_Movement.Position.Length() * Mo2RotateErrorRate * RandN->randn() );
        prdRotation2 = LocationStatus::FB_Movement.Direction * ( 1 + RotateErrorRate * RandN->randn() )
                       + ( LocationStatus::FB_Movement.Position.Length() * Mo2RotateErrorRate * RandN->randn() ); //*/

        // ²¾°Ê¶q¹w´ú
        //Particles[i].Position = Particles[i].Position + ( prdMotion << prdRotation1 );
        Particles[i].Position   = Particles[i].Position +
                                        ( LocationStatus::FB_Movement.Position << Particles[i].Direction ) *( 1 + MoveErrorRate * NormalRand::GetInstance()->randn(0,1) );
        // Âà°Ê¶q¹w´ú
        //Particles[i].Direction += prdRotation2;
        Particles[i].Direction += LocationStatus::FB_Movement.Direction * ( 1 + RotateErrorRate * NormalRand::GetInstance()->randn(0,1));
    }
    //--------------------
    return "ParticleFilter Divination Successful"; 
}
//------------------------------------------------------------------------------
string ParticleFilter::CorrectParticles()
{
    int i=0 , i_size;
    i_size = (int)Particles.size();
    for(i=0 ; i< i_size; i++){

        if(   Particles[i].Position.x < 0 || Particles[i].Position.y < 0
            ||Particles[i].Position.x > ProbEvaluation::GetInstance()->VirtualLineMap.cols
            ||Particles[i].Position.y > ProbEvaluation::GetInstance()->VirtualLineMap.rows)
        {
            Particles[i].Position.x = (ProbEvaluation::GetInstance()->VirtualLineMap.cols) * ((float)rand()/(float)RAND_MAX) ;
            Particles[i].Position.y = (ProbEvaluation::GetInstance()->VirtualLineMap.rows)* ((float)rand()/(float)RAND_MAX) ;
            Particles[i].Direction  = 2*M_PI * ((float)rand()/(float)RAND_MAX);

        }

    }

    return "ParticleFilter Correct Successful" ;
}
//------------------------------------------------------------------------------
string ParticleFilter::CorrectParticles( int x,int y,float r,float range )
{
	int i=0 , i_size;
	i_size = (int)Particles.size();
	for(i=0 ; i< i_size; i++){

		if(   Particles[i].Position.x < 0 || Particles[i].Position.y < 0
			||Particles[i].Position.x > ProbEvaluation::GetInstance()->VirtualLineMap.cols
			||Particles[i].Position.y > ProbEvaluation::GetInstance()->VirtualLineMap.rows)
		{

			Particles[i].Position.x =  x+range *((float)rand()/(float)RAND_MAX);
			if ( Particles[i].Position.x < 0 )
			{
				Particles[i].Position.x  = fabs(Particles[i].Position.x );
			}
			else if(Particles[i].Position.x > ProbEvaluation::GetInstance()->VirtualLineMap.cols )
			{
				Particles[i].Position.x = ProbEvaluation::GetInstance()->VirtualLineMap.cols - 
                                                                       (Particles[i].Position.x - ProbEvaluation::GetInstance()->VirtualLineMap.cols);
			}

			Particles[i].Position.y = y+range *((float)rand()/(float)RAND_MAX);
			if ( Particles[i].Position.y < 0 )
			{
				Particles[i].Position.y  = fabs(Particles[i].Position.y );
			}
			else if(Particles[i].Position.y > ProbEvaluation::GetInstance()->VirtualLineMap.rows )
			{
				Particles[i].Position.y = ProbEvaluation::GetInstance()->VirtualLineMap.rows - 
					(Particles[i].Position.y - ProbEvaluation::GetInstance()->VirtualLineMap.rows);
			}

			Particles[i].Direction  = r+M_PI/18 * NormalRand::GetInstance()->randn(0,1);
		}

	}

	return "ParticleFilter Correct Successful" ;

}
//------------------------------------------------------------------------------
string ParticleFilter::EvaluatuonParticles()
{
    vector<tsParticle> TempParticles;
    vector<tsParticle>::iterator itParticles = Particles.begin();
    while(itParticles != Particles.end()){
        itParticles->Probabilty =
            ProbEvaluation::GetInstance()->GetProbability( itParticles->Position.x,
                                                         itParticles->Position.y,
                                                         itParticles->Direction); //*/
        if(itParticles == Particles.begin()) TempParticles.push_back(*itParticles) ;
        vector<tsParticle>::iterator it = TempParticles.begin();
        while(it != TempParticles.end()){
            if( itParticles->Probabilty > it->Probabilty){
                TempParticles.insert(it , *itParticles);
                break;
            }
            ++it;
        }
        if(it == TempParticles.end()) TempParticles.push_back(*itParticles);
        ++itParticles;
    }
    Particles.clear();
    Particles = TempParticles;
    TempParticles.clear();

    //BestParticle = Particles[0];
    return "ParticleFilter Evaluatuon Successful" ;
}
//------------------------------------------------------------------------------

string ParticleFilter::ResamplingParticles()
{

    int i=0 , i_size;
    i_size = (int)Particles.size();

    Particles[0].acProbability = Particles[0].Probabilty;
    for(i=1 ; i< i_size; i++){
        Particles[i].acProbability = Particles[i-1].acProbability + Particles[i].Probabilty;
    }
    //-----------------
    tsParticle ParticleTemp;

    int start_i = (int)((float)i_size*BPKeepRate) ;
    //keep best 30% of particle
    double Rdis,Rangle;
    for(i=start_i ; i<i_size ;i++){
        double dart;
        dart = Particles[start_i].acProbability*NormalRand::GetInstance()->randn(0,1);
        int k = 0;
        if(Particles[start_i].acProbability >= 0){
            ParticleTemp.acProbability = 0;
            while(ParticleTemp.acProbability < dart){
                ParticleTemp = Particles[k];
                k++;
                if (k>start_i) break;
            }

            Rdis   = SearchRate*((float)rand()/(float)RAND_MAX)*ProbEvaluation::GetInstance()->VirtualLineMap.cols;
            Rangle = 2*M_PI*((float)rand()/(float)RAND_MAX);
            Particles[i].Position.x = ParticleTemp.Position.x + Rdis*cos(Rangle);
            Particles[i].Position.y = ParticleTemp.Position.y + Rdis*sin(Rangle);
            Particles[i].Direction  = ParticleTemp.Direction
                                            +M_PI/90*((float)rand()/(float)RAND_MAX);
        }
    } //*/
    //---------------
    // position estimation _yao
    ParticleTemp.Position.x = 0.0;
    ParticleTemp.Position.y = 0.0;
    ParticleTemp.Direction  = 0.0;
    for(i=0 ; i< i_size; i++){
        ParticleTemp.Position.x += Particles[i].Position.x;
        ParticleTemp.Position.y += Particles[i].Position.y;
        ParticleTemp.Direction  += Particles[i].Direction;
    }
    BestParticle.Position.x = ParticleTemp.Position.x / i_size;
    BestParticle.Position.y = ParticleTemp.Position.y / i_size;
    BestParticle.Direction  = ParticleTemp.Direction  / i_size;
    BestParticle.Probabilty = 0.0;
    BestParticle.acProbability = 0.0;
    //-------------
    return "ParticleFilter Selection Successful" ;
}

