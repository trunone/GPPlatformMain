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
	TiXmlDocument doc("Status.xml");
	doc.LoadFile();
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	
	
	//TiXmlDocument doc;
	//doc.Parse(printer.CStr());


	TiXmlElement* root = doc.RootElement();//Status
	TiXmlElement* element = root->FirstChildElement();//ColorModel
	TiXmlElement* model = element->FirstChildElement();//Model
	TiXmlAttribute* type= model->FirstAttribute();//Model type
	const char *www="www";
	double x[7]={0.1,0.2,0.3,0.4,0.5,0.6,0.7};
	int i=0;

	const char *fileName = "string2xml";
	cout<<type->Value()<<endl;
	for(;model != NULL;model=model->NextSiblingElement()){
		TiXmlElement* modelchild=model->FirstChildElement();
		for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//get node information
			string informationType=modelchild->Value();
			string information=modelchild->FirstAttribute()->Value();
			modelchild->SetDoubleAttribute(www,x[i++]);
			//modelchild->RemoveAttribute(www);
			//doc.SaveFile(fileName);
			doc.SaveFile();
			cout <<	informationType << ":" << information << endl;
		}
	}
	doc.Accept( &printer );
	fscanf( stdout, "%s", printer.CStr() );
	printf("%s", printer.CStr());
	int port=1234;
	string aa="a";
	char w;
	w=0x08;
	LinuxServer new_sock;
        LinuxServer server (port);
	while(true){
        	cout << "[Waiting..]" << endl;         
            	server.accept ( new_sock );
            	cout << "[Accepted..]" << endl;	
		
		new_sock << printer.CStr();
		//printf("%s", printer.CStr());
		cout << "[success]" << endl;		
		
		
	}
return 0;
}
