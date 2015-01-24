#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QThread>
#include <QQmlContext>
#include "StreamClient.hpp"
#include "CameraItem.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CameraItem>("CameraItem", 1, 0, "CameraItem");

    StreamClient client( "192.168.1.7", 778, 0 );

    QThread thread;
    client.moveToThread( &thread );

    client.connect( &thread, &QThread::started, &client, &StreamClient::init );
    thread.start();

    QQuickView view;
    view.rootContext()->setContextProperty( "streamObj", &client );

    view.resize( 800, 600 );
    view.setSource( QUrl( QStringLiteral( "qrc:/main.qml" ) ) );

    view.show();
    view.raise();

    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
