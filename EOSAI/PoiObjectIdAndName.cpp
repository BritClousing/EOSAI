#include "stdafx.h"
#include "PoiObjectIdAndName.h"

void EOSAI::PoiObjectIdAndName::Serialize(CEOSAISerial* pSerial)
{
	long iVersion = 1000;
	pSerial->Serialize(iVersion);
	pSerial->Serialize(m_iOwner);

	long iPoiObjectType = (long)m_ePoiObjectType.m_v;
	pSerial->Serialize(iPoiObjectType);

	float fDummyGameTime=0.0f;
	pSerial->Serialize(fDummyGameTime);
	//m_GameTime.Serialize(pSerial);

	pSerial->Serialize(m_iObjectId);
	pSerial->SerializeANSI8(m_strName);
	pSerial->Serialize(m_iProduction);
	pSerial->Serialize(m_bLandResource);
	pSerial->SerializeANSI8(m_strType);
	/*
	Old data
	pSerial->Serialize( m_iOwner );

	long iPoiObjectType = (long) m_ePoiObjectType.m_v;
	pSerial->Serialize( iPoiObjectType );

	m_GameTime.Serialize( pSerial );

	pSerial->Serialize( m_iObjectId );
	pSerial->SerializeANSI8( m_strName );
	pSerial->Serialize( m_iProduction );
	pSerial->SerializeANSI8( m_strType );
	*/
}

void EOSAI::PoiObjectIdAndName::Deserialize(CEOSAISerial* pSerial)
{
	long iVersion = 0;
	pSerial->Deserialize(iVersion);
	if (iVersion >= 1000)
	{
		pSerial->Deserialize(m_iOwner);

		long iPoiObjectType = 0;
		pSerial->Deserialize(iPoiObjectType);
		m_ePoiObjectType.m_v = (EnumAIPoiObjectType::Value) iPoiObjectType;

		float fDummyGameTime = 0.0f;
		pSerial->Deserialize(fDummyGameTime);
		//m_GameTime.Deserialize(pSerial);

		pSerial->Deserialize(m_iObjectId);
		pSerial->DeserializeANSI8(m_strName);
		pSerial->Deserialize(m_iProduction);
		pSerial->Deserialize(m_bLandResource);
		pSerial->DeserializeANSI8(m_strType);
	}
	else
	{
		// Old Version
		m_iOwner = iVersion;

		long iPoiObjectType = 0;
		pSerial->Deserialize(iPoiObjectType);
		m_ePoiObjectType.m_v = (EnumAIPoiObjectType::Value) iPoiObjectType;

		float fDummyGameTime = 0.0f;
		pSerial->Deserialize(fDummyGameTime);
		//m_GameTime.Deserialize(pSerial);

		pSerial->Deserialize(m_iObjectId);
		pSerial->DeserializeANSI8(m_strName);
		pSerial->Deserialize(m_iProduction);
		pSerial->DeserializeANSI8(m_strType);
	}
}
