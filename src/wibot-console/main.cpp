/*************************************************************************
 * wibot-console
 *************************************************************************
 * @file    main.cpp
 * @date
 * @author  Hlieb Romanov
 * @brief   Main module of WiBot remote control console
 ************************************************************************/
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QThread>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QDebug>
#include "StreamClient.hpp"
#include "CameraItem.hpp"
#include "CommunicationController.hpp"
#include "CameraController.hpp"
#include "MotorController.hpp"
#include <unistd.h>

const char* APP_VERSION = "0.1";
const char* DEFAULT_VIDEO_STREAM_PORT = "778";
const char* DEFAULT_COMMAND_STREAM = "777";
const char* CONFIG_FILE_NAME = "config.json";

/**
 * @brief registerQmlTypes - helper function for registering custom types in QML
 */
void registerQmlTypes()
{
    qmlRegisterType<CameraItem>("CameraItem", 1, 0, "CameraItem");
    qmlRegisterType<CommunicationController>( "CommunicationController", 1, 0, "CommunicationController" );
    qmlRegisterType<CameraController>( "CameraController", 1, 0, "CameraController" );
}

/**
 * @brief runtimeConnectionErrorHandler - catches error signal from StreamClient and exit the application
 * @param errorMessage - error message from StreamClient
 */
void runtimeConnectionErrorHandler( const QString& errorMessage )
{
    QMessageBox::critical( NULL, "Runtime error", errorMessage );
    QGuiApplication::exit( 2 );
}

/**
 * @brief loadConfig - helper function for loading configuration from config file
 * and command line args
 * @param parser - command line args parser instance
 * @param configuration - empty JSON object instance that will store resulting configuration
 * @return true if configuration has been loaded, false - in case of errors or if configuration is not valid
 */
bool loadConfig( const QCommandLineParser& parser, QJsonObject& configuration )
{
    bool result = false;
    QFile configFile( CONFIG_FILE_NAME );
    if ( configFile.open( QIODevice::ReadOnly ) )
    {
        QByteArray configData = configFile.readAll();
        QJsonParseError jsonError;
        QJsonDocument configDocument( QJsonDocument::fromJson( configData,&jsonError ) );
        if ( jsonError.error != QJsonParseError::NoError )
        {
            qWarning( QString( "Configuration parse error: %1" ).arg( jsonError.errorString() ).toStdString().c_str() );
        } else {
            QJsonObject::const_iterator it = configDocument.object().constBegin();
            for ( it; it != configDocument.object().constEnd(); ++it ) {
                configuration.insert( it.key(), it.value() );
            }
        }
    }
    else
    {
        qWarning( "Couldn't open config file" );
    }
    foreach ( QString optionName, parser.optionNames() ) {
        configuration[ optionName ] = parser.value( optionName );
    }
    result = configuration.contains( "host" )
            && configuration.contains( "camPort" )
            && configuration.contains( "controlPort" );

    return result;
}

/**
 * @brief main - application entry point
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName( "WiBot Console" );
    QGuiApplication::setApplicationVersion( APP_VERSION );

    qDebug() << "Registering QML types...";

    registerQmlTypes();

    QCommandLineParser parser;
    parser.setApplicationDescription( "WiBot Console - application for controlling WiBot" );
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption wiBotIP( "host", "IP address or hostname of the WiBot" );
    QCommandLineOption videoStreamPort( "camPort", "Port number of video streamer", "", DEFAULT_VIDEO_STREAM_PORT );
    QCommandLineOption controlStreamPort( "controlPort", "Port number for commands and control stream", "", DEFAULT_COMMAND_STREAM );

    parser.addOptions( QList<QCommandLineOption>()
                       << wiBotIP
                       << videoStreamPort
                       << controlStreamPort );
    qDebug() << "Parsing options...";
    parser.process( app );
    QJsonObject config;
    qDebug() << "Loading config...";
    if ( !loadConfig( parser, config ) )
    {
        qDebug() << "Unable to load configuration.";
        return 1;
    }

    qDebug() << "Creating StreamClient...";
    StreamClient client( config );

    qDebug() << "Creating Communicator...";
    CommunicationController communicator( config );
    if ( !communicator.connectToHost() ) {
        qDebug() << "Unable to connect to WiBot.\nCheck the configuration and try again.";
        return 2;
    }

    CameraController cameraController;
    cameraController.setCommunicator( &communicator );

    MotorController motorController;
    motorController.setCommunicator( &communicator );

    QThread thread;
    client.moveToThread( &thread );

    client.connect( &thread, &QThread::started, &client, &StreamClient::init );
    client.connect( &thread, &QThread::finished, &client, &StreamClient::teardown );
    client.connect( &client, &StreamClient::error, &thread, &QThread::quit );
    client.connect( &client, &StreamClient::error, runtimeConnectionErrorHandler );

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

    int result = app.exec();

    thread.quit();

    return result;
}
