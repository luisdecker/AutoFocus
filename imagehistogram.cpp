#include "imagehistogram.h"
#include "cmath"

//-----------------------------------------------
ImageHistogram::ImageHistogram( int bins, cv::Mat image ) {
	this->bins = bins;
	createHistogram( image );
}
//-----------------------------------------------
void ImageHistogram::createHistogram( cv::Mat image ) {
    assert( checkImage( image ) );
    int valuesPerBin = floor( 256 / bins );//256 valores, de 0 a 255.
    int * histogram = ( int * )calloc( bins, sizeof( int ) );
    image.convertTo( image, CV_8U );
    for( int x = 0; x < image.rows; x++ ) {
        for( int y = 0; y < image.cols; y++ ) {
            int pixel = ( int )image.at<uchar>( x, y );
            int destinyBin = ( int ) pixel / ( int )valuesPerBin;
            histogram[destinyBin]++;
        }
    }
    this->hist = histogram;
}
//-----------------------------------------------
int ImageHistogram::pixelsInBin( int bin ) {
    assert( bin <= bins &&
            bin > 0 );
    return hist[bin - 1];
}

//-----------------------------------------------
bool ImageHistogram::pixelsAboveThreshold( int bin, int threshold ) {
    assert( bin > 0 );
    assert(	bin <= bins );
    assert( threshold >= 0 );
    assert( threshold < 256 );

    return hist[bin - 1] >= threshold;



}

//-----------------------------------------------

/************************************************
 * Auxiliar Functions
 * *********************************************/
bool ImageHistogram::checkImage( cv::Mat image ) const {
	if(
		image.empty() ||
		image.rows < 1 ||
		image.cols < 1
	) {return false;}
	return true;
}


/************************************************
 * Destructor
 * *********************************************/
ImageHistogram::~ImageHistogram() {
    free( ( void * )hist );
}
