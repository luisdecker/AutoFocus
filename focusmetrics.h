#ifndef FOCUSMETRICS_H
#define FOCUSMETRICS_H
#include <cv.hpp>
#include "imagehistogram.h"

namespace FM {

    class FocusMetric {
    public:
        FocusMetric() {}
        virtual double measure_focus( cv::Mat ) const {return -1.;}
        bool check_image( cv::Mat image ) const;
        enum minMax {MAXIMIZATION, MINIMIZATION};
        minMax minMaxType;
    };


    /*****************************************************
    *Thresholded Gradient-Squared:
    This function computes the square of one dimensional gradient of the image to emphasize the larger values.
    Then, it sums up the values that are greater than a threshold θ:
    *****************************************************/
    class ThreshGradient : public FocusMetric {
    public:
        ThreshGradient( int threshold ) : threshold( ( uchar )threshold ) {
            this->minMaxType = MINIMIZATION;
        }
        double measure_focus( cv::Mat image ) const;
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
        BrennerGradient( uchar threshold ): threshold( threshold ) {
            this->minMaxType = MINIMIZATION;
        }
        double measure_focus( cv::Mat image ) const;
    private:
        uchar threshold;
    };
    /*****************************************************
    This function computes the energy of the image after Sobel operators are applied. Sobel operators look for
    both wider and longer edges in the image compared to Brenner and squared gradient metrics.
    *****************************************************/
    class TenenbaumGradient : public FocusMetric {
    public:
        TenenbaumGradient() {
            this->minMaxType = MINIMIZATION;
        }
        double measure_focus( cv::Mat image ) const;
    private://No threshold here!
    };
    /*****************************************************
    This function computes the discrete approximation of the second-order derivative of an image in both vertical
    and horizontal axes by convolving it with the following Laplacian mask:
    *****************************************************/
    class LaplacianEnergy : public FocusMetric {
    public:
        LaplacianEnergy() {
            this->minMaxType = MINIMIZATION;
        }

        double measure_focus( cv::Mat image ) const;
    private://No threshold here!
    };

    /*****************************************************
    This function considers only the histogram bins which contain at least the average number of pixels per bin,
    and then outputs the range between the gray levels of the first and the last qualified histogram bins.
    *****************************************************/
    class ThresholdedHistogram: public FocusMetric {
    public:
        ThresholdedHistogram( uchar threshold, uchar bins = 64 ): threshold( threshold ), bins( ( int )bins ) {
            this->minMaxType = MINIMIZATION;
        }
        double measure_focus( cv::Mat ) const;
    private:
        uchar threshold;
        int bins;
    };


    /*****************************************************
    This metric computes the variance among the image with respect to the mean intensity value μ. After summing
    the variance values over the image, the final scalar value is further divided by μ to compensate abnormally
    high values in the data.
    *****************************************************/
    class NormalizedVariance : public FocusMetric {
    public :
        NormalizedVariance() {
            this->minMaxType = MINIMIZATION;
        }
        double measure_focus( cv::Mat ) const;
    private://No threshold here!
    };

    /*****************************************************
    This function computes the correlation of neighboring pixels
    *****************************************************/
    class AutoCorrelation : public FocusMetric {
    public:
        AutoCorrelation() {
            this->minMaxType = MAXIMIZATION;
        }
        double measure_focus( cv::Mat ) const;
    private://No threshold here!
    };

    /*****************************************************
    In this function, correlation of adjacent pixels are computed with respect to a constant intensity image that has
    gray-level value of μ at all pixels.
    *****************************************************/
    class StandartDeviationCorrelation : public FocusMetric {

    public:
        StandartDeviationCorrelation() {
            this->minMaxType = MAXIMIZATION;
        }
        double measure_focus( cv::Mat ) const;
    private://No threshold here!


    };

    /*****************************************************
    *Image Power:
    The intensity at each pixel is squared and summed if the pixel is above a given threshold
    *****************************************************/
    class ImagePower : public FocusMetric {
    public:
        ImagePower( int threshold ): threshold( ( uchar )threshold ) {
            this->minMaxType = MAXIMIZATION;
        }
        double measure_focus( cv::Mat image ) const;
    private:
        uchar threshold;

    };

    /*****************************************************
    Thresholded-pixel count
    In this metric, the number of pixels which has lesser intensity than a given threshold is counted as the quality
    measure.
    *****************************************************/
    class PixelCount : public FocusMetric {
    public:
        PixelCount( uchar threshold ): threshold( threshold ) {
            this->minMaxType = MINIMIZATION;
        }
        double measure_focus( cv::Mat ) const;
    private:
        uchar threshold = 255;
    };
}


#endif // FOCUSMETRICS_H
