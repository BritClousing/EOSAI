#include "mymodel.h"
#include "GameData.h"
/*
MyModel::MyModel(QObject *parent) : QAbstractTableModel(parent) {

    SimpleData *m_simpledata;

    // Create junk model data
    m_simpledata = new SimpleData(true, "a1", "b1");
    m_data_list.append(m_simpledata);
    m_simpledata = new SimpleData(false, "a2", "b2");
    m_data_list.append(m_simpledata);
}
*/
/*
int MyModel::rowCount(const QModelIndex & parent) const {
    return g_pGameData->m_iNumberOfPoi;
    //return m_data_list.length();
}

int MyModel::columnCount(const QModelIndex & parent) const {
    return 3;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= g_pGameData->m_iNumberOfPoi ){ return QVariant(); }
    switch(role)
    {
        case TypeRole:
            return g_pGameData->m_PoiList[index.row()].m_szPoiType;
            //return m_data_list.at(index.row())->enable;
        case DescRole:
            return g_pGameData->m_PoiList[index.row()].m_szDesc;
            //return "sdsdfds";
            //return m_data_list.at(index.row())->m_one;
        case OwnerRole:
            return g_pGameData->m_PoiList[index.row()].m_iOwner;
            //return m_data_list.at(index.row())->m_two;
    }

    return QVariant();
}
*/
/*
bool MyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{

    qDebug() << "setData() called with:" << value;
    switch(role)
    {
        case TypeRole:
            m_data_list[index.row()]->enable = value.toBool();
        case DescRole:
            m_data_list[index.row()]->m_one = value.toString();
        case OwnerRole:
            m_data_list[index.row()]->m_two = value.toString();
    }
    return true;
}
*/
/*
QHash<int, QByteArray> MyModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    //roles[RoleTwo] = "role_m_one";
    roles[DescRole] = "desc";
    roles[OwnerRole] = "owner";
    return roles;
}
*/
/*
void MyModel::print_rows() {

    qDebug() << "Model Data:";

    for (int i=0; i < m_data_list.length(); i++) {
        qDebug() << "At" << i << ":" << m_data_list.at(i)->enable << m_data_list.at(i)->m_one << m_data_list.at(i)->m_two;
    }
}
*/
