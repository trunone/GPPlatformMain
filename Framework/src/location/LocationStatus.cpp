/*
 *   LocationStatus.cpp
 *
 *   Author: Wu Chih-En
 *
 */


#include "LocationStatus.h"
#include "TCoordinate.h"
#include <stdio.h>


using namespace Robot;
/*
LocationStatus::LivingRM_Sofa = {0,0,0,0,0};
LocationStatus::LivingRM_Table = {0,0,0,0,0};
LocationStatus::LivingRM_Cabinet = {0,0,0,0,0};
LocationStatus::DiningRM_Table = {0,0,0,0,0};
LocationStatus::DiningRM_Chair = {0,0,0,0,0};
LocationStatus::Library_Desk = {0,0,0,0,0};
LocationStatus::Library_Chair = {0,0,0,0,0};
LocationStatus::Library_Cabinet = {0,0,0,0,0};
LocationStatus::BedRM_Bed = {0,0,0,0,0};
LocationStatus::DiningRM_door = {0,0,0,0,0};
LocationStatus::BedRM_door = {0,0,0,0,0};
LocationStatus::ChargeArea = {0,0,0,0,0};
LocationStatus::EndArea = {0,0,0,0,0};
LocationStatus::LivingRM_Sofa = {0,0,0,0,0};
*/

LocationStatus::Furniture LocationStatus::LivingRM_Sofa;
LocationStatus::Furniture LocationStatus::LivingRM_Table;
LocationStatus::Furniture LocationStatus::LivingRM_Cabinet;
LocationStatus::Furniture LocationStatus::DiningRM_Table;
LocationStatus::Furniture LocationStatus::DiningRM_Chair;
LocationStatus::Furniture LocationStatus::Library_Desk;
LocationStatus::Furniture LocationStatus::Library_Chair;
LocationStatus::Furniture LocationStatus::Library_Cabinet;
LocationStatus::Furniture LocationStatus::BedRM_Bed;
LocationStatus::Furniture LocationStatus::DiningRM_door;
LocationStatus::Furniture LocationStatus::BedRM_door;
LocationStatus::Furniture LocationStatus::ChargeArea;
LocationStatus::Furniture LocationStatus::EndArea;

/*
int LocationStatus::loadxml (TiXmlElement* element){
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
*/

float LocationStatus::ScanLineData[24];
bool LocationStatus::FlagEvaluatuonEnable;
bool LocationStatus::FlagCoerceEvaluatuon;
TCoordinate LocationStatus::Position;
double LocationStatus::Handle;
bool LocationStatus::FlagNewFeedback;
bool LocationStatus::enable;
TCoordinate LocationStatus::LaserGap;

