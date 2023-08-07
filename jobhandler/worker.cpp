#include "worker.h"

#include <QUuid>
#include <QByteArray>


Worker::Worker(QObject *parent)
    : QObject{parent}
    , mConfig{config::Configuration::instance()}
    , mMqttClient{new QMqttClient(this)}
    , mSubscribed{false}
    , mMqttSubscription{nullptr}
    , mProtocol{new protocol::Protocol(this)}
{
    QObject::connect(mProtocol, &protocol::Protocol::onDeserializedJobMirror, this, &Worker::onProcessJobMirror);
    QObject::connect(mProtocol, &protocol::Protocol::onDeserializedJobRgb2Gbr, this, &Worker::onProcessJobRgb2Gbr);
    QObject::connect(mProtocol, &protocol::Protocol::onDeserializedJobSubRect, this, &Worker::onProcessJobSubRect);
}

void Worker::run()
{
    qDebug() << "Running";

    mMqttClient->setClientId(QUuid::createUuid().toString());
    mMqttClient->setHostname(QString::fromStdString(mConfig->mqttServerHostName()));
    mMqttClient->setPort(mConfig->mqttServerPort());
    mMqttClient->setProtocolVersion(QMqttClient::MQTT_5_0);
    mMqttClient->setAutoKeepAlive(true);

    QObject::connect(mMqttClient, &QMqttClient::connected, this, &Worker::onConnected);
    QObject::connect(mMqttClient, &QMqttClient::errorChanged, this, [](QMqttClient::ClientError error){
        switch(error)
        {
            case QMqttClient::NoError:
                qCritical() << "QMqttClient -> NoError";
                break;
            case QMqttClient::InvalidProtocolVersion:
                qCritical() << "QMqttClient -> InvalidProtocolVersion";
                break;
            case QMqttClient::IdRejected:
                qCritical() << "QMqttClient -> IdRejected";
                break;
            case QMqttClient::ServerUnavailable:
                qCritical() << "QMqttClient -> ServerUnavailable";
                break;
            case QMqttClient::BadUsernameOrPassword:
                qCritical() << "QMqttClient -> BadUsernameOrPassword";
                break;
            case QMqttClient::NotAuthorized:
                qCritical() << "QMqttClient -> NotAuthorized";
                break;
            case QMqttClient::TransportInvalid:
                qCritical() << "QMqttClient -> TransportInvalid";
                break;
            case QMqttClient::ProtocolViolation:
                qCritical() << "QMqttClient -> ProtocolViolation";
                break;
            case QMqttClient::UnknownError:
                qCritical() << "QMqttClient -> UnknownError";
                break;
            case QMqttClient::Mqtt5SpecificError:
                qCritical() << "QMqttClient -> Mqtt5SpecificError";
                break;
        }
    });

    qDebug() << "Connecting as: " << mMqttClient->clientId();
    mMqttClient->connectToHost();
}

void Worker::onConnected()
{
    qDebug() << "Connected";

    QString subscribeTopicName = QString::fromStdString(mConfig->mqttJobHandlerSubscriptionTopic());
    QMqttTopicFilter topicFilter{subscribeTopicName};
    qDebug() << "Subscribe to: " << topicFilter;

    mMqttSubscription = mMqttClient->subscribe(topicFilter, /* QoS */ 1);
    QObject::connect(mMqttSubscription, &QMqttSubscription::messageReceived, this, &Worker::onMessage);
    QObject::connect(mMqttSubscription, &QMqttSubscription::stateChanged, this, [](QMqttSubscription::SubscriptionState state){
        switch(state)
        {
        case QMqttSubscription::Error:
            qDebug() << "Subscription -> Error";
            break;
        case QMqttSubscription::SubscriptionPending:
            qDebug() << "Subscription -> SubscriptionPending";
            break;
        case QMqttSubscription::Subscribed:
            qDebug() << "Subscription -> Subscribed";
            break;
        case QMqttSubscription::UnsubscriptionPending:
            qDebug() << "Subscription -> UnsubscriptionPending";
            break;
        case QMqttSubscription::Unsubscribed:
            qDebug() << "Subscription -> Unsubscribed";
            break;
        }
    });
}

void Worker::onMessage(QMqttMessage message)
{
    const QMqttPublishProperties properties = message.publishProperties();

    qDebug() << "Message received:";
    qDebug() << "\tid: " << message.id();
    qDebug() << "\ttopic: " << message.topic();
    qDebug() << "\tsize: " << message.payload().size();

    if(!mProtocol->deserializeJobRequest(std::move(message)))
    {
        qDebug() << "Deserialization message failed";
    }
}

void Worker::onProcessJobMirror(const QMqttMessage& requestMessage,
                                const QString& jobId,
                                const QImage& sourceImage,
                                bool mirrorHorizontal,
                                bool mirrorVertical)
{
    qDebug() << "onProcessJobMirror";

    const QString responseTopic{requestMessage.publishProperties().responseTopic()};
    qDebug() << "Response topic: " << responseTopic;

    if(sourceImage.isNull())
    {
        sendResponse(
            responseTopic,
            jobId,
            "Invalid image data");
    }
    else
    {
        QImage resultImage = sourceImage.mirrored(mirrorHorizontal, mirrorVertical);
        if(resultImage.isNull())
        {
            sendResponse(
                responseTopic,
                jobId,
                "Mirror failed");
        }
        else
        {
            sendResponse(
                responseTopic,
                jobId,
                resultImage);
        }
    }
}

void Worker::onProcessJobRgb2Gbr(const QMqttMessage& requestMessage,
                                 const QString& jobId,
                                 const QImage& sourceImage)
{
    qDebug() << "onProcessJobRgb2Gbr";

    const QString responseTopic{requestMessage.publishProperties().responseTopic()};
    qDebug() << "Response topic: " << responseTopic;

    if(sourceImage.isNull())
    {
        sendResponse(
            responseTopic,
            jobId,
            "Invalid image data");
    }
    else
    {
        QImage resultImage = sourceImage.rgbSwapped();
        if(resultImage.isNull())
        {
            sendResponse(
                responseTopic,
                jobId,
                "RGB->GBR failed");
        }
        else
        {
            sendResponse(
                responseTopic,
                jobId,
                resultImage);
        }
    }
}

void Worker::onProcessJobSubRect(const QMqttMessage& requestMessage,
                                const QString& jobId,
                                const QImage& sourceImage,
                                int xStart,
                                int yStart,
                                int xEnd,
                                int yEnd)
{
    qDebug() << "onProcessJobSubRect";

    const QString responseTopic{requestMessage.publishProperties().responseTopic()};
    qDebug() << "Response topic: " << responseTopic;

    if(sourceImage.isNull())
    {
        sendResponse(
            responseTopic,
            jobId,
            "Invalid image data");
    }
    else
    {
        QRect rectToCopy;
        rectToCopy.setTopLeft({xStart, yStart});
        rectToCopy.setBottomRight({xEnd, yEnd});
        if(!rectToCopy.isValid())
        {
            sendResponse(
                responseTopic,
                jobId,
                "SubRect invalid arguments");
        }
        else
        {
            QImage resultImage = sourceImage.copy(rectToCopy);
            if(resultImage.isNull())
            {
                sendResponse(
                    responseTopic,
                    jobId,
                    "SubRect failed");
            }
            else
            {
                sendResponse(
                    responseTopic,
                    jobId,
                    resultImage);
            }
        }
    }
}

void Worker::sendResponse(const QString& responseTopic, const QString& jobId, const QImage& resultImage)
{
    if(resultImage.isNull())
    {
        sendResponse(responseTopic, jobId, "Result image is invalid");
    }
    else
    {
        const QByteArray responseData = mProtocol->serializeJobResult(jobId, resultImage);
        const qint32 msgId = mMqttClient->publish(responseTopic, responseData, /* QoS */ 1);
        qInfo() << "Published -> Message Id: " << msgId << " to topic: " << responseTopic;
    }
}

void Worker::sendResponse(const QString& responseTopic, const QString& jobId, const QString& resultError)
{
    const QByteArray responseData = mProtocol->serializeJobResult(jobId, resultError);
    const qint32 msgId = mMqttClient->publish(responseTopic, responseData, /* QoS */ 1);
    qInfo() << "Published -> Message Id: " << msgId << " to topic: " << responseTopic;
}
