#include "uicore.h"

#include <QQmlContext>
#include <QQmlProperty>
#include <QUuid>
#include <QImage>


namespace jobemu
{

UiCore::UiCore(QQmlApplicationEngine& engine, QObject* parent)
    : QObject(parent)
    , mLogger()
    , mEngine{engine}
    , mEmulator(mLogger)
    , btnConnect{nullptr}
    , btnDisconnect{nullptr}
    , jobMirrorTab{nullptr}
    , jobRgb2GbrTab{nullptr}
    , jobSubRectTab{nullptr}
{
    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("logger", &mLogger);
    ctxt->setContextProperty("uicore", this);
    ctxt->setContextProperty("emulator", &mEmulator);

    engine.load(QUrl("qrc:///Main.qml"));

    QObject* root = mEngine.rootObjects().value(0);

    btnConnect = root->findChild<QQuickItem*>("btnConnect");
    btnDisconnect = root->findChild<QQuickItem*>("btnDisconnect");

    jobMirrorTabButton = root->findChild<QQuickItem*>("tabButtonJobMirror");
    jobRgb2GbrTabButton = root->findChild<QQuickItem*>("tabButtonJobRgb2Gbr");
    jobSubRectTabButton = root->findChild<QQuickItem*>("tabButtonJobSubRect");

    jobMirrorTab = root->findChild<QQuickItem*>("jobMirrorTab");
    jobRgb2GbrTab = root->findChild<QQuickItem*>("jobRgb2GbrTab");
    jobSubRectTab = root->findChild<QQuickItem*>("jobSubRectTab");

    refreshControls();
}

void UiCore::refreshControls()
{
    qDebug() << "UiCore::refreshControls";

    const bool connected = mEmulator.isConnected();

    if(btnConnect != nullptr)
        btnConnect->setEnabled(!connected);

    if(btnDisconnect != nullptr)
        btnDisconnect->setEnabled(connected);

    if(jobMirrorTabButton != nullptr)
        jobMirrorTabButton->setEnabled(connected);

    if(jobRgb2GbrTabButton != nullptr)
        jobRgb2GbrTabButton->setEnabled(connected);

    if(jobSubRectTabButton != nullptr)
        jobSubRectTabButton->setEnabled(connected);

    if(jobMirrorTab != nullptr)
        jobMirrorTab->setEnabled(connected);

    if(jobRgb2GbrTab != nullptr)
        jobRgb2GbrTab->setEnabled(connected);

    if(jobSubRectTab != nullptr)
        jobSubRectTab->setEnabled(connected);
}

QString UiCore::generateJobId() const
{
    return QUuid::createUuid().toString();
}

QString UiCore::sourceImagePath(QString id) const
{
    QObject* root = mEngine.rootObjects().value(0);
    auto sourceImageSelector = root->findChild<QQuickItem*>(std::move(id));
    auto sourceImage = sourceImageSelector->findChild<QQuickItem*>("sourceImage");
    auto sourceImageUrlPath = QQmlProperty::read(sourceImage, "text").toString();
    QUrl sourceImageUrl(sourceImageUrlPath);
    return sourceImageUrl.toLocalFile();
}

QString UiCore::resultLocationPath(QString id) const
{
    QString result;
    return result;
}

bool UiCore::isChecked(QString id) const
{
    QObject* root = mEngine.rootObjects().value(0);
    auto chekbox = root->findChild<QQuickItem*>(id);
    return QQmlProperty::read(chekbox, "checked").toBool();
}

void UiCore::doSendJobMirror()
{
    qDebug() << "UiCore::doSendJobMirror";

    const QString jobId{generateJobId()};
    mLogger.addMessage("Requested: Mirror /JobId: " + jobId);

    const QString sourceImageFilename {sourceImagePath("mirrorSourceImageSelector")};
    mLogger.addMessage("Mirror image: " + sourceImageFilename);

    const bool chkboxMirrorVertical {isChecked("chkboxMirrorVertical")};
    mLogger.addMessage("Mirror Vertical: " + QString(chkboxMirrorVertical ? "YES" : "NO"));

    const bool chkboxMirrorHorizontal {isChecked("chkboxMirrorHorizontal")};
    mLogger.addMessage("Mirror Horizontal: " + QString(chkboxMirrorHorizontal ? "YES" : "NO"));

    QImage sourceImage;
    if(!sourceImage.load(sourceImageFilename))
    {
        mLogger.addMessage("Unable to load image from: " + sourceImageFilename);
        return;
    }

    const QString resultLocation {sourceImagePath("mirrorImageResultLocationSelector")};

    if(mEmulator.sendJobMirror(jobId, sourceImage, chkboxMirrorVertical, chkboxMirrorHorizontal))
    {
        mLogger.addMessage("Requested: Mirror /JobId: " + jobId);
    }
    else
    {
        mLogger.addMessage("Unable to send request");
    }
}

void UiCore::doSendJobRgb2Gbr()
{
    qDebug() << "UiCore::doSendJobRgb2Gbr";

    const QString jobId{generateJobId()};
    const QString sourceImageFilename {sourceImagePath("rgb2GbrSourceImageSelector")};
    const QImage sourceImage{sourceImageFilename};
    const QString resultLocation {sourceImagePath("rgb2GbrImageResultLocationSelector")};
}

void UiCore::doSendJobSubRect()
{
    qDebug() << "UiCore::doSendJobSubRect";

    const QString jobId{generateJobId()};
}



}
