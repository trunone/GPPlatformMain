/*
 *   LocationModule.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _LOCATION_MODULE_H_
#define _LOCATION_MODULE_H_

namespace Robot
{
class LocationModule
{
private:

protected:

public:

    static const int TIME_UNIT = 50; //msec

    virtual void Initialize() = 0;
    virtual void Process() = 0;
};
}

#endif
