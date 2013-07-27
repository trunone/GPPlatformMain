/*
 *   FaceDetection.h
 *
 *   Author: Wu Chih-En 
 *
 */

#ifndef _FACEDETECTION_H_
#define _MOTION_H_

#include <string.h>

#include "VisionModule.h"
#include "Wheeled.h"

namespace Robot
{
	class FaceDetection : public VisionModule
	{
	private:
		static FaceDetection* m_UniqueInstance;

	public:
		static FaceDetection* GetInstance() { return m_UniqueInstance; }	
		FaceDetection();
		~FaceDetection();
		void Initialize();
		void Process();
	};
}

#endif
