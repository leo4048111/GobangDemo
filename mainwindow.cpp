#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("GobangDemo");
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(), this->height());

    //controllers
    connect(this->ui->btnRestart, &QPushButton::clicked, this->ui->board, &QGobangBoard::slot_clearBoard);
    connect(this->ui->btnUndo, &QPushButton::clicked, this->ui->board, &QGobangBoard::slot_undoMove);

    //log
    connect(Log::getInstance(), &Log::signal_log, this, &MainWindow::slot_onReceiveLog);

    LOG("Program has initialized...", Log::LogType::normal);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_onReceiveLog(const QString str, const Log::LogType type)
{
    QString s;
    switch(type)
    {
    case Log::LogType::normal:
        s = "[Log]";
        break;
    case Log::LogType::error:
        s = "[Error]";
        break;
    case Log::LogType::runtime:
        s = "[Runtime]";
        break;
    default:
        s = "[Log]";
        break;
    }

    this->ui->logOutput->append(s + str);
}
