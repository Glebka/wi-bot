#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*************************************************************************
 * WiBot project. Service tool for Arduino Board
 *************************************************************************
 * @file    MainWindow.h
 * @date
 * @author  Hlieb Romanov
 * @brief   MainWindow class declaration
 ************************************************************************/

#include <QMainWindow>
#include <QTimer>
#include <CSerialPortListModel.h>
#include <CSerialCommunication.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mBtnInitialize_clicked();

    void on_mSerialPortBox_activated(int index);

    void on_mBtnCameraSend_clicked();

    void on_mButtonCameraReset_clicked();

    void on_mBtnMotorSend_clicked();

    void on_mButtonMotorReset_clicked();    

    void on_mX_valueChanged(int arg1);

    void on_mZ_valueChanged(int arg1);

    void on_mLeftMotor_valueChanged(int arg1);

    void on_mRightMotor_valueChanged(int arg1);    

    void on_mMotorAutoUpdate_toggled(bool checked);

    void onTimerTriggered();

    void onHeartBeatReceived();
private:
    void setupUI();

private:
    Ui::MainWindow *ui;
    CSerialPortListModel mSerialPorts;
    SerialCommunicationPtr mController;
    QTimer mTimer;
};

#endif // MAINWINDOW_H
