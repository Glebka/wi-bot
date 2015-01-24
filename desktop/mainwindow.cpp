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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
    {
        e->ignore();
        return;
    }
    mCommandBuilder.appendFlag( e->key() );
    mClient.sendCommand( mCommandBuilder.getCommand() );
    e->accept();
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if ( e->isAutoRepeat() )
    {
        e->ignore();
        return;
    }
    mCommandBuilder.removeFlag( e->key() );
    mClient.sendCommand( mCommandBuilder.getCommand() );
    e->accept();
}

void MainWindow::on_txtServerAddr_textEdited(const QString &arg1)
{
    ui->btnConnect->setEnabled(ui->txtServerAddr->hasAcceptableInput());
}

void MainWindow::on_btnDisconnect_clicked()
{
    mClient.disconnectFromHost();
}
