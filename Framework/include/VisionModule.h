/*
 *   VisionModule.h
 *
 *   Author: Wu Chih-En
 *
 */

#ifndef _VISION_MODULE_H_
#define _VISION_MODULE_H_

namespace Robot
{
class VisionModule
{
private:

protected:

public:

    static const int TIME_UNIT = 8; //msec

    virtual void Initialize() = 0;
    virtual void Process() = 0;
};
}

#endif
