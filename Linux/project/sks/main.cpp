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

//#define INI_FILE_PATH       "../../../Data/config.ini"

using namespace std;

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

    ////////////////// Framework Initialize ////////////////////////////
    if(VisionManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Vision Manager!\n");
        return 1;
    }

    //Motion::GetInstance()->LoadINISettings(ini);
 
    
    VisionManager::GetInstance()->AddModule((VisionModule*)VisionCapture::GetInstance());
    LinuxVisionTimer *vision_timer = new LinuxVisionTimer(VisionManager::GetInstance());
    vision_timer->Start();

    //-----------------------------------------------------------------------------------//

    if(StrategyManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //Motion::GetInstance()->LoadINISettings(ini);

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Motion::GetInstance());
    StrategyManager::GetInstance()->AddModule((StrategyModule*)ReadVision::GetInstance());

    LinuxStrategyTimer *stragey_timer = new LinuxStrategyTimer(StrategyManager::GetInstance());
    stragey_timer->Start();
    //-----------------------------------------------------------------------------------//

    if(LocationManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Location Manager!\n");
        return 1;
    }

    //Motion::GetInstance()->LoadINISettings(ini);

    LocationManager::GetInstance()->AddModule((LocationModule*)LaserCapture::GetInstance());

    LinuxLocationTimer *location_timer = new LinuxLocationTimer(LocationManager::GetInstance());
    location_timer->Start();
    //-----------------------------------------------------------------------------------//

    if(StrategyManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //Motion::GetInstance()->LoadINISettings(ini);

    //StrategyManager::GetInstance()->AddModule((StrategyModule*)Motion::GetInstance());
    StrategyManager::GetInstance()->AddModule((StrategyModule*)ReadLaser::GetInstance());

    LinuxStrategyTimer *stragey_timer1 = new LinuxStrategyTimer(StrategyManager::GetInstance());
    stragey_timer1->Start();



    ///////////////////////////////////////////////////////////////////
    
    //StrategyManager::GetInstance()->LoadINISettings(ini);

    //StrategyManager::GetInstance()->SetEnable(true);

    //LinuxActionScript::PlayMP3("../../../Data/mp3/Demonstration ready mode.mp3");

	while(1) {
            //cout << "Input";
            //cin >> Status::vector.X >> Status::vector.Y >> Status::w;
	}
	
    return 0;
}
