#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "protocol/job.pb.h"

#include <QObject>
#include <QByteArray>
#include <QImage>
#include <QString>
#include <QMqttMessage>


namespace protocol
{

class Protocol : public QObject
{
    Q_OBJECT
public:
    Protocol(QObject* parent = nullptr);

    QByteArray serializeJobRequestMirror(
        const QString& jobId,
        const QImage& image,
        bool mirrorHorizontal,
        bool mirrorVertical);

    QByteArray serializeJobRequestRgb2Gbr(
        const QString& jobId,
        const QImage& image);

    QByteArray serializeJobRequestSubRect(
        const QString& jobId,
        const QImage& image,
        int xStart,
        int yStart,
        int xEnd,
        int yEnd);

    QByteArray serializeJobResult(
        const QString& jobId,
        const QImage& image);

    QByteArray serializeJobResult(
        const QString& jobId,
        const QString& errorMessage);

    bool deserializeJobRequest(const QMqttMessage& message);

signals:
    void onDeserializedJobMirror(const QMqttMessage& message,
                              const QString& jobId,
                              const QImage& image,
                              bool mirrorHorizontal,
                              bool mirrorVertical);
    void onDeserializedJobRgb2Gbr(const QMqttMessage& message,
                               const QString& jobId,
                               const QImage& image);
    void onDeserializedJobSubRect(const QMqttMessage& message,
                               const QString& jobId,
                               const QImage& image,
                               int xStart,
                               int yStart,
                               int xEnd,
                               int yEnd);
private:
    QImage deserializeImage(const std::string& imageData);
};

} // namespace protocol

#endif // PROTOCOL_H
