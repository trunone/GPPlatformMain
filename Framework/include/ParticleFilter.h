#ifndef ParticleFilterH
#define ParticleFilterH

#include "VisionStatus.h"
#include "LocationModule.h"
#include "LocationStatus.h"
#include "TCoordinate.h"
#include "ParticleFilterTool.h"
#include "ProbabilityEvaluation.h"
namespace Robot{
	class ParticleFilter : LocationModule
	{
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
		
		VisionStatus::tsBmpPtr tsBmpVirtualMap;
		void SetParticleFilter(int W, int H, unsigned char *Mapdata);
		void InitialParticleFilter();	
	};
}
#endif
