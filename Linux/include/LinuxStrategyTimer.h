/*
 *   LinuxStrategyTimer.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LINUX_STRATEGY_MANAGER_H_
#define _LINUX_STRATEGY_MANAGER_H_

#include <pthread.h>
#include "StrategyManager.h"
#include <time.h>

namespace Robot
{
class LinuxStrategyTimer
{
private:
    pthread_t m_Thread;// thread structure
    unsigned long m_Interval_ns;
    StrategyManager *m_Manager;// reference to the motion manager class.
    bool m_TimerRunning;
    bool m_FinishTimer;

protected:
    static void *TimerProc(void *param);// thread function

public:
    LinuxStrategyTimer(StrategyManager* manager);
    ~LinuxStrategyTimer();

    void Start();
    void Stop();
    inline bool IsRunning() {
        return this->m_TimerRunning;
    }
};
}

#endif
