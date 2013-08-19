/*
 *   FaceDetection.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _FACEDETECTION_H_
#define _FACEDETECTION_H_

#include <string.h>

#include "VisionModule.h"

namespace Robot
{
class FaceDetection : public VisionModule
{
private:
    static FaceDetection* m_UniqueInstance;
    FaceDetection();

public:
    static FaceDetection* GetInstance() {
        return m_UniqueInstance;
    }
    ~FaceDetection();
    void Initialize();
    void Process();
};
}

#endif
