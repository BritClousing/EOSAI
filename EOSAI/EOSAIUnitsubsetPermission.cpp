
#include "stdafx.h"
#include "EOSAIUnitsubsetPermission.h"
#include "EOSAIGameRules.h"
//#include "BCXMLReader.h"
//#include "BCString.h"
#include "EOSAISerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
void  CUnitsubsetPermission::Copy( CItemAndUnitsubset* pItemPlus )
{
	m_bListContainsPositiveItems = pItemPlus->m_bListContainsPositiveItems;
	POSITION pos = pItemPlus->m_Unitsubsets.GetHeadPosition();
	while( pos )
	{
		CString strUnitsubset = pItemPlus->m_Unitsubsets.GetNext( pos );
		m_Unitsubsets.AddTail( strUnitsubset );
	}
}
*/
void  CEOSAIUnitsubsetPermission::SetUnitsubsetToPositive( CString strUnitsubset )
{
	if( UnitsubsetIsPositive( strUnitsubset ) ) return;

	if( m_bListContainsPositiveItems )
	{
		m_Unitsubsets.AddTail( strUnitsubset );
	}
	else
	{
		POSITION pos = m_Unitsubsets.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CString strUnitsubsetInList = m_Unitsubsets.GetNext( pos );
			if( strUnitsubsetInList == strUnitsubset )
			{
				m_Unitsubsets.RemoveAt( prevPos );
			}
		}
	}
}

void  CEOSAIUnitsubsetPermission::SetUnitsubsetToNegative( CString strUnitsubset )
{
	if( UnitsubsetIsPositive( strUnitsubset ) == false ) return;

	if( m_bListContainsPositiveItems )
	{
		POSITION pos = m_Unitsubsets.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CString strUnitsubsetInList = m_Unitsubsets.GetNext( pos );
			if( strUnitsubsetInList == strUnitsubset )
			{
				m_Unitsubsets.RemoveAt( prevPos );
			}
		}
	}
	else
	{
		m_Unitsubsets.AddTail( strUnitsubset );
	}
}

bool CEOSAIUnitsubsetPermission::UnitsubsetIsPositive( CString strUnitsubset )
{
	POSITION pos = m_Unitsubsets.GetHeadPosition();
	while( pos )
	{
		CString strUnitsubsetInList = m_Unitsubsets.GetNext( pos );
		if( strUnitsubsetInList == strUnitsubset )
		{
			return m_bListContainsPositiveItems;
		}
	}
	return !m_bListContainsPositiveItems;
}

void CEOSAIUnitsubsetPermission::ToggleState( CString strUnitsubset )
{
	if( UnitsubsetIsPositive( strUnitsubset ) ){ SetUnitsubsetToNegative( strUnitsubset ); }
	else{ SetUnitsubsetToPositive( strUnitsubset ); }
}

void CEOSAIUnitsubsetPermission::SwitchPositiveNegative()
{
	bool bOldState = m_bListContainsPositiveItems;

	POSITION pos = m_pGameRules->GetUnitsubsetList()->GetHeadPosition();
	while( pos )
	{
		CString strUnitsubset = m_pGameRules->GetUnitsubsetList()->GetNext( pos );
		if( UnitsubsetIsPositive( strUnitsubset ) )
		{
			m_bListContainsPositiveItems = !bOldState;
			SetUnitsubsetToPositive( strUnitsubset );
			m_bListContainsPositiveItems = bOldState;
		}
		else
		{
			m_bListContainsPositiveItems = !bOldState;
			SetUnitsubsetToNegative( strUnitsubset );
			m_bListContainsPositiveItems = bOldState;
		}
	}
	m_bListContainsPositiveItems = !bOldState;
}

long  CEOSAIUnitsubsetPermission::GetNumberOfPositiveUnitsubsets()
{
	long iCount = 0;
	POSITION pos = m_pGameRules->GetUnitsubsetList()->GetHeadPosition();
	while( pos )
	{
		CString strUnitsubset = m_pGameRules->GetUnitsubsetList()->GetNext( pos );
		if( UnitsubsetIsPositive( strUnitsubset ) )
		{
			iCount++;
		}
	}
	return iCount;
}

bool  CEOSAIUnitsubsetPermission::AllUnitsubsetsArePositive()
{
	return GetNumberOfPositiveUnitsubsets() == m_pGameRules->GetUnitsubsetList()->GetCount();
}

void  CEOSAIUnitsubsetPermission::Serialize( CEOSAISerial* pSerial )
{
	char cVersion = 1;
	pSerial->Serialize( cVersion );

	pSerial->Serialize( m_bListContainsPositiveItems );

	long iCount = m_Unitsubsets.GetCount();
	pSerial->Serialize( iCount );
	POSITION pos = m_Unitsubsets.GetHeadPosition();
	while( pos )
	{
		CString strUnitsubset = m_Unitsubsets.GetNext( pos );
		pSerial->SerializeANSI8( strUnitsubset );
	}
}

void  CEOSAIUnitsubsetPermission::Deserialize( CEOSAISerial* pSerial )
{
	char cVersion = 1;
	pSerial->Deserialize( cVersion );

	pSerial->Deserialize( m_bListContainsPositiveItems );

	long iCount = 0;
	pSerial->Deserialize( iCount );
	for( long i=0; i<iCount; i++ )
	{
		CString strUnitsubset;
		pSerial->DeserializeANSI8( strUnitsubset );
		m_Unitsubsets.AddTail( strUnitsubset );
	}
}
/*
void  CEOSAIUnitsubsetPermission::ReadXMLData( CBCXMLItem* pItem )
{
	//m_strItemName = pItem->GetValue( "ItemName" );
	m_bListContainsPositiveItems = pItem->GetValue( ANSI8("Pos") ) == ANSI8("1") ? true : false;

	long iCount = pItem->GetCount( ANSI8("Unitsubset") );
	for( long i=0; i<iCount; i++ )
	{
		CStringANSI8 strVal = pItem->GetValue( ANSI8("Unitsubset") );
		m_Unitsubsets.AddTail( CString( strVal ) );
	}
}
*/
#ifdef DO_NOT_COMPILE
void  CEOSAIUnitsubsetPermission::AppendDataToXMLString( CStringANSI8& strData )
{
	//strData += "<Unitsubsets>";
	//strData += "<ItemName>" + m_strItemName + "</ItemName>\n";

	strData += _T("<Pos>") + CBCString::BoolAsString10( m_bListContainsPositiveItems ) + _T("</Pos>\n");

	POSITION pos = m_Unitsubsets.GetHeadPosition();
	while( pos )
	{
		strData += _T("<Unitsubset>") + m_Unitsubsets.GetNext( pos ) + _T("</Unitsubset>\n");
	}
	//strData += "</Unitsubsets>\n";
}
#endif DO_NOT_COMPILE

bool  CEOSAIUnitsubsetPermission::ExistsInList( CString strUnitsubset )
{
	POSITION pos = m_Unitsubsets.GetHeadPosition();
	while( pos )
	{
		CString strUnitsubsetInList = m_Unitsubsets.GetNext( pos );
		if( strUnitsubset == strUnitsubsetInList ){ return true; }
	}
	return false;
}

