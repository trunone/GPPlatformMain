/*
 *   LinuxLocationTimer.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LINUX_LOCATION_MANAGER_H_
#define _LINUX_LOCATION_MANAGER_H_

#include <pthread.h>
#include "LocationManager.h"
#include <time.h>

namespace Robot
{
  class LinuxLocationTimer
  {
    private:
      pthread_t m_Thread;// thread structure
      unsigned long m_Interval_ns;
      LocationManager *m_Manager;// reference to the motion manager class.
      bool m_TimerRunning;
      bool m_FinishTimer;

    protected:
      static void *TimerProc(void *param);// thread function

    public:
      LinuxLocationTimer(LocationManager* manager);
      ~LinuxLocationTimer();

      void Start();
      void Stop();
	  inline bool IsRunning() { return this->m_TimerRunning; }
  };
}

#endif
