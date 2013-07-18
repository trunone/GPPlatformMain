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
		//cout << xml_by_char<<endl;
		if(strcmp(xml_by_char,just_cmp)==0){
			new_sock << "noting to trans";
		}else{
			TiXmlDocument doc;
			doc.Parse(xml_by_char);
			TiXmlElement* root = doc.RootElement();
			TiXmlElement* element;
////////////////////////////////////////////////////////////			
			element= root->FirstChildElement("ManualInterfaceConfig");
			if(element != NULL){	
				TiXmlElement* modelchild;
				modelchild=element->FirstChildElement("Interface");
				if(modelchild != NULL){
					modelchild->Attribute("HandControl", &HandControl);
				}
				modelchild=element->FirstChildElement("Manual");
				if(modelchild != NULL){
					modelchild->Attribute("MaxSpeed", &MaxSpeed);
					modelchild->Attribute("CurrentSpeed", &CurrentSpeed);						
				}
			}
/////////////////////////////////////////////////////////
			element= root->FirstChildElement("SV-BasicConfig");
			if(element != NULL){	
				TiXmlElement* modelchild;
				modelchild=element->FirstChildElement("RootHandle");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("StartPos");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("EndPos");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("FindBallEn");
				if(modelchild != NULL){
					modelchild->Attribute("FindBallEn", &w);
				}
				modelchild=element->FirstChildElement("LivRMDoor");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("LivRMCen");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("DinRMDoor");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("DinRMCen");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("LibDoor");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("LibCen");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("BedRMDoor");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("BedRMCen");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("ChrgDoor");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
				modelchild=element->FirstChildElement("ChrgCen");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);						
				}
			}

/////////////////////////////////////////////////////////
			element= root->FirstChildElement("DirectionObject");
			if(element != NULL){	
				TiXmlElement* modelchild;
				modelchild=element->FirstChildElement("LivingRM_Sofa");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("LivingRM_Table");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("LivingRM_Cabinet");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("DiningRM_Table");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("DiningRM_Chair");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("Library_Desk");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("Library_Chair");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("Library_Cabinet");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("BedRM_Bed");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("DiningRM_door");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("BedRM_door");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("ChargeArea");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
				modelchild=element->FirstChildElement("EndArea");
				if(modelchild != NULL){
					modelchild->Attribute("x", &x);
					modelchild->Attribute("y", &y);	
					modelchild->Attribute("Width", &Width);
					modelchild->Attribute("Height", &Height);
					modelchild->Attribute("Angle", &Angle);						
				}
			}
/////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////
			element= root->FirstChildElement("StraConfig");
			if(element != NULL){	
				TiXmlElement* modelchild;
				modelchild=element->FirstChildElement("Stra_Astar");
				if(modelchild != NULL){
					modelchild->Attribute("PathErrRange", &w);
					modelchild->Attribute("AchieveErrRange", &w);
				}
				modelchild=element->FirstChildElement("Stra_Avoid");
				if(modelchild != NULL){
					modelchild->Attribute("StartAngle_du", &StartAngle_du);
					modelchild->Attribute("ScanLineSize", &ScanLineSize);
					modelchild->Attribute("ScanLineScale_du", &ScanLineScale_du);
					modelchild->Attribute("SafeRange", &SafeRange);
					modelchild->Attribute("SafeArc_D", &SafeArc_D);
					modelchild->Attribute("SafeArc_A", &SafeArc_A);
					modelchild->Attribute("AvoidConfig1", &AvoidConfig1);
					modelchild->Attribute("AvoidForce_du", &AvoidForce_du);
					modelchild->Attribute("FixDirect", &FixDirect);						
				}
				modelchild=element->FirstChildElement("Stra_MotorCommand");
				if(modelchild != NULL){
					modelchild->Attribute("MaxSpeed", &MaxSpeed);
					modelchild->Attribute("MoveAddition", &MoveAddition);
					modelchild->Attribute("RotateAddition", &RotateAddition);
				}
				modelchild=element->FirstChildElement("Stra_PathPlan");
				if(modelchild != NULL){
					modelchild->Attribute("DetourConst", &DetourConst);
				}
				modelchild=element->FirstChildElement("Stra_VelocityControl");
				if(modelchild != NULL){
					modelchild->Attribute("dis_max", &dis_max);
					modelchild->Attribute("dis_min", &dis_min);
					modelchild->Attribute("spd_max", &spd_max);
					modelchild->Attribute("spd_min", &spd_min);
					modelchild->Attribute("sita_max", &sita_max);
					modelchild->Attribute("sita_min", &sita_min);
					modelchild->Attribute("w_max", &w_max);
					modelchild->Attribute("w_min", &w_min);
				}
			}

/////////////////////////////////////////////////////////
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
