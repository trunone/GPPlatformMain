/*
 *
 *   DXL.h
 *
 */
#ifndef DXL_H
#define DXL_H


namespace Robot
{
    class DXL
    {
    private:

    public:
        enum {
            STOP,
            CATCH,
            THROW
        };

        DXL();
        ~DXL();

        int OpenDevice(int deviceIndex);

        void GoToDegree(int deg);

        void EndlessTurn(int mode); //0:stop, 1:catch ball, 2:throw ball

        void CloseDevice();
    };
}

#endif
