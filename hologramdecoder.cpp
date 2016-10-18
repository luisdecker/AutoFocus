#include "hologramdecoder.h"
//#define PRINT_FRESNELL
#define debugging_enabled 0
#define DEBUG(x) do { \
        if (debugging_enabled) { std::cerr << x << std::endl; } \
    } while (0)

HologramDecoder::HologramDecoder() {
}

cv::Mat HologramDecoder::decode_hologram( cv::Mat hologram, float z, float lambda, float dx, float dy ) {
    return fresnel( hologram, dx, dy, lambda * z, lambda );
}

cv::Mat HologramDecoder::fresnel( cv::Mat hologramColor, float dx, float dy, float z, float lambda ) {

#ifdef PRINT_FRESNELL
    std::cout << "      **INFORMACOES FRESNEL**\n"
              << "      dx: " << dx << std::endl
              << "      dy: " << dy << std::endl
              << "      z: " << z << std::endl
              << "      lamda " << lambda << std::endl;
#endif

    cv::Mat hologram;



    cv::cvtColor( hologramColor, hologram, CV_BGR2GRAY );
    hologram.convertTo( hologram, CV_32FC1, 1.0 / 255.0 );
    hologram = HologramDecoder::optimizeToFresnel( hologram );
    float k = ( ( 2 * M_PI ) );
    float Nxx = hologram.cols;
    float Nyy = hologram.rows;
    //Cria duas matrizes linha com o tamanho em x e y da imagem
    //x = ones(Nyy,1)*[-Nxx/2:Nxx/2-1]*dx; [MATLAB]
    cv::Mat part1x = cv::Mat::ones( hologram.rows, 1, CV_32FC1 );  //ones(Nyy,1)
    cv::Mat part2x = cv::Mat( 1, hologram.cols, CV_32FC1 );  //[-Nxx/2 : Nxx/2-1]
    //
    float *aux = new float[hologram.cols];
    int index = 0;
    /*Cria um vetor gradiente com o tamanho em x da imagem*/
    for( int i = ( -1 * ( hologram.cols / 2 ) ); i < ( hologram.cols / 2 ); i++ ) {
        aux[index] = ( float ) i;
        index++;
    }
    part2x = cv::Mat( part2x.rows, part2x.cols, CV_32FC1, aux ); //O vetor gradiente
    //
    cv::Mat x = part1x * part2x * dx;
    x.convertTo( x, CV_32FC1 ); // X pronto
    //
    //
    //Reinicia as variaveis auxiliares
    aux = new float[hologram.cols];
    index = 0;
    //Cria outro gradiente, com tamanho em y
    for( int i = ( -1 * ( hologram.rows ) / 2 ); i < ( hologram.rows ) / 2; i++ ) {
        aux[index] = ( float ) i;
        index++;
    }
    cv::Mat part1y = cv::Mat( 1, hologram.rows, CV_32FC1, aux );
    cv::Mat part2y = cv::Mat::ones( 1, hologram.cols, CV_32F );
    cv::Mat part1ytanspost = cv::Mat( part1y.rows, part1y.cols, part1y.type() );
    cv::transpose( part1y, part1ytanspost );
    cv::Mat y = part1ytanspost * part2y * dy;
    y.convertTo( y, CV_32FC1 ); // Y pronto
    //
    //
    float Lx = dx * Nxx;
    float Ly = dy * Nyy;
    float criti = dx * Lx / lambda;
    float dfx = 1 / Lx;
    float dfy = 1 / Ly;
    //
    //u = ones(Nyy,1)*[-Nxx/2:Nxx/2-1]*dfx;
    //v = [-Nyy/2:Nyy/2-1]'*ones(1,Nxx)*dfy;l
    cv::Mat part1u = cv::Mat::ones( hologram.rows, 1, CV_32F );
    cv::Mat part2u = cv::Mat( 1, hologram.cols, CV_32F );
    aux = new float[hologram.cols];
    index = 0;
    for( int i = ( -1 * ( hologram.cols / 2 ) ); i < ( hologram.cols / 2 ); i++ ) {
        aux[index] = ( float ) i;
        index++;
    }
    part2u = cv::Mat( part2x.rows, part2x.cols, CV_32F, aux );
    cv::Mat u = part1u * part2u * dfx; //U pronto
    //
    delete aux;
    aux = new float[hologram.rows];
    index = 0;
    for( int i = ( -1 * ( hologram.rows ) / 2 ); i < ( hologram.rows ) / 2; i++ ) {
        aux[index] = ( float ) i;
        index++;
    }
    cv::Mat part1v = cv::Mat( 1, hologram.rows, CV_32FC1, aux );
    delete aux;
    cv::Mat part2v = cv::Mat::ones( 1, hologram.cols, CV_32F );
    cv::Mat part1vtanspost = cv::Mat( part1y.rows, part1y.cols, part1v.type() );
    cv::transpose( part1v, part1vtanspost );
    cv::Mat v = part1vtanspost * part2v * dfy;//V pronto
    //
    //
    cv::Mat zero = cv::Mat( hologram.cols, hologram.rows, CV_32FC1 );
    cv::Mat shifted = hologram.clone();
    fftshift( shifted );
    cv::Mat real = cv::Mat( hologram.rows, hologram.cols, CV_32FC1 ), imaginary = cv::Mat( hologram.rows, hologram.cols, CV_32FC1 );
    cv::Mat mats[] = {real, imaginary};
    cv::Mat result;
    cv::merge( mats, 2, result );
    cv::dft( shifted, result, cv::DFT_COMPLEX_OUTPUT );
    zero = result;
    cv::Mat partialFinalResult;

//   if( z > criti ) {
    if( false ) {
        std::cout << " ENTROU NO IF" << std::endl;
        std::complex<float> ifPart1;
        ifPart1.imag( lambda * z );
        cv::Mat sumMats;
        cv::Mat xSquare = squareMatrix( x );
        cv::Mat ySquare = squareMatrix( y );
        sumMats = xSquare + ySquare;

        //	std::cout << "  IF\n  sumMats.channels: " << sumMats.channels() << std::endl;

        std::complex<float> ifPart2_1;
        ifPart2_1.imag( k / 2 * z );

        //	std::cout << " IF[1]" << std::endl;

        cv::Mat hpart2 = ComplexMatrix::mulComplexMat( sumMats, ifPart2_1 );

        //	std::cout << "IF[2]" << std::endl;

        cv::Mat expPart2 = ComplexMatrix::exp( hpart2 );

        //	std::cout << "IF[3]" << std::endl;

        cv::Mat h = ComplexMatrix::mulComplexMat( expPart2, ifPart1 );
        cv::Mat ifShifted = h.clone();
        fftshift( ifShifted );
        cv::Mat outputReal = cv::Mat( ifShifted.rows, ifShifted.cols, CV_32FC1 );
        cv::Mat outputImaginary = cv::Mat( ifShifted.rows, ifShifted.cols, CV_32FC1 );
        cv::Mat transformed;
        cv::Mat matrixes[] = {outputReal, outputImaginary};
        cv::merge( matrixes, 2, transformed );
        cv::dft( ifShifted, transformed, cv::DFT_COMPLEX_OUTPUT );
        std::complex<float> complexAux( dx, 0 );
        complexAux.real( pow( complexAux.real(), 2 ) );
        complexAux.imag( pow( complexAux.imag(), 2 ) );
        partialFinalResult = ComplexMatrix::mulComplexMat( transformed, complexAux );
    } else {
        //std::cout << " ENTROU NO ELSE" << std::endl;
        std::complex<float> part0else( 0, k * z );
        std::complex<float>part1else = std::exp<float> ( part0else );
        std::complex<float> part2else( 0, - ( M_PI * lambda * z ) );
        cv::Mat uSquared = squareMatrix( u );
        cv::Mat vSquared = squareMatrix( v );
        cv::Mat sumMats = vSquared + uSquared;
        cv::Mat part3else = ComplexMatrix::mulComplexRealMat( sumMats, part2else );
        cv::Mat expPart3 = ComplexMatrix::exp( part3else );
        cv::Mat semiFinal = ComplexMatrix::mulComplexMat( expPart3, part1else );
        cv::Mat final = fftshift( semiFinal );
        partialFinalResult = final;
    }
    cv::Mat object = ComplexMatrix::perElementMult( zero, partialFinalResult );
    cv::Mat outputReal = cv::Mat::zeros( object.rows, object.cols, CV_32FC1 );
    cv::Mat outputImaginary = cv::Mat::zeros( object.rows, object.cols, CV_32FC1 );
    cv::Mat outputs[] = {outputReal, outputImaginary};
    cv::Mat output2;
    cv::merge( outputs, 2, output2 );
    cv::dft( object, output2, cv::DFT_INVERSE );
    return cv::abs( output2 );
}

cv::Mat HologramDecoder::optimizeToFresnel( cv::Mat hologram ) {
    hologram = ( hologram.cols % 2 != 0 ? cv::Mat( hologram, cv::Rect( 0, 0, hologram.cols - 1, hologram.rows ) ) : hologram );
    hologram = ( hologram.rows % 2 != 0 ? cv::Mat( hologram, cv::Rect( 0, 0, hologram.cols, hologram.rows - 1 ) ) : hologram );
    return hologram;
}

cv::Mat HologramDecoder::fftshift( cv::Mat input ) {
    int middleX, middleY;
    middleX = ( input.cols / 2 );
    middleY = ( input.rows / 2 );
    cv::Mat quadrante0( input, cv::Rect( 0, 0, middleX, middleY ) );
    cv::Mat quadrante1( input, cv::Rect( middleX, 0, middleX, middleY ) );
    cv::Mat quadrante2( input, cv::Rect( 0, middleY, middleX, middleY ) );
    cv::Mat quadrante3( input, cv::Rect( middleX, middleY, middleX, middleY ) );
    cv::Mat aux;
    //Troca quadrante topo-esquerda com baixo-direita
    quadrante0.copyTo( aux );
    quadrante3.copyTo( quadrante0 );
    aux.copyTo( quadrante3 );
    //Troca quadrantes topo-direita com baixo-esquerda
    quadrante1.copyTo( aux );
    quadrante2.copyTo( quadrante1 );
    aux.copyTo( quadrante2 );
    return input;
}

cv::Mat HologramDecoder::squareMatrix( cv::Mat matrix ) {
    cv::Mat result = cv::Mat( matrix.rows, matrix.cols, CV_32F );
    for( int row = 0; row < matrix.rows; row++ ) {
        for( int col = 0; col < matrix.cols; col++ ) {
            float element = matrix.at<float> ( row, col );
            float elementResult = element * element;
            result.at<float> ( row, col ) = elementResult;
        }
    }
    return result;
}
