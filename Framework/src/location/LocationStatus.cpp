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

float LocationStatus::ScanLineData[24];
bool LocationStatus::FlagEvaluatuonEnable;
bool LocationStatus::FlagCoerceEvaluatuon;
TCoordinate LocationStatus::Position;
double LocationStatus::Handle;
bool LocationStatus::FlagNewFeedback;
bool LocationStatus::enable;


