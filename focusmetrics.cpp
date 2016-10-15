#include "focusmetrics.h"


/************************************************
Auxiliary Functions
************************************************/
bool FM::FocusMetric::check_image( cv::Mat image ) const {
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
//_______________________________________________

double FM::ThreshGradient::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
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
//_______________________________________________
double FM::BrennerGradient::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
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
//_______________________________________________
double FM::ImagePower::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
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
//_______________________________________________
double FM::TenenbaumGradient::measure_focus( cv::Mat image ) const {

    assert( check_image( image ) );
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
//_______________________________________________
double FM::LaplacianEnergy::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
	image.convertTo( image, CV_8UC1 );
	cv::Mat processed;

	cv::Laplacian( image, processed, CV_8U );
	cv::pow( processed, 2, processed );
	return cv::sum( processed )[0];
}
//_______________________________________________
double FM::ThresholdedHistogram::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
	image.convertTo( image, CV_8UC1 );
	ImageHistogram * histogram = new ImageHistogram( bins, image );
	int smallerBin =  bins + 1, biggestBin = 0;
	for( int bin = 1; bin <= bins; bin++ ) {
		if( histogram->pixelsAboveThreshold( bin, threshold ) ) {
			smallerBin = bin < smallerBin ? bin : smallerBin;
			biggestBin = bin > biggestBin ? bin : biggestBin;
		}
	}
	int pixelPerBin = floor( 256 / bins );
	int smallerPixelRange = ( -1 * ( pixelPerBin ) ) + smallerBin * pixelPerBin;
	int biggestPixelRange = pixelPerBin * biggestBin;
	return ( double )biggestPixelRange - smallerPixelRange;
}
//_______________________________________________
double FM::NormalizedVariance::measure_focus( cv::Mat image ) const {

    assert( check_image( image ) );
	image.convertTo( image, CV_8UC1 );
    double mean = cv::mean( image )[0];
    double sum = 0;
    for( int row = 0; row < image.rows; row++ ) {
		for( int column = 0; column < image.cols; column ++ ) {
            uchar pixel = image.at<uchar>( row, column );
            sum += std::pow( ( pixel - mean ), 2 );
		}
	}
    sum = sum / mean;
    return sum;
}
//_______________________________________________
double FM::AutoCorrelation::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
    image.convertTo( image, CV_8UC1 );
    double part1 = 0, part2 = 0;
    for( int row = 0; row < image.rows; row++ ) {
        for( int column = 0; column < image.cols; column ++ ) {
            if( row + 1 < image.rows ) {
                part1 += image.at<uchar>( row, column ) * image.at<uchar>( row + 1, column );
            }
            if( row + 2 < image.rows ) {
                part2 += image.at<uchar>( row, column ) * image.at<uchar>( row + 2, column );
            }
        }
    }
    return std::abs( part1 - part2 );
}
//_______________________________________________
double FM::StandartDeviationCorrelation::measure_focus( cv::Mat image ) const {
    assert( check_image( image ) );
    image.convertTo( image, CV_8UC1 );
    double mean = cv::mean( image )[0], part1;
    for( int row = 0; row < image.rows; row++ ) {
        for( int column = 0; column < image.cols; column ++ ) {
            if( row + 1 < image.rows ) {
                part1 += std::abs( image.at<uchar>( row, column ) * image.at<uchar>( row + 1, column ) - mean );
            }
        }
    }
    return part1;
}
//_______________________________________________
double FM::PixelCount::measure_focus( cv::Mat image )const {
    assert( check_image( image ) );
    image.convertTo( image, CV_8UC1 );
    int sum = 0;
    for( int row = 0; row < image.rows; row++ ) {
        for( int column = 0; column < image.cols; column ++ ) {
            if( image.at<uchar>( row, column ) <= threshold ) {
                sum++;
            }
        }
    }
    return sum;
}

