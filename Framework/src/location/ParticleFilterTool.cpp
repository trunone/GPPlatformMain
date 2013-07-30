#include "ParticleFilterTool.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

//---------------------------------------------------------------------------
using namespace Robot;

ParticleFilterTool* ParticleFilterTool::m_UniqueInstance = new ParticleFilterTool();

ParticleFilterTool::ParticleFilterTool()
{
    RandN = new NormalRand(1,0.001);
}
//------------------------------------------------------------------------------

ParticleFilterTool::~ParticleFilterTool()
{
    Particles.clear();
}
//------------------------------------------------------------------------------

string ParticleFilterTool::InitialParticles(int ParticlesNum)
{

    if(ProbabilityEvaluation->VirtualLineMap == NULL)
        return "ParticleFilter Intial Failed" ;
    else{
        srand(time(NULL)+rand());           //selection rand model
        Particles.clear();

        tsParticle tempParticle;
        BestParticle.Probabilty = 0;
        for(int i=0 ; i<ParticlesNum ; i++ ){
            tempParticle.Position.x = (ProbabilityEvaluation->VirtualLineMap->Width)/2 * ((float)rand()/(float)RAND_MAX) ;
            tempParticle.Position.y = (ProbabilityEvaluation->VirtualLineMap->Height)* ((float)rand()/(float)RAND_MAX) ;
            tempParticle.Direction  = 2.0*M_PI * ((float)rand()/(float)RAND_MAX);
            tempParticle.Probabilty = ProbabilityEvaluation->GetProbability(  tempParticle.Position.x,
                                                                                    tempParticle.Position.y,
                                                                                    tempParticle.Direction);

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
string ParticleFilterTool::InitialParticles(int ParticlesNum,int x ,int y , float r,float range)
{
    if(ProbabilityEvaluation->VirtualLineMap == NULL)
        return "ParticleFilter Intial Failed" ;
    else{
        srand(time(NULL)+rand());           //selection rand model
        Particles.clear();

        tsParticle tempParticle;
        BestParticle.Probabilty = 0;
        for(int i=0 ; i<ParticlesNum ; i++ ){
            tempParticle.Position.x = x+range * RandN->randn();
            tempParticle.Position.y = y+range * RandN->randn();
            tempParticle.Direction  = r+M_PI/18.0 * RandN->randn();
            tempParticle.Probabilty = ProbabilityEvaluation->GetProbability(  tempParticle.Position.x,
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
string ParticleFilterTool::PredictionParticles()
{
    int i=0 , i_size;

    i_size = (int)Particles.size();
    if(i_size <=0 ) return "ParticleFilter Prediction Failed" ;
    // 里程計資訊 -> 樣本預測
    BestParticle.Position  = BestParticle.Position + (FeedbackMovement.Position<< BestParticle.Direction) ;
    BestParticle.Direction += FeedbackMovement.Direction;
    TCoordinate prdMotion;
    double prdRotation1, prdRotation2;
    for(i=0 ; i< i_size; i++){
        //----------------------
        /*prdMotion = FeedbackMovement.Position * ( 1 + MoveErrorRate * RandN->randn());
        double tmpNoise;
        tmpNoise = Ro2MoveErrorRate * RandN->randn();
        prdMotion.x += cos(FeedbackMovement.Direction) * tmpNoise;
        prdMotion.y += sin(FeedbackMovement.Direction) * tmpNoise;

        TCoordinate tmpPos;
        tmpPos = FeedbackMovement.Position << Particles[i].Direction;
        prdRotation1 = tmpPos.Angle() * ( 1 + RotateErrorRate * RandN->randn() )
                       + ( FeedbackMovement.Position.Length() * Mo2RotateErrorRate * RandN->randn() );
        prdRotation2 = FeedbackMovement.Direction * ( 1 + RotateErrorRate * RandN->randn() )
                       + ( FeedbackMovement.Position.Length() * Mo2RotateErrorRate * RandN->randn() ); //*/

        // 移動量預測
        //Particles[i].Position = Particles[i].Position + ( prdMotion << prdRotation1 );
        Particles[i].Position   = Particles[i].Position +
                                        ( FeedbackMovement.Position << Particles[i].Direction ) *( 1 + MoveErrorRate * RandN->randn() );
        // 轉動量預測
        //Particles[i].Direction += prdRotation2;
        Particles[i].Direction += FeedbackMovement.Direction * ( 1 + RotateErrorRate * RandN->randn() );
    }
    //--------------------
    return "ParticleFilter Divination Successful" ;
}
//------------------------------------------------------------------------------

string ParticleFilterTool::CorrectParticles()
{
    int i=0 , i_size;
    i_size = (int)Particles.size();
    for(i=0 ; i< i_size; i++){

        if(   Particles[i].Position.x < 0 || Particles[i].Position.y < 0
            ||Particles[i].Position.x > ProbabilityEvaluation->VirtualLineMap->Width
            ||Particles[i].Position.y > ProbabilityEvaluation->VirtualLineMap->Height)
        {

            Particles[i].Position.x = (ProbabilityEvaluation->VirtualLineMap->Width) * ((float)rand()/(float)RAND_MAX) ;
            Particles[i].Position.y = (ProbabilityEvaluation->VirtualLineMap->Height)* ((float)rand()/(float)RAND_MAX) ;
            Particles[i].Direction  = 2*M_PI * ((float)rand()/(float)RAND_MAX);

        }

    }

    return "ParticleFilter Correct Successful" ;
}
//------------------------------------------------------------------------------
string ParticleFilterTool::CorrectParticles( int x,int y,float r,float range )
{
	int i=0 , i_size;
	i_size = (int)Particles.size();
	for(i=0 ; i< i_size; i++){

		if(   Particles[i].Position.x < 0 || Particles[i].Position.y < 0
			||Particles[i].Position.x > ProbabilityEvaluation->VirtualLineMap->Width
			||Particles[i].Position.y > ProbabilityEvaluation->VirtualLineMap->Height)
		{

			Particles[i].Position.x =  x+range * RandN->randn();
			if ( Particles[i].Position.x < 0 )
			{
				Particles[i].Position.x  = fabs(Particles[i].Position.x );
			}
			else if(Particles[i].Position.x > ProbabilityEvaluation->VirtualLineMap->Width )
			{
				Particles[i].Position.x = ProbabilityEvaluation->VirtualLineMap->Width - 
                                                                        (Particles[i].Position.x - ProbabilityEvaluation->VirtualLineMap->Width);
			}

			Particles[i].Position.y = y+range * RandN->randn();
			if ( Particles[i].Position.y < 0 )
			{
				Particles[i].Position.y  = fabs(Particles[i].Position.y );
			}
			else if(Particles[i].Position.y > ProbabilityEvaluation->VirtualLineMap->Height )
			{
				Particles[i].Position.y = ProbabilityEvaluation->VirtualLineMap->Height - 
					(Particles[i].Position.y - ProbabilityEvaluation->VirtualLineMap->Height);
			}

			Particles[i].Direction  = r+M_PI/18 * RandN->randn();
		}

	}

	return "ParticleFilter Correct Successful" ;

}
//------------------------------------------------------------------------------
string ParticleFilterTool::EvaluatuonParticles()
{
    int i=0 , i_size , k;
    i_size = (int)Particles.size();
    vector <tsParticle> TempParticles;
    for(i=0 ; i< i_size; i++){
        Particles[i].Probabilty =
            ProbabilityEvaluation->GetProbability( Particles[i].Position.x,
                                                         Particles[i].Position.y,
                                                         Particles[i].Direction    ); //*/
            k = 0;
            if(i==0){
                TempParticles.push_back(Particles[i]) ;
            }
            while(k<i){

                if( Particles[i].Probabilty > TempParticles[k].Probabilty){
                    //original: TempParticles.insert(&TempParticles[k], Particles[i]);
                    vector<tsParticle>::iterator it = TempParticles.begin();
                    TempParticles.insert(it+k , Particles[i]);
                    break;
                }
                k++;
                if(k==i)TempParticles.push_back(Particles[i]);
            }
    }
    Particles.clear();
    Particles = TempParticles;
    TempParticles.clear();

    //BestParticle = Particles[0];
    return "ParticleFilter Evaluatuon Successful" ;
}
//------------------------------------------------------------------------------

string ParticleFilterTool::ResamplingParticles()
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
        dart = Particles[start_i].acProbability*((float)rand()/(float)RAND_MAX);
        int k = 0;
        if(Particles[start_i].acProbability >= 0){
            ParticleTemp.acProbability = 0;
            while(ParticleTemp.acProbability < dart){
                ParticleTemp = Particles[k];
                k++;
                if (k>start_i) break;
            }

            Rdis   = SearchRate*RandN->randn()*ProbabilityEvaluation->VirtualLineMap->Width;;
            Rangle = 2*M_PI*((float)rand()/(float)RAND_MAX );
            Particles[i].Position.x = ParticleTemp.Position.x + Rdis*cos(Rangle);
            Particles[i].Position.y = ParticleTemp.Position.y + Rdis*sin(Rangle);
            Particles[i].Direction  = ParticleTemp.Direction
                                            +M_PI/90*RandN->randn();
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
//------------------------------------------------------------------------------
