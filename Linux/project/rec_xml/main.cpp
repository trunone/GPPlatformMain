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
///////////////////////////////////////////////////intial
	
	TiXmlPrinter printer;
	const char *fileName = "last_mode.xml";
	const char *just_cmp = "halt";
	
	double x,y,w;
//////////////////////////////////////////////////////////////////////////////////////////
	while(true){	
		new_sock >> xml_by_str;
		cout << "[success recv]" << endl;
		char *xml_by_char=new char[xml_by_str.length()+1];
		strcpy(xml_by_char,xml_by_str.c_str());
		if(strcmp(xml_by_char,just_cmp)==0){
			new_sock << "noting to trans";
		}else{
			TiXmlDocument doc;
			doc.Parse(xml_by_char);
			TiXmlElement* root=doc.FirstChildElement("Command");
			if(root != NULL){
				TiXmlElement* elementforManualDirection=root->FirstChildElement("ManualDirection");
			}
			root=doc.FirstChildElement("Config");
			if(root != NULL){
				TiXmlElement* element_DirectionObject=root->FirstChildElement("DirectionObject");
				if(element_DirectionObject != NULL){
					cout<<"I got DirectionObject"<<endl;
				
				}
				TiXmlElement* element_ParameterModel=root->FirstChildElement("ParameterModel");
				if(element_ParameterModel != NULL){
					cout<<"I got ParameterModel"<<endl;
				
				}
				TiXmlElement* element_ColorModel=root->FirstChildElement("ColorModel");
				if(element_ColorModel != NULL){
					cout<<"I got ColorModel"<<endl;
				
				}
				TiXmlElement* element_ManualInterfaceConfig= root->FirstChildElement("ManualInterfaceConfig");
				if(element_ManualInterfaceConfig != NULL){
					cout<<"I got ManualInterfaceConfig"<<endl;
				
				}
				TiXmlElement* element_SV-BasicConfig= root->FirstChildElement("SV-BasicConfig");
				if(element_SV-BasicConfig != NULL){
					cout<<"I got SV-BasicConfig"<<endl;
				
				}
				TiXmlElement* element_StraConfig= root->FirstChildElement("StraConfig");
				if(element_StraConfig != NULL){
					cout<<"I got StraConfig"<<endl;
				
				}
				TiXmlElement* element_Vision= root->FirstChildElement("Vision");
				if(element_Vision != NULL){
					cout<<"I got Vision"<<endl;
				
				}
			}

			//doc.SaveFile(fileName);
			//doc.SaveFile();
			printer.SetStreamPrinting();
			doc.Accept( &printer );
			//new_sock << printer.Str();
			new_sock << "recv";
			
		}
		

		delete[]xml_by_char;
	}	
	
return 0;
}
