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

void print_data(const Urg_driver& urg,
                const vector<long>& data, long time_stamp)
{
    cout << data.size() << endl;
    int front_index = urg.deg2index(0);
    cout << "front_index: " 
         << data[front_index] << " [mm], ("
         << time_stamp << " [msec]) " << front_index << endl;
    int left_index = urg.deg2index(+90);
    cout << "left_index: "
         << data[left_index] << " [mm], ("
         << time_stamp << " [msec]) " << left_index << endl;
    int right_index = urg.deg2index(-90);
    cout << "right_index: "
         << data[right_index] << " [mm], ("
         << time_stamp << " [msec]) " << right_index << endl;

}

int main(void)
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    change_current_dir();

    TiXmlDocument doc;

    //Urg_driver urg;
//    if (!urg.open(LASER_DEV_NAME, 115200, Urg_driver::Serial )) {
//        cout << "Urg_driver::open(    ): "<< LASER_DEV_NAME << ": " << urg.what() << endl;
//        return 1;
//    }

//    urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(+90), 0);
//    urg.start_measurement(Urg_driver::Distance, 0, 0);

    ////////////////// Framework Initialize ////////////////////////////
    if(VisionManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //Motion::GetInstance()->LoadINISettings(ini);

    VisionManager::GetInstance()->AddModule((VisionModule*)VisionCapture::GetInstance());

    LinuxVisionTimer *vision_timer = new LinuxVisionTimer(VisionManager::GetInstance());
    vision_timer->Start();
    //-----------------------------------------------------------------------------------//

    if(LocationManager::GetInstance()->Initialize() == false)
    {
        printf("Fail to initialize Strategy Manager!\n");
        return 1;
    }

    //Motion::GetInstance()->LoadINISettings(ini);

    LocationManager::GetInstance()->AddModule((LocationModule*)LaserCapture::GetInstance());

    LinuxLocationTimer *location_timer = new LinuxLocationTimer(LocationManager::GetInstance());
//    location_timer->Start();
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



    ///////////////////////////////////////////////////////////////////
    
    //StrategyManager::GetInstance()->LoadINISettings(ini);

//    StrategyManager::GetInstance()->SetEnable(true);

//    LinuxActionScript::PlayMP3("../../../Data/mp3/Demonstration ready mode.mp3");

	while(1) {
	sleep(1);
        //vector<long> data;
        //long time_stamp = 0;
        //if (!urg.get_distance(data, &time_stamp)) {
        //    cout << "Urg_driver:: get_distance(): " << urg.what() << endl;
        //    return 1;
        //}
        //print_data(urg, data, time_stamp); 
	}
	
    return 0;
}
