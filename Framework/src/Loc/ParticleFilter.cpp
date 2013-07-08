//---------------------------------------------------------------------------


//#pragma hdrstop

#include "ParticleFilter.h"
#include <time.h>
#include <math.h>

//#pragma package(smart_init)
//---------------------------------------------------------------------------
TLocParticleFilter::TLocParticleFilter()
{
    this->ProbabilityEvaluation = new TLocProbEvaluation;
    this->RandN = new NormalRand(1,0.001);

}
//------------------------------------------------------------------------------

TLocParticleFilter::~TLocParticleFilter()
{
    delete this->ProbabilityEvaluation;
    this->Particles.clear();
}
//------------------------------------------------------------------------------

string TLocParticleFilter::InitialParticles(int ParticlesNum)
{

    if(this->ProbabilityEvaluation->VirtualLineMap == NULL)
        return "ParticleFilter Intial Failed" ;
    else{
        srand(time(NULL)+rand());           //selection rand model
        this->Particles.clear();

        tsParticle tempParticle;
        this->BestParticle.Probabilty = 0;
        for(int i=0 ; i<ParticlesNum ; i++ ){
            tempParticle.Position.x = (this->ProbabilityEvaluation->VirtualLineMap->Width)/2 * ((float)rand()/(float)RAND_MAX) ;
            tempParticle.Position.y = (this->ProbabilityEvaluation->VirtualLineMap->Height)* ((float)rand()/(float)RAND_MAX) ;
            tempParticle.Direction  = 2.0*M_PI * ((float)rand()/(float)RAND_MAX);
            tempParticle.Probabilty = this->ProbabilityEvaluation->GetProbability(  tempParticle.Position.x,
                                                                                    tempParticle.Position.y,
                                                                                    tempParticle.Direction);

            if(tempParticle.Probabilty > this->BestParticle.Probabilty){
                this->BestParticle = tempParticle;
                this->BestParticleNum = i;
            }

            this->Particles.push_back(tempParticle);
        }
        return "ParticleFilter Intial Successful" ;
    }
}
//------------------------------------------------------------------------------
string TLocParticleFilter::InitialParticles(int ParticlesNum,int x ,int y , float r,float range)
{
    if(this->ProbabilityEvaluation->VirtualLineMap == NULL)
        return "ParticleFilter Intial Failed" ;
    else{
        srand(time(NULL)+rand());           //selection rand model
        this->Particles.clear();

        tsParticle tempParticle;
        this->BestParticle.Probabilty = 0;
        for(int i=0 ; i<ParticlesNum ; i++ ){
            tempParticle.Position.x = x+range * this->RandN->randn();
            tempParticle.Position.y = y+range * this->RandN->randn();
            tempParticle.Direction  = r+M_PI/18.0 * this->RandN->randn();
            tempParticle.Probabilty = this->ProbabilityEvaluation->GetProbability(  tempParticle.Position.x,
                                                                                    tempParticle.Position.y,
                                                                                    tempParticle.Direction);
            if(tempParticle.Probabilty > this->BestParticle.Probabilty){
                this->BestParticle = tempParticle;
                this->BestParticleNum = i;
            }

            this->Particles.push_back(tempParticle);
        }
        this->BestParticle.Position.x = x;
        this->BestParticle.Position.y = y;
        this->BestParticle.Direction = r;
        return "ParticleFilter Intial Successful";
    }

}

//------------------------------------------------------------------------------
string TLocParticleFilter::PredictionParticles()
{
    int i=0 , i_size;

    i_size = (int)this->Particles.size();
    if(i_size <=0 ) return "ParticleFilter Prediction Failed" ;
    // 里程計資訊 -> 樣本預測
    this->BestParticle.Position  = this->BestParticle.Position + (this->FeedbackMovement.Position<< this->BestParticle.Direction) ;
    this->BestParticle.Direction += this->FeedbackMovement.Direction;
    TCoordinate prdMotion;
    double prdRotation1, prdRotation2;
    for(i=0 ; i< i_size; i++){
            //----------------------
            /*prdMotion = this->FeedbackMovement.Position * ( 1 + MoveErrorRate * this->RandN->randn());
            double tmpNoise;
            tmpNoise = Ro2MoveErrorRate * this->RandN->randn();
            prdMotion.x += cos(this->FeedbackMovement.Direction) * tmpNoise;
            prdMotion.y += sin(this->FeedbackMovement.Direction) * tmpNoise;

            TCoordinate tmpPos;
            tmpPos = this->FeedbackMovement.Position << this->Particles[i].Direction;
            prdRotation1 = tmpPos.Angle() * ( 1 + RotateErrorRate * this->RandN->randn() )
                           + ( this->FeedbackMovement.Position.Length() * Mo2RotateErrorRate * this->RandN->randn() );
            prdRotation2 = this->FeedbackMovement.Direction * ( 1 + RotateErrorRate * this->RandN->randn() )
                           + ( this->FeedbackMovement.Position.Length() * Mo2RotateErrorRate * this->RandN->randn() ); //*/

            // 移動量預測
            //this->Particles[i].Position = this->Particles[i].Position + ( prdMotion << prdRotation1 );
            this->Particles[i].Position   = this->Particles[i].Position +
                                            ( this->FeedbackMovement.Position << this->Particles[i].Direction ) *( 1 + MoveErrorRate * this->RandN->randn() );
            // 轉動量預測
            //this->Particles[i].Direction += prdRotation2;
            this->Particles[i].Direction += this->FeedbackMovement.Direction * ( 1 + RotateErrorRate * this->RandN->randn() );
    }
    //--------------------
    return "ParticleFilter Divination Successful" ;
}
//------------------------------------------------------------------------------

string TLocParticleFilter::CorrectParticles()
{
    int i=0 , i_size;
    i_size = (int)this->Particles.size();
    for(i=0 ; i< i_size; i++){

        if(   this->Particles[i].Position.x < 0 || this->Particles[i].Position.y < 0
            ||this->Particles[i].Position.x > this->ProbabilityEvaluation->VirtualLineMap->Width
            ||this->Particles[i].Position.y > this->ProbabilityEvaluation->VirtualLineMap->Height)
        {

            this->Particles[i].Position.x = (this->ProbabilityEvaluation->VirtualLineMap->Width) * ((float)rand()/(float)RAND_MAX) ;
            this->Particles[i].Position.y = (this->ProbabilityEvaluation->VirtualLineMap->Height)* ((float)rand()/(float)RAND_MAX) ;
            this->Particles[i].Direction  = 2*M_PI * ((float)rand()/(float)RAND_MAX);

        }

    }

    return "ParticleFilter Correct Successful" ;
}
//------------------------------------------------------------------------------
string TLocParticleFilter::CorrectParticles( int x,int y,float r,float range )
{
	int i=0 , i_size;
	i_size = (int)this->Particles.size();
	for(i=0 ; i< i_size; i++){

		if(   this->Particles[i].Position.x < 0 || this->Particles[i].Position.y < 0
			||this->Particles[i].Position.x > this->ProbabilityEvaluation->VirtualLineMap->Width
			||this->Particles[i].Position.y > this->ProbabilityEvaluation->VirtualLineMap->Height)
		{

			this->Particles[i].Position.x =  x+range * this->RandN->randn();
			if ( this->Particles[i].Position.x < 0 )
			{
				this->Particles[i].Position.x  = fabs(this->Particles[i].Position.x );
			}
			else if(this->Particles[i].Position.x > this->ProbabilityEvaluation->VirtualLineMap->Width )
			{
				this->Particles[i].Position.x = this->ProbabilityEvaluation->VirtualLineMap->Width - 
                                                                        (this->Particles[i].Position.x - this->ProbabilityEvaluation->VirtualLineMap->Width);
			}

			this->Particles[i].Position.y = y+range * this->RandN->randn();
			if ( this->Particles[i].Position.y < 0 )
			{
				this->Particles[i].Position.y  = fabs(this->Particles[i].Position.y );
			}
			else if(this->Particles[i].Position.y > this->ProbabilityEvaluation->VirtualLineMap->Height )
			{
				this->Particles[i].Position.y = this->ProbabilityEvaluation->VirtualLineMap->Height - 
					(this->Particles[i].Position.y - this->ProbabilityEvaluation->VirtualLineMap->Height);
			}

			this->Particles[i].Direction  = r+M_PI/18 * this->RandN->randn();
		}

	}

	return "ParticleFilter Correct Successful" ;

}
//------------------------------------------------------------------------------
string TLocParticleFilter::EvaluatuonParticles()
{
    int i=0 , i_size , k;
    i_size = (int)this->Particles.size();
    vector <tsParticle> TempParticles;
    for(i=0 ; i< i_size; i++){
        this->Particles[i].Probabilty =
            this->ProbabilityEvaluation->GetProbability( this->Particles[i].Position.x,
                                                         this->Particles[i].Position.y,
                                                         this->Particles[i].Direction    ); //*/
            k = 0;
            if(i==0){
                TempParticles.push_back(this->Particles[i]) ;
            }
            while(k<i){

                if( this->Particles[i].Probabilty > TempParticles[k].Probabilty){
                    TempParticles.insert(&TempParticles[k] , this->Particles[i])  ;
                    break;
                }
                k++;
                if(k==i)TempParticles.push_back(this->Particles[i]);
            }
    }
    this->Particles.clear();
    this->Particles = TempParticles;
    TempParticles.clear();

    //this->BestParticle = this->Particles[0];
    return "ParticleFilter Evaluatuon Successful" ;
}
//------------------------------------------------------------------------------

string TLocParticleFilter::ResamplingParticles()
{

    int i=0 , i_size;
    i_size = (int)this->Particles.size();

    this->Particles[0].acProbability = this->Particles[0].Probabilty;
    for(i=1 ; i< i_size; i++){
        this->Particles[i].acProbability = this->Particles[i-1].acProbability + this->Particles[i].Probabilty;
    }
    //-----------------
    tsParticle ParticleTemp;

    int start_i = (int)((float)i_size*BPKeepRate) ;
    //keep best 30% of particle
    double Rdis,Rangle;
    for(i=start_i ; i<i_size ;i++){
        double dart;
        dart = this->Particles[start_i].acProbability*((float)rand()/(float)RAND_MAX);
        int k = 0;
        if(this->Particles[start_i].acProbability >= 0){
            ParticleTemp.acProbability = 0;
            while(ParticleTemp.acProbability < dart){
                ParticleTemp = this->Particles[k];
                k++;
                if (k>start_i) break;
            }

            Rdis   = SearchRate*this->RandN->randn()*this->ProbabilityEvaluation->VirtualLineMap->Width;;
            Rangle = 2*M_PI*((float)rand()/(float)RAND_MAX );
            this->Particles[i].Position.x = ParticleTemp.Position.x + Rdis*cos(Rangle);
            this->Particles[i].Position.y = ParticleTemp.Position.y + Rdis*sin(Rangle);
            this->Particles[i].Direction  = ParticleTemp.Direction
                                            +M_PI/90*this->RandN->randn();
        }
    } //*/
    //---------------
    // position estimation _yao
    ParticleTemp.Position.x = 0.0;
    ParticleTemp.Position.y = 0.0;
    ParticleTemp.Direction  = 0.0;
    for(i=0 ; i< i_size; i++){
        ParticleTemp.Position.x += this->Particles[i].Position.x;
        ParticleTemp.Position.y += this->Particles[i].Position.y;
        ParticleTemp.Direction  += this->Particles[i].Direction;
    }
    this->BestParticle.Position.x = ParticleTemp.Position.x / i_size;
    this->BestParticle.Position.y = ParticleTemp.Position.y / i_size;
    this->BestParticle.Direction  = ParticleTemp.Direction  / i_size;
    this->BestParticle.Probabilty = 0.0;
    this->BestParticle.acProbability = 0.0;
    //-------------
    return "ParticleFilter Selection Successful" ;
}
//------------------------------------------------------------------------------
