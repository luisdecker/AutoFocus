
#ifndef COMPLEXMATRIX_H
#define	COMPLEXMATRIX_H

#include <complex>
#include <assert.h>
#include <opencv2/core/core.hpp>

#include <iostream>


class ComplexMatrix {
public:
    /*Constroi um objeto vazio.*/
    ComplexMatrix( int linhas, int colunas );

    /*Constroi um objeto a partir de uma cv::Mat
     @param base A cv::Mat que será transformada para uma ComplexMatrix. Deve ter dois canais.*/
    ComplexMatrix( cv::Mat base );

    /*Modifica o elemento na posicao fornecida*/
    void modify( int linha, int coluna, std::complex<float> data );

    /*Transforma uma cv::Mat para uma ComplexMatrix
     @param base A cv::Mat que sera transformada. Deve ter dois canais.*/
    static inline ComplexMatrix* cvMat2compMat( cv::Mat base );

    /*Transforma uma ComplexMatrix para uma cv::Mat de 2 canais.
     @param base A ComplexMatrix que será transformada em cv::Mat.*/
    static inline cv::Mat compMat2cvMat( ComplexMatrix* entrada );

    /*Calcula o exp elemento-a-elemento na matriz fornecida. Recebe e retorna ComplexMatrix*/
    static ComplexMatrix exp( ComplexMatrix entrada );

    /*Multiplica um número complexo por uma matrix real*/
    static cv::Mat mulComplexRealMat( cv::Mat entrada, std::complex<float> constante );
    /*Calcula o exp elemento-a-elemento na matriz fornecida. Recebe e retorna cvMat*/
    static cv::Mat exp( cv::Mat entrada );

    /*Retorna a sua versão em cv::Mat.*/
    cv::Mat toMat();

    /*Retorna o dado na posição (linha,coluna).*/
    std::complex<float> at( int linha, int coluna );

    /*Multiplica a cvMat pro um complexo, elemento a elemento.*/
    static cv::Mat mulComplexMat( cv::Mat entrada, const std::complex<float>constante );

    /*Multiplica dois complexos*/
    static std::complex<float> mulComplex( std::complex<float> left, std::complex<float> right );

    /*Multiplica elemento a elemento as duas matrizes*/
    static cv::Mat perElementMult( cv::Mat esquerda, cv::Mat direita );
private:

    /*Numero de linhas da matriz*/
    int linhas;

    /*Numero de colunas da matriz*/
    int colunas;

    /*As informações da matriz*/
    std::complex<float>** data;
};

#endif	/* COMPLEXMATRIX_H */
