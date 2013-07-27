//=============================================
//File name: ParticleFilterTool
//
//Author   :Hsieh Ming-Hua
//          Transplant form 2012 SKS project
//Phone    :0921307020
//E-mail   :ta180109@hotmail.com
//Update   :2013/07/19
//
//Function :The tool function Particle filter will use
//===============================================ifndef ParticleFilterToolH
#define ParticleFilteToolrH
#include <vector>
#include "LocationStatus.h"
#include "ProbabilityEvaluation.h"

#define Def_ResamplingDis 0
#define Def_ResamplingAngle 0
#define Def_ParticlesNum 200

#define MoveErrorRate   0.5       //+-50% error
#define RotateErrorRate 0.6
#define Ro2MoveErrorRate 0.2
#define Mo2RotateErrorRate 0.1

#define BPKeepRate      0.3      //Bast Particle keeprate
#define SearchRate      0.03       //5% of map size;

using namespace std;

namespace Robot{
	class ParticleFilterTool 
	{
		typedef struct{
	        TCoordinate Position;
	        float Direction;
	
	        double Probabilty;
	        double acProbability;  //Accumulate Probability;
	    }tsParticle;

	private:
		static ParticleFilterTool* m_UniqueInstance;

	public:
		ParticleFilterTool();
		~ParticleFilterTool();
		static ParticleFilterTool* GetInstance() { return m_UniqueInstance; }
		//-----Particles------------------------------------------------
		static vector <tsParticle> Particles;
		static NormalRand *RandN;

		//-----Best Partiicle information-------------------------------
		static tsParticle BestParticle;  // ������m - 2012/07/29 yao
		static int BestParticleNum;

		//----Environment area-----------------------------------
		//moveing distance for divination
		static tsParticle FeedbackMovement;
		//For Evaluation particles
		static ProbEvaluation *ProbabilityEvaluation;

		// it will give probability Evaluation map
		//this map black part is mean unable area be careful


		//----Main flow step--------------------------------------------
		static string InitialParticles(int ParticlesNum);
		static string InitialParticles(int ParticlesNum,int x ,int y , float r,float range);
		// string EnvironmentInfoUpdate();
		// Prediction step
	    	static string PredictionParticles();
		static string CorrectParticles();
		static string CorrectParticles(int x,int y,float r,float range);
		// Correction step
		static string EvaluatuonParticles();
		static string ResamplingParticles();

		//---------------------------------------------------------------
	};
}