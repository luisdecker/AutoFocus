#ifndef HOLOGRAMDECODER_H
#define HOLOGRAMDECODER_H
#include <opencv.hpp>
#include <math.h>
#include "ComplexMatrix.h"

class HologramDecoder {
public:
    HologramDecoder();
   static  cv::Mat decode_hologram( cv::Mat hologram,float z, float lambda=385e-9,float dx = 1.67e-6 ,  float dy = 1.67e-6 );
   static cv::Mat fftshift ( cv::Mat input );

private:
    /*
    Transformada de Fresnel (Convolucao)
    Parâmetros
    dx,dy - > pixel pitch do objeto (Distancia entre dois pixeis em x e y)
    z - > distancia para a transformacao
    lambda - > comprimento de onda
    */
    static cv::Mat fresnel ( cv::Mat hologram, float dx, float dy, float z, float lambda );
    static cv::Mat optimizeToFresnel( cv::Mat hologram );
    static cv::Mat squareMatrix ( cv::Mat matrix );

};

#endif // HOLOGRAMDECODER_H
