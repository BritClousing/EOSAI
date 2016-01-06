
#include "PoiListModel.h"
#include "GameData.h"

int PoiListModel::rowCount(const QModelIndex & /*parent*/) const { return m_iPoiLineCount; }

int PoiListModel::columnCount(const QModelIndex & /*parent*/) const { return 2; }

// Reorder Data
void PoiListModel::recreateList()
{
    //if( m_bRecreateList == false ) return;

    // Iterate over the players
    //m_bRecreateList = false;

    // What players are in the game?
    QModelIndex parent;
    //beginResetModel();
    //beginInsertRows( parent, 0,0 );
    //beginInsertRows( parent, 0,m_iPoiLineCount );
    m_PoiLineList.clear();
    for( int iPlayer=0; iPlayer <= g_pGameData->m_iNumberOfPlayers; iPlayer++ )
    {
        PoiLine poiLine;
        if( iPlayer == 0 ){ poiLine.m_strName = QString("Unowned"); }
        else{               poiLine.m_strName = QString("Player %1 \"%2\"").arg(iPlayer).arg( g_pGameData->m_strPlayerName[iPlayer] ); }
        m_PoiLineList.append(poiLine);

        for( int iPoiType=0; iPoiType<3; iPoiType++ )
        {
            char* pType = 0;
            if( iPoiType == 0 )
            {
                pType = "City";

                PoiLine poiLine;
                poiLine.m_strName = QString("   Cities");
                m_PoiLineList.append(poiLine);
            }
            if( iPoiType == 1 )
            {
                pType = "Resource";

                PoiLine poiLine;
                poiLine.m_strName = QString("   Resources");
                m_PoiLineList.append(poiLine);
            }
            if( iPoiType == 2 )
            {
                pType = "Unit";

                PoiLine poiLine;
                poiLine.m_strName = QString("   Units");
                m_PoiLineList.append(poiLine);
            }

            for( int i=0; i<g_pGameData->m_iNumberOfPoi; i++ )
            {
                if( g_pGameData->m_PoiList[i].m_iOwner == iPlayer )
                {
                    if( strcmp( g_pGameData->m_PoiList[i].m_szPoiType.m_szText, pType) == 0 )
                    {
                        PoiLine poiLine;
                        poiLine.m_strName = QString("      %1").arg(g_pGameData->m_PoiList[i].m_szName.m_szText);
                        poiLine.m_strDesc = QString("%1").arg(g_pGameData->m_PoiList[i].m_szLongDesc.m_szText);
                        m_PoiLineList.append(poiLine);
                    }
                }
            }
        }
    }
    //endInsertRows();
    //endResetModel();
    emit dataChanged( QModelIndex(), QModelIndex() );

    m_iPoiLineCount = m_PoiLineList.count();
}

QVariant PoiListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_iPoiLineCount ){ return QVariant(); }
    switch(role)
    {
        case NameRole:
            return m_PoiLineList[index.row()].m_strName;
        case DescRole:
            return m_PoiLineList[index.row()].m_strDesc;
        //case OwnerRole:
        //    return m_PoiLineList[index.row()].m_strDesc;
    }
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
QHash<int, QByteArray> PoiListModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    //roles[TypeRole] = "type";
    roles[DescRole] = "desc";
    //roles[OwnerRole] = "owner";
    return roles;
}
