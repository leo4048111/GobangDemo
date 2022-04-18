#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <cstring>

#include "qgobangboard.h"

const uint32_t block_width = 30;
const uint32_t block_height = block_width;
const uint32_t board_border_width = 5;
const uint32_t board_margin = 20;
const uint32_t board_width = block_width * (PIECES_PER_ROW - 1) + 2 * (board_border_width + board_margin);
const uint32_t board_height = board_width;
const QPoint board_start_coord = {50, 50};

QGobangBoard::QGobangBoard(QWidget* parent) : QFrame(parent)
{
    //set outer wrapper styles
    this->setFrameStyle(QFrame::Box|QFrame::Plain);
    this->setLineWidth(board_border_width);
    this->setGeometry(board_start_coord.x(),board_start_coord.y(), board_width, board_height);
    this->setStyleSheet("QGobangBoard{color: black}");

    connect(this, &QGobangBoard::signal_placePiece, this, &QGobangBoard::slot_placePiece);
    connect(this, &QGobangBoard::signal_clearBoard, this, &QGobangBoard::slot_clearBoard);

    //init board
    this->slot_clearBoard();

    //set inner wrapper styles
    QPoint coord{board_border_width + board_margin, board_border_width +board_margin};
    QFrame* boardInnerWrapper = new QFrame(this);
    boardInnerWrapper->setFrameStyle(QFrame::Box|QFrame::Plain);
    boardInnerWrapper->setGeometry(coord.x(),
                                   coord.y(),
                                   board_width - 2*(board_border_width + board_margin),
                                   board_height - 2*(board_border_width + board_margin));
    boardInnerWrapper->setLineWidth(3);
    boardInnerWrapper->setStyleSheet("color: black");
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(1);
    boardInnerWrapper->setGraphicsEffect(shadow);

    //init blocks
    for(uint32_t i =0;i<PIECES_PER_ROW - 1;i++)
    {
        for(uint32_t j=0;j<PIECES_PER_ROW - 1;j++)
        {
            QFrame* block= new QFrame(this);
            block->setFrameStyle(QFrame::Box|QFrame::Plain);
            block->setLineWidth(1);
            block->setGeometry(coord.x(), coord.y(), block_width,block_height);
            block->setStyleSheet("color: black");
            coord.setX(coord.x() + block_width);
        }
        coord.setX(board_border_width + board_margin);
        coord.setY(coord.y() + block_height);
    }
}

void QGobangBoard::slot_clearBoard()
{
    //clear mat_board
    for(uint32_t i = 1;i <= PIECES_PER_ROW + 1;i++)
    {
        for(uint32_t j = 1; j <= PIECES_PER_ROW + 1;j++)
        {
            if(this->mat_board[i][j] != nullptr)
            {
                delete mat_board[i][j];
                mat_board[i][j] = nullptr;
            }
        }
    }

    //clear board
    memset(this->board, (int)ABPruning::BlockStatus::empty, sizeof(ABPruning::ABPruningEngine::Board));
    for (int i = 0; i < PIECES_PER_ROW + 2; i++)
    {
        board[0][i] = ABPruning::BlockStatus::border;
        board[PIECES_PER_ROW + 1][i] = ABPruning::BlockStatus::border;
        board[i][0] = ABPruning::BlockStatus::border;
        board[i][PIECES_PER_ROW + 1] = ABPruning::BlockStatus::border;
    }

    //clear flags
    this->isBlackTurn = true;
}

void QGobangBoard::slot_placePiece(int posX, int posY, bool isBlack)
{
    if(posX > PIECES_PER_ROW) posX = PIECES_PER_ROW;
    if(posY > PIECES_PER_ROW) posX = PIECES_PER_ROW;
    if(posX < 1) posX = 1;
    if(posY < 1) posY = 1;
    uint32_t midX = board_border_width + board_margin + block_width * (posX - 1);
    uint32_t midY = board_border_width + board_margin + block_height * (posY - 1);
    QChessPiece* chessPiece = new QChessPiece(midX, midY, isBlack, this);
    chessPiece->show();
    this->mat_board[posX][posY] = chessPiece;
    this->isBlackTurn = !this->isBlackTurn;
    this->board[posX][posY] = isBlack?ABPruning::BlockStatus::black:ABPruning::BlockStatus::white;
}

void QGobangBoard::mousePressEvent(QMouseEvent* event)
{
    auto coord = event->localPos();
    if((coord.x()- board_border_width - board_margin < 0) || (coord.y()- board_border_width - board_margin < 0)) return;
    if((coord.x()+ board_border_width + board_margin > board_width) || (coord.y()+ board_border_width + board_margin > board_height)) return;
    int posX = (coord.x() - board_border_width - board_margin + block_width/2)/block_width + 1;
    int posY = (coord.y() - board_border_width - board_margin + block_height/2)/block_height + 1;
    if(posX > PIECES_PER_ROW || posY >PIECES_PER_ROW) return;
    if(this->mat_board[posX][posY] != nullptr) return;
    emit signal_placePiece(posX, posY, this->isBlackTurn);

    ABPruning::Vec2 result = ABPruning::ABPruningEngine::getInstance()->run(this->board);
    emit signal_placePiece(result.x, result.y, this->isBlackTurn);
}

