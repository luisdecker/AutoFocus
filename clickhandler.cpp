#include "clickhandler.h"
#include <iostream>

ClickHandler::ClickHandler(ClicableLabel *parent,QObject *labelToWatch) {
    this->watchedLabel = labelToWatch;
    this->parent = parent;
}



bool ClickHandler::eventFilter(QObject *watched, QEvent *event) {
    //Verifies if the object wathed is the desired one
    if(watched != watchedLabel) {
        return false;
    }
    //Verifies if the event was a mouse click
    //TODO -> Quando apertar o botão, gravar a posicao. Quando mudar a posicao, atualizar o segundo ponto (cuidar pra sempre deixar o maior como primeiro!) e quando soltar o botão, parar de capturar
    if ( event->type() == QEvent::MouseButtonPress ) {
        std::cout << "Clicou o mouse em [Ṕress]" << std::endl;
        parent->firstClick = QPoint(static_cast<const QMouseEvent *>( event )->pos());
        parent->onClick(QEvent::MouseButtonPress);
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        std::cout << "Clicou o mouse em [Move]" << std::endl;
        parent->lastMovement == QPoint(static_cast<const QMouseEvent *>( event )->pos());
        parent->onClick(QEvent::MouseMove);
        return true;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        std::cout << "Clicou o mouse em [Release]" << std::endl;
        parent->lastClick = QPoint(static_cast<const QMouseEvent *>( event )->pos());
        parent->onClick(QEvent::MouseButtonRelease);
        return true;
    }
//    const QMouseEvent *const me = static_cast<const QMouseEvent *>( event );
    //might want to check the buttons here
//	const QPoint point = me->pos(); //...or ->globalPos();
//	lastClick = point;
//	//Now, send the signal to the right parent
//	//gambiarra
//	this->parent->onClick(point.x(),point.y());
    return false;
}



