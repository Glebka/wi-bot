#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "roboclient.h"
#include "CommandBuilder.hpp"

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
    void on_btnConnect_clicked();
    void on_txtServerAddr_textEdited(const QString &arg1);

    void on_btnDisconnect_clicked();

private:
    Ui::MainWindow *ui;
    RoboClient mClient;
    QRegExp mAddressMatcher;
    CommandBuilder mCommandBuilder;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H
