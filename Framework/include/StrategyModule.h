/*
 *   StrategyModule.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _STRATEGY_MODULE_H_
#define _STRATEGY_MODULE_H_

namespace Robot
{
class StrategyModule
{
private:

protected:

public:

    static const int TIME_UNIT = 100; //msec

    virtual void Initialize() = 0;
    virtual void Process() = 0;
};
}

#endif
