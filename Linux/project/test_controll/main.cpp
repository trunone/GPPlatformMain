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
	
	
////////////////////////////////////////////////////rec
	TiXmlDocument doc_2("Status.xml");
	doc_2.LoadFile();
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc_2.Accept( &printer );
	fscanf( stdout, "%s", printer.CStr() );
	//printf("%s", printer.CStr());
	//printf("%s", printer.CStr());
	int port=1234;
	string aa;
	LinuxServer new_sock;
        LinuxServer server (port);
	
        cout << "[Waiting..]" << endl;         
        server.accept ( new_sock );
        cout << "[Accepted..]" << endl;	
	while(true){	
		new_sock >> aa;
	
		cout << "[success]" << endl;
/////////////////////////////////////////////////////
		char *xml=new char[aa.length()+1];
		strcpy(xml,aa.c_str());	
		TiXmlDocument doc;
		doc.Parse(xml);

		TiXmlElement* root = doc.RootElement();//Status
		TiXmlElement* element = root->FirstChildElement();//ColorModel
		TiXmlElement* model = element->FirstChildElement();//Model
		TiXmlAttribute* type= model->FirstAttribute();//Model type
		//const char *www="www";
	
		//cout<<type->Value()<<endl;
		for(;model != NULL;model=model->NextSiblingElement()){
			TiXmlElement* modelchild=model->FirstChildElement();
			for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//node information
				string informationType=modelchild->Value();
				string information=modelchild->FirstAttribute()->Value();
				//modelchild->SetDoubleAttribute(www,x[i++]);
				//doc.SaveFile(fileName);
				//doc.SaveFile();
				cout <<	informationType << ":" << information << endl;
			}
		}
	
		delete[]xml;
	}	
	
return 0;
}
