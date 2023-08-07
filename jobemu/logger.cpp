#include "logger.h"

namespace jobemu
{

Logger::Logger(QObject *parent) : QAbstractListModel(parent)
{
    mRoles[Qt::DisplayRole] = QStringLiteral("display").toUtf8();
    mRoles[LogMessageLine] = QStringLiteral("logMessageLine").toUtf8();
}

void Logger::clear()
{
    beginResetModel();
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    mMessages.clear();
    endRemoveRows();
    endResetModel();
}

void Logger::addMessage(QString message)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mMessages.append(std::move(message));
    endInsertRows();
}

int Logger::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return mMessages.count();
}

QHash<int, QByteArray> Logger::roleNames() const
{
    return mRoles;
}

QVariant Logger::data(const QModelIndex & index, int role) const
{
    Q_UNUSED(role);
    if ((index.row() < 0) || (index.row() >= mMessages.count()))
    {
        return QVariant();
    }

    // Whatever the role is..
    // the result will be the same - we have only one field in the model

    if (role == LogMessageLine) {
        return QVariant(mMessages.at(index.row()));
    }

    return mMessages.at(index.row());
}

}
