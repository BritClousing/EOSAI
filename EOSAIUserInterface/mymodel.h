#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>
#include <QDebug>

/*
struct SimpleData {
    bool enable;
    QString m_one;
    QString m_two;

    SimpleData() {
        enable = true;
        m_one = "1";
        m_two = "2";
    }

    SimpleData(bool enable, const QString m_one, const QString m_two) :
    enable(enable), m_one(m_one), m_two(m_two) {}
};
*/
/*
class MyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum MyRoles {
        TypeRole = Qt::UserRole + 1,
        DescRole,
        OwnerRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
    QHash<int, QByteArray> roleNames() const;

signals:
};
*/
#endif // MYMODEL_H

