###############################################################
#
# Purpose: Makefile for "Wheeled Robot Linux Framework"
# Author.: Wu Chih-En
# Version: 0.1
# License: 
#
###############################################################

CXX = g++
AR = ar
ARFLAGS = cr

TARGET = wheeled.a

INCLUDE_DIRS = -I../include -I../../Framework/include

CXXFLAGS += -O2 -DLINUX -Wall -shared -fopenmp $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall -shared $(INCLUDE_DIRS)
LFLAGS += -lpthread -ldl

OBJS =  ../../Framework/src/location/LocationManager.o\
        ../../Framework/src/location/LocationStatus.o\
		../../Framework/src/location/ProbabilityEvaluation.o\
        ../../Framework/src/location/modules/ParticleFilter.o\
		../../Framework/src/location/modules/Odometer.o\
		../../Framework/src/math/Matrix.o\
        ../../Framework/src/math/Plane.o\
        ../../Framework/src/math/Point.o\
        ../../Framework/src/math/Vector.o\
		../../Framework/src/math/TCoordinate.o\
		../../Framework/src/math/Normalrand.o\
		../../Framework/src/vision/VisionStatus.o\
		../../Framework/src/vision/modules/SegmentationFunction.o\
        ../../Framework/src/vision/modules/FaceDetection.o\
		../../Framework/src/vision/modules/SendImage.o\
		../../Framework/src/vision/modules/ColorModel.o\
		../../Framework/src/vision/VisionManager.o\
		../../Framework/src/vision/VisionStatus.o\
		../../Framework/src/vision/ImgProcess.o\
        ../../Framework/src/strategy/StrategyManager.o\
        ../../Framework/src/strategy/StrategyStatus.o\
		../../Framework/src/strategy/AstarTool.o\
        ../../Framework/src/strategy/modules/Stra_Task.o\
		../../Framework/src/strategy/modules/Stra_FindBall.o\
		../../Framework/src/strategy/modules/Stra_Avoid.o\
		../../Framework/src/strategy/modules/Stra_PathPlan.o\
		../../Framework/src/strategy/modules/Stra_VelocityControl.o\
		../../Framework/src/strategy/modules/Stra_AStar.o\
		../../Framework/src/strategy/modules/Motion.o\
        ../../Framework/src/Motors.o\
        LinuxStrategyTimer.o\
        LinuxVisionTimer.o\
        LinuxLocationTimer.o\
        LinuxActionScript.o\
        LinuxNetwork.o

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p ../lib
	$(AR) $(ARFLAGS) ../lib/$(TARGET) $(OBJS)
#	$(CXX) -o $(TARGET) $(OBJS) $(LFLAGS)

clean:
	rm -f $(OBJS) ../lib/$(TARGET)


