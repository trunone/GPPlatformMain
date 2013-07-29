/*
 *   StretagyManager.cpp
 *
 *   Author: Wu Chih-En
 *
 */

#include <stdio.h>
#include <math.h>
#include <string>
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

    mMotors->SetVelocityProfileAll(1000, 500);
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
        m_LogFileStream << "ID_" << id << "_CMD,"<< "ID_" << id << "_VELOCITY,"<< "ID_" << id << "_CURRENT,";
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
                -StrategyStatus::MotorSpeed[0],
                -StrategyStatus::MotorSpeed[1],
                -StrategyStatus::MotorSpeed[2]);

    if(m_IsLogging)
    {
        for(int id = 1; id <= mMotors->NUMBER_OF_MOTORS; id++)
        {
            short current;
            long velocity;
            mMotors->GetCurrentIs(id-1, &current);
            mMotors->GetVelocityIs(id-1, &velocity);
            m_LogFileStream << -StrategyStatus::MotorSpeed[id-1] << "," << velocity << "," << current << ",";
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
	if(mMotors)
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

int StrategyManager::LoadXMLSettings(TiXmlElement* element){
    if(element != NULL){	
		TiXmlElement* modelchild;
		modelchild=element->FirstChildElement("RootHandle");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::RootHandle.x);
			modelchild->Attribute("y", &StrategyStatus::RootHandle.y);	
		}
		modelchild=element->FirstChildElement("StartPos");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::StartPosition.x);
			modelchild->Attribute("y", &StrategyStatus::StartPosition.y);						
		}
		modelchild=element->FirstChildElement("EndPos");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::EndPosition.x);
			modelchild->Attribute("y", &StrategyStatus::EndPosition.y);						
		}
		modelchild=element->FirstChildElement("FindBallEn");
		if(modelchild != NULL){
			modelchild->Attribute("FindBallEn", &StrategyStatus::FindBallEn);
		}
		modelchild=element->FirstChildElement("LivRMDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LivRMDoor.x);
			modelchild->Attribute("y", &StrategyStatus::LivRMDoor.y);						
		}
		modelchild=element->FirstChildElement("LivRMCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LivRMCen.x);
			modelchild->Attribute("y", &StrategyStatus::LivRMCen.y);						
		}
		modelchild=element->FirstChildElement("DinRMDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::DinRMDoor.x);
			modelchild->Attribute("y", &StrategyStatus::DinRMDoor.y);						
		}
		modelchild=element->FirstChildElement("DinRMCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::DinRMCen.x);
			modelchild->Attribute("y", &StrategyStatus::DinRMCen.y);						
		}
		modelchild=element->FirstChildElement("LibDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LibDoor.x);
			modelchild->Attribute("y", &StrategyStatus::LibDoor.y);						
		}
		modelchild=element->FirstChildElement("LibCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::LibCen.x);
			modelchild->Attribute("y", &StrategyStatus::LibCen.y);						
		}
		modelchild=element->FirstChildElement("BedRMDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::BedRMDoor.x);
			modelchild->Attribute("y", &StrategyStatus::BedRMDoor.y);						
		}
		modelchild=element->FirstChildElement("BedRMCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::BedRMCen.x);
			modelchild->Attribute("y", &StrategyStatus::BedRMCen.y);						
		}
		modelchild=element->FirstChildElement("ChrgDoor");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::ChrgDoor.x);
			modelchild->Attribute("y", &StrategyStatus::ChrgDoor.y);						
		}
		modelchild=element->FirstChildElement("ChrgCen");
		if(modelchild != NULL){
			modelchild->Attribute("x", &StrategyStatus::ChrgCen.x);
			modelchild->Attribute("y", &StrategyStatus::ChrgCen.y);						
        }
    }
	return 0;
}
