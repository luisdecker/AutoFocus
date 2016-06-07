#ifndef CLICKABLELABEL
#define CLICKABLELABEL

#include <QPoint>
#include <QEvent>
class ClicableLabel {


public:
    virtual void onClick(QEvent::Type clickEvent) = 0;
    QPoint firstClick, lastMovement, lastClick;
};
#endif // CLICKABLELABEL

