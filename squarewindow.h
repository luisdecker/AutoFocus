#ifndef SQUAREWINDOW_H
#define SQUAREWINDOW_H

#include <QMainWindow>
#include <clickhandler.h>
#include "clickablelabel.h"
#include <opencv.hpp>
#include "rectanglewatcher.h"

namespace Ui {
    class SquareWindow;
}

class SquareWindow : public QMainWindow, public  ClicableLabel {
    Q_OBJECT

public:
    explicit SquareWindow( QWidget *parent = 0, cv::Mat image=cv::Mat::zeros( 1,1,CV_8UC3 ),RectangleWatcher * = 0 );
    ~SquareWindow();
    void onClick( QEvent::Type event );
private:

    Ui::SquareWindow *ui;
    cv::Mat originalImage;
    bool selectionStarted;
    cv::Rect lastRect;
    RectangleWatcher * parentWatcher;
};

#endif // SQUAREWINDOW_H
