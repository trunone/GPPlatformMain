/*
 *   LinuxStrategyTimer.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include "StrategyModule.h"
#include "LinuxStrategyTimer.h"

#include <stdlib.h>
#include <string.h>

using namespace Robot;

LinuxStrategyTimer::LinuxStrategyTimer(StrategyManager* manager)
    : m_Manager(manager)
{
    this->m_FinishTimer = false;
    this->m_TimerRunning = false;
}

void *LinuxStrategyTimer::TimerProc(void *param)
{
    LinuxStrategyTimer *timer = (LinuxStrategyTimer *)param;
    static struct timespec next_time;

    // The clock_gettime()  function returns the current time (in seconds and nanoseconds) for the specified clock
    // CLOCK_MONOTONIC : this clock always increases at a constant rate and can't be adjusted
    clock_gettime(CLOCK_MONOTONIC,&next_time);

    while(!timer->m_FinishTimer)
    {
        next_time.tv_sec += (next_time.tv_nsec + StrategyModule::TIME_UNIT * 1000000) / 1000000000;
        next_time.tv_nsec = (next_time.tv_nsec + StrategyModule::TIME_UNIT * 1000000) % 1000000000;

        if(timer->m_Manager != NULL)
            timer->m_Manager->Process();

        // clock_nanosleep() function allows the calling thread to sleep for an interval specified with nanosecond precision
        // TIMER_ABSTIME : absolute time
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_time, NULL);
    }

    pthread_exit(NULL);
}

void LinuxStrategyTimer::Start(void)
{
    int error;
    struct sched_param param;
    pthread_attr_t attr;//build inital attributes of pthread

    // The pthread_attr_init() function initializes the thread attributes object pointed to by attr with default attribute values
    pthread_attr_init(&attr);//start pthread

    // The pthread_attr_setschedpolicy() function sets the scheduling policy attribute of the thread attributes object referred to by attr to the value specified in policy
    // SCHED_RR performs a round-robin with a certain timeslice
    error = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(error != 0)
        printf("error = %d\n",error);

    // The pthread_attr_setinheritsched() function sets the inherit scheduler attribute of the thread attributes object referred to by attr to the value specified in inheritsched
    // PTHREAD_INHERIT_SCHED : Threads that are created using attr inherit scheduling attributes from the thread of creater
    error = pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    if(error != 0)
        printf("error = %d\n",error);

    memset(&param, 0, sizeof(param));
    //Processes scheduled under one of the real-time policies (SCHED_FIFO, SCHED_RR) have a sched_priority value in the range 1 (low) to 99 (high)
    param.sched_priority = 31;// RT
    // The pthread_setschedparam() function sets the scheduling policy and parameters of the thread
    error = pthread_attr_setschedparam(&attr, &param);
    if(error != 0)
        printf("error = %d\n",error);

    // The pthread_create() function is used to create a new thread, with attributes specified by attr, within a process
    // first  parameter : variables of thread
    // second parameter : attributes of thread
    // third  parameter : a function of describing behavior
    // forth  parameter : parameters of running function
    if((error = pthread_create(&this->m_Thread, &attr, this->TimerProc, this))!= 0)
        exit(-1);

    this->m_TimerRunning=true;

}

void LinuxStrategyTimer::Stop(void)
{
    int error=0;

    // seti the flag to end the thread
    if(this->m_TimerRunning)
    {
        this->m_FinishTimer = true;
        // wait for the thread to end
        // The pthread_join() function suspends execution of the calling thread until the target thread terminates, unless the target thread has already terminated
        if((error = pthread_join(this->m_Thread, NULL))!= 0)
            exit(-1);
        this->m_FinishTimer = false;
        this->m_TimerRunning = false;
    }
}

LinuxStrategyTimer::~LinuxStrategyTimer()
{
    this->Stop();
    this->m_Manager = NULL;
}
