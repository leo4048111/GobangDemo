#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <cstring>
#include <Windows.h>

#include "qgobangboard.h"
#include "log.h"

const uint32_t board_width = 590;
const uint32_t board_height = board_width;
const uint32_t board_border_width = 5;
const uint32_t board_margin = 24;
const uint32_t block_width = (board_width - 2 * (board_border_width + board_margin))/(PIECES_PER_ROW - 1);
const uint32_t block_height = block_width;
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
    connect(this, &QGobangBoard::signal_judgeWin, this, &QGobangBoard::slot_judgeWin);

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

    //init adornment
    const int adornment_width = 10;
    const int adornment_x[5] = {4, 12, 8, 4, 12};
    const int adornment_y[5] = {4, 4, 8, 12, 12};
    for(int i  = 0; i < 5; i++)
    {
        uint32_t adornmentMidX = board_border_width + board_margin + block_width * (adornment_x[i] - 1);
        uint32_t adornmentMidY = board_border_width + board_margin + block_height * (adornment_y[i] - 1);
        QLabel* adornment = new QLabel(this);
        adornment->setGeometry(adornmentMidX - adornment_width/2, adornmentMidY - adornment_width/2, adornment_width, adornment_width);
        adornment->setStyleSheet("QLabel{border:1px solid black;\
                                   background-color: black;\
                                   border-radius:" + QString::number(adornment_width/2) + "px;}");
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
    while(!this->playerMovements.empty()) this->playerMovements.pop();
    while(!this->aiMovements.empty()) this->aiMovements.pop();

    //reset engine
    ABPruning::ABPruningEngine::getInstance()->reset();

    //reset board
    this->setDisabled(false);

    LOG("Board has been reset.", Log::LogType::normal)
}

void QGobangBoard::slot_placePiece(int posX, int posY, bool isPlayer)
{
    if(posX > PIECES_PER_ROW) posX = PIECES_PER_ROW;
    if(posY > PIECES_PER_ROW) posX = PIECES_PER_ROW;
    if(posX < 1) posX = 1;
    if(posY < 1) posY = 1;
    uint32_t midX = board_border_width + board_margin + block_width * (posX - 1);
    uint32_t midY = board_border_width + board_margin + block_height * (posY - 1);
    QChessPiece* chessPiece = new QChessPiece(midX, midY, !(isPlayer ^ this->isPlayerBlack), this);
    chessPiece->show();
    this->mat_board[posX][posY] = chessPiece;
    this->board[posX][posY] = (!(isPlayer ^ this->isPlayerBlack))?ABPruning::BlockStatus::black:ABPruning::BlockStatus::white;
    if(isPlayer) this->playerMovements.push({posX, posY});
    else this->aiMovements.push({posX, posY});

    if(isPlayer) LOG("Player plays (" + QString::number(posX) + "," + QString::number(posY) +")", Log::LogType::runtime)
    else LOG("Bot plays (" + QString::number(posX) + "," + QString::number(posY) +")", Log::LogType::runtime)

}

void QGobangBoard::slot_undoMove()
{
    if(this->playerMovements.empty()) return;

    if(!this->playerMovements.empty())
    {
        ABPruning::Vec2 pos = this->playerMovements.top();
        this->playerMovements.pop();
        if(this->mat_board[pos.x][pos.y] != nullptr)
        {
            delete this->mat_board[pos.x][pos.y];
            this->mat_board[pos.x][pos.y] = nullptr;
        }

        this->board[pos.x][pos.y] = ABPruning::BlockStatus::empty;
        LOG("Player undoes (" + QString::number(pos.x) + "," + QString::number(pos.y) +")", Log::LogType::runtime)
    }

    if(!this->aiMovements.empty())
    {
        ABPruning::Vec2 pos = this->aiMovements.top();
        this->aiMovements.pop();
        if(this->mat_board[pos.x][pos.y] != nullptr)
        {
            delete this->mat_board[pos.x][pos.y];
            this->mat_board[pos.x][pos.y] = nullptr;
        }

        this->board[pos.x][pos.y] = ABPruning::BlockStatus::empty;
        LOG("Bot undoes (" + QString::number(pos.x) + "," + QString::number(pos.y) +")", Log::LogType::runtime)
    }
}

DWORD WINAPI FindBestMoveThread(LPVOID params)
{
    qRegisterMetaType<Log::LogType>("Log::LogType");
    QGobangBoard* pThis = reinterpret_cast<QGobangBoard*>(params);
    pThis->findBestMove();
    return NULL;
}

void QGobangBoard::findBestMove()
{
    ABPruning::Vec2 result = ABPruning::ABPruningEngine::getInstance()->run(this->board);
    if(result.x != -1 && result.y != -1 && result.x != -2 && result.y != -2)
    {
        emit signal_placePiece(result.x, result.y, false);
    }

    emit signal_judgeWin();
}

void QGobangBoard::slot_judgeWin()
{
    auto curBoardStats = ABPruning::ABPruningEngine::getInstance()->estimate(this->board);
    if(curBoardStats.status == ABPruning::ABPruningEngine::WinningStatus::BLACK_WIN)
    {
        this->setDisabled(true);
        LOG("Player wins", Log::LogType::runtime)
    }
    else if(curBoardStats.status == ABPruning::ABPruningEngine::WinningStatus::WHITE_WIN)
    {
        this->setDisabled(true);
        LOG("Bot wins", Log::LogType::runtime)
    }
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
    emit signal_placePiece(posX, posY, true);
    HANDLE hThread = CreateThread(nullptr, NULL, FindBestMoveThread, (LPVOID)this, NULL, nullptr);
    CloseHandle(hThread);
}

