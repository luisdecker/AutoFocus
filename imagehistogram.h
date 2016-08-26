#ifndef IMAGEHISTOGRAM_H
#define IMAGEHISTOGRAM_H

#include <opencv2/opencv.hpp>

class ImageHistogram {
public:
    ImageHistogram( int bins, cv::Mat image );
    int pixelsInBin( int bin );
    bool pixelsAboveThreshold( int bin, int threshold );//Bins começam a ser contadas a partir de 1.
    ~ImageHistogram();
private:

    int	bins;
    int * hist;
    bool checkImage( cv::Mat image ) const;
    void createHistogram( cv::Mat image );
};

#endif // IMAGEHISTOGRAM_H
