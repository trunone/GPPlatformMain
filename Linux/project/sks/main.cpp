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
using namespace Robot;

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
    if(VisionManager::GetInstance()->Initialize(VisionCapture) == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //VisionManager::GetInstance()->AddModule((VisionModule*)VisionCapture::GetInstance());

    LinuxVisionTimer *vision_timer = new LinuxVisionTimer(VisionManager::GetInstance());
    vision_timer->Start();
    //-----------------------------------------------------------------------------------//

    if(LocationManager::GetInstance()->Initialize(&urg) == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //LocationManager::GetInstance()->AddModule((LocationModule*)LaserCapture::GetInstance());

    LinuxLocationTimer *location_timer = new LinuxLocationTimer(LocationManager::GetInstance());
    location_timer->Start();
    ////-----------------------------------------------------------------------------------//

    if(StrategyManager::GetInstance()->Initialize(&motors) == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)ReadVision::GetInstance());

    LinuxStrategyTimer *stragey_timer = new LinuxStrategyTimer(StrategyManager::GetInstance());
    stragey_timer->Start();
    ///////////////////////////////////////////////////////////////////
//    StrategyManager::GetInstance()->SetEnable(true);

//    LinuxActionScript::PlayMP3("../../../Data/mp3/Demonstration ready mode.mp3");
//
	while(1) {

        TiXmlDocument doc("Status.xml");
        doc.LoadFile();
        TiXmlPrinter printer;
        printer.SetStreamPrinting();
        //TiXmlDocument doc;
        //doc.Parse(printer.CStr());
        TiXmlElement* root = doc.RootElement();//Status
        TiXmlElement* element = root->FirstChildElement();//ColorModel
        TiXmlElement* model = element->FirstChildElement();//Model
        TiXmlAttribute* type= model->FirstAttribute();//Model type
        const char *www="www";
        double x[7]={0.1,0.2,0.3,0.4,0.5,0.6,0.7};
        int i=0;

        const char *fileName = "string2xml";
        cout<<type->Value()<<endl;
        for(;model != NULL;model=model->NextSiblingElement()){
            TiXmlElement* modelchild=model->FirstChildElement();
            for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//get node information
                string informationType=modelchild->Value();
                string information=modelchild->FirstAttribute()->Value();
                modelchild->SetDoubleAttribute(www,x[i++]);
                //modelchild->RemoveAttribute(www);
                //doc.SaveFile(fileName);
                doc.SaveFile();
                cout <<	informationType << ":" << information << endl;
            }
        }
        doc.Accept( &printer );
        fscanf( stdout, "%s", printer.CStr() );
        printf("%s", printer.CStr());
        int port=1234;
        string aa="a";
        char w;
        w=0x08;

        LinuxServer new_sock;
        LinuxServer server (port);

        while(true) {
            cout << "[Waiting..]" << endl;         
            server.accept ( new_sock );
            cout << "[Accepted..]" << endl;	
            new_sock << printer.CStr();
            //printf("%s", printer.CStr());
            cout << "[success]" << endl;		
        }
    return 0;
}
