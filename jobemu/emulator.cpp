#include "emulator.h"

#include "protocol/protocol.h"
#include "config/configuration.h"

#include <QUuid>


namespace
{

constexpr auto kMqttServerHostName = "localhost";
constexpr quint16 kMqttServerPort = 1883;

}

namespace jobemu
{

Emulator::Emulator(Logger& logger, QObject *parent)
    : QObject{parent}
    , mLogger{logger}
    , mMqttClient{new QMqttClient(this)}
    , mMqttSubscription{nullptr}
{
    mMqttClient->setClientId(QUuid::createUuid().toString());
    mMqttClient->setHostname(kMqttServerHostName);
    mMqttClient->setPort(kMqttServerPort);
    mMqttClient->setProtocolVersion(QMqttClient::MQTT_5_0);
    mMqttClient->setAutoKeepAlive(true);

    connect(mMqttClient, &QMqttClient::connected, this, [&](){
        qDebug() << "signal QMqttClient::connected";
        addLogMessage("Connected");
        emit connected();
    });

    connect(mMqttClient, &QMqttClient::disconnected, this, [&](){
        qDebug() << "signal QMqttClient::disconnected";
        addLogMessage("Disconnected");
        emit disconnected();
    });

    connect(mMqttClient, &QMqttClient::messageSent, this, [&](){
        qDebug() << "signal QMqttClient::messageSent";
        addLogMessage("Message sent");
    });

    connect(mMqttClient, &QMqttClient::errorChanged, this, [&](QMqttClient::ClientError error){
        qDebug() << "signal QMqttClient::errorChanged";
        addLogMessage("Mqtt error");
    });
}

bool Emulator::isConnected() const
{
    return (mMqttClient->state() == QMqttClient::Connected);
}

void Emulator::addLogMessage(QString message)
{
    mLogger.addMessage(message);
}

void Emulator::doConnect()
{
    qDebug() << "Emulator::doConnect";
    addLogMessage("Connecting...");
    mMqttClient->connectToHost();
}

void Emulator::doDisconnect()
{
    qDebug() << "Emulator::doDisconnect";
    addLogMessage("Disconnecting...");
    mMqttClient->disconnectFromHost();
}

bool Emulator::sendJobMirror(
    const QString& jobId,
    const QImage& image,
    bool mirrorHorizontal,
    bool mirrorVertical)
{
    qDebug() << "Emulator::sendJobMirror";

    protocol::Protocol protocol;
    return sendJob(
        protocol.serializeJobRequestMirror(jobId, image, mirrorHorizontal, mirrorVertical),
        QUuid::createUuid().toString());
}

bool Emulator::sendJobRgb2Gbr(
    const QString& jobId,
    const QImage& image)
{
    qDebug() << "Emulator::sendJobRgb2Gbr";

    protocol::Protocol protocol;
    return sendJob(
        protocol.serializeJobRequestRgb2Gbr(jobId, image),
        QUuid::createUuid().toString());
}

bool Emulator::sendJobSubRect(
    const QString& jobId,
    const QImage& image,
    int xStart,
    int yStart,
    int xEnd,
    int yEnd)
{
    qDebug() << "Emulator::sendJobSubRect";

    protocol::Protocol protocol;
    return sendJob(
        protocol.serializeJobRequestSubRect(jobId, image, xStart, yStart, xEnd, yEnd),
        QUuid::createUuid().toString());
}

bool Emulator::sendJob(const QByteArray& message, const QString& expectedResponseTopic)
{
    qInfo() << "Serialized data: " << message.size() << " bytes";
    if(message.size() > 0)
    {
        QMqttPublishProperties properties;
        properties.setResponseTopic(expectedResponseTopic);

        const QString requestTopicName = QString::fromStdString(config::Configuration::instance()->mqttClientRequestTopic());
        const QMqttTopicName requestTopic{requestTopicName};

        const qint32 msgId = mMqttClient->publish(requestTopic, properties, message, /* QoS */ 1);
        qInfo() << "Published -> Message Id: " << msgId << " to topic: " << requestTopicName << " /Respose at " << expectedResponseTopic;
        return (msgId != 0);
    }

    qCritical() << "Unable to send empty request";
    return false;
}

}
