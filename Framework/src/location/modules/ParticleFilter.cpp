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
    //---------------------------------------------------
    InitialParticleFilter();
    EvaluatuonParticles();
    ResamplingParticles();
}

void ParticleFilter::Process( void )
{
	LocationStatus::FB_Movement.Position =LocationStatus::FB_Movement.Position + (LocationStatus::LaserGap << LocationStatus::FB_Movement.Direction) - LocationStatus::LaserGap;
	PredictionParticles();
	CorrectParticles( LocationStatus::Position.x, LocationStatus::Position.y, LocationStatus::Handle, 50);
	PastMovement += LocationStatus::FB_Movement.Position.Length();
	PastDirection += fabs(LocationStatus::FB_Movement.Direction);

    //printf("Best: %f, %f, %f\n", BestParticle.Position.x, BestParticle.Position.y, BestParticle.Direction);
    //---- 計算蒙地卡羅粒子感測修正 (當總位移量 或 總旋轉量 超過設定數值後執行)
    //if (PastMovement > Def_ResamplingDis|| fabs(PastDirection) > Def_ResamplingAngle ||
    //    LocationStatus::FlagCoerceEvaluatuon  )
    if(true)
    {
        if(LocationStatus::FlagEvaluatuonEnable || LocationStatus::FlagCoerceEvaluatuon)
        {
            // Correction Step of PF
            EvaluatuonParticles();
            ResamplingParticles();

            PastMovement  = 0;
            PastDirection = 0;
        }
    }
    LocationStatus::Position = BestParticle.Position;
    LocationStatus::Handle   = BestParticle.Direction;
    LocationStatus::Position.x -= Def_LaserGap * cos(LocationStatus::Handle);
    LocationStatus::Position.y -= Def_LaserGap * sin(LocationStatus::Handle);
    //getchar();
}

void ParticleFilter::InitialParticleFilter(){

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
            //tempParticle.Position.x = (ProbEvaluation::GetInstance()->VirtualLineMap.cols)*((float)rand()/(float)RAND_MAX) ;
            tempParticle.Position.x = 20*((float)rand()/(float)RAND_MAX)-10+LocationStatus::Position.x;
            //tempParticle.Position.x = LocationStatus::Position.x;
            //tempParticle.Position.y = (ProbEvaluation::GetInstance()->VirtualLineMap.rows)*((float)rand()/(float)RAND_MAX) ;
            tempParticle.Position.y = 20*((float)rand()/(float)RAND_MAX)-10+LocationStatus::Position.y;
            //tempParticle.Position.y = LocationStatus::Position.y;
            tempParticle.Direction  = M_PI * ((float)rand()/(float)RAND_MAX) - M_PI;
            //tempParticle.Direction = LocationStatus::Handle;
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
    for(i=0 ; i< i_size; i++){
        //Particles[i].Position   = Particles[i].Position +
        //                        ( LocationStatus::FB_Movement.Position << Particles[i].Direction ) *( 1 + MoveErrorRate * NormalRand::GetInstance()->randn(0,1) );
        Particles[i].Position   = Particles[i].Position +
                                ( LocationStatus::FB_Movement.Position << Particles[i].Direction );
//        Particles[i].Direction += LocationStatus::FB_Movement.Direction * ( 1 + RotateErrorRate * NormalRand::GetInstance()->randn(0,1));
        Particles[i].Direction += LocationStatus::FB_Movement.Direction;
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

			Particles[i].Position.x =  x+range *(((float)rand()/(float)RAND_MAX) - 0.5);
			if ( Particles[i].Position.x < 0 )
			{
				Particles[i].Position.x  = fabs(Particles[i].Position.x );
			}
			else if(Particles[i].Position.x > ProbEvaluation::GetInstance()->VirtualLineMap.cols )
			{
				Particles[i].Position.x = ProbEvaluation::GetInstance()->VirtualLineMap.cols - 
                                                                       (Particles[i].Position.x - ProbEvaluation::GetInstance()->VirtualLineMap.cols);
			}

			Particles[i].Position.y = y+range *(((float)rand()/(float)RAND_MAX) - 0.5);
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
        //printf("%f\n", itParticles->Probabilty);
        if(itParticles == Particles.begin()) TempParticles.push_back(*itParticles);
        else {
            vector<tsParticle>::iterator it = TempParticles.begin();
            while(it != TempParticles.end()){
                if( itParticles->Probabilty > it->Probabilty){
                    TempParticles.insert(it, *itParticles);
                    break;
                }
                ++it;
            }
            if(it == TempParticles.end()) TempParticles.push_back(*itParticles);
        }
        ++itParticles;
    }
    Particles.clear();
    Particles = TempParticles;
    TempParticles.clear();

    //printf("Best: %f\n", Particles[0].Probabilty);

    return " ";
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
        double dart = Particles[start_i].acProbability*((float)rand()/(float)RAND_MAX);
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
                                            +M_PI/90*(((float)rand()/(float)RAND_MAX)-0.5);
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

