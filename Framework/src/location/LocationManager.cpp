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
	m_Enabled = false;
	m_ProcessEnable = true;
	Position = aVector(26,273);
    Handle   = 0.0;
    FlagEvaluatuonEnable = true;
    FlagCoerceEvaluatuon = false;
	LocationStatus::Position = aVector(26,273);
    LocationStatus::Handle   = 0.0;
    LocationStatus::FlagEvaluatuonEnable = true;
    LocationStatus::FlagCoerceEvaluatuon = false;

    mUrg = urg;
    mMotors = motors;

    mUrg->set_scanning_parameter(mUrg->deg2step(-90), mUrg->deg2step(+90), 0);
    mUrg->start_measurement(Urg_driver::Distance, 0, 0);

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

    if (!mUrg->get_distance(LocationStatus::data, &LocationStatus::time_stamp))
        fprintf(stderr,  "mUrg_driver:: get_distance(): %s\n", mUrg->what());

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

int LocationManager::LoadXMLSettings(TiXmlElement* element){
	if(element != NULL){	
		TiXmlElement* modelchild;
		modelchild=element->FirstChildElement("LivingRM_Sofa");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::LivingRM_Sofa.x);
			modelchild->Attribute("y", &LocationStatus::LivingRM_Sofa.y);	
			modelchild->Attribute("Width", &LocationStatus::LivingRM_Sofa.Width);
			modelchild->Attribute("Height", &LocationStatus::LivingRM_Sofa.Height);
			modelchild->Attribute("Angle", &LocationStatus::LivingRM_Sofa.Angle);						
		}
		modelchild=element->FirstChildElement("LivingRM_Table");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::LivingRM_Table.x);
			modelchild->Attribute("y", &LocationStatus::LivingRM_Table.y);	
			modelchild->Attribute("Width", &LocationStatus::LivingRM_Table.Width);
			modelchild->Attribute("Height", &LocationStatus::LivingRM_Table.Height);
			modelchild->Attribute("Angle", &LocationStatus::LivingRM_Table.Angle);						
		}
		modelchild=element->FirstChildElement("LivingRM_Cabinet");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::LivingRM_Cabinet.x);
			modelchild->Attribute("y", &LocationStatus::LivingRM_Cabinet.y);	
			modelchild->Attribute("Width", &LocationStatus::LivingRM_Cabinet.Width);
			modelchild->Attribute("Height", &LocationStatus::LivingRM_Cabinet.Height);
			modelchild->Attribute("Angle", &LocationStatus::LivingRM_Cabinet.Angle);						
		}
		modelchild=element->FirstChildElement("DiningRM_Table");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::DiningRM_Table.x);
			modelchild->Attribute("y", &LocationStatus::DiningRM_Table.y);	
			modelchild->Attribute("Width", &LocationStatus::DiningRM_Table.Width);
			modelchild->Attribute("Height", &LocationStatus::DiningRM_Table.Height);
			modelchild->Attribute("Angle", &LocationStatus::DiningRM_Table.Angle);						
		}
		modelchild=element->FirstChildElement("DiningRM_Chair");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::DiningRM_Chair.x);
			modelchild->Attribute("y", &LocationStatus::DiningRM_Chair.y);	
			modelchild->Attribute("Width", &LocationStatus::DiningRM_Chair.Width);
			modelchild->Attribute("Height", &LocationStatus::DiningRM_Chair.Height);
			modelchild->Attribute("Angle", &LocationStatus::DiningRM_Chair.Angle);						
		}
		modelchild=element->FirstChildElement("Library_Desk");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::Library_Desk.x);
			modelchild->Attribute("y", &LocationStatus::Library_Desk.y);	
			modelchild->Attribute("Width", &LocationStatus::Library_Desk.Width);
			modelchild->Attribute("Height", &LocationStatus::Library_Desk.Height);
			modelchild->Attribute("Angle", &LocationStatus::Library_Desk.Angle);						
		}
		modelchild=element->FirstChildElement("Library_Chair");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::Library_Chair.x);
			modelchild->Attribute("y", &LocationStatus::Library_Chair.y);	
			modelchild->Attribute("Width", &LocationStatus::Library_Chair.Width);
			modelchild->Attribute("Height", &LocationStatus::Library_Chair.Height);
			modelchild->Attribute("Angle", &LocationStatus::Library_Chair.Angle);						
		}
		modelchild=element->FirstChildElement("Library_Cabinet");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::Library_Cabinet.x);
			modelchild->Attribute("y", &LocationStatus::Library_Cabinet.y);	
			modelchild->Attribute("Width", &LocationStatus::Library_Cabinet.Width);
			modelchild->Attribute("Height", &LocationStatus::Library_Cabinet.Height);
			modelchild->Attribute("Angle", &LocationStatus::Library_Cabinet.Angle);						
		}
		modelchild=element->FirstChildElement("BedRM_Bed");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::BedRM_Bed.x);
			modelchild->Attribute("y", &LocationStatus::BedRM_Bed.y);	
			modelchild->Attribute("Width", &LocationStatus::BedRM_Bed.Width);
			modelchild->Attribute("Height", &LocationStatus::BedRM_Bed.Height);
			modelchild->Attribute("Angle", &LocationStatus::BedRM_Bed.Angle);						
		}
		modelchild=element->FirstChildElement("DiningRM_door");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::DiningRM_door.x);
			modelchild->Attribute("y", &LocationStatus::DiningRM_door.y);	
			modelchild->Attribute("Width", &LocationStatus::DiningRM_door.Width);
			modelchild->Attribute("Height", &LocationStatus::DiningRM_door.Height);
			modelchild->Attribute("Angle", &LocationStatus::DiningRM_door.Angle);						
		}
		modelchild=element->FirstChildElement("BedRM_door");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::BedRM_door.x);
			modelchild->Attribute("y", &LocationStatus::BedRM_door.y);	
			modelchild->Attribute("Width", &LocationStatus::BedRM_door.Width);
			modelchild->Attribute("Height", &LocationStatus::BedRM_door.Height);
			modelchild->Attribute("Angle", &LocationStatus::BedRM_door.Angle);						
		}
		modelchild=element->FirstChildElement("ChargeArea");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::ChargeArea.x);
			modelchild->Attribute("y", &LocationStatus::ChargeArea.y);	
			modelchild->Attribute("Width", &LocationStatus::ChargeArea.Width);
			modelchild->Attribute("Height", &LocationStatus::ChargeArea.Height);
			modelchild->Attribute("Angle", &LocationStatus::ChargeArea.Angle);						
		}
		modelchild=element->FirstChildElement("EndArea");
		if(modelchild != NULL){
			modelchild->Attribute("x", &LocationStatus::EndArea.x);
			modelchild->Attribute("y", &LocationStatus::EndArea.y);	
			modelchild->Attribute("Width", &LocationStatus::EndArea.Width);
			modelchild->Attribute("Height", &LocationStatus::EndArea.Height);
			modelchild->Attribute("Angle", &LocationStatus::EndArea.Angle);						
			}
	}
	return 0;
}
