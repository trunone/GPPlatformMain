/*
 *   main.cpp
 *
 *   Author: Wu Chih-En
 */
//#define ENABLE_STRATEGY
//#define ENABLE_VISION
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

int main(void)
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    change_current_dir();

    motors.OpenDeviceAll();
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

    VisionManager::GetInstance()->AddModule((VisionModule*)FaceDetection::GetInstance());

    LinuxVisionTimer *vision_timer = new LinuxVisionTimer(VisionManager::GetInstance());
    vision_timer->Start();
#endif
    //-----------------------------------------------------------------------------------//
#ifdef ENABLE_LOCATION
    if(LocationManager::GetInstance()->Initialize(&urg, &motors) == false)
    {
        printf("Fail to initialize Location Manager!\n");
        return 1;
    }

    LocationManager::GetInstance()->AddModule((LocationModule*)Speedometer::GetInstance());
    LocationManager::GetInstance()->AddModule((LocationModule*)ParticleFilter::GetInstance());

    LinuxLocationTimer *location_timer = new LinuxLocationTimer(LocationManager::GetInstance());
    location_timer->Start();
    LocationManager::GetInstance()->StartLogging();
#endif
    //-----------------------------------------------------------------------------------//
#ifdef ENABLE_STRATEGY
    if(StrategyManager::GetInstance()->Initialize(&motors) == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_Task::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_FindBall::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_AStar::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_PathPlan::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_Avoid::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_VelocityControl::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Motion::GetInstance());

    //StrategyManager::GetInstance()->SetEnable(true);

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
                while(true){	
                    TiXmlDocument doc;
                    new_sock >> xml;
                    doc.Parse(xml.c_str());
                    TiXmlElement* root = doc.FirstChildElement("Command");
                    if(root != NULL) {
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
                                //child->Attribute("ang", ????);
                            }
                            child = element->FirstChildElement("TakeBall");
                            if(child != NULL){
                                //child->Attribute("ballFlag", ????);
                            }
                            child = element->FirstChildElement("Sim_flag");
                            if(child != NULL){
                               
                            }
                        }
                    }
                    root = doc.FirstChildElement("Simulator");
                    if(root != NULL) {
                        TiXmlElement* element;
                        element = root->FirstChildElement("Sim_status");
                        if(element != NULL) {
                            TiXmlElement* child;
                            child = element->FirstChildElement("Site");
                            if(child != NULL){
                                child->Attribute("x", &StrategyStatus::x);
                                child->Attribute("y", &StrategyStatus::y);
                                child->Attribute("sita", &StrategyStatus::w);
                            }
                        }
                    }
                    root = doc.FirstChildElement("Request");
                    if(root != NULL) {
                        TiXmlElement* element;
//-----------------------------------------------------------------------------initial for send xml
                        TiXmlElement* root = new TiXmlElement("Status");
//-----------------------------------------------------------------------------
                        element = root->FirstChildElement("Laser");
                        if(element != NULL) {
                            TiXmlElement* element=new TiXmlElement("Laser");
                            for(int i=1;i<=1000;i++){
                                TiXmlElement* child=new TiXmlElement("Value");
                                //child->SetDoubleAttribute("angle",???);
                                //child->SetDoubleAttribute("distance",???);
                                element->InsertEndChild(*(child->Clone()));
                            }
                            root->InsertEndChild(*(element->Clone()));
                        }
                        element = root->FirstChildElement("Position");
                        if(element != NULL) {
                            TiXmlElement* element=new TiXmlElement("Position");
                            //element->SetDoubleAttribute("x",???);
                            //element->SetDoubleAttribute("y",???);
                            //element->SetDoubleAttribute("sita",???);
                            root->InsertEndChild(*(element->Clone()));
                        }
                        element = root->FirstChildElement("Camera_Angle");
                        if(element != NULL) {
                            TiXmlElement* element=new TiXmlElement("Camera_Angle");
                            //element->SetDoubleAttribute("ang",???);
                            root->InsertEndChild(*(element->Clone()));
                        }
                        element = root->FirstChildElement("Movement");
                        if(element != NULL) {
                            TiXmlElement* element=new TiXmlElement("Movement");
                            //element->SetDoubleAttribute("x",???);
                            //element->SetDoubleAttribute("y",???);
                            //element->SetDoubleAttribute("sita",???);
                            root->InsertEndChild(*(element->Clone()));	
                        }
                        TiXmlDocument RequestDoc;
                        RequestDoc.InsertEndChild(*(root->Clone()));
                        TiXmlPrinter send;
                        RequestDoc.Accept( &send );
                        new_sock << send.CStr();
                    }
                    root = doc.FirstChildElement("Config");
                    if(root != NULL){
                        TiXmlElement* element = root->FirstChildElement("DirectionObject");
                        if(element != NULL){
                            //LocationStatus::GetInstance()->LoadXMLSettings(element);
                        }
                        delete element;
                        element = root->FirstChildElement("ColorModel");
                        if(element != NULL){
                            ColorModel::GetInstance()->LoadXMLSettings(element);
                        }
                        delete element;
                        element = root->FirstChildElement("AStar_PathFinde");
                        if(element != NULL){
                            AstarTool::GetInstance()->LoadXMLSettings(element);
                        }
                        delete element;
                        element = root->FirstChildElement("BasicConfig");
                        if(element != NULL){
                            //StrategyStatus::GetInstance()->LoadXMLSettings(element);
                        }
                        delete element;
                        element = root->FirstChildElement("StraConfig");
                        if(element != NULL){
                            TiXmlElement* child = element->FirstChildElement("Stra_Astar");
                            if(child != NULL){
                                Stra_AStar::GetInstance()->LoadXMLSettings(child);
                            }
                            delete child;
                            child = element->FirstChildElement("Stra_Avoid");
                            if(child != NULL){
                                Stra_Avoid::GetInstance()->LoadXMLSettings(child);
                            }
                            delete child;
                            child = element->FirstChildElement("Stra_PathPlan");
                            if(child != NULL){
                                Stra_PathPlan::GetInstance()->LoadXMLSettings(child);
                            }
                            delete child;
                            child = element->FirstChildElement("Stra_VelocityControl");
                            if(child != NULL){
                                Stra_VelocityControl::GetInstance()->LoadXMLSettings(child);
                            }
                        }
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
