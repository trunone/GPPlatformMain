/*
 *   main.cpp
 *
 *   Author: Wu Chih-En
 */

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>
#include <signal.h>
#include <iostream>
#include <tinyxml.h>
#include <omp.h>

#include "mjpg_streamer.h"
#include "LinuxWheeled.h"
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

//#define INI_FILE_PATH       "../../../Data/config.ini"

#define LASER_DEV_NAME      "/dev/ttyACM0"

using namespace qrk;
using namespace std;
using namespace Robot;

int main(){
	
	
	//TiXmlDocument doc;
	//doc.Parse(printer.CStr());


	int port=1234;
	int yy;
	TiXmlDocument doc("Robot_Test.xml");
	doc.LoadFile();
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept( &printer );
	fprintf( stdout, "%s", printer.CStr() );
	//printf("%s", printer.CStr());
	string host = "127.0.0.1";
	LinuxSocket rec;
	string aa = "test";
	rec.create();
	cout << aa << endl;
	if(rec.connect ( host , port )) {	
			
		rec.send(printer.CStr());
		cout << aa;	
	
	}
return 0;
}
