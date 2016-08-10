#ifndef FOCUSMETRICS_H
#define FOCUSMETRICS_H
#include <cv.hpp>

namespace FM {

    class FocusMetric {
    public:
        FocusMetric() {};
        virtual double measureFocus( cv::Mat ) const {return 1.;}
        bool checkImage( cv::Mat image ) const;
    };
    /*****************************************************
    *Image Power:
    The intensity at each pixel is squared and summed if the pixel is above a given threshold
    *****************************************************/
    class ImagePower : public FocusMetric {
    public:
        ImagePower( int threshold ): threshold( ( uchar )threshold ) {}
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
    private:
        uchar threshold;
    };
    /*****************************************************
    Thresholded Brenner Gradient
    Brenner proposed this function to compensate the effect of noise on gradient function. It computes the difference
    of a pixel and its second neighboring pixel as the gradient. Compared to the squared gradient, this metric looks
    for wider edges on the image:
    *****************************************************/
    class BrennerGradient : public FocusMetric {
    public:
        double measureFocus( cv::Mat image ) const;
    private:
        uchar threshold;
    };
    /*****************************************************
    This function computes the energy of the image after Sobel operators are applied. Sobel operators look for
    both wider and longer edges in the image compared to Brenner and squared gradient metrics.
    *****************************************************/
    class TenenbaumGradient : public FocusMetric {
    public:
        double measureFocus( cv::Mat image ) const;
    private://No threshold here!
    };
    /*****************************************************
    This function computes the discrete approximation of the second-order derivative of an image in both vertical
    and horizontal axes by convolving it with the following Laplacian mask:
    *****************************************************/
    class LaplacianEnergy : public FocusMetric {
        double measureFocus( cv::Mat image ) const;
    private://No threshold here!
    };


}


#endif // FOCUSMETRICS_H
