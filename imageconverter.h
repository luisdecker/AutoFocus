#ifndef IMAGECONVERTER
#define IMAGECONVERTER
#include "opencv.hpp"
#include "QImage"
#include <iostream>
#include "hologramdecoder.h"
inline cv::Mat QImageTocvMat( QImage image ) {
    cv::Mat alce( image.height(), image.width(), CV_8U, ( uchar * ) image.bits(), image.bytesPerLine() );
    cv::Mat result;
    std::cout << "Convertendo de QImage para cvMat. Canais = " << alce.channels() << std::endl;
    cv::imshow( "Dentro Converter", alce );
    cv::waitKey();
    cv::cvtColor( alce, result, CV_BGR2RGB );
    return result;
}

inline QImage cvMatToQImage( cv::Mat image ) {
    //assert( !image.empty()  && image.channels() == 3 );
    std::cout << "Convertendo MAT para QImage!\n"
              << "Canais: " << image.channels()
              << std::endl;
    cv::Mat alce;
    if( image.channels() == 2 ) { //Imagem complexa
        std::vector<cv::Mat> channels( 2 );
        cv::split( image, channels );
        cv::Mat imageReal = channels[0].clone();
        cv::normalize( channels[1], imageReal, 0, 1, CV_MINMAX );
        HologramDecoder::fftshift( imageReal );
        imageReal.convertTo( imageReal, CV_8UC3 );
        cv::cvtColor( imageReal, image, CV_GRAY2BGR );
    }
    cv::cvtColor( image, alce, CV_BGR2RGB );

    QImage result( ( const uchar * ) alce.data, alce.cols, alce.rows, alce.step, QImage::Format_RGB888 );
    result.bits(); //força cópia
    return result;
}


#endif // IMAGECONVERTER

