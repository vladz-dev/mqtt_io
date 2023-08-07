#pragma once

#include "emulator.h"
#include "logger.h"

#include <QQmlApplicationEngine>
#include <QObject>
#include <QQuickItem>

namespace jobemu
{

class UiCore : public QObject
{
    Q_OBJECT
public:
    explicit UiCore(QQmlApplicationEngine& engine, QObject* parent = nullptr);

public slots:
    void refreshControls();
    void doSendJobMirror();
    void doSendJobRgb2Gbr();
    void doSendJobSubRect();

    QString generateJobId() const;
    QString sourceImagePath(QString id) const;
    QString resultLocationPath(QString id) const;
    bool isChecked(QString id) const;

private:
    Logger mLogger;
    QQmlApplicationEngine& mEngine;
    Emulator mEmulator;
    QQuickItem* btnConnect;
    QQuickItem* btnDisconnect;
    QQuickItem* jobMirrorTabButton;
    QQuickItem* jobRgb2GbrTabButton;
    QQuickItem* jobSubRectTabButton;
    QQuickItem* jobsArgs;
    QQuickItem* jobMirrorTab;
    QQuickItem* jobRgb2GbrTab;
    QQuickItem* jobSubRectTab;
};

}
