#ifndef QCHESSPIECE_H
#define QCHESSPIECE_H

#include <QLabel>

class QChessPiece:public QLabel
{
    Q_OBJECT

public:
    QChessPiece(int midX, int midY, bool isBlack=true, QWidget* parent=nullptr);
};

#endif // QCHESSPIECE_H
