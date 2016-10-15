#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include "rectanglewatcher.h"
#include "focusmetrics.h"
#include "autofocusmethods.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow , public RectangleWatcher {
    Q_OBJECT

public:
    explicit MainWindow( QWidget * parent = 0 );
    void updateRect( cv::Rect newRect );
    ~MainWindow();
    static void cvMouseHandler( int evento, int x, int y, int flags, void * parametro );
    cv::Point ponto1, ponto2;
    bool roiSelecionado = false;
    bool arrasto = false;
    cv::Rect rect;
    bool pointOnImage( cv::Point ponto );

private:
    bool loaded = false;
    //Main window
    Ui::MainWindow
    * ui;

    cv::Mat
    loadedImage;

    cv::Mat
    ROI;

    cv::Rect
    rectangle;

    void
    updateDisplayImage( cv::Mat img );

    bool
    loadedImageOk();

    cv::Point2d
    ROICenter;

    cv::Point2d
    getROICenter( cv::Rect rectangle );

    //Generate a 50x50 ROI
    cv::Mat
    generate50ROI();

    //Generate a 100x100 ROI
    cv::Mat
    generate100ROI();

    //Generate a 200x200 ROI
    cv::Mat
    generate200ROI();

private slots:
    void exitApp() {exit( 0 );}
    void selectArea();
    void loadImage();

    void on_selectAreaButton_clicked();
    void on_decodeButton_clicked();
};

#endif // MAINWINDOW_H
