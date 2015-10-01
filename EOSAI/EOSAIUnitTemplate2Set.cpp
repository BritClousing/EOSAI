
#include <stdafx.h>
#include "EOSAIUnitTemplate2Set.h"
#include "EOSAIUnitTemplate2.h"
#include "EOSAIBrain.h"
#include "EOSAIJobsToDo.h"
//#include "AIUnitTemplateAndFloatSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void  CEOSAIUnitTemplate2Set::Clear()
{
	m_List.RemoveAll();
	m_bSummaryInformationIsValid = false;
}

void  CEOSAIUnitTemplate2Set::AddUnitTemplate( CEOSAIUnitTemplate2* pAIUnitTemplate )
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplateInList = m_List.GetNext( pos );
		if( pAIUnitTemplate == pAIUnitTemplateInList ) return; // already exists
	}
	m_List.AddTail( pAIUnitTemplate );
	m_bSummaryInformationIsValid = false;
}

void  CEOSAIUnitTemplate2Set::Set( CEOSAIUnitTemplate2Set* pOtherSet )
{
	m_pAICitResValueMultiplier = pOtherSet->m_pAICitResValueMultiplier;

	m_List.RemoveAll();
	POSITION pos = pOtherSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplateInList = pOtherSet->m_List.GetNext( pos );
		m_List.AddTail( pAIUnitTemplateInList );
	}

	m_bSummaryInformationIsValid = false;
}

//void CEOSAIUnitTemplate2Set::GetUnitsToCompleteJobs( CEOSAIUnitTemplate2Set* pInputSet, CEOSAIUnitTemplate2Set* pOutputSet )
void CEOSAIUnitTemplate2Set::SetToUsefulUnits( CEOSAIJobsToDo& Jobs, CEOSAIUnitTemplate2Set* pOtherSet )
{
	Clear();
	AddUsefulUnits( Jobs, pOtherSet );
}

void  CEOSAIUnitTemplate2Set::AddUsefulUnits( CEOSAIJobsToDo& Jobs, CEOSAIUnitTemplate2Set* pOtherSet )
{
	if( Jobs.IsEmpty() )//.GetNumberOfActiveJobs() == 0 )
	{
		ASSERT( false );
		return;
	}
	POSITION pos = pOtherSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = pOtherSet->m_List.GetNext( pos );
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
		this->Add( CEOSAIUnitTemplate2Set::AirfieldBuildersAndTheirTransports, pOtherSet );
	}
	if( Jobs.m_iGroundCitResHunter > 0 )
	{
		this->Add( CEOSAIUnitTemplate2Set::GroundCitResHuntersAndTheirTransports, pOtherSet );
	}
	if( Jobs.m_iSeaResHunter > 0 )
	{
		this->Add( CEOSAIUnitTemplate2Set::SeaResHunters, pOtherSet );
	}
	if( Jobs.m_iDegradeCity > 0 )
	{
		this->Add( CEOSAIUnitTemplate2Set::StrategicBombingAndTheirTransports, pOtherSet );
	}
	*/
	//
}

/*
void  CEOSAIUnitTemplate2Set::Filter( EnumUnitTemplateFilter eFilter )
{
	CEOSAIUnitTemplate2Set  Primary;
	CEOSAIUnitTemplate2Set  Transports;

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

void  CEOSAIUnitTemplate2Set::Add( CEOSAIUnitTemplate2Set* pOtherSet )
{
	POSITION pos = pOtherSet->m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = pOtherSet->m_List.GetNext( pos );
		AddUnitTemplate( pAIUnitTemplate );
	}
}

void  CEOSAIUnitTemplate2Set::Add( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplate2Set* pOtherSet )
{
	CEOSAIUnitTemplate2Set PrimaryUnits;
	CEOSAIUnitTemplate2Set Transports;
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

//void  CEOSAIUnitTemplate2Set::OutputFilteredSets( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplate2Set* pPrimaryUnits, CEOSAIUnitTemplate2Set* pTransports )
void  CEOSAIUnitTemplate2Set::OutputSets( EnumUnitTemplateFilter eFilter, CEOSAIUnitTemplate2Set* pPrimaryUnits, CEOSAIUnitTemplate2Set* pTransports )
{
	pPrimaryUnits->Clear();
	if( pTransports ){ pTransports->Clear(); }

	if( eFilter == AirfieldBuildersAndTheirTransports )
	{
		POSITION pos = m_List.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnitTemplate2* pPrimaryUnit = m_List.GetNext( pos );
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
				CEOSAIUnitTemplate2* pTransport = m_List.GetNext( pos );
				if( pTransport->CanContainUnits() )
				{
					POSITION pos2 = pPrimaryUnits->m_List.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnitTemplate2* pPrimaryUnit = pPrimaryUnits->m_List.GetNext( pos2 );
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
			CEOSAIUnitTemplate2* pPrimaryUnit = m_List.GetNext( pos );
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
				CEOSAIUnitTemplate2* pTransport = m_List.GetNext( pos );
				if( pTransport->CanContainUnits() )
				{
					POSITION pos2 = pPrimaryUnits->m_List.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnitTemplate2* pPrimaryUnit = pPrimaryUnits->m_List.GetNext( pos2 );
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
			CEOSAIUnitTemplate2* pPrimaryUnit = m_List.GetNext( pos );
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
			CEOSAIUnitTemplate2* pPrimaryUnit = m_List.GetNext( pos );
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
				CEOSAIUnitTemplate2* pTransport = m_List.GetNext( pos );
				if( pTransport->CanContainUnits() )
				{
					POSITION pos2 = pPrimaryUnits->m_List.GetHeadPosition();
					while( pos2 )
					{
						CEOSAIUnitTemplate2* pPrimaryUnit = pPrimaryUnits->m_List.GetNext( pos2 );
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
void  CEOSAIUnitTemplate2Set::OutputSets( CEOSAIJobsToDo& Jobs, CList< CEOSAIUnitTemplate2Set* >* pUnitTemplateSets )
{
	
}
*/
void  CEOSAIUnitTemplate2Set::GetTransports( CEOSAIUnitTemplate2* pTraveller, CEOSAIUnitTemplate2Set* pOutTransports )
{
	pOutTransports->Clear();

	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAITransport = m_List.GetNext( pos );
		if( pAITransport->CanContainUnits() && pAITransport->CanContain( pTraveller ) )
		{
			pOutTransports->AddUnitTemplate_Unsafe( pAITransport );
		}
	}
}

/*
void  CEOSAIUnitTemplate2Set::Filter( CEOSAITaskForceDesc* pSimpleTaskForceDesc )
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
void  CEOSAIUnitTemplate2Set::UpdateSummaryInformation()
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
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );

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

float CEOSAIUnitTemplate2Set::GetFastestUnitMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplate2Set::GetFastestAirfieldBuilderAndTransportMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->CanBuildAirfields() )
		{
			fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );

			float fTransportMovementRate = GetFastestTransportMovementRate( pAIUnitTemplate );
			fFastestMovementRate = max( fFastestMovementRate, fTransportMovementRate );
		}
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplate2Set::GetFastestGroundCitResHunterAndTransportMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->IsGroundUnit() &&
		  ( pAIUnitTemplate->CanCaptureCities() || pAIUnitTemplate->CanCaptureResources() ) )
		{
			fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );
		}
		fFastestMovementRate = GetFastestTransportMovementRate();
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplate2Set::GetFastestSeaResHunterMovementRate()
{
	float fFastestMovementRate = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->IsSeaUnit() && pAIUnitTemplate->CanCaptureResources() )
		{
			fFastestMovementRate = max( fFastestMovementRate, pAIUnitTemplate->GetMovementRate() );
		}
	}
	return fFastestMovementRate;
}

float CEOSAIUnitTemplate2Set::GetFastestTransportMovementRate( CEOSAIUnitTemplate2* pTransportee )
{
	float fFastestTransport = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->CanContain( pTransportee ) )
		{
			fFastestTransport = max( fFastestTransport, pAIUnitTemplate->GetMovementRate() );
		}
	}
	return fFastestTransport;
}

float CEOSAIUnitTemplate2Set::GetFastestSeaTransportMovementRate( CEOSAIUnitTemplate2* pTransportee )
{
	float fFastestTransport = 0.0f;
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
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
void CEOSAIUnitTemplate2Set::GetAttritionCostVs( CEOSAIUnitTemplate2* pTarget, CUnitTemplateAndFloatSet* pResults )
{
	ASSERT( false );

	pResults->Clear();

	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );

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
float CEOSAIUnitTemplate2Set::GetBestMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplate2Set::GetBestGroundUnitMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsGroundUnit() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplate2Set::GetBestGroundTransportMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsGroundTransport() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplate2Set::GetBestSeaUnitMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsSeaUnit() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplate2Set::GetBestAirUnitMovementRate()
{
	float fFastest = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
		if( pAIUnitTemplate->GetUnitTemplate()->IsAirUnit() &&
			pAIUnitTemplate->GetUnitTemplate()->GetMovementRate() > fFastest )
		{
			fFastest = pAIUnitTemplate->GetUnitTemplate()->GetMovementRate();
		}
	}

	return fFastest;
}

float CEOSAIUnitTemplate2Set::GetBestAirUnitMovementRange()
{
	float fBestRange = 0.0f;

	POSITION pos;
	pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate2* pAIUnitTemplate = m_List.GetNext( pos );
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
