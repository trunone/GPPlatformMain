/*
 *   FaceDetection_Stage2.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _FACEDETECTION_STAGE2_H_
#define _FACEDETECTION_STAGE2_H_

#include <string.h>

#include "VisionModule.h"

namespace Robot
{
class FaceDetection_Stage2 : public VisionModule
{
private:
    static FaceDetection_Stage2* m_UniqueInstance;
    FaceDetection_Stage2();

public:
    static FaceDetection_Stage2* GetInstance() {
        return m_UniqueInstance;
    }
    ~FaceDetection_Stage2();
    void Initialize();
    void Process();
};
}

#endif
