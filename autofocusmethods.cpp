#include "autofocusmethods.h"
#define debugging_enabled 1
#define DEBUG(x) do { \
        if (debugging_enabled) { std::cerr << x << std::endl; } \
    } while (0)
using AFM::Classic;

Classic::Classic( cv::Mat image, cv::Mat ROI, FM::FocusMetric *focusMetric ) {
    this->image = image;
    this->ROI = ROI;
    this->focusMetric = focusMetric;
}

cv::Mat Classic::splitHologram( cv::Mat image ) {
    cv::Mat splited[2];
    cv::split( image, splited );
    cv::Mat alce = splited[0].clone();
    cv::normalize( splited[0], alce, 0, 1, CV_MINMAX );
    HologramDecoder::fftshift( alce );
    alce.convertTo( alce, CV_8U, 255 );
    return alce.clone();
}

void Classic::showHologram( cv::Mat image , int delay ) {
    cv::Mat splited[2];
    cv::split( image, splited );
    cv::Mat alce = splited[0].clone();
    cv::normalize( splited[0], alce, 0, 1, CV_MINMAX );
    HologramDecoder::fftshift( alce );
    cv::imshow( "Holograma", alce );
    cv::waitKey( delay );
}

cv::Mat Classic::findFocus( cv::Mat image, cv::Mat ROI, FM::FocusMetric *metric ) {
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
        /*split the channels, and convert it to uchar*/
        cv::Mat splitted[2];
        cv::split( decoded, splitted );
        cv::Mat real = splitted[0].clone();
        cv::normalize( splitted[0], real, 0, 1, CV_MINMAX );
        real.convertTo( real, CV_8U, 255 );

        std::cout << ".     Decodificando em " << i << "/" << maxDecode << std::endl;
        showHologram( decoded );
        focusValues.insert( std::pair<int, double>( i, metric->measureFocus( real ) ) );
    }
    double maior = 0;
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
    showHologram( decodeTo( image, parMenor.first ), 1000 );
    cv::imwrite( "decoded.bmp", splitHologram( decodeTo( ROI, parMenor.first ) ) );
    cv::imwrite( "coded.bmp", ROI );

    return splitHologram(decodeTo( image, parMenor.first ));
}

cv::Mat Classic::decodeTo( cv::Mat image, int focalPoint ) {
    return HologramDecoder::decodeHologram( image, focalPoint );
}

