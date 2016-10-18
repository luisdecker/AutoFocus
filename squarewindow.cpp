#include "squarewindow.h"
#include "ui_squarewindow.h"
#include "imageconverter.h"
#include <opencv.hpp>

SquareWindow::SquareWindow( QWidget *parent,cv::Mat img , RectangleWatcher *parentWatcher ) :
    QMainWindow( parent ),ui( new Ui::SquareWindow ),parentWatcher( parentWatcher ) {
    ui->setupUi( this );
    this->originalImage = img.clone();
    std::cout<<"Iniciou uma Square window\nCanais = " << img.channels()<<"\nrows = " << img.rows << "\ncols" << img.cols << std::endl;
    ClickHandler *clickHandler = new ClickHandler( this,ui->imageLabel );
    this->ui->imageLabel->installEventFilter( clickHandler );
    QPixmap labelPixmap = QPixmap::fromImage( cvMatToQImage( img ) );
    this->ui->imageLabel->setPixmap( labelPixmap );
    QSizePolicy labelPolicy( QSizePolicy::Fixed,QSizePolicy::Fixed );
    this->ui->imageLabel->setSizePolicy( labelPolicy );
    this->ui->imageLabel->resize( labelPixmap.size() );
}

cv::Mat drawSquare( cv::Mat img, QPoint point1, QPoint point2 ) {
    if( !(img.data == nullptr) ) {
        std::cout << "Entrou no drowSquare" << std::endl;
        cv::Mat original = img.clone();
//        cv::imshow("Dentro Square",original);
//        cv::waitKey();
        cv::Point point1CV( point1.x(),point1.y() );
        cv::Point point2CV( point2.x(),point2.y() );
        cv::Scalar color( 0,0,255 );
        cv::Rect rect ( point1CV,point2CV );
        cv::rectangle( original, point1CV , point2CV, color );
        return original;
    }
    std::cerr <<"NULL IMAGE IN DRAW SQUARE!"<<std::endl;
    return img;
}

SquareWindow::~SquareWindow() {
    delete ui;
}
void SquareWindow::onClick( QEvent::Type event ) {
    switch ( event ) {
        case QEvent::MouseButtonPress:
            selectionStarted = true;
            break;
        case QEvent::MouseMove:
            break;
        case QEvent::MouseButtonRelease:
            if( selectionStarted ) {
                std::cout<<"=====\nVai pintar um quadrado\nCanais = " << this->originalImage.channels()<<"\nrows = "
                         << this->originalImage.rows << "\ncols" << this->originalImage.cols <<"\nFirst Click ["<< lastClick.x() <<","<< lastClick.y()
                         << "]\nLast Move ["<<lastMovement.x()<< "," <<lastMovement.y()<< "]\n====="<< std::endl;
                cv::Mat withSquare = drawSquare( this->originalImage,this->firstClick,this->lastClick );
                QImage QwithSquare = cvMatToQImage( withSquare );
                cv::Point point1CV( this->firstClick.x(),this->firstClick.y() );
                cv::Point point2CV( this->lastClick.x(),this->lastClick.y() );
                cv::Rect rect( point1CV,point2CV );
                this->lastRect = rect;
                if( this->parentWatcher != NULL ) {
                    this->parentWatcher->updateRect( rect );
                }
                ui->imageLabel->setPixmap( QPixmap::fromImage( QwithSquare ) );
            }
            this->selectionStarted=false;
            break;
        default:
            break;
    }
}
