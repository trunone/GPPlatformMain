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

    TiXmlDocument doc;

    motors.OpenDeviceAll();
    motors.SetEnableAll();
    motors.ActivateProfileVelocityModeAll();

	VisionCapture = cvCaptureFromCAM( -1 );

    ////////////////// Framework Initialize ////////////////////////////
    //if(VisionManager::GetInstance()->Initialize(VisionCapture) == false)
    //{
    //    printf("Fail to initialize Strategy Manager!\n");
    //    return 1;
    //}

    ////VisionManager::GetInstance()->AddModule((VisionModule*)VisionCapture::GetInstance());

    //LinuxVisionTimer *vision_timer = new LinuxVisionTimer(VisionManager::GetInstance());
    //vision_timer->Start();
    //-----------------------------------------------------------------------------------//

    //if(LocationManager::GetInstance()->Initialize(&urg) == false)
    //{
    //    printf("Fail to initialize Strategy Manager!\n");
    //    return 1;
    //}

    ////LocationManager::GetInstance()->AddModule((LocationModule*)LaserCapture::GetInstance());

    //LinuxLocationTimer *location_timer = new LinuxLocationTimer(LocationManager::GetInstance());
    //location_timer->Start();
    ////-----------------------------------------------------------------------------------//

    if(StrategyManager::GetInstance()->Initialize(&motors) == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    StrategyManager::GetInstance()->AddModule((StrategyModule*)Motion::GetInstance());

    LinuxStrategyTimer *stragey_timer = new LinuxStrategyTimer(StrategyManager::GetInstance());
    stragey_timer->Start();
    ///////////////////////////////////////////////////////////////////
//    StrategyManager::GetInstance()->SetEnable(true);

//    LinuxActionScript::PlayMP3("../../../Data/mp3/Demonstration ready mode.mp3");
//
    try
    {
        while(1) {

            string xml;
            LinuxServer new_sock;
            LinuxServer server (1234);
	
            cout << "[Waiting..]" << endl;
            server.accept ( new_sock );
            cout << "[Accepted..]" << endl;	

            try
            {
                while(true){	
                    new_sock >> xml;
                    cout << "[success recv]" << endl << xml;
                    TiXmlDocument doc;
                    doc.Parse(xml.c_str());
                    TiXmlElement* root = doc.RootElement();
                    TiXmlElement* element;
                    element = root->FirstChildElement("ManualDirection");
                    if(element != NULL) {
                        TiXmlElement* modelchild;
                        modelchild = element->FirstChildElement("Rotate");
                        if(modelchild != NULL){
                            modelchild->Attribute("w", &StrategyStatus::w);
                        }
                        modelchild = element->FirstChildElement("Vector");
                        if(modelchild != NULL){
                            modelchild->Attribute("x", &StrategyStatus::x);
                            modelchild->Attribute("y", &StrategyStatus::y);
                        }
                    }
                    element = root->FirstChildElement("Vision");
                    if(element != NULL){
                        cout<<"I got vision"<<endl;
                    }
                    new_sock << "recv";
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
