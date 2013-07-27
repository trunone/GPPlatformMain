###############################################################
#
# Purpose: Makefile for "SKS"
# Author.: Wu Chih-En
# Version: 0.1
# License: 
#
###############################################################

TARGET = sks

INCLUDE_DIRS = -I../../include -I../../../Framework/include

CXX = g++
CXXFLAGS += -O2 -DLINUX -Wall -fopenmp $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -lpthread -ljpeg -lrt -lurg_cpp -ltinyxml -lEposCmd -lopencv_core -lopencv_imgproc -lopencv_highgui

OBJECTS = main.o

all: $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo

libclean:
	make -C ../../build clean

distclean: clean libclean

wheeled.a:
	make -C ../../build

$(TARGET): wheeled.a $(OBJECTS)
	$(CXX) $(OBJECTS) ../../lib/wheeled.a -o $(TARGET) $(LFLAGS)
	chmod 755 $(TARGET)

# useful to make a backup "make tgz"
tgz: clean
	mkdir -p backups
	tar czvf ./backups/Wheeled_sks_`date +"%Y_%m_%d_%H.%M.%S"`.tgz --exclude backups *
