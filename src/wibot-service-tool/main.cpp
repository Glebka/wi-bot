/*************************************************************************
 * WiBot project. Service tool for Arduino Board
 *************************************************************************
 * @file    main.cpp
 * @date
 * @author  Hlieb Romanov
 * @brief   Main module of service tool
 ************************************************************************/
#include <QApplication>
#include <MainWindow.h>

/**
 * @brief main - application entry point
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName( "WiBot Service Tool" );
    MainWindow w;
    w.show();
    return app.exec();
}
