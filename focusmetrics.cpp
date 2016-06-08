#include "focusmetrics.h"
bool FM::FocusMetric::checkImage( cv::Mat image ) const {
    if(
        image.empty() ||
        image.rows < 1 ||
        image.cols < 1
    ) {return false;}
    return true;
}
/////////////////////////////////////////////////
/*
The intensity at each pixel is squared and summed if the pixel is above a given threshold
*/
double FM::ImagePower::measureFocus( cv::Mat image ) const {
    assert ( checkImage( image ) );
    image.convertTo( image,CV_8UC1 );
    double sum;
    for ( int x = 0; x < image.cols; x++ ) {
        for ( int y = 0; y < image.rows; y++ ) {
            uchar pixelXY = image.at<uchar>( y,x );
            uchar pixelXY1;
            if( ( x + 1 ) < image.cols ) {
                pixelXY1 = image.at<uchar> ( y,x+1 );
            } else {
                pixelXY1 = ( uchar )0;
            }
            uchar alce = ( int ) pixelXY1 - ( int ) pixelXY;
            alce = alce*alce;
            if( ( int ) alce >= ( int ) threshold )
                sum += ( int ) alce;
        }
    }
    return sum;
}
/////////////////////////////////////////////////
