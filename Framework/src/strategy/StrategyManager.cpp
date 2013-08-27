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
bool StrategyManager::Initialize(Motors *motors,DXL *dxl)
{
    mMotors = motors;
    mDXL = dxl;
    m_Enabled = false;
    m_ProcessEnable = true;

    if(motors == NULL)
        return false;
    mMotors->SetEnableAll();
    mMotors->SetVelocityProfileAll(1000, 500);
    mMotors->ActivateProfileVelocityModeAll();

    if(dxl == NULL)
        return false;
    dxl->GoToDegree(17);
    dxl->EndlessTurn(0);
    return true;
}

bool StrategyManager::Initialize()
{
    mMotors = NULL;
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
    if(mMotors != NULL) {
        mMotors->SetVelocityAll(
            StrategyStatus::MotorSpeed[0],
            StrategyStatus::MotorSpeed[1],
            StrategyStatus::MotorSpeed[2]);
    }

    if(mDXL != NULL) {
        mDXL->GoToDegree(
            StrategyStatus::AX12_Angle);
        mDXL->EndlessTurn(
            StrategyStatus::CatchBallMode);
    }

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

int StrategyManager::LoadXMLSettings(TiXmlElement* element) {
    if(element != NULL) {
        TiXmlElement* child;
        child=element->FirstChildElement("RootHandle");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::RootHandle.x);
            child->Attribute("y", &StrategyStatus::RootHandle.y);
        }
        child=element->FirstChildElement("StartPos");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::StartPosition.x);
            child->Attribute("y", &StrategyStatus::StartPosition.y);
        }
        child=element->FirstChildElement("EndPos");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::EndPosition.x);
            child->Attribute("y", &StrategyStatus::EndPosition.y);
        }
        child=element->FirstChildElement("FindBallEn");
        if(child != NULL) {
            child->Attribute("FindBallEn", &StrategyStatus::FindBallEn);
        }
        child=element->FirstChildElement("LivRMDoor");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::LivRMDoor.x);
            child->Attribute("y", &StrategyStatus::LivRMDoor.y);
        }
        child=element->FirstChildElement("LivRMCen");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::LivRMCen.x);
            child->Attribute("y", &StrategyStatus::LivRMCen.y);
        }
        child=element->FirstChildElement("DinRMDoor");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::DinRMDoor.x);
            child->Attribute("y", &StrategyStatus::DinRMDoor.y);
        }
        child=element->FirstChildElement("DinRMCen");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::DinRMCen.x);
            child->Attribute("y", &StrategyStatus::DinRMCen.y);
        }
        child=element->FirstChildElement("LibDoor");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::LibDoor.x);
            child->Attribute("y", &StrategyStatus::LibDoor.y);
        }
        child=element->FirstChildElement("LibCen");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::LibCen.x);
            child->Attribute("y", &StrategyStatus::LibCen.y);
        }
        child=element->FirstChildElement("BedRMDoor");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::BedRMDoor.x);
            child->Attribute("y", &StrategyStatus::BedRMDoor.y);
        }
        child=element->FirstChildElement("BedRMCen");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::BedRMCen.x);
            child->Attribute("y", &StrategyStatus::BedRMCen.y);
        }
        child=element->FirstChildElement("ChrgDoor");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::ChrgDoor.x);
            child->Attribute("y", &StrategyStatus::ChrgDoor.y);
        }
        child=element->FirstChildElement("ChrgCen");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::ChrgCen.x);
            child->Attribute("y", &StrategyStatus::ChrgCen.y);
        }
        child=element->FirstChildElement("EndPosition");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::EndPosition.x);
            child->Attribute("y", &StrategyStatus::EndPosition.y);
        }
		child=element->FirstChildElement("DinRMAStarPos");
		if(child != NULL){
			child->Attribute("x", &StrategyStatus::DinRMAStarPos.x);
			child->Attribute("y", &StrategyStatus::DinRMAStarPos.y);						
        }
		child=element->FirstChildElement("LibAStarPos");
		if(child != NULL){
			child->Attribute("x", &StrategyStatus::LibAStarPos.x);
			child->Attribute("y", &StrategyStatus::LibAStarPos.y);						
        }
		child=element->FirstChildElement("BedRMAStarPos");
		if(child != NULL){
			child->Attribute("x", &StrategyStatus::LibAStarPos.x);
			child->Attribute("y", &StrategyStatus::LibAStarPos.y);				
        }
		child=element->FirstChildElement("EatBall");
		if(child != NULL){
			child->Attribute("flag", &StrategyStatus::FlagEatBall);
        }
		child=element->FirstChildElement("ThiefPos");
		if(child != NULL){
			child->Attribute("x", &StrategyStatus::ThiefPos.x);
			child->Attribute("y", &StrategyStatus::ThiefPos.y);				
        }

    }
    else
        return 1;
    return 0;
}
