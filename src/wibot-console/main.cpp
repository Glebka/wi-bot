#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QThread>
#include <QQmlContext>
#include "StreamClient.hpp"
#include "CameraItem.hpp"
#include "CommunicationController.hpp"
#include "CameraController.hpp"
#include "MotorController.hpp"
#include <unistd.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CameraItem>("CameraItem", 1, 0, "CameraItem");
    qmlRegisterType<CommunicationController>( "CommunicationController", 1, 0, "CommunicationController" );
    qmlRegisterType<CameraController>( "CameraController", 1, 0, "CameraController" );

    StreamClient client( "192.168.1.7", 778, 0 );

    CommunicationController communicator;
    communicator.connectToHost( "192.168.1.7", 777 );

    CameraController cameraController;
    cameraController.setCommunicator( &communicator );

    MotorController motorController;
    motorController.setCommunicator( &communicator );

    /*uint16_t value = 140;

    while ( true )
    {
       while( value != 500 )
       {
          communicator.changeCameraPosition( value, value );
          //usleep( 50000 );
          ++value;
       }
       break;
       /*while( value != 140 )
       {
          communicator.changeCameraPosition( value, value );
          usleep( 50000 );
          --value;
       }*/
    //}

    QThread thread;
    client.moveToThread( &thread );

    client.connect( &thread, &QThread::started, &client, &StreamClient::init );
    client.connect( &thread, &QThread::finished, &client, &StreamClient::teardown );

    thread.start();

    QQuickView view;
    view.rootContext()->setContextProperty( "streamObj", &client );
    view.rootContext()->setContextProperty( "commObj", &communicator );
    view.rootContext()->setContextProperty( "camControl", &cameraController );
    view.rootContext()->setContextProperty( "motoControl", &motorController );

    view.resize( 800, 600 );
    view.setSource( QUrl( QStringLiteral( "qrc:/main.qml" ) ) );

    view.show();
    view.raise();

    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    int result = app.exec();

    thread.quit();

    return result;
}
