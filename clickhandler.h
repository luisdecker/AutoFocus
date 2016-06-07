#ifndef CLICKHANDLER_H
#define CLICKHANDLER_H
#include "qobject.h"
#include <qevent.h>

#include "squarewindow.h"
#include "clickablelabel.h"

class ClickHandler : public QObject {
public:
    ClickHandler(ClicableLabel *parent,QObject *labelToWatch);
    bool eventFilter( QObject *watched, QEvent *event );
    QPoint lastClick;
signals:

private:
    QObject *watchedLabel;
    ClicableLabel *parent;

public slots:
};

#endif // CLICKHANDLER_H
