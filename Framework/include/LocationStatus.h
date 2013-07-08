/*
 *   LocationStatus.h
 *
 *   Author: Wu Chih-En
 *
 */

#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>

#ifndef _LOCATION_STATUS_H_
#define _LOCATION_STATUS_H_

#define Def_AngularToRadian 0.017453293		
#define Def_RadianToAngular 57.295779513
#define Def_ResamplingDis 0
#define Def_ResamplingAngle 0
#define Def_ParticlesNum 200
#define MoveErrorRate   0.5       
#define RotateErrorRate 0.6
#define Ro2MoveErrorRate 0.2
#define Mo2RotateErrorRate 0.1
#define BPKeepRate      0.3      
#define SearchRate      0.03       

using namespace std;

namespace Robot
{
	class LocationStatus
	{
	private:

	public:
	
    	//TLocParticleFilter();
    	//~TLocParticleFilter();

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
    	//string  __fastcall EnvironmentInfoUpdate();
    	// Prediction step
    	string PredictionParticles();
    	string CorrectParticles();
    	string CorrectParticles(int x,int y,float r,float range);
    	// Correction step
    	string EvaluatuonParticles();
    	string ResamplingParticles();
	//---------------------------------------------------------------
	string  Initial(void);                  
    	string  Main(void);
	inline double GetMin(const double &dX_,const double &dY_){ 
				return (dX_>=dY_)?dY_:dX_; }
	string State;
	static TInfo* Info;
    	bool enable;
	ifstream *FPtr_Parameter;
	int Test_rf;
	};
}

#endif
