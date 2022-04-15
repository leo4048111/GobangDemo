#include "qgobangboard.h"
#include <QPainter>

#define BOARD_BLOCKS_PER_ROW 14
#define BOARD_BLOCK_COUNT BOARD_BLOCKS_PER_ROW*BOARD_BLOCKS_PER_ROW

const uint32_t board_width = 500;
const uint32_t board_height = board_width;
const uint32_t block_width = board_width/BOARD_BLOCKS_PER_ROW;
const uint32_t block_height = block_width;


QGobangBoard::QGobangBoard(QWidget* parent) : QFrame(parent)
{
    this->setFrameStyle(QFrame::Box|QFrame::Plain);
    this->setLineWidth(5);
    this->setGeometry(50, 50, board_width, 500);
    this->setStyleSheet("color: black");
    this->setStyleSheet("boarder-radius: 13");

    //init blocks
    QPoint coord{5,5};
    for(uint32_t i =0;i<BOARD_BLOCKS_PER_ROW;i++)
    {
        for(uint32_t j=0;j<BOARD_BLOCKS_PER_ROW;j++)
        {
            QFrame* block= new QFrame(this);
            block->setFrameStyle(QFrame::Box|QFrame::Plain);
            block->setLineWidth(1);
            block->setGeometry(coord.x(), coord.y(), block_width,block_height);
            block->setStyleSheet("color: black");
            coord.setX(coord.x() + block_width);
        }
        coord.setX(5);
        coord.setY(coord.y() + block_height);
    }
}

