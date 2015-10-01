
#pragma once

#include "EOSAIUnitPathwayResultStep.h"
#include "EOSAIAirUnitPathwayData.h"
class CEOSAIUnit2PathwayFinder;
class CEOSAIPoiObject;
//class CUnitTemplate;

namespace EOSAI
{
class UnitPathwayResult
{
public:
	UnitPathwayResult(CEOSAIUnit2PathwayFinder* pPathwayFinder)
	{
		ASSERT(pPathwayFinder != NULL);
		m_pAIUnitPathwayFinder = pPathwayFinder;
		Clear();
	}
	~UnitPathwayResult();
	//
	void  Clear();
	bool  PathIsValid() { return m_bPathIsValid; }
	void  UseTransport(CEOSAIUnit2* pAITransport);
	//
	bool  CreateOrders();
	bool  AllOrdersWereCreated() { return m_bAllOrdersWereCreated; }
	//
	CEOSAIUnit2PathwayFinder*  m_pAIUnitPathwayFinder;
	//CEOSAIPoiObject*   m_pAIActor;
	//CUnitTemplate*  m_pUnitTemplate;

	bool  m_bPathIsValid;

	// Summary information (compilation of all the data in the steps)
	//
	CAIAirResistanceSet  m_Resistance; // Used by aircraft
	float  m_fPathfindingValue; // TEMPORARY - because I merged this with the AirUnitPathwayResult
	float  m_fCompletionTime;
	float  m_fAttackTime;     // TEMPORARY - because I merged this with the AirUnitPathwayResult
	bool   m_bResultsInDeath; // TEMPORARY - because I merged this with the AirUnitPathwayResult
	float  m_fSurvivalOdds;   // TEMPORARY - because I merged this with the AirUnitPathwayResult

// Pathway results
//
	CList< EOSAI::UnitPathwayResultStep* >  m_Steps;

	bool  m_bAllOrdersWereCreated;
};
}
