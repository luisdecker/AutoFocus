#include "autofocusmethods.h"

using namespace FM;

AFM::Comparative::Comparative( cv::Mat image, int threshold, int minDecode, int maxDecode, int step ) {
//Populating the methods vector
	for( int method = 1; method <= 10; method++ ) {
		switch( method ) {

			case 1: {//Treshold Gradient-Squared
				this->metrics.push_back( new ThreshGradient( 50 ) );
				break;
			}//1

			case 2: {//Thresholded Brenner Gradient
				this->metrics.push_back( new BrennerGradient( 50 ) );
				break;
			}//2

			case 3: {//Tenenbaum Gradient
				this->metrics.push_back( new TenenbaumGradient() );
				break;
			}//3

			case 4: {//Energy of Laplacian
				this->metrics.push_back( new LaplacianEnergy() );
				break;
			}//4

			case 5: {// Thresholded Histogram Range
				this->metrics.push_back( new ThresholdedHistogram( threshold ) );
				break;
			}//5

			case 6: {//Normalized Variance
				this->metrics.push_back( new NormalizedVariance() );
				break;
			}//6

			case 7: { //Auto Corretalion
				this->metrics.push_back( new AutoCorrelation() );
				break;
			}//7

			case 8: { //Standard deviation based correlation
				this->metrics.push_back( new StandartDeviationCorrelation );
				break;
			}//8

			case 9: { //Image Power
				this->metrics.push_back( new ImagePower( threshold ) );
				break;
			}//9

			case 10: { //Thresholded pixel-count
				this->metrics.push_back( new PixelCount( threshold ) );
				break;
			}//10

		}//Switch
	}//For

	//Update the reconstruction parameters
	this->maxDecode = maxDecode;
	this->minDecode = minDecode;
	this->step = step;

	//Update the image to analise
	this->image = image;
	//

}//Comparative constructor
//_______________________________________________
AFM::Comparative::MetricsTable
AFM::Comparative::create_measure_table() {
	//Rows of the table-> Methods
    this->calculateMinMax();

    cv::Mat workingImage = prepare_image( this->image );
	std::vector <std::vector <double>> rowMethod;

	for( FocusMetric * focusMetric : this->metrics ) {
		std::vector<double> colPlane;
		for( int plane = minDecode; plane <= maxDecode; plane = plane + step ) {
			cv::Mat decodedImage = decode_to( workingImage, plane, false );

			double focusValue = focusMetric->measure_focus( decodedImage );
			colPlane.push_back( focusValue );
		}
		rowMethod.push_back( colPlane );
	}
	return rowMethod;

}//Create Measures Table
//_______________________________________________
std::string
AFM::Comparative::operator()() {
    return  generate_csv();
}

//_______________________________________________
cv::Mat
AFM::Comparative::prepare_image( cv::Mat img ) {
	check_image( img );
	return image.clone();
	//If something else...
}//Prepare Image
//_______________________________________________
void
AFM::Comparative::check_image( cv::Mat image ) {
	assert( !image.empty() );
	assert( !( image.rows < 1 ) );
	assert( !( image.cols < 1 ) );
}//Check Image
//_______________________________________________
cv::Mat
AFM::Comparative::decode_to( cv::Mat input, int plane , bool complexChannel ) {
	cv::Mat decoded = HologramDecoder::decode_hologram( input, plane );
	cv::Mat splitted[2];
	cv::split( decoded, splitted );
    cv::Mat channel = splitted[( int )complexChannel].clone();
    assert( haveMinMax );
    AFM::normalize( channel, minVal, maxVal );
    channel.convertTo( channel, CV_8U, 255 );
    HologramDecoder::fftshift( channel );

    cv::imshow( "", channel );
    cv::waitKey( 1 );
    return channel;
}
//_______________________________________________
std::string
AFM::Comparative::generate_csv( ) {
	std::stringstream ss;
	ss.str( "" );
	//Generate the file header
	ss << "Method;";
	std::vector<int> distances = generate_distances();
	for( int distance : distances ) {
		ss << distance << ( distance == distances.back() ? "\n" : ";" ) ;
	}
	std::string header( ss.str() );
    ss.str( "" );
	std::string methodsNames[] = {"Treshold Gradient-Squared", "Thresholded Brenner Gradient" ,
								  "Tenenbaum Gradient", "Energy of Laplacian", "Thresholded Histogram Range",
								  "Normalized Variance", "Auto Corretalion", "Standard deviation based correlation" ,
								  "Image Power" , "Thresholded pixel-count"
								 };

	int currentMethodIndex = 0;
	MetricsTable results = create_measure_table();

    std::vector <std::string> lines;
	for( std::string methodName : methodsNames ) {
        std::vector<double> methodMeasures = results.at( currentMethodIndex );
		ss.str( "" );
		ss << methodName << ";";
        uint measureIndex = 1;
		for( double measure : methodMeasures ) {
            ss << std::setprecision( 50 ) << measure << ( measureIndex == methodMeasures.size() ? "\n" : ";" );
            measureIndex++;
		}
        lines.push_back( std::string( ss.str() ) );
        currentMethodIndex++;
	}
    ss.str( "" );
    ss << header;
    for( std::string line : lines ) {
        ss << line;
    }

    //  std::cout << ss.str();
    return ss.str();
    //Return here
}
//_______________________________________________
std::vector<int>
AFM::Comparative::generate_distances() {
	std::vector<int> distances;
	for( int distance = this->minDecode; distance <= this->maxDecode; distance += this->step ) {
		distances.push_back( distance );
    }
    return distances;
}
//_______________________________________________
void
AFM::Comparative::calculateMinMax() {
    double minValue = DBL_MAX;
    double maxValue = DBL_MIN;

    for( int z = minDecode; z <= maxDecode; z += step ) {
        cv::Mat decodedImage = HologramDecoder::decode_hologram( image, z ); //Decode the image
        cv::Mat splitted[2];
        cv::split( decodedImage, splitted );//Split the two channels of the image (real and complex)
        cv::Mat real = splitted[0].clone();//Copy the image
        double minLocal, maxLocal;//The local extremas
        cv::minMaxLoc( real, &minLocal, &maxLocal );//Get the extremas from the reconstructed hologram
        if( minLocal < minValue ) minValue = minLocal; //Update the global minimum
        if( maxLocal > maxValue ) maxValue = maxLocal; //Update the global maximum
    }//For each analised reconstruction plane
    this->minVal = minValue;
    this->maxVal = maxValue;
    std::cout << "Min = " << minVal << "\nMax = " << maxVal << std::endl;

    this->haveMinMax = true;

}
