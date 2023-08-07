#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <QString>
#include <QHash>

namespace jobemu
{

class Logger : public QAbstractListModel
{
    Q_OBJECT
public:
    enum LoggerRoles {
        LogMessageLine = Qt::UserRole + 1,
    };

    Logger(QObject *parent = nullptr);

    void addMessage(QString message);
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

public slots:
    void clear();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<int, QByteArray> mRoles;
    QStringList mMessages;
};

}
