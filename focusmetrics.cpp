#include "focusmetrics.h"


/************************************************
Auxiliary Functions
************************************************/
bool FM::FocusMetric::checkImage( cv::Mat image ) const {
    if(
        image.empty() ||
        image.rows < 1 ||
        image.cols < 1
    ) {return false;}
    return true;
}

/************************************************
Main focus measurement functions
************************************************/
//-----------------------------------------------

double FM::ThreshGradient::measureFocus( cv::Mat image ) const {
    assert( checkImage( image ) );
    image.convertTo( image, CV_8UC1 );
    double sum;
    for( int x = 0; x < image.cols; x++ ) {
        for( int y = 0; y < image.rows; y++ ) {
            uchar pixelXY = image.at<uchar>( y, x );
            uchar pixelXY1;
            if( ( x + 1 ) < image.cols ) {
                pixelXY1 = image.at<uchar> ( y, x + 1 );
            } else {
                pixelXY1 = ( uchar )0;
            }
            uchar alce = ( int ) pixelXY1 - ( int ) pixelXY;
            alce = alce * alce;
            if( ( int ) alce >= ( int ) threshold )
                sum += ( int ) alce;
        }
    }
    return sum;
}
//-----------------------------------------------
double FM::BrennerGradient::measureFocus( cv::Mat image ) const {
    assert( checkImage( image ) );
    image.convertTo( image, CV_8UC1 );
    double sum;
    for( int x = 0; x < image.cols; x++ ) {
        for( int y = 0; y < image.rows; y++ ) {
            uchar pixelXY = image.at<uchar>( y, x );
            uchar pixelXY1;
            if( ( x + 2 ) < image.cols ) {
                pixelXY1 = image.at<uchar> ( y, x + 2 );
            } else {
                pixelXY1 = ( uchar )0;
            }
            uchar alce = ( int ) pixelXY1 - ( int ) pixelXY;
            alce = alce * alce;
            if( ( int ) alce >= ( int ) threshold )
                sum += ( int ) alce;
        }
    }
    return sum;
}
//-----------------------------------------------
double FM::ImagePower::measureFocus( cv::Mat image ) const {
    assert( checkImage( image ) );
    image.convertTo( image, CV_8UC1 );
    double sum;
    for( int x = 0; x < image.cols; x++ ) {
        for( int y = 0; y < image.rows; y++ ) {
            uchar pixelXY = image.at<uchar>( x, y );
            if( ( int )pixelXY >= ( int ) threshold ) {
                sum += ( int )pixelXY * ( int ) pixelXY;
            }
        }
    }
    return sum;
}
//-----------------------------------------------
double FM::TenenbaumGradient::measureFocus( cv::Mat image ) const {

    assert( checkImage( image ) );
    image.convertTo( image, CV_8UC1 );
    cv::Mat processedX, processedY, total;

    cv::Sobel( image, processedX, CV_16S, 1, 0 );//Calculates sobel for X cordinates
    cv::pow( processedX, 2, processedX );//Squares the result
    cv::Sobel( image, processedY, CV_16S, 0, 1 );//Calculates sobel for Y cordinates
    cv::pow( processedY, 2, processedY );
    cv::add( processedX, processedY, total );
    cv::Scalar totalSum = cv::sum( total );
    return totalSum[0];

}
//-----------------------------------------------
double FM::LaplacianEnergy::measureFocus( cv::Mat image ) const {
    assert( checkImage( image ) );
    image.convertTo( image, CV_8UC1 );
    cv::Mat processed;

    cv::Laplacian( image, processed, CV_8U );
    cv::pow( processed, 2, processed );
    return cv::sum( processed )[0];
}
//-----------------------------------------------





