#include <QGraphicsDropShadowEffect>
#include "qchesspiece.h"

const uint32_t piece_width = 30;
const uint32_t piece_height = piece_width;

QChessPiece::QChessPiece(int midX, int midY, bool isBlack, QWidget* parent):QLabel(parent)
{
    if(!isBlack)
        this->setStyleSheet("QLabel:hover{border:3px solid red;\
                                   }\
                                   QLabel{border:1px solid black;\
                                   background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, \
                                   stop:0 rgba(230, 230, 230, 253),\
                                   stop:0.284819 rgba(225, 225, 225, 245),\
                                   stop:0.504819 rgba(220, 220, 220, 255),\
                                   stop:0.79 rgba(210, 210, 210, 255),\
                                   stop:1 rgba(200, 200, 200, 255));\
                                   border-radius:" + QString::number(piece_width/2) + "px;}");
    else
        this->setStyleSheet("QLabel:hover{border:3px solid red;\
                                   }\
                                   QLabel{border:2px solid black;\
                                   background-color:black;\
                                   border-radius:" + QString::number(piece_width/2) + "px;}");
   uint32_t rectX = midX - piece_width/2;
   uint32_t rectY = midY - piece_height/2;
   this->setGeometry(rectX, rectY, piece_width, piece_height);
   this->setFrameStyle(QLabel::Sunken);
   QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
   effect->setBlurRadius(10);
   effect->setColor(Qt::black);
   effect->setOffset(0, 0);
   this->setGraphicsEffect(effect);
}
