
#include <stdafx.h>
#include "EOSAIUnitTemplateSet.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIBrain.h"
#include "EOSAIJobsToDo.h"
//#include "AIUnitTemplateAndFloatSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void  CEOSAIUnitTemplateSet::Clear()
{
	m_List.RemoveAll();
	m_bSummaryInformationIsValid = false;
}

void  CEOSAIUnitTemplateSet::AddUnitTemplate( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplateInList = m_List.GetNext( pos );
		if( pAIUnitTemplate == pAIUnitTemplateInList ) return; // already exists
	}
	m_List.AddTail( pAIUnitTemplate );
	m_bSummaryInformationIsValid = false;
}

void  CEOSAIUnitTemplateSet::Set( CEOSAIUnitTemplateSet* pOtherSet )
{
	m_pAICitResValueMultiplier = pOtherSet->m_pAICitResValueMultiplier;

	m_List.RemoveAll();
	POSITION pos = pOtherSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplateInList = pOtherSet->m_List.GetNext( pos );
		m_List.AddTail( pAIUnitTemplateInList );
	}

	m_bSummaryInformationIsValid = false;
}

//void CEOSAIUnitTemplateSet::GetUnitsToCompleteJobs( CEOSAIUnitTemplateSet* pInputSet, CEOSAIUnitTemplateSet* pOutputSet )
void CEOSAIUnitTemplateSet::SetToUsefulUnits( CEOSAIJobsToDo& Jobs, CEOSAIUnitTemplateSet* pOtherSet )
{
	Clear();
	AddUsefulUnits( Jobs, pOtherSet );
}

void  CEOSAIUnitTemplateSet::AddUsefulUnits( CEOSAIJobsToDo& Jobs, CEOSAIUnitTemplateSet* pOtherSet )
{
	if( Jobs.IsEmpty() )//.GetNumberOfActiveJobs() == 0 )
	{
		ASSERT( false );
		return;
	}
	POSITION pos = pOtherSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = pOtherSet->m_List.GetNext( pos );
		if( pAIUnitTemplate->IsTransport() ||
			pAIUnitTemplate->IsAirUnitCarrier() ||
			Jobs.CanFulfillAJob( pAIUnitTemplate ) )
		{
			this->AddUnitTemplate( pAIUnitTemplate );
		}
	}
	/*
	if( Jobs.HasCombatTask() )
	{
		this->Add( pOtherSet ); // If there's a combat task, add all units (because they can all fight)
		return; // no need to look at any other jobs since they're all added
	}

	if( Jobs.m_iAirfieldBuilder > 0 ) // limit it to airfield builders + transports
	{
		this->Add( CEOSAIUnitTemplateSet::AirfieldBuildersAndTheirTransports, pOtherSet );
	}
	if( Jobs.m_iGroundCitResHunter > 0 )
	{
		this->Add( CEOSAIUnitTemplateSet::GroundCitResHuntersAndTheirTransports, pOtherSet );
	}
	if( Jobs.m_iSeaResHunter > 0 )
	{
		this->Add( CEOSAIUnitTemplateSet::SeaResHunters, pOtherSet );
	}
	if( Jobs.m_iDegradeCity > 0 )
	{
		this->Add( CEOSAIUnitTemplateSet::StrategicBombingAndTheirTransports, pOtherSet );
	}
	*/
	//
}

/*
void  CEOSAIUnitTemplateSet::Filter( EnumUnitTemplateFilter eFilter )
{
	CEOSAIUnitTemplateSet  Primary;
	CEOSAIUnitTemplateSet  Transports;

	Filter( eFilter, &Primary, &Transports );

	//
	pos = AirfieldBuilders.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pAirfieldBuilder = AirfieldBuilders.GetNext( pos );
		AddUnitTemplate( pAirfieldBuilder );
	}
	pos = Transports.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pTransport = Transports.GetNext( pos );
		AddUnitTemplate( pTransport );
	}
}
*/

void  CEOSAIUnitTemplateSet::Add( CEOSAIUnitTemplateSet* pOtherSet )
{
	POSITION pos = pOtherSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = pOtherSet->m_List.GetNext( pos );
		AddUnitTemplate( pAIUnitTemplate );
	}
}

void  CEOSAIUnitTemplateSet::Add( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplateSet* pOtherSet )
{
	CEOSAIUnitTemplateSet PrimaryUnits;
	CEOSAIUnitTemplateSet Transports;
	pOtherSet->OutputSets( eFilter, &PrimaryUnits, &Transports );
	Add( &PrimaryUnits );
	Add( &Transports );
	/*
	if( eFilter == AirfieldBuildersAndTheirTransports )
	{
		CList< CUnitTemplate* >  PrimaryUnits;
		POSITION pos = pOtherSet->m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pPrimaryUnit = pOtherSet->m_List.GetNext( pos );
			if( pPrimaryUnit->CanBuildAirfields() )
			{
				PrimaryUnits.AddTail( pPrimaryUnit );
				AddUnitTemplate( pPrimaryUnit );
			}
		}
		pos = pOtherSet->m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pTransport = pOtherSet->m_List.GetNext( pos );
			if( pTransport->CanContainUnits() )
			{
				POSITION pos2 = PrimaryUnits.GetHeadPosition();
				while( pos2 )
				{
					CUnitTemplate* pPrimaryUnit = PrimaryUnits.GetNext( pos2 );
					if( pTransport->CanContain( pPrimaryUnit ) )
					{
						AddUnitTemplate( pPrimaryUnit );
						break;
					}
				}
			}
		}
	}
	*/

}

//void  CEOSAIUnitTemplateSet::OutputFilteredSets( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplateSet* pPrimaryUnits, CEOSAIUnitTemplateSet* pTransports )
void  CEOSAIUnitTemplateSet::OutputSets( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplateSet* pPrimaryUnits, CEOSAIUnitTemplateSet* pTransports )
{
	pPrimaryUnits->Clear();
	if( pTransports ){ pTransports->Clear(); }

	if( eFilter == AirfieldBuildersAndTheirTransports )
	{
		POSITION pos = m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pPrimaryUnit = m_List.GetNext( pos );
			if( pPrimaryUnit->CanBuildAirfields() )
			{
				pPrimaryUnits->AddUnitTemplate_Unsafe( pPrimaryUnit );
			}
		}
		if( pTransports )
		{
			pos = m_List.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnitTemplate* pTransport = m_List.GetNext( pos );
				if( pTransport->CanContainUnits() )
				{
					POSITION pos2 = pPrimaryUnits->m_List.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnitTemplate* pPrimaryUnit = pPrimaryUnits->m_List.GetNext( pos2 );
						if( pTransport->CanContain( pPrimaryUnit ) )
						{
							pTransports->AddUnitTemplate_Unsafe( pTransport );
							break;
						}
					}
				}
			}
		}
	}
	if( eFilter == GroundCitResHuntersAndTheirTransports )
	{
		POSITION pos = m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pPrimaryUnit = m_List.GetNext( pos );
			if( pPrimaryUnit->IsGroundUnit() && pPrimaryUnit->IsCityHunter() )
			{
				pPrimaryUnits->AddUnitTemplate_Unsafe( pPrimaryUnit );
			}
		}
		if( pTransports )
		{
			pos = m_List.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnitTemplate* pTransport = m_List.GetNext( pos );
				if( pTransport->CanContainUnits() )
				{
					POSITION pos2 = pPrimaryUnits->m_List.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnitTemplate* pPrimaryUnit = pPrimaryUnits->m_List.GetNext( pos2 );
						if( pTransport->CanContain( pPrimaryUnit ) )
						{
							pTransports->AddUnitTemplate_Unsafe( pTransport );
							break;
						}
					}
				}
			}
		}
	}
	if( eFilter == SeaResHunters )
	{
		POSITION pos = m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pPrimaryUnit = m_List.GetNext( pos );
			if( pPrimaryUnit->IsSeaUnit() && pPrimaryUnit->CanCaptureResources() )
			{
				pPrimaryUnits->AddUnitTemplate_Unsafe( pPrimaryUnit );
			}
		}
	}
	if( eFilter == StrategicBombingAndTheirTransports )
	{
		POSITION pos = m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate* pPrimaryUnit = m_List.GetNext( pos );
			if( pPrimaryUnit->CanTargetCities() )
			{
				pPrimaryUnits->AddUnitTemplate_Unsafe( pPrimaryUnit );
			}
		}
		if( pTransports )
		{
			pos = m_List.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnitTemplate* pTransport = m_List.GetNext( pos );
				if( pTransport->CanContainUnits() )
				{
					POSITION pos2 = pPrimaryUnits->m_List.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnitTemplate* pPrimaryUnit = pPrimaryUnits->m_List.GetNext( pos2 );
						if( pTransport->CanContain( pPrimaryUnit ) )
						{
							pTransports->AddUnitTemplate_Unsafe( pTransport );
							break;
						}
					}
				}
			}
		}
	}
}
/*
void  CEOSAIUnitTemplateSet::OutputSets( CEOSAIJobsToDo& Jobs, CList< CEOSAIUnitTemplateSet* >* pUnitTemplateSets )
{
	
}
*/
void  CEOSAIUnitTemplateSet::GetTransports( CEOSAIUnitTemplate* pTraveller, CEOSAIUnitTemplateSet* pOutTransports )
{
	pOutTransports->Clear();

	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAITransport = m_List.GetNext( pos );
		if( pAITransport->CanContainUnits() && pAITransport->CanContain( pTraveller ) )
		{
			pOutTransports->AddUnitTemplate_Unsafe( pAITransport );
		}
	}
}

/*
void  CEOSAIUnitTemplateSet::Filter( CEOSAITaskForceDesc* pSimpleTaskForceDesc )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CUnitTemplate* pUnitTemplateInList = m_List.GetNext( pos );

		//if( pSimpleTaskForceDesc->m_bAirfieldBuilder )
		//{
		//}

		//m_List.AddTail( pUnitTemplateInList );
	}
}
*/
void  CEOSAIUnitTemplateSet::UpdateSummaryInformation()
{
	if( m_bSummaryInformationIsValid ) return; // nothing to update

	// Clear data
	m_fFastestAirUnitMovementRate = 0.0f;
	m_fLargestAirUnitMovementRange = 0.0f;
	m_fMaximumAirUnitHP = 0.0f;
	//
	m_fFastestGroundUnitMovementRate = 0.0f;
	m_fFastestSeaUnitMovementRate = 0.0f;
	//
	m_fFastestSeaTransportMovementRate = 0.0f;
	m_fFastestAirTransportMovementRate = 0.0f;
	m_fLargestAirTransportMovementRange = 0.0f;
	//
	m_fFastestAircraftCarrierMovementRate = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );

		// Air Units
		if( pAIUnitTemplate->IsAirUnit() )
		{
			if( pAIUnitTemplate->GetMovementRate() > m_fFastestAirUnitMovementRate )
			{
				m_fFastestAirUnitMovementRate = pAIUnitTemplate->GetMovementRate();
			}
			if( pAIUnitTemplate->GetMovementLimit() > m_fLargestAirUnitMovementRange )
			{
				m_fLargestAirUnitMovementRange = pAIUnitTemplate->GetMovementLimit();
			}
			if( pAIUnitTemplate->GetMaxHP() > m_fMaximumAirUnitHP )
			{
				m_fMaximumAirUnitHP = (float) pAIUnitTemplate->GetMaxHP();
			}
		}
		// Ground Units
		if( pAIUnitTemplate->IsGroundUnit() )
		{
			if( pAIUnitTemplate->GetMovementRate() > m_fFastestGroundUnitMovementRate )
			{
				m_fFastestGroundUnitMovementRate = pAIUnitTemplate->GetMovementRate();
			}
			if( pAIUnitTemplate->GetMaxHP() > m_fMaximumGroundUnitHP )
			{
				m_fMaximumGroundUnitHP = (float) pAIUnitTemplate->GetMaxHP();
			}
		}
		// Sea Units
		if( pAIUnitTemplate->IsSeaUnit() )
		{
			if( pAIUnitTemplate->GetMovementRate() > m_fFastestSeaUnitMovementRate )
			{
				m_fFastestSeaUnitMovementRate = pAIUnitTemplate->GetMovementRate();
			}
			if( pAIUnitTemplate->GetMaxHP() > m_fMaximumSeaUnitHP )
			{
				m_fMaximumSeaUnitHP = (float) pAIUnitTemplate->GetMaxHP();
			}
		}

		// Transports
		if( pAIUnitTemplate->IsSeaUnit() &&
			pAIUnitTemplate->IsTransport() )
		{
			if( pAIUnitTemplate->GetMovementRate() > m_fFastestSeaTransportMovementRate )
			{
				m_fFastestSeaTransportMovementRate = pAIUnitTemplate->GetMovementRate();
			}
		}
		if( pAIUnitTemplate->IsAirUnit() &&
			pAIUnitTemplate->IsTransport() )
		{
			if( pAIUnitTemplate->GetMovementRate() > m_fFastestAirTransportMovementRate )
			{
				m_fFastestAirTransportMovementRate = pAIUnitTemplate->GetMovementRate();
			}
			if( pAIUnitTemplate->GetMovementLimit() > m_fLargestAirTransportMovementRange )
			{
				m_fLargestAirTransportMovementRange = pAIUnitTemplate->GetMovementLimit();
			}
		}

		// Aircraft Carrier
		if( pAIUnitTemplate->GetNumberOfAirUnitsThatICanContain() > 0 )
		{
			if( pAIUnitTemplate->GetMovementRate() > m_fFastestAircraftCarrierMovementRate )
			{
				m_fFastestAircraftCarrierMovementRate = pAIUnitTemplate->GetMovementRate();
			}
		}
	}

	//
	// Unit Cost / Capability
	//
	//ASSERT( m_pAIBrain );
	m_fLowestCostDivGroundCitResHunter = 1000000.0f;
	m_fLowestCostDivSeaResHunter = 1000000.0f;
	m_fLowestCostDivAirfieldBuilder = 1000000.0f;
	//
	for( long i=0; i<MAX_TRANSPORT_CAPACITY+1; i++ ){ m_fLowestCostDivTransportCapacity[i] = 1000000.0f; }
	for( long i=0; i<MAX_TRANSPORT_CAPACITY+1; i++ ){ m_fLowestCostDivCarrierCapacity[i] = 1000000.0f; }
	//
/*
	m_fLowestCostDivAntiSoftCombat = 1000000.0f;
	m_fLowestCostDivAntiArmorCombat = 1000000.0f;
	m_fLowestCostDivAntiAircraftCombat = 1000000.0f;
	m_fLowestCostDivAntiMissileCombat = 1000000.0f;
	m_fLowestCostDivAntiShipCombat = 1000000.0f;
	m_fLowestCostDivAntiSubmarineCombat = 1000000.0f;
*/
	//
	/*
	m_fHighestCostDivUnit = 0.0f;
	m_fHighestCostDivSoftUnit = 0.0f;
	m_fHighestCostDivArmorUnit = 0.0f;
	m_fHighestCostDivAircraftUnit = 0.0f;
	m_fHighestCostDivMissileUnit = 0.0f;
	m_fHighestCostDivShipUnit = 0.0f;
	m_fHighestCostDivSubmarineUnit = 0.0f;
	*/
	//
/*
	m_fGroundUnits_LowestSizeInContainerDivAntiSoftCombat = 1000000.0f;
	m_fGroundUnits_LowestSizeInContainerDivAntiArmorCombat = 1000000.0f;
*/
	//HighestHPDefenseDivCost_UnitTypeSet2.SetValue( "Armor", 4.0f );
	//HighestHPDefenseDivCost_UnitTypeSet2.SetValue( "Armor", 5.0f );
	//HighestHPDefenseDivCost_UnitTypeSet2.SetValue( "Inf", 6.0f );
// find the CombatType class - maybe create an intelligent List/Template (indexable by a string)


	//
	m_bSummaryInformationIsValid = true;
}

float CEOSAIUnitTemplateSet::GetFastestUnitMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplateSet::GetFastestAirfieldBuilderAndTransportMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->CanBuildAirfields() )
		{
			fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );

			float fTransportMovementRate = GetFastestTransportMovementRate( pAIUnitTemplate );
			fFastestMovementRate = max( fFastestMovementRate, fTransportMovementRate );
		}
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplateSet::GetFastestGroundCitResHunterAndTransportMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->IsGroundUnit() &&
		  ( pAIUnitTemplate->CanCaptureCities() || pAIUnitTemplate->CanCaptureResources() ) )
		{
			fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );
		}
		fFastestMovementRate = GetFastestTransportMovementRate();
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplateSet::GetFastestSeaResHunterMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->IsSeaUnit() && pAIUnitTemplate->CanCaptureResources() )
		{
			fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );
		}
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplateSet::GetFastestTransportMovementRate( CEOSAIUnitTemplate* pTransportee )
{
	float fFastestTransport = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->CanContain( pTransportee ) )
		{
			fFastestTransport = max( fFastestTransport, pAIUnitTemplate->GetMovementRate() );
		}
	}
	return fFastestTransport;
}

float CEOSAIUnitTemplateSet::GetFastestSeaTransportMovementRate( CEOSAIUnitTemplate* pTransportee )
{
	float fFastestTransport = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->IsSeaUnit() && pAIUnitTemplate->CanContain( pTransportee ) )
		{
			fFastestTransport = max( fFastestTransport, pAIUnitTemplate->GetMovementRate() );
		}
	}
	return fFastestTransport;
}

// For each UnitTemplate in this set, how much production do I need to spend to 
//   cause 1 production damage to target?
/*
void CEOSAIUnitTemplateSet::GetAttritionCostVs( CEOSAIUnitTemplate* pTarget, CUnitTemplateAndFloatSet* pResults )
{
	ASSERT( false );

	pResults->Clear();

	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );

		// For each UnitTemplate, calculate the value of this unit against each enemy unit
		//   This value will be stored in several forms: attrition, combat-time
		//   The ultimate goal here is to be able to compare my units against each other
		// I should be able to calculate a unit's combat value (and even an estimated production cost)
		// This will allow be to do a couple things:
		//   Is "Infantry" a better all-around unit than "Tank"?
		//   Are there some places where "Infantry" excels over "Tank"?
		//   What is the best way to attack unit X (given all my build options, from an attrition standpoint)?
		// 
		//if( pUnitTemplate->GetUnitCombatCapability()->InvokeValue( pTarget ) == false )
		//{
		//}

		//CAICombatPlanner  
	}
}
*/
/*
float CEOSAIUnitTemplateSet::GetBestMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplateSet::GetBestGroundUnitMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsGroundUnit() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplateSet::GetBestGroundTransportMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsGroundTransport() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplateSet::GetBestSeaUnitMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsSeaUnit() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplateSet::GetBestAirUnitMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsAirUnit() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplateSet::GetBestAirUnitMovementRange()
{
	float fBestRange = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsAirUnit() )
		{
			if( pAIUnitTemplate->GetUnitTemplate()->HasARange() &&
				pAIUnitTemplate->GetUnitTemplate()->GetMovementLimit() > fBestRange )
			{
				fBestRange = pAIUnitTemplate->GetUnitTemplate()->GetMovementLimit();
			}
			else
			{
				fBestRange = 1000000.0f;
			}
		}
	}

	return fBestRange;
}
*/
