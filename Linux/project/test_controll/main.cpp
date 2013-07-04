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
	/*TiXmlDocument doc("Status.xml");
	doc.LoadFile();
	TiXmlPrinter printer;
	printer.SetStreamPrinting();*/
	//doc_2.Accept( &printer );
	//fscanf( stdout, "%s", printer.CStr() );
	//printf("%s", printer.CStr());
	int port=1234;
	string xml_by_str;
	LinuxServer new_sock;
        LinuxServer server (port);
	
        cout << "[Waiting..]" << endl;         
        server.accept ( new_sock );
        cout << "[Accepted..]" << endl;	
//////////////////////////////////////////////////////////////////////////////////////////
	while(true){	
		new_sock >> xml_by_str;
		cout << "[success recv]" << endl;
		char *xml_by_char=new char[xml_by_str.length()+1];
		strcpy(xml_by_char,xml_by_str.c_str());	
	

		TiXmlDocument doc;
		doc.Parse(xml_by_char);
		TiXmlElement* root = doc.RootElement();//Status
		TiXmlElement* element = root->FirstChildElement();//ColorModel
		//TiXmlElement* model = element->FirstChildElement();//Model
		//TiXmlAttribute* type= model->FirstAttribute();//Model type
		const char *www="www";
	
		//cout<<type->Value()<<endl;
		for(;element != NULL;element=element->NextSiblingElement()){
			TiXmlElement* model=element->FirstChildElement();
			for(;model != NULL;model=model->NextSiblingElement()){
				cout<<model->FirstAttribute()->Value()<<endl;
				TiXmlElement* modelchild=model->FirstChildElement();
				for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//node information
					string informationType=modelchild->Value();
					string information=modelchild->FirstAttribute()->Value();
					modelchild->SetDoubleAttribute(www,0.85);
					
					//doc.SaveFile();
					cout <<	informationType << ":" << information << endl;
				}
			}
		}
		//doc.SaveFile(fileName);

		
		TiXmlPrinter printer;
		printer.SetStreamPrinting();
		doc.Accept( &printer );
		new_sock << printer.Str();

		delete[]xml_by_char;
	}	
	
return 0;
}
