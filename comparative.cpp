#include "autofocusmethods.h"

using namespace FM;

AFM::Comparative::Comparative( cv::Mat image, int threshold, int minDecode, int maxDecode, int step ) {
//Populating the methods vector
	for( int method = 1; method <= 10; method++ ) {
		switch( method ) {

			case 1: {//Treshold Gradient-Squared
                this->metrics.push_back( new ThreshGradient( threshold ) );
				break;
			}//1

			case 2: {//Thresholded Brenner Gradient
                this->metrics.push_back( new BrennerGradient( threshold ) );
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
                this->metrics.push_back( new StandartDeviationCorrelation() );
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
    cv::imshow( "Comparative Constructor", image );
    cv::waitKey( 2000 );
    cv::destroyWindow( "Comparative Constructor" );
	//

}//Comparative constructor
//_______________________________________________
AFM::Comparative::MetricsTable
AFM::Comparative::create_measure_table() {
	//Rows of the table-> Methods
    this->calculateMinMax();

    cv::Mat workingImage = prepare_image( this->image );
    //minmax
    //working
    std::vector<std::vector<double>> lineMethod;
    for( int plane = minDecode; plane <= maxDecode; plane += step ) {
        //for each plane
        std::vector<double> colPlaneValues;
        cv::Mat decodedImage = decode_to( workingImage, plane, false );
        for( FocusMetric * focusMetric : this->metrics ) {
            double focusValue = focusMetric->measure_focus( decodedImage );
            colPlaneValues.push_back( focusValue );
        }
        lineMethod.push_back( colPlaneValues );

    }
    return lineMethod;


}//Create Measures Table
//_______________________________________________
std::string
AFM::Comparative::operator()( bool focusPlane ) {
    return  !focusPlane ?
            generate_csv() :
            create_points_table();
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

    cv::Mat decoded = HologramDecoder::decode_hologram( input, plane ).clone();
	cv::Mat splitted[2];
	cv::split( decoded, splitted );
    cv::Mat channel = splitted[( int )complexChannel].clone();
    assert( haveMinMax );
    AFM::normalize( channel, minVal, maxVal );
    channel.convertTo( channel, CV_8UC1, 255 );
    HologramDecoder::fftshift( channel );

    cv::imshow( "Comparative decode_to", channel );
    cv::waitKey( 1 );
    return channel;
}
//_______________________________________________
std::string
AFM::Comparative::generate_csv( ) {
	std::stringstream ss;
	ss.str( "" );
	//Generate the file header


    ss << ";";
    //Gerar o cabeçalho
    for( std::string name : methodsNames ) {
        ss << name << ";";
    }
    ss << "\n";

	std::string header( ss.str() );
    ss.str( "" );
    std::vector<std::string> lines;
    std::vector<int> distances = generate_distances();
    int distanceIndex = 0;
    for( std::vector<double> column : create_measure_table() ) {
        ss << distances[distanceIndex] << ";";
        distanceIndex++;

        for( double measure : column ) {
            ss << std::setprecision( 100 ) << measure << ";";
        }
        ss << "\n";
        lines.push_back( ss.str() );
        ss.str( "" );
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
    if( !haveMinMax ) {
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
}
//_______________________________________________
std::string
AFM::Comparative::create_points_table() {

    std::stringstream ss;
    int metricNameIndex = 0;
    ss << ";Focus Plane\n" ;
    calculateMinMax();
    for( FocusMetric * metric : metrics ) {
        double measuredFocus = get_focal_plane( metric ) - 2600.;
        ss << methodsNames[metricNameIndex] << ";";
        ss << measuredFocus << "\n";
        metricNameIndex++;
    }
    return ss.str();

}

//_______________________________________________
double
AFM::Comparative::get_focal_plane( FocusMetric * metric ) {
    cv::imshow( "get_focal_plane", image );
    cv::waitKey( 1000 );
    cv::destroyWindow( "get_focal_plane" );
    return AFM::Classic( cv::Mat(), cv::Mat() ).find_focus( image, image, metric, minVal, maxVal );
}



























