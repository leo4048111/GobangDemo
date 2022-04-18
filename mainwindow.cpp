#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init board
    this->board = new QGobangBoard(this);
    this->board->show();

    connect(this->ui->testBtn, &QPushButton::clicked, this->board, &QGobangBoard::slot_clearBoard);
}

MainWindow::~MainWindow()
{
    delete ui;
}

