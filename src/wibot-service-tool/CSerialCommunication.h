#ifndef CSERIALCOMMUNICATION_H
#define CSERIALCOMMUNICATION_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QSharedPointer>

class CSerialCommunication;
typedef QSharedPointer<CSerialCommunication> SerialCommunicationPtr;

class CSerialCommunication : public QObject
{
    Q_OBJECT

public:
    explicit CSerialCommunication(const QSerialPortInfo& port);
    ~CSerialCommunication();

public slots:
    bool open(QSerialPort::BaudRate baudRate);
    void close();

    void initialize();
    void rotateCamera(quint16 x, quint16 z);
    void rotateMotors(quint16 leftSpeed, quint16 rightSpeed);

signals:
    void heartBeat();

private slots:
    void onDataArrived();

private:
    QSerialPort mSerialPort;
};

#endif // CSERIALCOMMUNICATION_H
