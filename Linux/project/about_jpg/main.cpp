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
	int port=1234;
	string www="aa.jpg";
	unsigned char *ab[7]="aa.jpg";
	FILE *aa=fopen("aa.jpg","rb");
	LinuxServer new_sock;
        LinuxServer server (port);
	unsigned char *img[1366*768];
	while(true){
        	cout << "[Waiting..]" << endl;         
            	server.accept ( new_sock );
            	cout << "[Accepted..]" << endl;	
		
		new_sock.send(aa,1366*768);
		//printf("%s", printer.CStr());
		cout << "[success]" << endl;		
		
		
	}
return 0;
}
