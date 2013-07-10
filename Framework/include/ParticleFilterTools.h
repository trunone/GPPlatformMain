#ifndef ParticleFilterH
#define ParticleFilterH
#include <vector>
#include "LocationManager.h"
#include "LocationStatus.h"
#include "ProbabilityEvaluation.h"
#include "Normalrand.h"
#define Def_ResamplingDis 0
#define Def_ResamplingAngle 0
#define Def_ParticlesNum 200
#define MoveErrorRate   0.5       //+-50% error
#define RotateErrorRate 0.6
#define Ro2MoveErrorRate 0.2
#define Mo2RotateErrorRate 0.1
#define BPKeepRate      0.3      //Bast Particle keeprate
#define SearchRate      0.03       //5% of map size;

class TLocParticleFilter:
{
    typedef struct{
        TCoordinate Position;
        float Direction;
        double Probabilty;
        double acProbability;  //Accumulate Probability;
    }tsParticle;

private:


public:
//--------------------------------------------------------------
    TLocParticleFilter();
    ~TLocParticleFilter();

//-----Particles------------------------------------------------
    vector <tsParticle> Particles;
    NormalRand *RandN;

//-----Best Partiicle information-------------------------------
    tsParticle BestParticle;
    int BestParticleNum;

//----Environment area-----------------------------------
    //moveing distance for divination
    tsParticle FeedbackMovement;
    //For Evaluation particles
    TLocProbEvaluation *ProbabilityEvaluation;

    // it will give probability Evaluation map
    //this map black part is mean unable area be careful


//----Main flow step--------------------------------------------
    string InitialParticles(int ParticlesNum);
    string InitialParticles(int ParticlesNum,int x ,int y , float r,float range);
    //string EnvironmentInfoUpdate();
    // Prediction step
    string PredictionParticles();
    string CorrectParticles();
    string CorrectParticles(int x,int y,float r,float range);
    // Correction step
    string EvaluatuonParticles();
    string ResamplingParticles();

//---------------------------------------------------------------
};
//---------------------------------------------------------------------------
#endif

