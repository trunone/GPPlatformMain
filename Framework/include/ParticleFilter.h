#ifndef ParticleFilterH
#define ParticleFilterH

#include "VisionStatus.h"
#include "LocationModule.h"
#include "LocationStatus.h"
#include "TCoordinate.h"
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

namespace Robot{
	class ParticleFilter : LocationModule
	{
        typedef struct{
            TCoordinate Position;
            float Direction;

            double Probabilty;
            double acProbability;  //Accumulate Probability;
        }tsParticle;

	public:
		~ParticleFilter();
		static ParticleFilter*  GetInstance() {return m_UniqueInstance;}

		void  Initialize(void);
	    void  Process(void);

	private:
		ParticleFilter();
		static ParticleFilter* m_UniqueInstance;
		double PastMovement;
		double PastDirection;	
		
		void InitialParticleFilter();	
        
        //-----Particles------------------------------------------------
        vector <tsParticle> Particles;

        //-----Best Partiicle information-------------------------------
        tsParticle BestParticle;  // ¦ô´ú¦ì¸m - 2012/07/29 yao
        int BestParticleNum;

        // it will give probability Evaluation map
        //this map black part is mean unable area be careful

        //----Main flow step--------------------------------------------
        string InitialParticles(int ParticlesNum);
        string InitialParticles(int ParticlesNum,int x ,int y , float r,float range);
        // string EnvironmentInfoUpdate();
        // Prediction step
        string PredictionParticles();
        string CorrectParticles();
        string CorrectParticles(int x,int y,float r,float range);
        // Correction step
        string EvaluatuonParticles();
        string ResamplingParticles();

        //---------------------------------------------------------------
	};
}
#endif
