#include "autofocusmethods.h"
#define debugging_enabled 1
#define DEBUG(x) do { \
        if (debugging_enabled) { std::cerr << x << std::endl; } \
    } while (0)

AFM::AFM( cv::Mat image, cv::Mat ROI, FM::FocusMetric *focusMetric ) {
    this->image = image;
    this->ROI = ROI;
    this->focusMetric = focusMetric;
}

void AFM::showHologram( cv::Mat image ) {
    cv::Mat splited[2];
    cv::split( image, splited );
    cv::Mat alce = splited[0].clone();
    cv::normalize( splited[0], alce, 0, 1, CV_MINMAX );
    HologramDecoder::fftshift( alce );
    cv::imshow( "Holgrama", alce );
    cv::waitKey( 100 );
}

cv::Mat AFM::findFocus( cv::Mat image, cv::Mat ROI, FM::FocusMetric *metric ) {
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
    for( int i = this->minDecode; i < this->maxDecode; i = i + this->step ) {
        cv::Mat decoded = decodeTo( originalROI, i ).clone();
        std::cout << ".     Decodificando em " << i << "/" << maxDecode << std::endl;
        showHologram( decoded );
        focusValues.insert( std::pair<int, double>( i, metric->measureFocus( decoded ) ) );
    }
    double maior = -DBL_MAX;
    for( std::pair<int, double> par : focusValues ) {
        maior = ( ( par.second > maior ) ? par.second : maior );
    }
    double menor = DBL_MAX;
    for( std::pair<int, double> par : focusValues ) {
        menor = ( ( par.second < menor ) ? par.second : maior );
    }
    DEBUG( "Achou ponto focal igual a " << menor );
    return decodeTo( image, menor );
}

cv::Mat AFM::decodeTo( cv::Mat image, int focalPoint ) {
    return HologramDecoder::decodeHologram( image, focalPoint );
}

