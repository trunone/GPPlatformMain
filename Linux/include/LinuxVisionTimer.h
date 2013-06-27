/*
 *   LinuxVisionTimer.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LINUX_VISION_MANAGER_H_
#define _LINUX_VISION_MANAGER_H_

#include <pthread.h>
#include "VisionManager.h"
#include <time.h>

namespace Robot
{
  class LinuxVisionTimer
  {
    private:
      pthread_t m_Thread;// thread structure
      unsigned long m_Interval_ns;
      VisionManager *m_Manager;// reference to the motion manager class.
      bool m_TimerRunning;
      bool m_FinishTimer;

    protected:
      static void *TimerProc(void *param);// thread function

    public:
      LinuxVisionTimer(VisionManager* manager);
      ~LinuxVisionTimer();

      void Start();
      void Stop();
	  inline bool IsRunning() { return this->m_TimerRunning; }
  };
}

#endif
