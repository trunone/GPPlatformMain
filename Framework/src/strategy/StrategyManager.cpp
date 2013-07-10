/*
 *   StretagyManager.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include "StrategyManager.h"

using namespace Robot;

StrategyManager* StrategyManager::m_UniqueInstance = new StrategyManager();

StrategyManager::StrategyManager() :
        m_ProcessEnable(false),
        m_Enabled(false),
        m_IsRunning(false),
        m_IsThreadRunning(false),
        m_IsLogging(false),
        DEBUG_PRINT(false) {}

StrategyManager::~StrategyManager()
{
}

bool StrategyManager::Initialize(Motors *motors)
{
    mMotors = motors;
	m_Enabled = false;
	m_ProcessEnable = true;

    mMotors->SetVelocityProfileAll(1000, 1000);
    mMotors->ActivateProfileVelocityModeAll();

	return true;
}

bool StrategyManager::Reinitialize()
{
	m_ProcessEnable = false;

	return true;
}

void StrategyManager::StartLogging()
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
    for(short id = 1; id <= mMotors->NUMBER_OF_MOTORS; id++)
        m_LogFileStream << "ID_" << id << "_CURRENT,";
    m_LogFileStream << std::endl;

    m_IsLogging = true;
}

void StrategyManager::StopLogging()
{
    m_IsLogging = false;
    m_LogFileStream.close();
}

void StrategyManager::Process()
{
    if(m_ProcessEnable == false || m_IsRunning == true)
        return;

    m_IsRunning = true;

    if(m_Modules.size() != 0)
    {
        for(std::list<StrategyModule*>::iterator i = m_Modules.begin(); i != m_Modules.end(); i++)
        {
            (*i)->Process();
        }
    }

        mMotors->SetVelocityAll(
                -StrategyStatus::Motor1Speed,
                -StrategyStatus::Motor2Speed,
                -StrategyStatus::Motor3Speed);

    if(m_IsLogging)
    {
        for(int id = 1; id <= mMotors->NUMBER_OF_MOTORS; id++)
        {
            short current;
            mMotors->GetCurrentIs(id-1, &current);
            m_LogFileStream << current << ",";
        }

        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_GYRO_Y_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_GYRO_X_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_ACCEL_Y_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[CM730::ID_CM].ReadWord(CM730::P_ACCEL_X_L) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_L_FSR].ReadByte(FSR::P_FSR_X) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_L_FSR].ReadByte(FSR::P_FSR_Y) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_R_FSR].ReadByte(FSR::P_FSR_X) << ",";
        //m_LogFileStream << m_CM730->m_BulkReadData[FSR::ID_R_FSR].ReadByte(FSR::P_FSR_Y) << ",";
        m_LogFileStream << std::endl;
    }

    m_IsRunning = false;
}

void StrategyManager::SetEnable(bool enable)
{
	m_Enabled = enable;
	if(m_Enabled == true)
        mMotors->SetEnableAll();
}

void StrategyManager::AddModule(StrategyModule *module)
{
	module->Initialize();
	m_Modules.push_back(module);
}

void StrategyManager::RemoveModule(StrategyModule *module)
{
	m_Modules.remove(module);
}
