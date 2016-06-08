#ifndef FOCUSMETRICS_H
#define FOCUSMETRICS_H
#include <cv.hpp>

namespace FM {
    class FocusMetric {
    public:
        FocusMetric() {};
        virtual double measureFocus( cv::Mat ) const {return 0.;}
        bool checkImage( cv::Mat image ) const;
    };
    /*****************************************************
    *Image Power:
    The intensity at each pixel is squared and summed if the pixel is above a given threshold
    *****************************************************/
    class ImagePower : public FocusMetric {
    public:
        ImagePower( int threshold ):threshold( ( uchar )threshold ) {}
        double measureFocus( cv::Mat image ) const;
    private:
        uchar threshold;

    };
    /*****************************************************
    *Thresholded Gradient-Squared:
    This function computes the square of one dimensional gradient of the image to emphasize the larger values.
    Then, it sums up the values that are greater than a threshold θ:
    *****************************************************/
    class ThreshGradient : public FocusMetric {
    public:

        double measureFocus( cv::Mat image ) const;
    };
}


#endif // FOCUSMETRICS_H
