
#include "ComplexMatrix.h"

std::complex<float> ComplexMatrix::at( int linha, int coluna ) {
    return this->data[linha][coluna];

}

ComplexMatrix::ComplexMatrix( int linhas, int colunas ) {
    this->colunas = colunas;
    this->linhas = linhas;
    this->data = new std::complex<float>*[linhas];
    for( int lin = 0; lin < linhas; lin++ ) {
        this->data[lin] = new std::complex<float>[colunas];
    }
}

ComplexMatrix::ComplexMatrix( cv::Mat base ) {
    ComplexMatrix * transform = cvMat2compMat( base );
    this->colunas = transform->colunas;
    this->linhas = transform->linhas;
    this->data = transform->data;
}

cv::Mat ComplexMatrix::compMat2cvMat( ComplexMatrix* entrada ) {
    assert( entrada != NULL );
    cv::Mat saida = cv::Mat( entrada->linhas, entrada->colunas, CV_32FC2 );
    #pragma omp paralel for

    for( int lin = 0; lin < saida.rows; lin++ ) {
        #pragma omp paralel for

        for( int col = 0; col < saida.cols; col++ ) {
            cv::Vec2f elemento;
            std::complex<float> elementoEntrada = entrada->at( lin, col );
            // std::cout << "Elemento Entrada -> " << elementoEntrada << "\n";
            elemento[0] = elementoEntrada.real();
            elemento[1] = elementoEntrada.imag();
            saida.at<cv::Vec2f> ( lin, col ) = elemento;
        }//fim coluna
    }//fim linha
    return saida;
}

ComplexMatrix* ComplexMatrix::cvMat2compMat( cv::Mat base ) {
    //  std::cout << "  mat2comp\n  channels: " << base.channels() << std::endl;
    assert( base.channels() == 2 );
    ComplexMatrix * novaMatrizComplex = new ComplexMatrix( base.rows, base.cols );
    std::complex<float>** matData = novaMatrizComplex->data;
    #pragma omp paralel for

    for( int row = 0; row < base.rows; row++ ) {
        #pragma omp paralel for

        for( int col = 0; col < base.cols; col++ ) {
            cv::Vec2f elemento = base.at<cv::Vec2f> ( row, col );
            matData[row][col].real( elemento[0] );
            matData[row][col].imag( elemento[1] );
        }
    }
    return novaMatrizComplex;
}

cv::Mat ComplexMatrix::exp( cv::Mat entrada ) {
    ComplexMatrix* convertida = cvMat2compMat( entrada );
    #pragma omp paralel for

    for( int lin = 0; lin < convertida->linhas; lin++ ) {
        #pragma omp paralel for

        for( int col = 0; col < convertida->colunas; col++ ) {
            std::complex<float> alce = std::exp<float> ( convertida->at( lin, col ) );
            entrada.at<cv::Vec2f> ( lin, col ) [0] = alce.real();
            entrada.at<cv::Vec2f> ( lin, col ) [1] = alce.imag();
        }
    }
    return entrada;
}

cv::Mat ComplexMatrix::mulComplexMat( cv::Mat entrada, const std::complex<float> constante ) {
    ComplexMatrix *convertida = cvMat2compMat( entrada );
    ComplexMatrix* alce = new ComplexMatrix( convertida->linhas, convertida->colunas );

    #pragma omp paralel for
    for( int lin = 0; lin < alce->linhas; lin++ ) {
        #pragma omp paralel for
        for( int col = 0; col < alce->colunas; col++ ) {
            std::complex<float> elemento = convertida->at( lin, col );
            std::complex<float> resultado = mulComplex( constante, elemento );
            alce->modify( lin, col, resultado );
            // std::cout << " Lendo de volta -> " << alce->at(lin, col);

        }// fim coluna
    }// fim linha
    cv::Mat saida = compMat2cvMat( alce );


    return saida;
}

std::complex<float> ComplexMatrix::mulComplex( std::complex<float> left, std::complex<float> right ) {
    //(a+bi) * (c + di)

    float a = left.real();
    float b = left.imag();
    float c = right.real();
    float d = right.imag();


    std::complex<float> result;

    result.real( ( a * c ) - ( b * d ) );
    result.imag( ( b * c ) + ( a * d ) );
    return result;

}

void ComplexMatrix::modify( int linha, int coluna, std::complex<float> data ) {
    if( this->data != NULL ) {
        this->data[linha][coluna] = data;

    }
}

cv::Mat ComplexMatrix::mulComplexRealMat( cv::Mat entrada, std::complex<float> constante ) {

    assert( entrada.channels() == 1 );
    cv::Mat saida = cv::Mat( entrada.rows, entrada.cols, CV_32FC2 );
    for( int lin = 0; lin < entrada.rows; lin++ ) {
        for( int col = 0; col < entrada.cols; col++ ) {
            float elementoEntrada = entrada.at<float> ( lin, col );
            cv::Vec2f elemento = saida.at<cv::Vec2f> ( lin, col );
            elemento[0] = elementoEntrada * constante.real();
            elemento[1] = elementoEntrada * constante.imag();
            saida.at<cv::Vec2f> ( lin, col ) = elemento;

        }
    }
    return saida;
}

cv::Mat ComplexMatrix::perElementMult( cv::Mat esquerda, cv::Mat direita ) {
    assert( esquerda.rows == direita.rows );
    assert( esquerda.cols == direita.cols );
    assert( esquerda.channels() == direita.channels() );
    ComplexMatrix* saida = new ComplexMatrix( esquerda.rows, esquerda.cols );
    ComplexMatrix * esq = cvMat2compMat( esquerda );
    ComplexMatrix * dir = cvMat2compMat( direita );
    for( int lin = 0; lin < esq->linhas; lin++ ) {
        for( int col = 0; col < esq->colunas; col++ ) {
            std::complex<float> result = mulComplex( esq->at( lin, col ), dir->at( lin, col ) );
            saida->modify( lin, col, result );
        }
    }
    cv::Mat alce = compMat2cvMat( saida );
    return alce;

}

ComplexMatrix::~ComplexMatrix(){

}





