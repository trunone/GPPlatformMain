/*
 *   StretagyManager.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include "StrategyManager.h"
#include "VisionCapture.h"
#include "LaserCapture.h"
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

bool StrategyManager::Initialize()
{
	m_Enabled = false;
	m_ProcessEnable = true;

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
    //for(int id = 1; id < JointData::NUMBER_OF_JOINTS; id++)
    //    m_LogFileStream << "ID_" << id << "_GP,ID_" << id << "_PP,";
    //m_LogFileStream << "GyroFB,GyroRL,AccelFB,AccelRL,L_FSR_X,L_FSR_Y,R_FSR_X,R_FSR_Y" << std::endl;

    m_IsLogging = true;
}

void StrategyManager::StopLogging()
{
    m_IsLogging = false;
    m_LogFileStream.close();
}

void StrategyManager::LoadINISettings(minIni* ini)
{
    LoadINISettings(ini, OFFSET_SECTION);
}
void StrategyManager::LoadINISettings(minIni* ini, const std::string &section)
{
    int ivalue = INVALID_VALUE;

    //for(int i = 1; i < JointData::NUMBER_OF_JOINTS; i++)
    //{
    //    char key[10];
    //    sprintf(key, "ID_%.2d", i);
    //    if((ivalue = ini->geti(section, key, INVALID_VALUE)) != INVALID_VALUE)  m_Offset[i] = ivalue;
    //}
}
void StrategyManager::SaveINISettings(minIni* ini)
{
    SaveINISettings(ini, OFFSET_SECTION);
}
void StrategyManager::SaveINISettings(minIni* ini, const std::string &section)
{
    //for(int i = 1; i < JointData::NUMBER_OF_JOINTS; i++)
    //{
    //    char key[10];
    //    sprintf(key, "ID_%.2d", i);
    //    ini->put(section, key, m_Offset[i]);
    //}
}

void StrategyManager::Process()
{
    if(m_ProcessEnable == false || m_IsRunning == true)
        return;

    m_IsRunning = true;

    if(m_Modules.size() != 0) {
        for(std::list<StrategyModule*>::iterator i = m_Modules.begin(); i != m_Modules.end(); i++)
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

void StrategyManager::SetEnable(bool enable)
{
	m_Enabled = enable;
	//if(m_Enabled == true)
	//	m_CM730->WriteWord(CM730::ID_BROADCAST, MX28::P_MOVING_SPEED_L, 0, 0);
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
