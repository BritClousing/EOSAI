#ifndef POILISTMODEL_H
#define POILISTMODEL_H

#include <QAbstractListModel>
#include <QDebug>

class PoiLine
{
    public:
        PoiLine(){}
        //QString m_strIcon;
        QString m_strName; // "Player 1", "  Infantry", "  London", "  OilField"
        QString m_strDesc; // "10 Prod", "5 Oil", "5/5 Health", etc
};

class PoiListModel : public QAbstractTableModel
{
    Q_OBJECT
    public:
        enum MyRoles {
            NameRole = Qt::UserRole + 1,
            //TypeRole,
            DescRole,
            OwnerRole
        };

        void recreateList();

        int rowCount(const QModelIndex &parent = QModelIndex()) const ;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        //bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
        QHash<int, QByteArray> roleNames() const;

    protected:
        bool m_bRecreateList = false;
        int            m_iPoiLineCount = 0;
        QList<PoiLine> m_PoiLineList;
};

#endif // POILISTMODEL_H

