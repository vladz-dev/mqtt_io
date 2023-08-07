#pragma once

#include "logger.h"

#include <QMqttClient>
#include <QStringList>
#include <QObject>


namespace jobemu
{

class Emulator : public QObject
{
    Q_OBJECT
public:
    explicit Emulator(Logger& logger, QObject *parent = nullptr);

    bool isConnected() const;

signals:
    void connected();
    void disconnected();

public slots:
    void doConnect();
    void doDisconnect();

    bool sendJobMirror(
        const QString& jobId,
        const QImage& image,
        bool mirrorHorizontal,
        bool mirrorVertical);

    bool sendJobRgb2Gbr(
        const QString& jobId,
        const QImage& image);

    bool sendJobSubRect(
        const QString& jobId,
        const QImage& image,
        int xStart,
        int yStart,
        int xEnd,
        int yEnd);

    bool sendJob(const QByteArray& message,
                 const QString& expectedResponseTopic);

private:
    void addLogMessage(QString message);

    Logger& mLogger;
    QMqttClient* mMqttClient;
    QMqttSubscription* mMqttSubscription;
};

}
