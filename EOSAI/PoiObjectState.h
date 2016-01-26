
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include <afxtempl.h>
#include "EOSAILocation.h"
//#include "AIPotentialActions.h"
class CEOSAIPoiObject;
namespace EOSAI
{
	class PoiMobile;
}
class CEOSAIUnit;
//class CWorldDesc;
class CEOSAIBrain;
class CEOSAIPathfinderPoint;
class CEOSAIRegion2;
class CEOSAIMultiRegion2;

namespace EOSAI
{
class DLLIMPEXP PoiObjectState
{
public:

	EOSAI::PoiObjectState();
	//CAIPoiObjectState( CEOSAIBrain* pAIBrain );

	//void         SetAIBrain( CEOSAIBrain* pAIBrain ){ m_pAIBrain = pAIBrain; }
	void         SetGroundUnit(bool b) { m_bGroundUnit = b; }
	//CWorldDesc*  GetWorldDesc();
	//CEOSAIBrain*    GetAIBrain(){ return m_pAIBrain; }

	//
	// Owner
	//
	void  SetOwner(long iOwner) { m_iOwner = iOwner; }
	long  GetOwner() { return m_iOwner; }

	//
	// Location/MultiRegion/Geo
	//
	void          SetLocationAndRegions(CEOSAILocation location);
	void          SetLocationAndRegions(CEOSAIMultiRegion2* pMultiRegion);
	void          SetLocationAndRegions(CEOSAIRegion2* pAIRegion);
	void          SetLocationAndRegions(CEOSAILocation location, CEOSAIMultiRegion2* pMultiRegion);//{ m_Location = location; m_pMultiRegion = pMultiRegion; }
	void          SetLocationAndRegions(CEOSAILocation location, CEOSAIRegion2* pAIRegion);//{ m_Location = location; m_pAIRegion = pAIRegion; }
	CEOSAILocation&    GetLocation() { return m_Location; }
	CEOSAILocation     GetLocation(long iTurn) { ASSERT(false); return m_Location; }

	CEOSAIRegion2*              GetAIRegion() { return m_pAIRegion; }
	CEOSAIPathfinderPoint*   GetPathfinderPoint() { return m_pPathfinderPoint; }
	CEOSAIMultiRegion2*           GetMultiRegion() { return m_pMultiRegion; }

	long  GetGeo();
	//
	// Container
	//
	void           SetContainer(CEOSAIPoiObject* pContainer);
	CEOSAIPoiObject* GetContainer() { return m_pContainer; }
	bool           IsInsideATransport();
	bool           IAmContainedInside(CEOSAIPoiObject* pAIPoiObject);
	bool           Contains(CEOSAIPoiObject* pAIPoiObject);
	long           GetNumberOfContainees();
	//EOSAI::PoiMobile* GetHighestPoiMobileContainer();
	//CEOSAIUnit*      GetHighestUnitContainer();
	CEOSAIUnit*      GetAIUnitContainer();

	void           AddContainee(EOSAI::PoiMobile* pObj) { m_Containees.AddTail(pObj); }
	void           RemoveContainee(EOSAI::PoiMobile* pObj);
	CList< EOSAI::PoiMobile* >* GetContaineesList() { return &m_Containees; }

	void           SetAirbase(CEOSAIPoiObject* pAirbase) { m_pAirbase = pAirbase; }
	CEOSAIPoiObject* GetAirbase() { return m_pAirbase; }

	//
	// Time
	//
	virtual void  SetTime(float f) { m_fTime = f; }
	float         GetTime() { return m_fTime; }

	//
	// HP
	//
	void   SetHP(float f) { m_fHP = f; }
	float  GetHP() { return m_fHP; }

	//
	//
	void operator=(EOSAI::PoiObjectState& Other)
	{
		ASSERT(false);
	}
	//
	void Copy(EOSAI::PoiObjectState* pOther);
	bool operator==(EOSAI::PoiObjectState& Other)
	{
		ASSERT(false);
		return (m_Location == Other.m_Location && m_fTime == Other.m_fTime);
	}
	bool SameLocationAndTime(EOSAI::PoiObjectState* pOther)
	{
		return (m_Location == pOther->m_Location && m_fTime == pOther->m_fTime);
	}

protected:

	//CEOSAIBrain* m_pAIBrain;
	float  m_fTime;
	long   m_iOwner;

	bool   m_bGroundUnit; // Used when finding the PathfindingPoint

	float  m_fHP;
	//float     m_fOddsOfSurvival; // 0-1 Used in combat planning

	CEOSAILocation         m_Location;
	CEOSAIPathfinderPoint* m_pPathfinderPoint;
	CEOSAIRegion2*        m_pAIRegion;
	CEOSAIMultiRegion2*    m_pMultiRegion;
	CList< CEOSAIMultiRegion2* >  m_MultiRegionsLeadingToHere; // the sequence of MultiRegions from the last state to this one

	CEOSAIPoiObject*    m_pContainer;
	CEOSAIPoiObject*    m_pAirbase; // landing location I was last in
	CList< EOSAI::PoiMobile* > m_Containees;
};

}