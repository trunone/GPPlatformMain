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


#include "LinuxWheeled.h"
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

//#define INI_FILE_PATH       "../../../Data/config.ini"

#define LASER_DEV_NAME      "/dev/ttyACM0"

using namespace qrk;
using namespace std;
using namespace Robot;
TiXmlDocument RequestXml(int DetermineNumber)
{
	TiXmlDocument xmlDoc;
	 
	
	TiXmlElement* root = new TiXmlElement("Status");
	
	if((DetermineNumber%2)==0){
		DetermineNumber=DetermineNumber/2;
		TiXmlElement* element=new TiXmlElement("Laser");
		for(int i=1;i<=10;i++){
			TiXmlElement* child=new TiXmlElement("Value");
			child->SetDoubleAttribute("angle",5);
			child->SetDoubleAttribute("distance",5);
			element->InsertEndChild(*(child->Clone()));
		}
		root->InsertEndChild(*(element->Clone()));
	}	
	if((DetermineNumber%3)==0){
		DetermineNumber=DetermineNumber/3; 
		TiXmlElement* element=new TiXmlElement("Position");
		element->SetDoubleAttribute("x",69);
		element->SetDoubleAttribute("y",89);
		element->SetDoubleAttribute("sita",89);
		root->InsertEndChild(*(element->Clone()));
	}
	if((DetermineNumber%5)==0){
		DetermineNumber=DetermineNumber/5; 
		TiXmlElement* element=new TiXmlElement("Camera_Angle");
		element->SetDoubleAttribute("ang",69);
		root->InsertEndChild(*(element->Clone()));
	}
	if((DetermineNumber%7)==0){
		DetermineNumber=DetermineNumber/7; 
		TiXmlElement* element=new TiXmlElement("Movement");
		element->SetDoubleAttribute("x",6988);
		element->SetDoubleAttribute("y",8988);
		element->SetDoubleAttribute("sita",8988);
		root->InsertEndChild(*(element->Clone()));
	}
	xmlDoc.InsertEndChild(*(root->Clone()));
	 
	return xmlDoc;
}
int main(){
	
	
////////////////////////////////////////////////////rec
	/*TiXmlDocument doc("Status.xml");
	doc.LoadFile();
	TiXmlPrinter printer;
	printer.SetStreamPrinting();*/
	//doc_2.Accept( &printer );
	//fscanf( stdout, "%s", printer.CStr() );
	//printf("%s", printer.CStr());
	
  /*      cout << "[Waiting..]" << endl;
        server.accept ( new_sock );
        cout << "[Accepted..]" << endl;	*/
///////////////////////////////////////////////////intial
	/*
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	const char *fileName = "last_mode.xml";
	const char *just_cmp = "halt";
	*/

//////////////////////////////////////////////////////////////////////////////////////////

TiXmlDocument xmlDoc=RequestXml(3*5*7*2);
xmlDoc.SaveFile("Status.xml");




/*	while(true){	
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
				TiXmlElement* element_ManualDirection=root->FirstChildElement("ManualDirection");
				if(element_ManualDirection != NULL){
					StrategyStatus::GetInstance()->loadxml(element_ManualDirection,1);
					cout<<"I got ManualDirection"<<endl;
					TiXmlElement* child_ManualDirection;
					child_ManualDirection = element_ManualDirection->FirstChildElement("Laser");
					if(child != NULL){
						
					}
					child_ManualDirection = element_ManualDirection->FirstChildElement("Sim_flag");
					if(child != NULL){
						TiXmlDocument doc_Sim("Simulator.xml");
						TiXmlElement* root_Sim=doc.FirstChildElement("Simulator");
						TiXmlElement* element_Sim=root->FirstChildElement("Sim_status");
						TiXmlElement* child_Sim=element_Sim->FirstChildElement("Site");
						child_Sim->SetDoubleAttribute("x",StrategyStatus::x);
						child_Sim->SetDoubleAttribute("y",StrategyStatus::y);
						child_Sim->SetDoubleAttribute("sita",StrategyStatus::w);
						doc_Sim.Accept( &printer );
						new_sock << printer.CStr();
					}
				
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
				TiXmlElement* element_AStar_PathFinde=root->FirstChildElement("AStar_PathFinde");
				if(element_AStar_PathFinde != NULL){
					AstarTool::GetInstance()->loadxml(element_AStar_PathFinde);
					cout<<"I got AStar_PathFinde"<<endl;
				
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
*/	
return 0;
}
