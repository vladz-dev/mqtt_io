#include "protocol.h"

#include <QDebug>
#include <QBuffer>

#include <memory>


namespace protocol
{

Protocol::Protocol(QObject* parent)
    : QObject(parent)
{

}

QByteArray Protocol::serializeJobRequestMirror(
    const QString& jobId,
    const QImage& image,
    bool mirrorHorizontal,
    bool mirrorVertical)
{
    if(jobId.isEmpty() || image.isNull() || (!mirrorHorizontal && !mirrorVertical))
    {
        qCritical() << "serializeJobRequestMirror -> Invalid arguments";
        return {};
    }

    protocol::job::Job jobData;
    jobData.set_job_uid(jobId.toStdString());

    auto imageData = jobData.mutable_mirror_image();
    imageData->set_mirror_horizontal(mirrorHorizontal);
    imageData->set_mirror_vertical(mirrorVertical);
    imageData->set_image_data(image.bits(), image.sizeInBytes());

    const size_t requestSize = jobData.ByteSizeLong();
    qInfo() << "Mirror request size: " << requestSize << " bytes";

    QByteArray buffer(requestSize, 0);
    if(!jobData.SerializeToArray(buffer.data(), requestSize))
    {
        qCritical() << "serializeJobRequestMirror -> Unable to serialize";
        return {};
    }

    return buffer;
}

QByteArray Protocol::serializeJobRequestRgb2Gbr(
    const QString& jobId,
    const QImage& image)
{
    if(jobId.isEmpty() || image.isNull())
    {
        qCritical() << "Invalid arguments";
        return {};
    }

    protocol::job::Job jobData;
    jobData.set_job_uid(jobId.toStdString());

    auto imageData = jobData.mutable_rgb2gbr_image();
    imageData->set_image_data(image.bits(), image.sizeInBytes());

    const size_t requestSize = jobData.ByteSizeLong();
    qInfo() << "Rgb2Gbr request size: " << requestSize << " bytes";

    QByteArray buffer(requestSize, 0);
    if(!jobData.SerializeToArray(buffer.data(), requestSize))
    {
        qCritical() << "serializeJobRequestRgb2Gbr -> Unable to serialize";
        return {};
    }

    return buffer;
}

QByteArray Protocol::serializeJobRequestSubRect(
    const QString& jobId,
    const QImage& image,
    int xStart,
    int yStart,
    int xEnd,
    int yEnd)
{
    if(jobId.isEmpty() || image.isNull())
    {
        qCritical() << "Invalid arguments";
        return {};
    }

    QRect rect;
    rect.setTopLeft({xStart, yStart});
    rect.setBottomRight({xEnd, yEnd});
    if(!rect.isValid())
    {
        qCritical() << "Invalid arguments";
        return {};
    }

    protocol::job::Job jobData;
    jobData.set_job_uid(jobId.toStdString());

    auto imageData = jobData.mutable_subrect_image();
    imageData->set_image_data(image.bits(), image.sizeInBytes());
    imageData->set_x_start(xStart);
    imageData->set_y_start(yStart);
    imageData->set_x_end(xEnd);
    imageData->set_y_end(yEnd);

    const size_t requestSize = jobData.ByteSizeLong();
    qInfo() << "SubRect request size: " << requestSize << " bytes";

    QByteArray buffer(requestSize, 0);
    if(!jobData.SerializeToArray(buffer.data(), requestSize))
    {
        qCritical() << "serializeJobRequestSubRect -> Unable to serialize";
        return {};
    }

    return buffer;
}

QByteArray Protocol::serializeJobResult(
    const QString& jobId,
    const QImage& image)
{
    protocol::job::JobResponse jobData;
    jobData.set_job_uid(jobId.toStdString());
    auto resultImage = jobData.mutable_result_image();
    resultImage->set_image_data(image.constBits(), image.sizeInBytes());

    const size_t responseSize = jobData.ByteSizeLong();
    qInfo() << "Response size: " << responseSize << " bytes";

    QByteArray buffer(responseSize, 0);
    if(!jobData.SerializeToArray(buffer.data(), responseSize))
    {
        qCritical() << "serializeJobResult -> Unable to serialize";
        return {};
    }

    return buffer;
}

QByteArray Protocol::serializeJobResult(
    const QString& jobId,
    const QString& errorMessage)
{
    protocol::job::JobResponse jobData;
    jobData.set_job_uid(jobId.toStdString());
    auto resultError = jobData.mutable_result_error();
    resultError->set_error_message(errorMessage.toStdString());

    const size_t responseSize = jobData.ByteSizeLong();
    qInfo() << "Response size: " << responseSize << " bytes";

    QByteArray buffer(responseSize, 0);
    if(!jobData.SerializeToArray(buffer.data(), responseSize))
    {
        qCritical() << "serializeJobResult -> Unable to serialize";
        return {};
    }

    return buffer;
}


QImage Protocol::deserializeImage(const std::string& imageData)
{
    qDebug() << "Deserialize Image. Size: " << imageData.size();

    QByteArray data(imageData.c_str(), imageData.size());

    QImage image;
    if(!image.loadFromData(data))
    {
        qCritical() << "Deserialization of image fail";
        return {};
    }

    return image;
}

bool Protocol::deserializeJobRequest(const QMqttMessage& message)
{
    const QByteArray& payload = message.payload();

    if(payload.isEmpty())
    {
        qCritical() << "Unable to desereilize empty payload";
        return false;
    }

    protocol::job::Job job;
    if(!job.ParseFromArray(payload.data(), payload.size()))
    {
        qCritical() << "Unable to desereilize payload";
        return false;
    }

    if(job.has_mirror_image())
    {
        qDebug() << "Mirror requested";
        const protocol::job::JobMirrorImage& jobMirror = job.mirror_image();
        emit onDeserializedJobMirror(
            message,
            QString::fromStdString(job.job_uid()),
            deserializeImage(jobMirror.image_data()),
            jobMirror.mirror_horizontal(),
            jobMirror.mirror_vertical()
            );
        return true;
    }
    else if(job.has_rgb2gbr_image())
    {
        qDebug() << "RGB->GBR requested";
        const protocol::job::JobRgb2GbrImage& jobRgb2Gbr = job.rgb2gbr_image();
        emit onDeserializedJobRgb2Gbr(
            message,
            QString::fromStdString(job.job_uid()),
            deserializeImage(jobRgb2Gbr.image_data())
            );
        return true;
    }
    else if(job.has_subrect_image())
    {
        qDebug() << "SubRect requested";
        const protocol::job::JobSubRectImage& jobSubRect = job.subrect_image();
        emit onDeserializedJobSubRect(
            message,
            QString::fromStdString(job.job_uid()),
            deserializeImage(jobSubRect.image_data()),
            jobSubRect.x_start(),
            jobSubRect.y_start(),
            jobSubRect.x_end(),
            jobSubRect.y_end()
            );
        return true;
    }

    qCritical() << "Unknown type of job";
    return false;
}

}  // namespace protocol
