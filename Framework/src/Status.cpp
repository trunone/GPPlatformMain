/*
 *   Status.cpp
 *
 *   Author: Wu Chih-En
 *
 */


#include "Status.h"

using namespace Robot;

CvCapture* Status::visioncapture(NULL);
int Status::a(NULL);
int Status::front(0);
int Status::left(0);
int Status::right(0);
long   Status::time(0);
double Status::FI(0);
double Status::w(0);
Vector3D Status::vector;

