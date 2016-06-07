#ifndef IMAGECONVERTER
#define IMAGECONVERTER
#include "opencv.hpp"
#include "QImage"
#include <iostream>
inline cv::Mat QImageTocvMat(QImage image) {
    cv::Mat alce(image.height(), image.width(), CV_8U, (uchar *) image.bits(), image.bytesPerLine());
    cv::Mat result;
    std::cout << "Convertendo de QImage para cvMat. Canais = " << alce.channels()<< std::endl;
    cv::imshow("Dentro Converter",alce);
    cv::waitKey();
    cv::cvtColor(alce, result, CV_BGR2RGB);
    return result;
}

inline QImage cvMatToQImage(cv::Mat image) {
    cv::Mat alce;
    cv::cvtColor(image, alce, CV_BGR2RGB);
    QImage result((const uchar *) alce.data, alce.cols, alce.rows, alce.step, QImage::Format_RGB888);
    result.bits(); //força cópia
    return result;
}


#endif // IMAGECONVERTER

