#include "StatusListModel.h"
//#include "PoiListModel.h"
#include "GameData.h"

int StatusListModel::rowCount(const QModelIndex & /*parent*/) const { return m_iStatusLineCount; }

int StatusListModel::columnCount(const QModelIndex & /*parent*/) const { return 2; }

// Reorder Data
void StatusListModel::recreateList()
{
//    qDebug() << "StatusListModel::recreateList() start";

    //if( m_bRecreateList == false ) return;

    // Iterate over the players
    //m_bRecreateList = false;

//    m_PoiLineList.clear();

    // What players are in the game?
    //QModelIndex parent;
    beginResetModel();
    //beginInsertRows( parent, 0,0 );
    //beginInsertRows( parent, 0,m_iPoiLineCount );
   // m_StatusLineList.clear();

//    StatusLine statusLine("E-sdfsdfjsdflskjdf");
    //statusLine.m_strIcon = "E";
    //statusLine.m_strText = "sfjdfslkjsdfsdf";
    //m_StatusLineList.append(statusLine);

    //endInsertRows();
    endResetModel();
    //emit dataChanged( QModelIndex(), QModelIndex() );
    emit dataChanged( QModelIndex(), QModelIndex() );

    m_iStatusLineCount = m_StatusLineList.count();

//    qDebug() << "StatusListModel::recreateList() m_iStatusLineCount=" << m_iStatusLineCount;
}

QVariant StatusListModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << "StatusListModel::data()-start index.row=" << index.row() << "m_iStatusLineCount=" << m_iStatusLineCount << "role=" << role;
    if (index.row() < 0 || index.row() >= m_iStatusLineCount ){ return QVariant(); }

    //return "sdfsdf";
    //qDebug() << "IconRole=" << IconRole;
    //qDebug() << "m_StatusLineList[index.row()].m_strText=" << m_StatusLineList[index.row()].m_strText;

    //qDebug() << "StatusListModel::data()-2 index.row=" << index.row();
    switch(role)
    {
        case IconRole: return m_StatusLineList[index.row()].m_strIcon;
        case StatusTextRole: return m_StatusLineList[index.row()].m_strText;
        //if ( TitleRole == role ){ return m_StatusLineList[index.row()].m_strText;}
        /*
        case NameRole:
            return m_PoiLineList[index.row()].m_strName;
        case DescRole:
            return m_PoiLineList[index.row()].m_strDesc;
        */
        //case OwnerRole:
        //    return m_PoiLineList[index.row()].m_strDesc;
    }
    //return m_StatusLineList[index.row()].m_strText;
/*
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
*/
    //qDebug() << "StatusListModel::data()-no role";
    return QVariant();
}
/*
bool PoiListModel::setData(const QModelIndex & index, const QVariant & value, int role)
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
QHash<int, QByteArray> StatusListModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[IconRole] = "icon";
    roles[StatusTextRole] = "statusText";
    //roles[TitleRole] = "title";
/*
    roles[NameRole] = "name";
    //roles[TypeRole] = "type";
    roles[DescRole] = "desc";
    //roles[OwnerRole] = "owner";
*/
    return roles;
}
