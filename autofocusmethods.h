#ifndef AUTOFOCUSMETHODS_H
#define AUTOFOCUSMETHODS_H
#include <opencv.hpp>

class AFM {

public:
    AFM( cv::Mat image, cv::Mat ROI = cv::Mat() );
private:
    cv::Mat image, ROI;

};

#endif // AUTOFOCUSMETHODS_H
