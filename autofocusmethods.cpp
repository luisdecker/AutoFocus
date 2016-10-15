#include "autofocusmethods.h"
#define debugging_enabled 1
#define DEBUG(x) do { \
        if (debugging_enabled) { std::cerr << x << std::endl; } \
    } while (0)
using AFM::Classic;
//_______________________________________________

Classic::Classic( cv::Mat image, cv::Mat ROI, FM::FocusMetric * focusMetric ) {
    this->image = image;
    this->ROI = ROI;
    this->focusMetric = focusMetric;
}
//_______________________________________________

cv::Mat
Classic::split_hologram( cv::Mat image ) {
    cv::Mat splited[2];
    cv::split( image, splited );
    cv::Mat alce = splited[0].clone();
    cv::normalize( splited[0], alce, 0, 1, CV_MINMAX );
    HologramDecoder::fftshift( alce );
    alce.convertTo( alce, CV_8U, 255 );
    return alce.clone();
}
//_______________________________________________

void
Classic::show_hologram( cv::Mat image , int delay ) {
    cv::Mat splited[2];
    cv::split( image, splited );
    cv::Mat alce = splited[0].clone();
    cv::normalize( splited[0], alce, 0, 1, CV_MINMAX );
    HologramDecoder::fftshift( alce );
    cv::imshow( "Holograma", alce );
    cv::waitKey( delay );
}
//_______________________________________________
std::pair<double, double> Classic::findMinMax( cv::Mat image ) {
    double minValue = DBL_MAX;
    double maxValue = DBL_MIN;

    for( int z = minDecode; z <= maxDecode; z += step ) {
        cv::Mat decodedImage = decode_to( image, z ); //Decode the image
        cv::Mat splitted[2];
        cv::split( decodedImage, splitted );//Split the two channels of the image (real and complex)
        cv::Mat real = splitted[0].clone();//Copy the image
        double minLocal, maxLocal;//The local extremas
        cv::minMaxLoc( real, &minLocal, &maxLocal );//Get the extremas from the reconstructed hologram
        if( minLocal < minValue ) minValue = minLocal; //Update the global minimum
        if( maxLocal > maxValue ) maxValue = maxLocal; //Update the global maximum
    }//For each analised reconstruction plane
    std::pair<double, double> minMax( minValue, maxValue );
    return minMax;

}

//_______________________________________________
double
Classic::find_focus( cv::Mat image, cv::Mat ROI, FM::FocusMetric * metric ) {
    std::map<int, double> focusValues; // map with key = focus plane, value = focus Value
    if( metric == nullptr ) {
        metric = focusMetric;//Use standart metric
    }
    if( ROI.empty() || !( ROI.rows > 0 ) || !( ROI.cols > 0 ) ) {
        ROI = this->ROI;//Use constuctor ROI
    }
    if( image.empty() || !( image.rows > 0 ) || !( image.cols > 0 ) ) {
        image = this->image;//Use constuctor image
    }

    /*
    Some assertions, to grant integrity of parameters.
    */
    assert( metric != nullptr );
    assert( !ROI.empty() && ROI.cols > 0 && ROI.rows > 0 );
    assert( !image.empty() && image.cols > 0 && image.rows > 0 );

    cv::Mat originalImage = image.clone(), originalROI = ROI.clone();
    double minVal, maxVal;
    std::pair <double, double> minMaxVals = findMinMax( originalROI );
    minVal = minMaxVals.first;
    maxVal = minMaxVals.second;


    for( int i = this->minDecode; i < this->maxDecode; i = i + this->step ) {
        cv::Mat decoded = decode_to( originalROI, i ).clone();
        /*split the channels, and convert it to uchar*/
        cv::Mat splitted[2];
        cv::split( decoded, splitted );
        cv::Mat real = splitted[0].clone();
        std::cout << "==> Pixel 10,10 da imagem crua = " << real.at<double>( 10, 10 ) << std::endl;
        AFM::normalize( real, minVal, maxVal );
        real.convertTo( real, CV_8UC1, 255 );

        std::cout << ".     Decodificando em " << i << "/" << maxDecode << std::endl;
        //show_hologram( real, 100 );
        HologramDecoder::fftshift( real );
        cv::imshow( "Measured", real );
        cv::waitKey();
        if( i == 4000 ) {
            cv::imwrite( "CODIFICADA.png", real );
        }



        focusValues.insert( std::pair<int, double>( i, metric->measure_focus( real ) ) );
    }
    double maior = DBL_MIN;
    std::pair<int, double> parMaior;
    for( std::pair<int, double> par : focusValues ) {
        if( par.second > maior ) {
            maior = par.second;
            parMaior = par;
        }

    }
    double menor = DBL_MAX;
    std::pair<int, double> parMenor;
    for( std::pair<int, double> par : focusValues ) {
        if( par.second < menor && par.second != 0 ) {
            menor = par.second;
            parMenor = par;
        }

    }
    DEBUG( "Achou ponto focal igual a " << parMenor.first );
    DEBUG( "Menor " << parMenor.first << " | " << menor );
    DEBUG( "Maior " << parMaior.first << " | " << maior );

    //  showHologram( decodeTo( image, parMaior.first ), 1000 );
//	showHologram( decodeTo( image, parMenor.first ), 1000 );
    cv::imwrite( "decoded.bmp", split_hologram( decode_to( ROI, parMenor.first ) ) );
    cv::imwrite( "coded.bmp", ROI );


    cv::destroyAllWindows();
    //Here, returns the maximum/minimum, according to the method minMax
    switch( focusMetric->minMaxType ) {
        case FM::FocusMetric::MAXIMIZATION: {
            return parMaior.first;
            break;
        }
        case FM::FocusMetric::MINIMIZATION: {
            return parMenor.first;
            break;
        }
    }
    return  -1;
}
//_______________________________________________

cv::Mat Classic::decode_to( cv::Mat image, int focalPoint ) {
    return HologramDecoder::decode_hologram( image, focalPoint );
}

