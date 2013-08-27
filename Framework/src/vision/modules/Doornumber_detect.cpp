#include "Doornumber_detect.h"
#include <iostream>
#include <stdio.h>
using namespace Robot;
using namespace std;



Doornumber_detect* Doornumber_detect::m_UniqueInstance = new Doornumber_detect();

Doornumber_detect::Doornumber_detect() {
}
Doornumber_detect::~Doornumber_detect() {
}
void Doornumber_detect::Initialize() {
    VisionStatus::Flagdoor_red = false;
    VisionStatus::Flagdoor_green = false;
    VisionStatus::Flagdoor_blue = false;
}

void Doornumber_detect::Process(){
    if(!VisionStatus::doordet_enable) return ;
    cv::Mat src = VisionStatus::VideoFrame;
// Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

    // Use Canny instead of threshold to catch squares with gradient shading
    cv::Mat bw;
    cv::Canny(gray, bw, 0, 50, 5);

    // Find contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> approx;
    cv::Mat dst = src.clone();

    for (int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

        // Skip small or non-convex objects 
        if (std::fabs(cv::contourArea(contours[i])) < 1500 || !cv::isContourConvex(approx))
            continue;

        if (approx.size() == 4)
        {
            VisionStatus::Flagdoor_green = true;
            //printf("R\n");
        }
        else if (approx.size() > 4 )
        {
            // Detect and label circles
            double area = cv::contourArea(contours[i]);
            cv::Rect r = cv::boundingRect(contours[i]);
            int radius = r.width / 2;

            if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                std::abs(1 - (area / (CV_PI * std::pow(radius, 2.0)))) <= 0.2)
                VisionStatus::Flagdoor_red = true;
           // printf("C\n");
        }
        else if (approx.size() == 3) 
        {
            VisionStatus::Flagdoor_blue = true;
           // printf("T\n");
        }
    }
    VisionStatus::doordet_enable = false;
}
