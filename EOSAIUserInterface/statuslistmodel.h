#ifndef STATUSLISTMODEL_H
#define STATUSLISTMODEL_H

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
        QString m_strIcon; // "W", "E"
        QString m_strText;
};

class StatusListModel : public QAbstractTableModel
{
    Q_OBJECT
    public:
        enum MyRoles {
            IconRole = Qt::UserRole + 1,
            StatusTextRole,
            //TitleRole
            //NameRole = Qt::UserRole + 1,
            //TypeRole,
            //DescRole,
            //OwnerRole
        };

        void recreateList();

        int rowCount(const QModelIndex &parent = QModelIndex()) const ;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        //bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
        QHash<int, QByteArray> roleNames() const;

    protected:
        bool m_bRecreateList = false;
    public:
        int               m_iStatusLineCount = 0;
        QList<StatusLine> m_StatusLineList;
};

#endif // STATUSLISTMODEL_H
