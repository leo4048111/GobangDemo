#ifndef QGOBANGBOARD_H
#define QGOBANGBOARD_H

#include <QObject>
#include <QFrame>

class QGobangBoard :public QFrame
{
    Q_OBJECT

public:
    explicit QGobangBoard(QWidget* parent = nullptr);
};

#endif // QGOBANGBOARD_H
