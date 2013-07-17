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
			new_sock << "fuck you";
		}else{
			TiXmlDocument doc;
			doc.Parse(xml_by_char);
			TiXmlElement* root = doc.RootElement();
			TiXmlElement* element;
			element= root->FirstChildElement("ManualDirection");
			if(element != NULL){	
				TiXmlElement* modelchild;
				modelchild=element->FirstChildElement("Rotate");
				if(modelchild != NULL){
					modelchild->Attribute("w", &w);
				}
				modelchild=element->FirstChildElement("Vector");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
			}
			element= root->FirstChildElement("Vision");
			if(element != NULL){
				cout<<"I got vision"<<endl;
					/*TiXmlElement* model=element->FirstChildElement();
					for(;model != NULL;model=model->NextSiblingElement()){
					//cout<<model->FirstAttribute()->Value()<<endl;
						TiXmlElement* modelchild=model->FirstChildElement();
						for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//node information
							string informationType=modelchild->Value();
							string parameter=modelchild->FirstAttribute()->Value();
							//modelchild->SetDoubleAttribute(www,0.85);
							//cout <<	informationType << ":" << parameter << endl;
						}
					}*/
				
			}
			cout<<"X:"<<x<<endl<<"Y:"<<y<<endl<<"W:"<<w<<endl;
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
