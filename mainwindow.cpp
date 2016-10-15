#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageconverter.h"
#include <iostream>
#include <QFileDialog>
#include "squarewindow.h"
#include <cv.hpp>
#include <iostream>

using AFM::Classic;

//_________________________________________________________
MainWindow::MainWindow( QWidget * parent ) :
	QMainWindow( parent ),
	ui( new Ui::MainWindow ) {
	ui->setupUi( this );
	connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( exitApp() ) );
	connect( ui->actionLoad_Image, SIGNAL( triggered() ), this, SLOT( loadImage() ) );
	ui->selectAreaButton->setEnabled( false );
	ui->decodeButton->setEnabled( false );
}
//_________________________________________________________
MainWindow::~MainWindow() {
	delete ui;
}
//_________________________________________________________
void MainWindow::loadImage() {
	QString fileName;
	fileName = QFileDialog::getOpenFileName( this, tr( "Selecione a imagem" ), ".",
                                             tr( "Image Files (*.png *.jpg *.bmp *.tiff *.tif)" ) );
	if( fileName.size() > 0 ) {
		this->loadedImage = cv::imread( fileName.toStdString() ).clone();
		updateDisplayImage( loadedImage );
		ui->selectAreaButton->setEnabled( true );
	} else {
		std::cerr << "Problem loading image, call the priest!" << std::endl;
	}
}
//_________________________________________________________
void MainWindow::updateDisplayImage( cv::Mat img ) {
	loaded = true;
	QSize size( img.cols, img.rows );
	this->ui->image->resize( size );


    QImage mat2Qim = cvMatToQImage( img );
	QPixmap qim2Pix = QPixmap::fromImage( mat2Qim );
	ui->image->setPixmap( qim2Pix );
}
//_________________________________________________________
void MainWindow::selectArea() {
	cv::Mat original = this->loadedImage.clone();
	cv::rectangle( original, rectangle, cv::Scalar( 0, 255, 0 ) );
	updateDisplayImage( original );
}
//_________________________________________________________
void MainWindow::updateRect( cv::Rect newRect ) {
	if( newRect.height > 0 && newRect.width > 0 ) {
		this->rectangle = newRect;
		ui->selectAreaButton->setEnabled( true );
	}
}
//_________________________________________________________
bool MainWindow::loadedImageOk() {
	if( loadedImage.rows < 1 ) return false;
	if( loadedImage.cols < 1 ) return false;
	return true;
}
//_________________________________________________________
cv::Point2d MainWindow::getROICenter( cv::Rect rectangle ) {
    int x = rectangle.tl().x / 2;
    int y = rectangle.br().y / 2;
    return cv::Point2d( x, y );

}

//_________________________________________________________
cv::Mat MainWindow::generate50ROI() {
    int xmin, xmax;
    int ymin, ymax;

    xmax = ROICenter.x + 25;
    ymax = ROICenter.y + 25;

    xmin = ROICenter.x - 25;
    ymin = ROICenter.y - 25;

    cv::Point br( xmin, ymax );
    cv::Point tl( xmax, ymin );

    return cv::Mat( this->loadedImage, cv::Rect( br, tl ) );
}
//_________________________________________________________
cv::Mat MainWindow::generate100ROI() {
    int xmin, xmax;
    int ymin, ymax;

    xmax = ROICenter.x + 50;
    ymax = ROICenter.y + 50;

    xmin = ROICenter.x - 50;
    ymin = ROICenter.y - 50;

    cv::Point br( xmin, ymax );
    cv::Point tl( xmax, ymin );

    return cv::Mat( this->loadedImage, cv::Rect( br, tl ) );
}
//_________________________________________________________
cv::Mat MainWindow::generate200ROI() {
    int xmin, xmax;
    int ymin, ymax;

    xmax = ROICenter.x + 100;
    ymax = ROICenter.y + 100;

    xmin = ROICenter.x - 100;
    ymin = ROICenter.y - 100;

    cv::Point br( xmin, ymax );
    cv::Point tl( xmax, ymin );

    return cv::Mat( this->loadedImage, cv::Rect( br, tl ) );
}
//_________________________________________________________
void MainWindow::on_selectAreaButton_clicked() {
    this->setHidden( 1 );
	cv::Mat imagem;
	cv::namedWindow( "Selecione a regiao de interesse!", CV_WINDOW_AUTOSIZE ); //Cria a janela de seleção de ROI - OCV pq peidei na farofa com o qt
	assert( loadedImageOk() );
	loadedImage.copyTo( imagem );//Copia a imagem carregada
	cv::setMouseCallback( "Selecione a regiao de interesse!", MainWindow::cvMouseHandler , this ); //Adiciona o tratador de mouse
	cv::imshow( "Selecione a regiao de interesse!", imagem ); //Mostra a imagem
	cv::waitKey( 0 );//Espera o elemento apertar um botao
	std::cout << "Selecionou ROI" << std::endl;
	cv::rectangle( imagem, rect, cv::Scalar( 0, 0, 255 ) );
	if( pointOnImage( rect.tl() )
		&& pointOnImage( rect.br() ) ) {

        this->ROICenter = getROICenter( rect );

		ROI = cv::Mat( loadedImage, rect );
		cv::imshow( "Roi selecionado", ROI );
	}
	cv::waitKey( 1000 );
	roiSelecionado = true;
	updateDisplayImage( imagem );
	ui->decodeButton->setEnabled( true );
	cv::destroyAllWindows();
    this->setHidden( 0 );
}
//_________________________________________________________
void MainWindow::cvMouseHandler( int evento, int x, int y, int flags, void * parametro ) {
    MainWindow * janela = static_cast<MainWindow *>( parametro );
	/*pressiona o botao esquerdo*/
	if( evento == CV_EVENT_LBUTTONDOWN && !janela->arrasto ) {
        //	std::cout << "Detectou clique" << std::endl;
		janela->ponto1 = cv::Point( x, y );
		janela->arrasto = true;
		janela->roiSelecionado = false;
	}
	/*arrasta o mouse, ROI sendo selecionado*/
	if( evento == CV_EVENT_MOUSEMOVE && janela->arrasto ) {
        //std::cout << "Detectou arrasto" << std::endl;
		janela->ponto2 = cv::Point( x, y );
		cv::Rect regiaoSelecionada( janela->ponto1, janela->ponto2 );
		janela->rect = regiaoSelecionada;
		cv::Mat alce = janela->loadedImage.clone();
		cv::rectangle( alce, regiaoSelecionada, cv::Scalar( 0, 0, 255 ) );
		cv::imshow( "Selecione a regiao de interesse!", alce );
	}
	/*Soltou o mouse*/
	if( evento == CV_EVENT_LBUTTONUP ) {
        //  std::cout << "Detectou clique solto" << std::endl;
		janela->roiSelecionado = true;
		janela->arrasto = false;
	}
}
//_________________________________________________________
bool MainWindow::pointOnImage( cv::Point point ) {
	if( point.x > loadedImage.cols
		|| point.x < 0
		|| point.y > loadedImage.rows
        || point.y < 0 ) {
		return false;
	}
	return true;
}
//_________________________________________________________
void MainWindow::on_decodeButton_clicked() {

    bool comparative = false;
    bool autoROI = false;

    if( comparative ) {
        if( ! autoROI ) {
            AFM::Comparative comparative( this->ROI, 150 );
            std::ofstream file( "comparativeReports.csv" );
            file << comparative();
            file.close();
            std::cout << "Savou!\n==========" << std::endl;
        } else {
            //For a 50x50 ROI
            cv::Mat roi50 = generate50ROI();
            AFM::Comparative comparative50( roi50, 100 );





        }


    } else {
        //FM::FocusMetric * metric = new FM::BrennerGradient( 50 );//OK
        FM::FocusMetric * metric = new FM::ImagePower( 50 ); //Ok
        metric = new FM::LaplacianEnergy();//OK
        // metric = new FM::TenenbaumGradient();//
        //metric = new FM::ThresholdedHistogram( 100 );
        metric = new FM::ThreshGradient(100);
        Classic autofocus( this->loadedImage, this->ROI, metric );
        cv::Mat bestFocus = autofocus().clone();
        std::cout << "Achou o foco [MAIN WINDOW]\n";
        updateDisplayImage( bestFocus );
    }


}
