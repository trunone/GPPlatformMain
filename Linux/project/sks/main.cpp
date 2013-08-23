/*
 *   main.cpp
 *
 *   Author: Wu Chih-En
 */
#define ENABLE_STRATEGY
//#define ENABLE_VISION
//#define ENABLE_VISION_FACEDETECTION
#define ENABLE_LOCATION

//#define NETWORK_INTERFACE
#define INTERACTIVE_INTERFACE

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

#include "LinuxWheeled.h"
#include "cmd_process.h"

using namespace Robot;
using namespace std;

Motors motors;
qrk::Urg_driver urg;
CvCapture *VisionCapture;
DXL dxl;

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

void sigtstp_handler(int sig)
{
    motors.SetDisableAll();
    exit(0);
}

int XMLReceiveCommand (TiXmlElement* root)
{
    TiXmlElement* element;
    element = root->FirstChildElement("ManualDirection");
    if(element != NULL) {
        TiXmlElement* child;
        child = element->FirstChildElement("Rotate");
        if(child != NULL) {
            child->Attribute("w", &StrategyStatus::w);
        }
        child = element->FirstChildElement("Vector");
        if(child != NULL) {
            child->Attribute("x", &StrategyStatus::x);
            child->Attribute("y", &StrategyStatus::y);
        }
        child = element->FirstChildElement("Camera");
        if(child != NULL) {
            //child->Attribute("ang", &StrategyStatus::CmeraAngle);
        }
        child = element->FirstChildElement("TakeBall");
        if(child != NULL) {
            child->Attribute("ballFlag", &StrategyStatus::CurrentBallState);
        }
        child = element->FirstChildElement("Sim_flag");
        if(child != NULL) {
            StrategyStatus::SimulatorFlag=true;
        } else {
            StrategyStatus::SimulatorFlag=false;
        }
    }
}

int XMLReceiveSimulator (TiXmlElement* root)
{
    TiXmlElement* element = root ->FirstChildElement("Sim_Position");;
    if(element != NULL) {
        element->Attribute("x", &LocationStatus::Position.x);
        element->Attribute("y", &LocationStatus::Position.y);
        element->Attribute("sita", &LocationStatus::Handle);
    }
}

TiXmlDocument XMLReceiveRequest (TiXmlElement* root)
{
    TiXmlElement* element;
    TiXmlElement RequestRoot("Status");
    element = root->FirstChildElement("Laser");
    if(element != NULL) {
        TiXmlElement element("Laser");
        vector<long>::iterator it = LocationStatus::LaserData.begin();
        while(it != LocationStatus::LaserData.end()) {
            TiXmlElement child("Value");
            child.SetDoubleAttribute("d", *it);
            element.InsertEndChild(*child.Clone());
            it++;
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
    TiXmlDocument request_doc;
    request_doc.InsertEndChild(*RequestRoot.Clone());
    return request_doc;
}

void XMLLoadConfig ()
{
    TiXmlDocument ConfigDoc("Robot_Config.xml");
    ConfigDoc.LoadFile();
    TiXmlElement* root = ConfigDoc.FirstChildElement("Config");

    TiXmlElement* element = root->FirstChildElement("DirectionObject");
    if(element != NULL) {
        //LocationStatus::GetInstance()->LoadXMLSettings(element);
    }
    element = root->FirstChildElement("ColorModel");
    if(element != NULL) {
        ColorModel::GetInstance()->LoadXMLSettings(element);
    }

    element = root->FirstChildElement("GridMap");
    if(element != NULL) {
        AstarTool::GetInstance()->LoadXMLSettings(element);

    }

    element = root->FirstChildElement("BasicConfig");
    if(element != NULL) {
        StrategyManager::GetInstance()->LoadXMLSettings(element);
    }
    element = root->FirstChildElement("StraConfig");
    if(element != NULL) {
        TiXmlElement* child = element->FirstChildElement("Stra_Astar");
        if(child != NULL) {
            Stra_AStar::GetInstance()->LoadXMLSettings(child);
        }
        child = element->FirstChildElement("Stra_Avoid");
        if(child != NULL) {
            Stra_Avoid::GetInstance()->LoadXMLSettings(child);
        }
        child = element->FirstChildElement("Stra_PathPlan");
        if(child != NULL) {
            Stra_PathPlan::GetInstance()->LoadXMLSettings(child);
        }
        child = element->FirstChildElement("Stra_VelocityControl");
        if(child != NULL) {
            Stra_VelocityControl::GetInstance()->LoadXMLSettings(child);
        }
        child = element->FirstChildElement("Stra_Task");
        if(child != NULL) {
            Stra_Task::GetInstance()->LoadXMLSettings(child);
        }
    }
}

int main(void)
{
    XMLLoadConfig();

    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);
    signal(SIGTSTP, &sigtstp_handler);

    change_current_dir();

    motors.OpenDeviceAll();

    dxl.OpenDevice(0);

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
    if(StrategyManager::GetInstance()->Initialize(&motors) == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_Task::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_FindBall::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_AStar::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_PathPlan::GetInstance());

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_Avoid::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Stra_VelocityControl::GetInstance());

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Motion::GetInstance());

    //StrategyManager::GetInstance()->SetEnable(true);
    
    LinuxStrategyTimer *strategy_timer = new LinuxStrategyTimer(StrategyManager::GetInstance());
    strategy_timer->Start();
    //StrategyManager::GetInstance()->StartLogging();
#endif

#ifdef NETWORK_INTERFACE
    try
    {
        while(1) {
            LinuxServer connection;
            LinuxServer server(10373);
            cout << "[Waiting..]" << endl;
            server.accept ( connection );
            cout << "[Accepted..]" << endl;

            try
            {
                while(true) {
                    string receive_data;
                    TiXmlDocument doc;
                    connection >> receive_data;
                    doc.Parse(receive_data.c_str());
                    TiXmlElement* root = doc.FirstChildElement("Command");
                    if(root != NULL) {
                        XMLReceiveCommand(root);
                    }
                    if(StrategyStatus::SimulatorFlag) {
                        root = doc.FirstChildElement("Simulator");
                        if(root != NULL) {
                            XMLReceiveSimulator(root);
                        }
                    }
                    root = doc.FirstChildElement("Request");
                    if(root != NULL) {
                        TiXmlDocument request_doc = XMLReceiveRequest(root);
                        TiXmlPrinter printer;
                        request_doc.Accept( &printer );
                        connection << printer.CStr();
                    }
                    root = doc.FirstChildElement("ReloadConfig");
                    if(root != NULL) {
                        XMLLoadConfig();
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
#endif

#ifdef INTERACTIVE_INTERFACE
    DrawIntro();
    while(1)
    {
        int ch = _getch();
        if(ch == 0x1b)
        {
            ch = _getch();
            if(ch == 0x5b)
            {
                ch = _getch();
                if(ch == 0x41) // Up arrow key
                    MoveUpCursor();
                else if(ch == 0x42) // Down arrow key
                    MoveDownCursor();
                else if(ch == 0x44) // Left arrow key
                    MoveLeftCursor();
                else if(ch == 0x43)
                    MoveRightCursor();
            }
        }
        else if( ch == '[' )
            DecreaseValue(false);
        else if( ch == ']' )
            IncreaseValue(false);
        else if( ch == '{' )
            DecreaseValue(true);
        else if( ch == '}' )
            IncreaseValue(true);
        else if( ch >= 'A' && ch <= 'z' )
        {
            char input[128] = {0,};
            char *token;
            int input_len;
            char cmd[80];
            char strParam[20][30];
            int num_param;
            int idx = 0;

            BeginCommandMode();

            printf("%c", ch);

            input[idx++] = (char)ch;

            while(1)
            {
                ch = _getch();
                if( ch == 0x0A )
                    break;
                else if( ch == 0x7F )
                {
                    if(idx > 0)
                    {
                        ch = 0x08;
                        printf("%c", ch);
                        ch = ' ';
                        printf("%c", ch);
                        ch = 0x08;
                        printf("%c", ch);
                        input[--idx] = 0;
                    }
                }
                else if( ch >= 'A' && ch <= 'z' )
                {
                    if(idx < 127)
                    {
                        printf("%c", ch);
                        input[idx++] = (char)ch;
                    }
                }
            }

            fflush(stdin);
            input_len = strlen(input);

            if(input_len > 0)
            {
                token = strtok( input, " " );
                if(token != 0)
                {
                    strcpy( cmd, token );
                    token = strtok( 0, " " );
                    num_param = 0;

                    while(token != 0)
                    {
                        strcpy(strParam[num_param++], token);
                        token = strtok( 0, " " );
                    }

                    if(strcmp(cmd, "exit") == 0)
                    {
                        if(AskSave() == false)
                            break;
                    }

                    if(strcmp(cmd, "re") == 0)
                        DrawScreen();
                    else if(strcmp(cmd, "load") == 0)
                    {
                        XMLLoadConfig();
                    }
                    else if(strcmp(cmd, "save") == 0)
                    {
                        //Walking::GetInstance()->SaveINISettings(ini);
                        SaveCmd();

                    }
                    else if(strcmp(cmd, "mon") == 0)
                    {
                        MonitorCmd();
                    }
                    else if(strcmp(cmd, "help") == 0)
                        HelpCmd();
                    else
                        PrintCmd("Bad command! please input 'help'");
                }
            }

            EndCommandMode();
        }
    }

    DrawEnding();
#endif
    return 0;
}
