/*
 *   VisionManager.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _VISION_MANGER_H_
#define _VISION_MANGER_H_

#include <list>
#include <fstream>
#include <iostream>
#include "VisionStatus.h"
#include "VisionModule.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace Robot
{
class VisionManager
{
private:
    static VisionManager* m_UniqueInstance;
    std::list<VisionModule*> m_Modules;
    bool m_ProcessEnable;
    bool m_Enabled;

    bool m_IsRunning;
    bool m_IsThreadRunning;
    bool m_IsLogging;

    std::ofstream m_LogFileStream;

    CvCapture *VisionCapture;

    VisionManager();

protected:

public:
    bool DEBUG_PRINT;

    ~VisionManager();

    static VisionManager* GetInstance() {
        return m_UniqueInstance;
    }

    bool Initialize(CvCapture*);
    bool Reinitialize();
    void Process();
    void SetEnable(bool enable);
    bool GetEnable()				{
        return m_Enabled;
    }
    bool IsRunning() { return m_IsRunning; }
    void AddModule(VisionModule *module);
    void RemoveModule(VisionModule *module);

    void StartLogging();
    void StopLogging();
};
}

#endif
