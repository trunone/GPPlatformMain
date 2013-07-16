/*
 *   LocationManager.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include <vector>
#include "LocationManager.h"
#include "TCoordinate.h"
#include "LocationStatus.h"

using namespace Robot;
using namespace qrk;

LocationManager* LocationManager::m_UniqueInstance = new LocationManager();

LocationManager::LocationManager() :
        m_ProcessEnable(false),
        m_Enabled(false),
        m_IsRunning(false),
        m_IsThreadRunning(false),
        m_IsLogging(false),
        DEBUG_PRINT(false) {}

LocationManager::~LocationManager()
{
}

bool LocationManager::Initialize(Urg_driver *urg)
{
	m_Enabled = false;
	m_ProcessEnable = true;
	Position = TCoordinate::aVector(26,273);
    	Handle   = 0.0;
    	FlagEvaluatuonEnable = true;
    	FlagCoerceEvaluatuon = false;
	LocationStatus::Position = TCoordinate::aVector(26,273);
    	LocationStatus::Handle   = 0.0;
    	LocationStatus::FlagEvaluatuonEnable = true;
    	LocationStatus::FlagCoerceEvaluatuon = false;

    urg = urg;

    if (!urg->open("/dev/ttyACM0", 115200, Urg_driver::Serial ))
        fprintf(stderr,  "Urg_driver::open(    ): %s\n", urg->what());

    urg->set_scanning_parameter(urg->deg2step(-90), urg->deg2step(+90), 0);
    urg->start_measurement(Urg_driver::Distance, 0, 0);

	return true;
}

bool LocationManager::Reinitialize()
{
	m_ProcessEnable = false;

	return true;
}

void LocationManager::StartLogging()
{
    char szFile[32] = {0,};

    int count = 0;
    while(1)
    {
        sprintf(szFile, "Logs/Log%d.csv", count);
        if(0 != access(szFile, F_OK))
            break;
        count++;
		if(count > 256) return;
    }

    m_LogFileStream.open(szFile, std::ios::out);
    //for(int id = 1; id < JointData::NUMBER_OF_JOINTS; id++)
    //    m_LogFileStream << "ID_" << id << "_GP,ID_" << id << "_PP,";
    //m_LogFileStream << "GyroFB,GyroRL,AccelFB,AccelRL,L_FSR_X,L_FSR_Y,R_FSR_X,R_FSR_Y" << std::endl;

    m_IsLogging = true;
}

void LocationManager::StopLogging()
{
    m_IsLogging = false;
    m_LogFileStream.close();
}

void LocationManager::Process()
{
    if(m_ProcessEnable == false || m_IsRunning == true)
        return;

    m_IsRunning = true;

    std::vector<long> data;
    long time_stamp = 0;
    if (!urg->get_distance(data, &time_stamp))
        fprintf(stderr,  "Urg_driver:: get_distance(): %s\n", urg->what());

    if(m_Modules.size() != 0)
    {
        for(std::list<LocationModule*>::iterator i = m_Modules.begin(); i != m_Modules.end(); i++)
        {
            (*i)->Process();
        }
    }

    if(m_IsLogging)
    {
        //for(int id = 1; id < JointData::NUMBER_OF_JOINTS; id++)
        //    m_LogFileStream << Status::m_CurrentJoints.GetValue(id) << "," << m_CM730->m_BulkReadData[id].ReadWord(MX28::P_PRESENT_POSITION_L) << ",";

        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_GYRO_Y_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_GYRO_X_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_ACCEL_Y_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_ACCEL_X_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_L_FSR].ReadByte(FSR::P_FSR_X) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_L_FSR].ReadByte(FSR::P_FSR_Y) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_R_FSR].ReadByte(FSR::P_FSR_X) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_R_FSR].ReadByte(FSR::P_FSR_Y) << ",";
        //m_LogFileStream << std::endl;
    }

    m_IsRunning = false;
}

void LocationManager::SetEnable(bool enable)
{
	m_Enabled = enable;
	//if(m_Enabled == true)
	//	m_CM730->WriteWord(CM730::ID_BROADCAST, MX28::P_MOVING_SPEED_L, 0, 0);
}

void LocationManager::AddModule(LocationModule *module)
{
	module->Initialize();
	m_Modules.push_back(module);
}

void LocationManager::RemoveModule(LocationModule *module)
{
	m_Modules.remove(module);
}
