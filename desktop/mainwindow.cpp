#include <QDebug>
#include <QKeyEvent>
#include <QRegExpValidator>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mAddressMatcher("(\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}):(\\d{1,4})")
{
    ui->setupUi(this);
    setWindowTitle("Robo Client");
    QRegExpValidator *validator = new QRegExpValidator(mAddressMatcher);
    ui->txtServerAddr->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    int pos = mAddressMatcher.indexIn(ui->txtServerAddr->text());
    if (pos > -1)
    {
        QString address = mAddressMatcher.cap(1);
        quint16 port = mAddressMatcher.cap(2).toUInt();
        mClient.connectToHost(address, port);
    }
}

void MainWindow::on_btnDisconnect_clicked()
{
    mClient.disconnectFromHost();
}

void MainWindow::on_btnW_clicked()
{
    mClient.sendCommand(RoboCommand::CAM_UP);
}

void MainWindow::on_btnA_clicked()
{
    mClient.sendCommand(RoboCommand::CAM_LEFT);
}

void MainWindow::on_btnD_clicked()
{
    mClient.sendCommand(RoboCommand::CAM_RIGTH);
}

void MainWindow::on_btnS_clicked()
{
    mClient.sendCommand(RoboCommand::CAM_DOWN);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        mClient.sendCommand(RoboCommand::CAM_UP);
        break;
    case Qt::Key_S:
        mClient.sendCommand(RoboCommand::CAM_DOWN);
        break;
    case Qt::Key_A:
        mClient.sendCommand(RoboCommand::CAM_LEFT);
        break;
    case Qt::Key_D:
        mClient.sendCommand(RoboCommand::CAM_RIGTH);
        break;
    default:
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    //qDebug() << e->key() << "released";
}

void MainWindow::on_txtServerAddr_textEdited(const QString &arg1)
{
    ui->btnConnect->setEnabled(ui->txtServerAddr->hasAcceptableInput());
}
