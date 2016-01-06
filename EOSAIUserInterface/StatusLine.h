/*
#ifndef STATUSLINE_H
#define STATUSLINE_H

#include <QAbstractListModel>
#include <QDebug>

class StatusLine
{
    public:
        StatusLine( QString statusLine )
        {
            if( statusLine[0] == 'W' && statusLine.contains("Warning:") ){ m_strIcon = "W"; }
            if( statusLine[0] == 'E' && statusLine.contains("Error:") ){ m_strIcon = "E"; }
            m_strText = statusLine;
        }
        QString m_strIcon;
        QString m_strText;
};

class StatusLineModel : public QAbstractListModel
{
    Q_OBJECT
    public:
        enum StatusRoles
        {
            IconRole = Qt::UserRole + 1,
            StatusTextRole,
            TitleRole
        };
        int rowCount(const QModelIndex &parent = QModelIndex()) const { return m_StatusLineList.size(); }
        virtual QHash<int, QByteArray> roleNames() const
        {
            QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
            roles[IconRole] = "icon";
            roles[StatusTextRole] = "statusText";
            roles[TitleRole] = "title";
            return roles;
        }
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
        {
            //qDebug() << "StatusLineModel::data() index=" << index;
            //return "xyz"; //m_StatusLineList[index.row()].m_strText;
            if (index.row() < 0 || index.row() >= m_StatusLineList.count()){ return QVariant(); }
            if ( IconRole == role ){ return m_StatusLineList[index.row()].m_strIcon; }
            if ( StatusTextRole == role ){ return m_StatusLineList[index.row()].m_strText; }
            if ( TitleRole == role ){ return m_StatusLineList[index.row()].m_strText;}
            return QVariant();
        }

    public:
        QList<StatusLine> m_StatusLineList;
};


#endif // STATUSLINE_H
*/
