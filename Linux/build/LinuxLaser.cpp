/*
 *   LinuxLaser.cpp
 *
 *   Author: Wu Chih-En
 *
 */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include "LinuxLaser.h"

using namespace Robot;


LinuxLaser::LinuxLaser(const char* name)
{
    DEBUG_PRINT = false;
    m_Socket_fd = -1;
    m_PacketStartTime = 0;
    m_PacketWaitTime = 0;
    m_UpdateStartTime = 0;
    m_UpdateWaitTime = 0;
    m_ByteTransferTime = 0;

    sem_init(&m_LowSemID, 0, 1);
    sem_init(&m_MidSemID, 0, 1);
    sem_init(&m_HighSemID, 0, 1);

    SetPortName(name);
}

LinuxLaser::~LinuxLaser()
{
    ClosePort();
}

void LinuxLaser::SetPortName(const char* name)
{
    strcpy(m_PortName, name);
}

bool LinuxLaser::OpenPort()
{
    struct termios newtio;
    double baudrate = 1.0; //bit per second

    ClosePort();

    if(DEBUG_PRINT == true)
        printf("\n%s open ", m_PortName);

    if((m_Socket_fd = open(m_PortName, O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0) {
        fprintf(stderr, "Cannot open serial port!\n");
        return false;
    }

    if(DEBUG_PRINT == true)
        printf("success!\n");

    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag      = B38400|CS8|CLOCAL|CREAD;
    newtio.c_iflag      = IGNPAR;
    newtio.c_oflag      = 0;
    newtio.c_lflag      = 0;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN]   = 0;
    tcsetattr(m_Socket_fd, TCSANOW, &newtio);

    if(DEBUG_PRINT == true)
        printf("Set %.1fbps ", baudrate);

    if(DEBUG_PRINT == true)
        printf("success!\n");

    tcflush(m_Socket_fd, TCIFLUSH);

    m_ByteTransferTime = (1000.0 / baudrate) * 12.0;

    return true;
}

bool LinuxLaser::SetBaud(int baud)
{
    struct serial_struct serinfo;

    //if(m_Socket_fd == -1)
    //    return false;

    //if(ioctl(m_Socket_fd, TIOCGSERIAL, &serinfo) < 0) {
    //    fprintf(stderr, "Cannot get serial info\n");
    //    return false;
    //}

    //serinfo.flags &= ~ASYNC_SPD_MASK;
    //serinfo.flags |= ASYNC_SPD_CUST;
    //serinfo.custom_divisor = serinfo.baud_base / (double)baud;

    //if(ioctl(m_Socket_fd, TIOCSSERIAL, &serinfo) < 0) {
    //    fprintf(stderr, "Cannot set serial info\n");
    //    return false;
    //}

    ClosePort();
    OpenPort();

    m_ByteTransferTime = (float)((1000.0 / (double)baud) * 12.0);

    return true;
}

void LinuxLaser::ClosePort()
{
    if(m_Socket_fd != -1)
        close(m_Socket_fd);
    m_Socket_fd = -1;
}

void LinuxLaser::ClearPort()
{
    tcflush(m_Socket_fd, TCIFLUSH);
}

int LinuxLaser::WritePort(unsigned char* packet, int numPacket)
{
    return write(m_Socket_fd, packet, numPacket);
}

int LinuxLaser::ReadPort(unsigned char* packet, int numPacket)
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO (&fds);
    FD_SET (m_Socket_fd, &fds);

    /* Timeout. */
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    r = select (m_Socket_fd + 1, &fds, NULL, NULL, &tv);

    if(r > 0)
        return read(m_Socket_fd, packet, numPacket);
    else
        return 0;
}

void sem_wait_nointr(sem_t *sem)
{
    int sem_result;
    do {
        sem_result = sem_wait(sem);
    } while((sem_result == -1) && (errno == EINTR));
}

void LinuxLaser::LowPriorityWait()
{
    sem_wait_nointr(&m_LowSemID);
}

void LinuxLaser::MidPriorityWait()
{
    sem_wait_nointr(&m_MidSemID);
}

void LinuxLaser::HighPriorityWait()
{
    sem_wait_nointr(&m_HighSemID);
}

void LinuxLaser::LowPriorityRelease()
{
    sem_post(&m_LowSemID);
}

void LinuxLaser::MidPriorityRelease()
{
    sem_post(&m_MidSemID);
}

void LinuxLaser::HighPriorityRelease()
{
    sem_post(&m_HighSemID);
}

double LinuxLaser::GetCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return ((double)tv.tv_sec*1000.0 + (double)tv.tv_usec/1000.0);
}

void LinuxLaser::SetPacketTimeout(int lenPacket)
{
    m_PacketStartTime = GetCurrentTime();
    m_PacketWaitTime = m_ByteTransferTime * (double)lenPacket + 5.0;
}

bool LinuxLaser::IsPacketTimeout()
{
    if(GetPacketTime() > m_PacketWaitTime)
        return true;

    return false;
}

double LinuxLaser::GetPacketTime()
{
    double time;

    time = GetCurrentTime() - m_PacketStartTime;
    if(time < 0.0)
        m_PacketStartTime = GetCurrentTime();

    return time;
}

void LinuxLaser::SetUpdateTimeout(int msec)
{
    m_UpdateStartTime = GetCurrentTime();
    m_UpdateWaitTime = msec;
}

bool LinuxLaser::IsUpdateTimeout()
{
    if(GetUpdateTime() > m_UpdateWaitTime)
        return true;

    return false;
}

double LinuxLaser::GetUpdateTime()
{
    double time;

    time = GetCurrentTime() - m_UpdateStartTime;
    if(time < 0.0)
        m_UpdateStartTime = GetCurrentTime();

    return time;
}

void LinuxLaser::Sleep(double msec)
{
    double start_time = GetCurrentTime();
    double curr_time = start_time;

    do {
        usleep((start_time + msec) - curr_time);
        curr_time = GetCurrentTime();
    } while(curr_time - start_time < msec);
}
