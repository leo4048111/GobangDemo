#ifndef QGOBANGBOARD_H
#define QGOBANGBOARD_H

#include <QObject>
#include <QFrame>
#include <stack>
#include "qchesspiece.h"
#include "ABPruning.h"

class QGobangBoard :public QFrame
{
    Q_OBJECT

public:
    explicit QGobangBoard(QWidget* parent = nullptr);

signals:
    void signal_placePiece(int posX, int posY, bool isPlayer);
    void signal_clearBoard();
    void signal_judgeWin();

private slots:
    void mousePressEvent(QMouseEvent* event);

public slots:
    void slot_placePiece(int posX, int posY, bool isPlayer);

    void slot_clearBoard();

    void slot_undoMove();

private slots:
    void slot_judgeWin();

public:
    void findBestMove();

private:
    bool isPlayerBlack{true};
    QChessPiece* mat_board[PIECES_PER_ROW + 2][PIECES_PER_ROW +2]{{nullptr}};
    ABPruning::ABPruningEngine::Board board;
    std::stack<ABPruning::Vec2> playerMovements;
    std::stack<ABPruning::Vec2> aiMovements;

};

#endif // QGOBANGBOARD_H
