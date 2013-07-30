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

bool LocationStatus::FlagEvaluatuonEnable;
bool LocationStatus::FlagCoerceEvaluatuon;
TCoordinate LocationStatus::Position;
double LocationStatus::Handle;
bool LocationStatus::enable;
TCoordinate LocationStatus::LaserGap;

//vector<long> LocationStatus::data;
//long LocationStatus::time_stamp(0);
