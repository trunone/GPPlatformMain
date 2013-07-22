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
				if(elementforManualDirection != NULL){
					StrategyStatus::GetInstance()->loadxml(elementforManualDirection,1);
					cout<<"I got ManualDirection"<<endl;
				
				}
			}
			root=doc.FirstChildElement("Config");
			if(root != NULL){
				TiXmlElement* element_DirectionObject=root->FirstChildElement("DirectionObject");
				if(element_DirectionObject != NULL){
					LocationStatus::GetInstance()->loadxml(element_DirectionObject);
					cout<<"I got DirectionObject"<<endl;
				
				}
				TiXmlElement* element_ColorModel=root->FirstChildElement("ColorModel");
				if(element_ColorModel != NULL){
					ColorModel::GetInstance()->loadxml(element_ColorModel);
					cout<<"I got ColorModel"<<endl;
				
				}
				TiXmlElement* element_BasicConfig= root->FirstChildElement("BasicConfig");
				if(element_BasicConfig != NULL){
					StrategyStatus::GetInstance()->loadxml(element_BasicConfig,2);
					cout<<"I got BasicConfig"<<endl;
				
				}
				TiXmlElement* element_StraConfig= root->FirstChildElement("StraConfig");
				if(element_StraConfig != NULL){
					TiXmlElement* modelchild_Stra_Astar=element->FirstChildElement("Stra_Astar");
					if(modelchild != NULL){
						Stra_Astar::GetInstance()->loadxml(modelchild_Stra_Astar);
					}
					TiXmlElement* modelchild_Stra_Avoid=element->FirstChildElement("Stra_Avoid");
					if(modelchild != NULL){
						Stra_Avoid::GetInstance()->loadxml(modelchild_Stra_Avoid);
					}
					TiXmlElement* modelchild_Stra_PathPlan=element->FirstChildElement("Stra_PathPlan");
					if(modelchild != NULL){
						Stra_PathPlan::GetInstance()->loadxml(modelchild_Stra_PathPlan);
					}
					TiXmlElement* modelchild_Stra_VelocityControl=element->FirstChildElement("Stra_VelocityControl");
					if(modelchild != NULL){
						Stra_VelocityControl::GetInstance()->loadxml(modelchild_Stra_VelocityControl);
					}
					cout<<"I got StraConfig"<<endl;
				
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
