#ifndef _Doornumber_detect_H_
#define _Doornumber_detect_H_
#include "VisionModule.h"
#include "VisionStatus.h"
#include "SegmentFunction.h"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
//----------------------------

namespace Robot
{
class Doornumber_detect : public VisionModule
{

private:
    Doornumber_detect();
    static Doornumber_detect* m_UniqueInstance;
public:

    static Doornumber_detect* GetInstance() {
        return m_UniqueInstance;
    }

    ~Doornumber_detect();
    void Initialize();
    void Process();
};
}

#endif
