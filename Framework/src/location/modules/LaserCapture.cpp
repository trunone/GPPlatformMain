/*
 *   LaserCapture.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <iostream>
#include <stdio.h>
#include "Status.h"
#include "LocationStatus.h"
#include "LaserCapture.h"

using namespace Robot;
using namespace std;
using namespace qrk;


LaserCapture* LaserCapture::m_UniqueInstance = new LaserCapture();

LaserCapture::LaserCapture()
{

}

LaserCapture::~LaserCapture()
{
}

void LaserCapture::Initialize()
{    
    if (!urg.open("/dev/ttyACM0", 115200, Urg_driver::Serial )) {
        cout << "Urg_driver::open(    ) : " << " /dev/ttyACM0 : " << urg.what() << endl;
    }
    urg.set_scanning_parameter(urg.deg2step(+90), urg.deg2step(-90), 0);
    urg.start_measurement(Urg_driver::Distance, 0, 0);

}

void LaserCapture::LoadINISettings(minIni* ini)
{
    LoadINISettings(ini, TASK_SECTION);
}

void LaserCapture::LoadINISettings(minIni* ini, const std::string &section)
{
    //if((value = ini->getd(section, "pan_p_gain", INVALID_VALUE)) != INVALID_VALUE)  m_Pan_p_gain = value;
    //if((value = ini->getd(section, "pan_d_gain", INVALID_VALUE)) != INVALID_VALUE)  m_Pan_d_gain = value;
    //if((value = ini->getd(section, "tilt_p_gain", INVALID_VALUE)) != INVALID_VALUE) m_Tilt_p_gain = value;
    //if((value = ini->getd(section, "tilt_d_gain", INVALID_VALUE)) != INVALID_VALUE) m_Tilt_d_gain = value;
    //if((value = ini->getd(section, "left_limit", INVALID_VALUE)) != INVALID_VALUE)  m_LeftLimit = value;
    //if((value = ini->getd(section, "right_limit", INVALID_VALUE)) != INVALID_VALUE) m_RightLimit = value;
    //if((value = ini->getd(section, "top_limit", INVALID_VALUE)) != INVALID_VALUE)   m_TopLimit = value;
    //if((value = ini->getd(section, "bottom_limit", INVALID_VALUE)) != INVALID_VALUE)m_BottomLimit = value;
    //if((value = ini->getd(section, "pan_home", INVALID_VALUE)) != INVALID_VALUE)    m_Pan_Home = value;
    //if((value = ini->getd(section, "tilt_home", INVALID_VALUE)) != INVALID_VALUE)   m_Tilt_Home = value;
}

void LaserCapture::SaveINISettings(minIni* ini)
{
    SaveINISettings(ini, TASK_SECTION);
}

void LaserCapture::SaveINISettings(minIni* ini, const std::string &section)
{
    //ini->put(section,   "pan_p_gain",   m_Pan_p_gain);
    //ini->put(section,   "pan_d_gain",   m_Pan_d_gain);
    //ini->put(section,   "tilt_p_gain",  m_Tilt_p_gain);
    //ini->put(section,   "tilt_d_gain",  m_Tilt_d_gain);
    //ini->put(section,   "left_limit",   m_LeftLimit);
    //ini->put(section,   "right_limit",  m_RightLimit);
    //ini->put(section,   "top_limit",    m_TopLimit);
    //ini->put(section,   "bottom_limit", m_BottomLimit);
    //ini->put(section,   "pan_home",     m_Pan_Home);
    //ini->put(section,   "tilt_home",    m_Tilt_Home);
}

void LaserCapture::Process()
{
    if (!urg.get_distance(LocationStatus::data, &LocationStatus::time_stamp)) {
        cout << "Urg_driver::get_distance() : " << urg.what() << endl;
    }
    Status::front = LocationStatus::data[urg.deg2index(0)];
    Status::left  = LocationStatus::data[urg.deg2index(+90)];
    Status::right = LocationStatus::data[urg.deg2index(-90)];
    Status::time = LocationStatus::time_stamp;
}

