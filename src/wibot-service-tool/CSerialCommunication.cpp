#include <QDebug>
#include <api/commands.h>
#include "CSerialCommunication.h"

CSerialCommunication::CSerialCommunication(const QSerialPortInfo &port)
    : mSerialPort(port)
{
    connect(&mSerialPort, &QSerialPort::readyRead, this, &CSerialCommunication::onDataArrived);
}

CSerialCommunication::~CSerialCommunication()
{
    mSerialPort.close();
}

bool CSerialCommunication::open(QSerialPort::BaudRate baudRate)
{
    bool result = mSerialPort.open(QIODevice::ReadWrite);
    mSerialPort.setBaudRate(baudRate);
    return result;
}

void CSerialCommunication::close()
{
    mSerialPort.close();
}

void CSerialCommunication::initialize()
{
    mSerialPort.write((const char*) &INIT_CMD, sizeof(Command));
    mSerialPort.waitForBytesWritten(3000);
}

void CSerialCommunication::rotateCamera(quint16 x, quint16 z)
{
    Command rotateCam = {CMD_CODE_CAM_ROTATE, x, z};
    mSerialPort.write((const char*) &rotateCam, sizeof(Command));
    mSerialPort.waitForBytesWritten(3000);
}

void CSerialCommunication::rotateMotors(quint16 leftSpeed, quint16 rightSpeed)
{
    Command rotateMot = {CMD_CODE_MOTOR_CONTROL, leftSpeed, rightSpeed};
    mSerialPort.write((const char*) &rotateMot, sizeof(Command));
    mSerialPort.waitForBytesWritten(3000);
}

void CSerialCommunication::onDataArrived()
{
    while (mSerialPort.bytesAvailable() >= sizeof(Command)) {
        Command cmd;
        mSerialPort.read((char*) &cmd, sizeof(Command));
        switch(cmd.code) {
        case CMD_CODE_HEARTBEAT:
            emit heartBeat();
            break;
        default:
            qDebug() << "Unknow command received";
            break;
        }
    }
}

