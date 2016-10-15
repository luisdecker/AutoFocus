#ifndef AUTOFOCUSMETHODS_H
#define AUTOFOCUSMETHODS_H
#include <opencv.hpp>
#include <map>
#include<float.h>

#include "focusmetrics.h"
#include "hologramdecoder.h"

namespace AFM {


    class Classic {

    public:
        Classic( cv::Mat image, cv::Mat ROI = cv::Mat() , FM::FocusMetric *focusMetric = 0 );
        cv::Mat findFocus( cv::Mat image = cv::Mat(), cv::Mat ROI = cv::Mat(), FM::FocusMetric *metric = 0 );
        cv::Mat operator()() {return findFocus();}

    private:

        static void showHologram( cv::Mat image, int delay = 1 );
        static cv::Mat splitHologram( cv::Mat image );
        cv::Mat decodeTo( cv::Mat image, int focalPoint );
        cv::Mat image, ROI;
        FM::FocusMetric *focusMetric;
        int minDecode = 0, maxDecode = 12000; //Minimal and maximal values to try to find the focus.
        int step = 50; //Step between focus planes;

    };
}
#endif // AUTOFOCUSMETHODS_H
