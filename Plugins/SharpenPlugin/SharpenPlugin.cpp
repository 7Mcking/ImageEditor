//
// Created by Naren Sadhwani on 02.04.24.
//

#include "SharpenPlugin.h"

QString SharpenPlugin::Name()
{
    return "Sharpen";
}

void SharpenPlugin::editImage(cv::Mat &input, cv::Mat &output) {
    int intensity = 2;
    cv::Mat smoothed;
    cv::GaussianBlur(input, smoothed, cv::Size(9, 9), 0);
    output = input + (input - smoothed) * intensity;
}

