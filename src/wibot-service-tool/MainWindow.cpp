#include <QDebug>
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mBtnInitialize_clicked()
{
    if (!mController) {
        on_mSerialPortBox_activated(ui->mSerialPortBox->currentIndex());
    }
    if(mController) {
        mController->initialize();
    }
}

void MainWindow::on_mSerialPortBox_activated(int index)
{
    mController = SerialCommunicationPtr(new CSerialCommunication(mSerialPorts.getSerialPortInfo(index)));
    if (!mController->open(QSerialPort::Baud115200)) {
        qDebug() << "Unable to open " << mSerialPorts.getSerialPortInfo(index).portName();
    } else {
        connect(mController.data(), &CSerialCommunication::heartBeat, this, &MainWindow::onHeartBeatReceived );
    }
}

void MainWindow::on_mBtnCameraSend_clicked()
{
    if (mController) {
        mController->rotateCamera(ui->mX->value(), ui->mZ->value());
    }
}

void MainWindow::on_mButtonCameraReset_clicked()
{
    ui->mX->setValue(90);
    ui->mZ->setValue(90);
    on_mBtnCameraSend_clicked();
}

void MainWindow::on_mBtnMotorSend_clicked()
{
    if(mController) {
        mController->rotateMotors(ui->mLeftMotor->value(), ui->mRightMotor->value());
    }
}

void MainWindow::on_mButtonMotorReset_clicked()
{
    ui->mLeftMotor->setValue(0);
    ui->mRightMotor->setValue(0);
    on_mBtnMotorSend_clicked();
}

void MainWindow::on_mX_valueChanged(int arg1)
{
    if (ui->mXAutoUpdate->isChecked()) {
        on_mBtnCameraSend_clicked();
    }
}

void MainWindow::on_mZ_valueChanged(int arg1)
{
    if (ui->mZAutoUpdate->isChecked()) {
        on_mBtnCameraSend_clicked();
    }
}

void MainWindow::on_mLeftMotor_valueChanged(int arg1)
{
    if (ui->mMotorAutoUpdate->isChecked()) {
        on_mBtnMotorSend_clicked();
    }
}

void MainWindow::on_mRightMotor_valueChanged(int arg1)
{
    if (ui->mMotorAutoUpdate->isChecked()) {
        on_mBtnMotorSend_clicked();
    }
}

void MainWindow::setupUI()
{
    ui->mSerialPortBox->setModel(&mSerialPorts);
    ui->camGroupBox->setLayout(ui->camLayout);
    ui->motorGroupBox->setLayout(ui->motorLayout);
    setWindowTitle("WiBot Arduino Service Tool");
    connect(&mTimer, &QTimer::timeout, this, &MainWindow::onTimerTriggered);
}

void MainWindow::on_mMotorAutoUpdate_toggled(bool checked)
{
    if (checked) {
        mTimer.setSingleShot(false);
        mTimer.start(300);
    } else {
        mTimer.stop();
    }
}

void MainWindow::onTimerTriggered()
{
    on_mBtnMotorSend_clicked();
}

void MainWindow::onHeartBeatReceived()
{
    ui->statusBar->showMessage("Board is online", 1000);
}
