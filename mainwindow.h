#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include "rectanglewatcher.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow , public RectangleWatcher {
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
    void updateRect( cv::Rect newRect );
    ~MainWindow();
    static void cvMouseHandler( int evento, int x, int y, int flags,void *parametro );
    cv::Point ponto1, ponto2;
    bool roiSelecionado = false;
    bool arrasto = false;
    cv::Rect rect;
    bool pointOnImage( cv::Point ponto );

private:
    bool loaded = false;

    Ui::MainWindow *ui;
    cv::Mat loadedImage;
    cv::Mat ROI;
    cv::Rect rectangle;
    void updateDisplayImage( cv::Mat img );
    bool loadedImageOk();
private slots:
    void exitApp() {exit( 0 );}
    void selectArea();
    void loadImage();

    void on_selectAreaButton_clicked();
};

#endif // MAINWINDOW_H
