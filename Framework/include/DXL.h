/*
 *
 *   DXL.h
 *
 */

#include "dynamixel.h"

namespace Robot
{
class DXL
{
private:

public:
    DXL();
    virtual ~DXL();

    int initialize(int deviceIndex);

    void Degree(int deg );

    void EndlessTurn(int mode); //0:stop, 1:catch ball, 2:throw ball

    void dxl_terminate();

};
}
