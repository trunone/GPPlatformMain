/*
 *   main.cpp
 *
 *   Author: Wu Chih-En
 */
#define ENABLE_STRATEGY
//#define ENABLE_VISION
//#define ENABLE_VISION_FACEDETECTION
#define ENABLE_LOCATION

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>
#include <signal.h>
#include <iostream>
#include <tinyxml.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Motors.h"
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

#include "LinuxWheeled.h"

using namespace Robot;
using namespace std;

Motors motors;
qrk::Urg_driver urg;
CvCapture *VisionCapture;

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
    {
        if(chdir(dirname(exepath)))
            fprintf(stderr, "chdir error!! \n");
    }
}

void sighandler(int sig)
{
    exit(0);
}

int DescompositionCommand (TiXmlElement* root)
{
	TiXmlElement* element;
    element = root->FirstChildElement("ManualDirection");
		if(element != NULL) {
			TiXmlElement* child;
			child = element->FirstChildElement("Rotate");
			if(child != NULL){
				child->Attribute("w", &StrategyStatus::w);
			}
			child = element->FirstChildElement("Vector");
			if(child != NULL){
				child->Attribute("x", &StrategyStatus::x);
				child->Attribute("y", &StrategyStatus::y);
			}
			child = element->FirstChildElement("Camera");
			if(child != NULL){
				//child->Attribute("ang", &StrategyStatus::CmeraAngle);
			}
			child = element->FirstChildElement("TakeBall");
			if(child != NULL){
				child->Attribute("ballFlag", &StrategyStatus::CurrentBallState);
			}
			child = element->FirstChildElement("Sim_flag");
			if(child != NULL){
			 	StrategyStatus::SimulatorFlag=true;
			}else{
				StrategyStatus::SimulatorFlag=false;
			}
		}
}
int DescompositionSimulator (TiXmlElement* root)
{
	TiXmlElement* element = root ->FirstChildElement("Sim_Position");;
	if(element != NULL) {
		element->Attribute("x", &LocationStatus::Position.x);
		element->Attribute("y", &LocationStatus::Position.y);
		element->Attribute("sita", &LocationStatus::Handle);
	}
}

int DescompositionRequest (TiXmlElement* root,LinuxServer *new_sock)
{
	TiXmlElement* element;
    TiXmlElement RequestRoot("Status");
    element = root->FirstChildElement("Laser");
    if(element != NULL) {
	TiXmlElement element("Laser");
		for(int i=1;i<=1000;i++){
			TiXmlElement child("Value");
			//child->SetDoubleAttribute("angle",???);
			//child->SetDoubleAttribute("distance",???);
			element.InsertEndChild(*child.Clone());
		}
		RequestRoot.InsertEndChild(*element.Clone());
	}
	element = root->FirstChildElement("Position");
	if(element != NULL) {
		TiXmlElement element("Position");
			element.SetDoubleAttribute("x",LocationStatus::Position.x);
			element.SetDoubleAttribute("y",LocationStatus::Position.y);
			element.SetDoubleAttribute("sita",LocationStatus::Handle);
			RequestRoot.InsertEndChild(*element.Clone());
	}
	element = root->FirstChildElement("Camera_Angle");
		if(element != NULL) {
			TiXmlElement element("Camera_Angle");
			//element->SetDoubleAttribute("ang",???);
			RequestRoot.InsertEndChild(*element.Clone());
		}
	element = root->FirstChildElement("Movement");
	if(element != NULL) {
		TiXmlElement element("Movement");
		element.SetDoubleAttribute("x",StrategyStatus::x);
		element.SetDoubleAttribute("y",StrategyStatus::y);
		element.SetDoubleAttribute("sita",StrategyStatus::w);
		RequestRoot.InsertEndChild(*element.Clone());	
	}
	TiXmlDocument RequestDoc;
	RequestDoc.InsertEndChild(*RequestRoot.Clone());
	TiXmlPrinter send;
	RequestDoc.Accept( &send );
	*new_sock << send.CStr();
}
void DescompositionReloadConfig ()
{
	TiXmlDocument ConfigDoc("Robot_Config.xml");
	ConfigDoc.LoadFile();
	TiXmlElement* root = ConfigDoc.FirstChildElement("Config");
	
    TiXmlElement* element = root->FirstChildElement("DirectionObject");
		if(element != NULL){
                //LocationStatus::GetInstance()->LoadXMLSettings(element);
        }
        element = root->FirstChildElement("ColorModel");
        if(element != NULL){
                ColorModel::GetInstance()->LoadXMLSettings(element);
        }

		element = root->FirstChildElement("GridMap");
		if(element != NULL){
				AstarTool::GetInstance()->LoadXMLSettings(element);

		}

        element = root->FirstChildElement("BasicConfig");
        if(element != NULL){
				StrategyManager::GetInstance()->LoadXMLSettings(element);
        }
        element = root->FirstChildElement("StraConfig");
        if(element != NULL){
                    TiXmlElement* child = element->FirstChildElement("Stra_Astar");
                    if(child != NULL){
                          Stra_AStar::GetInstance()->LoadXMLSettings(child);
                    }
                    child = element->FirstChildElement("Stra_Avoid");
                    if(child != NULL){
                          Stra_Avoid::GetInstance()->LoadXMLSettings(child);
                    }
                    child = element->FirstChildElement("Stra_PathPlan");
                    if(child != NULL){
                          Stra_PathPlan::GetInstance()->LoadXMLSettings(child);
                    }
                    child = element->FirstChildElement("Stra_VelocityControl");
                    if(child != NULL){
                          Stra_VelocityControl::GetInstance()->LoadXMLSettings(child);
                    }
                    child = element->FirstChildElement("Stra_Task");
                    if(child != NULL){
                          Stra_Task::GetInstance()->LoadXMLSettings(child);
                    }
       }
}

int main(void)
{
	DescompositionReloadConfig();

    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    change_current_dir();

    //motors.OpenDeviceAll();

#ifdef ENABLE_VISION
    VisionCapture = cvCaptureFromCAM( -1 );
#endif

#ifdef ENABLE_LOCATION
    if (!urg.open("/dev/ttyACM0", 115200, qrk::Urg_driver::Serial ))
        fprintf(stderr,  "Urg_driver::open(    ): %s\n", urg.what());
#endif

   ////////////////// Framework Initialize ////////////////////////////
#ifdef ENABLE_VISION
    if(VisionManager::GetInstance()->Initialize(VisionCapture) == false)
    {
        printf("Fail to initialize Vision Manager!\n");
        return 1;
    }

		//VisionManager::GetInstance()->AddModule((VisionModule*)Doornumber_detect::GetInstance());
		
		//VisionManager::GetInstance()->AddModule((VisionModule*)ColorModel::GetInstance());
	
		VisionManager::GetInstance()->AddModule((VisionModule*)Segmentation::GetInstance());

    #ifdef ENABLE_VISION_FACEDETECTION
    	VisionManager::GetInstance()->AddModule((VisionModule*)FaceDetection::GetInstance());
    #endif

		VisionManager::GetInstance()->AddModule((VisionModule*)SendImage::GetInstance());
		
    LinuxVisionTimer *vision_timer = new LinuxVisionTimer(VisionManager::GetInstance());
    vision_timer->Start();
#endif
    //-----------------------------------------------------------------------------------//
#ifdef ENABLE_LOCATION
    if(LocationManager::GetInstance()->Initialize(&urg, &motors) == false)
    //if(LocationManager::GetInstance()->Initialize(&motors) == false)
    {
        printf("Fail to initialize Location Manager!\n");
        return 1;
    }

    LocationManager::GetInstance()->AddModule((LocationModule*)Odometer::GetInstance());
    LocationManager::GetInstance()->AddModule((LocationModule*)ParticleFilter::GetInstance());

    LinuxLocationTimer *location_timer = new LinuxLocationTimer(LocationManager::GetInstance());
    location_timer->Start();
    //LocationManager::GetInstance()->StartLogging();
#endif
    //-----------------------------------------------------------------------------------//
#ifdef ENABLE_STRATEGY
    if(StrategyManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }
    
    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_Task::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_FindBall::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_AStar::GetInstance());
    
    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_PathPlan::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_Avoid::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_VelocityControl::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Motion::GetInstance());

    StrategyManager::GetInstance()->SetEnable(true);

	LinuxStrategyTimer *strategy_timer = new LinuxStrategyTimer(StrategyManager::GetInstance());
	strategy_timer->Start();
    //StrategyManager::GetInstance()->StartLogging();
#endif
    try
    {
        while(1) {


           string xml;
            LinuxServer new_sock;
            LinuxServer server(10373);
            cout << "[Waiting..]" << endl;
            server.accept ( new_sock );
            cout << "[Accepted..]" << endl;	

            try
            {
		cout<<"load"<<endl;
            	while(true){	
                    TiXmlDocument doc;
                    new_sock >> xml;
                    doc.Parse(xml.c_str());
                    TiXmlElement* root = doc.FirstChildElement("Command");
                    if(root != NULL) {
						DescompositionCommand(root);
                    }
					if(StrategyStatus::SimulatorFlag){
						    root = doc.FirstChildElement("Simulator");
							if(root != NULL) {
						    	DescompositionSimulator (root);
							}
					}
                    root = doc.FirstChildElement("Request");
                    if(root != NULL) {
                        DescompositionRequest (root, &new_sock);
                    }
                    root = doc.FirstChildElement("ReloadConfig");
                    if(root != NULL){
						DescompositionReloadConfig();
                    }
                }
            }
            catch ( LinuxSocketException& )
            {
                cout << "[Disconnected]" << endl;
            }
	}
    }
    catch ( LinuxSocketException& e )
    {
        cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
    return 0;
}
