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

#include "mjpg_streamer.h"
#include "LinuxWheeled.h"
#include "urg_cpp/Urg_driver.h"
#include "urg_cpp/math_utilities.h"

#define INI_FILE_PATH       "../../../Data/config.ini"
//#define SCRIPT_FILE_PATH    "script.asc"

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

    minIni* ini = new minIni(INI_FILE_PATH);
    Image* rgb_output = new Image(Camera::WIDTH, Camera::HEIGHT, Image::RGB_PIXEL_SIZE);

    LinuxCamera::GetInstance()->Initialize(0);
    LinuxCamera::GetInstance()->SetCameraSettings(CameraSettings());    // set default
    LinuxCamera::GetInstance()->LoadINISettings(ini);                   // load from ini

    mjpg_streamer* streamer = new mjpg_streamer(Camera::WIDTH, Camera::HEIGHT);

    ColorFinder* ball_finder = new ColorFinder();
    ball_finder->LoadINISettings(ini);
    httpd::ball_finder = ball_finder;

    ColorFinder* red_finder = new ColorFinder(0, 15, 45, 0, 0.3, 50.0);
    red_finder->LoadINISettings(ini, "RED");
    httpd::red_finder = red_finder;

    ColorFinder* yellow_finder = new ColorFinder(60, 15, 45, 0, 0.3, 50.0);
    yellow_finder->LoadINISettings(ini, "YELLOW");
    httpd::yellow_finder = yellow_finder;

    ColorFinder* blue_finder = new ColorFinder(225, 15, 45, 0, 0.3, 50.0);
    blue_finder->LoadINISettings(ini, "BLUE");
    httpd::blue_finder = blue_finder;

    httpd::ini = ini;

    //////////////////// Framework Initialize ////////////////////////////
    //if(StrageyManager::GetInstance()->Initialize(&cm730) == false)
    //{
    //    linux_cm730.SetPortName(U2D_DEV_NAME1);
    //    if(StrageyManager::GetInstance()->Initialize(&cm730) == false)
    //    {
    //        printf("Fail to initialize Stragey Manager!\n");
    //        return 0;
    //    }
    //}

    //Walking::GetInstance()->LoadINISettings(ini);

    //StrageyManager::GetInstance()->AddModule((StrageyModule*)Action::GetInstance());
    //StrageyManager::GetInstance()->AddModule((StrageyModule*)Head::GetInstance());
    //StrageyManager::GetInstance()->AddModule((StrageyModule*)Walking::GetInstance());

    //LinuxStrageyTimer *motion_timer = new LinuxStrageyTimer(StrageyManager::GetInstance());
    //motion_timer->Start();
    /////////////////////////////////////////////////////////////////////
    
    //StrageyManager::GetInstance()->LoadINISettings(ini);

    //StrageyManager::GetInstance()->SetEnable(true);

    //LinuxActionScript::PlayMP3("../../../Data/mp3/Demonstration ready mode.mp3");
	
    //Urg_driver urg;
    //if (!urg.open(LASER_DEV_NAME, 115200, Urg_driver::Serial )) {
    //    cout << "Urg_driver::open(    ): "<< LASER_DEV_NAME << ": " << urg.what() << endl;
    //    return 1;
    //}

    //urg.set_scanning_parameter(urg.deg2step(-135), urg.deg2step(+135), 0);
    //urg.start_measurement(Urg_driver::Distance, 0, 0);

	while(1) {
		Point2D ball_pos, red_pos, yellow_pos, blue_pos;
		
        LinuxCamera::GetInstance()->CaptureFrame();
        memcpy(rgb_output->m_ImageData, LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageData, LinuxCamera::GetInstance()->fbuffer->m_RGBFrame->m_ImageSize);

        ball_pos = ball_finder->GetPosition(LinuxCamera::GetInstance()->fbuffer->m_HSVFrame);
        red_pos = red_finder->GetPosition(LinuxCamera::GetInstance()->fbuffer->m_HSVFrame);
        yellow_pos = yellow_finder->GetPosition(LinuxCamera::GetInstance()->fbuffer->m_HSVFrame);
        blue_pos = blue_finder->GetPosition(LinuxCamera::GetInstance()->fbuffer->m_HSVFrame);
		
        unsigned char r, g, b;
        for(int i = 0; i < rgb_output->m_NumberOfPixels; i++)
        {
            r = 0; g = 0; b = 0;
            if(ball_finder->m_result->m_ImageData[i] == 1)
                {
                    r = 255;
                    g = 128;
                    b = 0;
                }
                if(red_finder->m_result->m_ImageData[i] == 1)
                {
                    if(ball_finder->m_result->m_ImageData[i] == 1)
                    {
                        r = 0;
                        g = 255;
                        b = 0;
                    }
                    else
                    {
                        r = 255;
                        g = 0;
                        b = 0;
                    }
                }
                if(yellow_finder->m_result->m_ImageData[i] == 1)
                {
                    if(ball_finder->m_result->m_ImageData[i] == 1)
                    {
                        r = 0;
                        g = 255;
                        b = 0;
                    }
                    else
                    {
                        r = 255;
                        g = 255;
                        b = 0;
                    }
                }
                if(blue_finder->m_result->m_ImageData[i] == 1)
                {
                    if(ball_finder->m_result->m_ImageData[i] == 1)
                    {
                        r = 0;
                        g = 255;
                        b = 0;
                    }
                    else
                    {
                        r = 0;
                        g = 0;
                        b = 255;
                    }
                }
		
                if(r > 0 || g > 0 || b > 0)
                {
                    rgb_output->m_ImageData[i * rgb_output->m_PixelSize + 0] = r;
                    rgb_output->m_ImageData[i * rgb_output->m_PixelSize + 1] = g;
                    rgb_output->m_ImageData[i * rgb_output->m_PixelSize + 2] = b;
                }
            }
        
        streamer->send_image(rgb_output);

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
