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

bool LocationManager::Initialize(Urg_driver *urg, Motors *motors)
{
    return true&&Initialize()&&Initialize(urg)&&Initialize(motors);
}
bool LocationManager::Initialize(Motors* motors) {
    mMotors = motors;
    return true&&Initialize();
}
bool LocationManager::Initialize(qrk::Urg_driver* urg) {
    mUrg = urg;

    mUrg->set_scanning_parameter(mUrg->deg2step(-90), mUrg->deg2step(+90), 10);
    mUrg->start_measurement(Urg_driver::Distance, 0, 0);

    if (!mUrg->get_distance(LocationStatus::LaserData, &LocationStatus::TimeStamp))
        fprintf(stderr,  "urg_driver:: get_distance(): %s\n", mUrg->what());

    return true&&Initialize();
}

bool LocationManager::Initialize() {
    m_Enabled = false;
    m_ProcessEnable = true;

    LocationStatus::Position = aVector(35, 260);
    LocationStatus::Handle   = 0.0;
    LocationStatus::FlagEvaluatuonEnable = true;
    LocationStatus::FlagCoerceEvaluatuon = false;
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

    if(mUrg != NULL)
        if (!mUrg->get_distance(LocationStatus::LaserData, &LocationStatus::TimeStamp))
            fprintf(stderr,  "urg_driver:: get_distance(): %s\n", mUrg->what());

    if(mMotors != NULL) {
        mMotors->GetPositionIs(0, &LocationStatus::MotorPulse[0]);
        mMotors->GetPositionIs(1, &LocationStatus::MotorPulse[1]);
        mMotors->GetPositionIs(2, &LocationStatus::MotorPulse[2]);
    }

    if(m_Modules.size() != 0)
    {
        for(std::list<LocationModule*>::iterator i = m_Modules.begin(); i != m_Modules.end(); i++)
        {
            (*i)->Process();
        }
    }

    //printf("%f, %f, %f\n", LocationStatus::Position.x, LocationStatus::Position.y, LocationStatus::Handle);

    if(m_IsLogging)
    {
        m_LogFileStream << "Encoder" << "," << std::endl;
        m_LogFileStream << LocationStatus::MotorPulse[0] << ","<< LocationStatus::MotorPulse[1] << ","<< LocationStatus::MotorPulse[2] << "," << std::endl;
        //m_LogFileStream << LocationStatus::FB_Movement.Position.x << ","<< LocationStatus::FB_Movement.Position.y << ","<< LocationStatus::FB_Movement.Direction << "," << std::endl;
        //m_LogFileStream << "Laser" << "," << std::endl;
        //for(int i = 0; i < LocationStatus::LaserData.size(); i++)
        //    m_LogFileStream << LocationStatus::LaserData[i] << ",";
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

int LocationManager::LoadXMLSettings(TiXmlElement* element) {
    if(element != NULL) {
        TiXmlElement* child;
        child=element->FirstChildElement("LivingRM_Sofa");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::LivingRM_Sofa.x);
            child->Attribute("y", &LocationStatus::LivingRM_Sofa.y);
            child->Attribute("Width", &LocationStatus::LivingRM_Sofa.Width);
            child->Attribute("Height", &LocationStatus::LivingRM_Sofa.Height);
            child->Attribute("Angle", &LocationStatus::LivingRM_Sofa.Angle);
        }
        delete child;
        child=element->FirstChildElement("LivingRM_Table");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::LivingRM_Table.x);
            child->Attribute("y", &LocationStatus::LivingRM_Table.y);
            child->Attribute("Width", &LocationStatus::LivingRM_Table.Width);
            child->Attribute("Height", &LocationStatus::LivingRM_Table.Height);
            child->Attribute("Angle", &LocationStatus::LivingRM_Table.Angle);
        }
        delete child;
        child=element->FirstChildElement("LivingRM_Cabinet");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::LivingRM_Cabinet.x);
            child->Attribute("y", &LocationStatus::LivingRM_Cabinet.y);
            child->Attribute("Width", &LocationStatus::LivingRM_Cabinet.Width);
            child->Attribute("Height", &LocationStatus::LivingRM_Cabinet.Height);
            child->Attribute("Angle", &LocationStatus::LivingRM_Cabinet.Angle);
        }
        delete child;
        child=element->FirstChildElement("DiningRM_Table");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::DiningRM_Table.x);
            child->Attribute("y", &LocationStatus::DiningRM_Table.y);
            child->Attribute("Width", &LocationStatus::DiningRM_Table.Width);
            child->Attribute("Height", &LocationStatus::DiningRM_Table.Height);
            child->Attribute("Angle", &LocationStatus::DiningRM_Table.Angle);
        }
        delete child;
        child=element->FirstChildElement("DiningRM_Chair");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::DiningRM_Chair.x);
            child->Attribute("y", &LocationStatus::DiningRM_Chair.y);
            child->Attribute("Width", &LocationStatus::DiningRM_Chair.Width);
            child->Attribute("Height", &LocationStatus::DiningRM_Chair.Height);
            child->Attribute("Angle", &LocationStatus::DiningRM_Chair.Angle);
        }
        delete child;
        child=element->FirstChildElement("Library_Desk");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::Library_Desk.x);
            child->Attribute("y", &LocationStatus::Library_Desk.y);
            child->Attribute("Width", &LocationStatus::Library_Desk.Width);
            child->Attribute("Height", &LocationStatus::Library_Desk.Height);
            child->Attribute("Angle", &LocationStatus::Library_Desk.Angle);
        }
        delete child;
        child=element->FirstChildElement("Library_Chair");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::Library_Chair.x);
            child->Attribute("y", &LocationStatus::Library_Chair.y);
            child->Attribute("Width", &LocationStatus::Library_Chair.Width);
            child->Attribute("Height", &LocationStatus::Library_Chair.Height);
            child->Attribute("Angle", &LocationStatus::Library_Chair.Angle);
        }
        delete child;
        child=element->FirstChildElement("Library_Cabinet");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::Library_Cabinet.x);
            child->Attribute("y", &LocationStatus::Library_Cabinet.y);
            child->Attribute("Width", &LocationStatus::Library_Cabinet.Width);
            child->Attribute("Height", &LocationStatus::Library_Cabinet.Height);
            child->Attribute("Angle", &LocationStatus::Library_Cabinet.Angle);
        }
        delete child;
        child=element->FirstChildElement("BedRM_Bed");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::BedRM_Bed.x);
            child->Attribute("y", &LocationStatus::BedRM_Bed.y);
            child->Attribute("Width", &LocationStatus::BedRM_Bed.Width);
            child->Attribute("Height", &LocationStatus::BedRM_Bed.Height);
            child->Attribute("Angle", &LocationStatus::BedRM_Bed.Angle);
        }
        delete child;
        child=element->FirstChildElement("DiningRM_door");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::DiningRM_door.x);
            child->Attribute("y", &LocationStatus::DiningRM_door.y);
            child->Attribute("Width", &LocationStatus::DiningRM_door.Width);
            child->Attribute("Height", &LocationStatus::DiningRM_door.Height);
            child->Attribute("Angle", &LocationStatus::DiningRM_door.Angle);
        }
        delete child;
        child=element->FirstChildElement("BedRM_door");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::BedRM_door.x);
            child->Attribute("y", &LocationStatus::BedRM_door.y);
            child->Attribute("Width", &LocationStatus::BedRM_door.Width);
            child->Attribute("Height", &LocationStatus::BedRM_door.Height);
            child->Attribute("Angle", &LocationStatus::BedRM_door.Angle);
        }
        delete child;
        child=element->FirstChildElement("ChargeArea");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::ChargeArea.x);
            child->Attribute("y", &LocationStatus::ChargeArea.y);
            child->Attribute("Width", &LocationStatus::ChargeArea.Width);
            child->Attribute("Height", &LocationStatus::ChargeArea.Height);
            child->Attribute("Angle", &LocationStatus::ChargeArea.Angle);
        }
        delete child;
        child=element->FirstChildElement("EndArea");
        if(child != NULL) {
            child->Attribute("x", &LocationStatus::EndArea.x);
            child->Attribute("y", &LocationStatus::EndArea.y);
            child->Attribute("Width", &LocationStatus::EndArea.Width);
            child->Attribute("Height", &LocationStatus::EndArea.Height);
            child->Attribute("Angle", &LocationStatus::EndArea.Angle);
        }
    }
    else
        return 1;
    return 0;
}
