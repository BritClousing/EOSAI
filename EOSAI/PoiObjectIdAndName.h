#pragma once

#include "EOSAIEnumPoiObjectType.h"
#include "EOSAISerial.h"

namespace EOSAI
{
class PoiObjectIdAndName
{
public:
	PoiObjectIdAndName() { m_iOwner = 0; m_ePoiObjectType = EnumAIPoiObjectType::enum_Undefined; m_iObjectId = 0; m_iProduction = 0; m_bLandResource = true; }

	PoiObjectIdAndName* Duplicate()
	{
		PoiObjectIdAndName* pNew = new PoiObjectIdAndName();
		*pNew = *this;
		/*
		pNew->m_iOwner = m_iOwner;
		pNew->m_ePoiObjectType = m_ePoiObjectType;
		pNew->m_GameTime = m_GameTime;
		pNew->m_iObjectId = m_iObjectId;
		pNew->m_strName = m_strName;
		pNew->m_iProduction = m_iProduction;
		pNew->m_strType = m_strType;
		*/
		return pNew;
	}
	void operator=(PoiObjectIdAndName& other)
	{
		m_iOwner = other.m_iOwner;
		m_ePoiObjectType = other.m_ePoiObjectType;
		//m_GameTime = other.m_GameTime;
		m_iObjectId = other.m_iObjectId;
		m_strName = other.m_strName;
		m_iProduction = other.m_iProduction;
		m_bLandResource = other.m_bLandResource;
		m_strType = other.m_strType;
	}

	void Serialize(CEOSAISerial* pSerial);
	void Deserialize(CEOSAISerial* pSerial);

	long    m_iOwner;
	EnumAIPoiObjectType  m_ePoiObjectType;
	//
	//CGameTime  m_GameTime; // used only when it's stored in the WorldDesc
							//
	long    m_iObjectId;
	CString m_strName;
	long    m_iProduction; // Used by cities and resources
	bool    m_bLandResource;
	CString m_strType;     // UnitTemplate or ResourceType
};
}