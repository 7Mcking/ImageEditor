//
// Created by Naren Sadhwani on 02.04.24.
//

#include "ErodePlugin.h"


QString ErodePlugin::Name()
{
    return "Erode";
}

void ErodePlugin::editImage(cv::Mat &input, cv::Mat &output) {
    erode(input, output, cv::Mat());
}
