###############################################################
#
# Purpose: Makefile for "Wheeled Robot Linux Framework"
# Author.: Wu Chih-En
# Version: 0.1
# License: 
#
###############################################################

CC = g++
AR = ar
ARFLAGS = cr

TARGET = wheeled.a

INCLUDE_DIRS = -I../include -I../../Framework/include

CXXFLAGS += -O2 -DLINUX -Wall -shared $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall -shared $(INCLUDE_DIRS)
LFLAGS += -lpthread -ldl

OBJS =  ../../Framework/src/Laser.o\
        ../../Framework/src/math/Matrix.o\
        ../../Framework/src/math/Plane.o\
        ../../Framework/src/math/Point.o\
        ../../Framework/src/math/Vector.o\
        ../../Framework/src/vision/Image.o\
        ../../Framework/src/vision/ImgProcess.o\
        ../../Framework/src/vision/Camera.o\
        ../../Framework/src/minIni/minIni.o\
        ../../Framework/src/vision/ColorFinder.o\
        ../../Framework/src/vision/BallFollower.o\
        ../../Framework/src/vision/BallTracker.o\
        ../../Framework/src/strategy/StrategyManager.o\
        ../../Framework/src/strategy/StrategyStatus.o\
        ../../Framework/src/strategy/modules/Task.o\
        LinuxStrategyTimer.o\
        streamer/httpd.o\
        streamer/jpeg_utils.o\
        streamer/mjpg_streamer.o\
        LinuxCamera.o\
        LinuxLaser.o\
        LinuxActionScript.o\
        LinuxNetwork.o

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) ../lib/$(TARGET) $(OBJS)
#	$(CC) -o $(TARGET) $(OBJS) $(LFLAGS)

all: $(TARGET)

clean:
	rm -f $(OBJS) ../lib/$(TARGET)
