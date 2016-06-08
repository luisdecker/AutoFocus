#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageconverter.h"
#include <iostream>
#include <QFileDialog>
#include "squarewindow.h"
#include <cv.hpp>

//=========================================================
MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow ) {
    ui->setupUi( this );
    connect( ui->actionExit,SIGNAL( triggered() ),this,SLOT( exitApp() ) );
    connect( ui->actionLoad_Image,SIGNAL( triggered() ),this,SLOT( loadImage() ) );
    ui->selectAreaButton->setEnabled( false );
}
//=========================================================
MainWindow::~MainWindow() {
    delete ui;
}
//=========================================================
void MainWindow::loadImage() {
    QString fileName;
    fileName = QFileDialog::getOpenFileName( this, tr( "Selecione a imagem" ), ".",
                                             tr( "Image Files (*.png *.jpg *.bmp *.tiff )" ) );
    if( fileName.size() > 0 ) {
        this->loadedImage = cv::imread( fileName.toStdString() ).clone();
        updateDisplayImage( loadedImage );
        ui->selectAreaButton->setEnabled( true );
    } else {
        std::cerr << "Problem loading image, call the priest!" << std::endl;
    }
}
//=========================================================
void MainWindow::updateDisplayImage( cv::Mat img ) {
    loaded = true;
    QSize size( img.cols,img.rows );
    this->ui->image->resize( size );
    QImage mat2Qim = cvMatToQImage( img );
    QPixmap qim2Pix = QPixmap::fromImage( mat2Qim );
    ui->image->setPixmap( qim2Pix );
}
//=========================================================
void MainWindow::selectArea() {
    cv::Mat original = this->loadedImage.clone();
    cv::rectangle( original,rectangle,cv::Scalar( 0,255,0 ) );
    updateDisplayImage( original );
}
//=========================================================
void MainWindow::updateRect( cv::Rect newRect ) {
    if( newRect.height > 0 && newRect.width > 0 ) {
        this->rectangle = newRect;
        ui->selectAreaButton->setEnabled( true );
    }
}
//=========================================================
bool MainWindow::loadedImageOk() {
    if( loadedImage.rows <1 ) return false;
    if( loadedImage.cols <1 ) return false;
    return true;
}

//=========================================================
void MainWindow::on_selectAreaButton_clicked() {
    cv::Mat imagem;
    cv::namedWindow( "Selecione a regiao de interesse!",CV_WINDOW_AUTOSIZE ); //Cria a janela de seleção de ROI - OCV pq peidei na farofa com o qt
    assert( loadedImageOk() );
    loadedImage.copyTo( imagem );//Copia a imagem carregada
    cv::setMouseCallback( "Selecione a regiao de interesse!",MainWindow::cvMouseHandler ,this );//Adiciona o tratador de mouse
    cv::imshow( "Selecione a regiao de interesse!",imagem );//Mostra a imagem
    cv::waitKey( 0 );//Espera o elemento apertar um botao
    std::cout << "Selecionou ROI" << std::endl;
    cv::rectangle( imagem,rect,cv::Scalar( 0,0,255 ) );
    if( pointOnImage( rect.tl() )
        &&pointOnImage( rect.br() ) ) {
        ROI = cv::Mat( loadedImage,rect );
        cv::imshow( "Roi selecionado", ROI );
    }
    cv::waitKey( 1000 );
    roiSelecionado = true;
    updateDisplayImage( imagem );
    cv::destroyAllWindows();
}
//=========================================================
void MainWindow::cvMouseHandler( int evento, int x, int y, int flags,void *parametro ) {
    MainWindow *janela = static_cast<MainWindow *> ( parametro );
    /*pressiona o botao esquerdo*/
    if( evento == CV_EVENT_LBUTTONDOWN && !janela->arrasto ) {
        std::cout << "Detectou clique" << std::endl;
        janela->ponto1 = cv::Point( x,y );
        janela->arrasto = true;
        janela->roiSelecionado = false;
    }
    /*arrasta o mouse, ROI sendo selecionado*/
    if( evento == CV_EVENT_MOUSEMOVE && janela->arrasto ) {
        std::cout << "Detectou arrasto" << std::endl;
        janela->ponto2= cv::Point( x,y );
        cv::Rect regiaoSelecionada( janela->ponto1,janela->ponto2 );
        janela->rect = regiaoSelecionada;
        cv::Mat alce = janela->loadedImage.clone();
        cv::rectangle( alce,regiaoSelecionada,cv::Scalar( 0,0,255 ) );
        cv::imshow( "Selecione a regiao de interesse!",alce );
    }
    /*Soltou o mouse*/
    if( evento == CV_EVENT_LBUTTONUP ) {
        std::cout << "Detectou clique solto" << std::endl;
        janela->roiSelecionado = true;
        janela->arrasto = false;
    }
}
//=========================================================
bool MainWindow::pointOnImage( cv::Point point ) {
    if( point.x > loadedImage.cols
        || point.x < 0
        || point.y > loadedImage.rows
        || point.y < 0 ) {
        return false;
    }
    return true;
}
