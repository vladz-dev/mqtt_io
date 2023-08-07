#ifndef WORKER_H
#define WORKER_H

#include "config/configuration.h"
#include "protocol/protocol.h"

#include <QObject>
#include <QMqttClient>
#include <QMqttMessage>

#include <memory>


class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void run();

signals:

private slots:
    void onConnected();
    void onMessage(QMqttMessage message);
    void onProcessJobMirror(const QMqttMessage& requestMessage,
                             const QString& jobId,
                             const QImage& sourceImage,
                             bool mirrorHorizontal,
                             bool mirrorVertical);
    void onProcessJobRgb2Gbr(const QMqttMessage& requestMessage,
                              const QString& jobId,
                              const QImage& sourceImage);
    void onProcessJobSubRect(const QMqttMessage& requestMessage,
                              const QString& jobId,
                              const QImage& sourceImage,
                              int xStart,
                              int yStart,
                              int xEnd,
                              int yEnd);
private:
    void sendResponse(const QString& responseTopic,
                      const QString& jobId,
                      const QImage& resultImage);

    void sendResponse(const QString& responseTopic,
                      const QString& jobId,
                      const QString& resultError);

    std::shared_ptr<config::Configuration> mConfig;
    QMqttClient* mMqttClient;
    bool mSubscribed;
    QMqttSubscription* mMqttSubscription;
    protocol::Protocol* mProtocol;
};

#endif // WORKER_H
