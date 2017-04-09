
#include "stdafx.h"
#include "EOSAIHistoricalSightings.h"
#include "PoiObject.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit.h"
//#include "AISpatialMap.h"
//#include "EOSAIBrain.h"
#include "AIPlayer.h"
#include "EOSAIBrain.h"

/*
CAIPoiObjectInvolvedInSighting::CAIPoiObjectInvolvedInSighting( CEOSAIPoiObject* pAIPoiObject )
{
	m_pAIPoiObject = pAIPoiObject;
	m_iOwner = m_pAIPoiObject->GetOwner();
	m_bIsMobile = pAIPoiObject->IsMobile();
}

CAIPoiObjectInvolvedInSighting::~CAIPoiObjectInvolvedInSighting()
{
	while( m_Sightings.empty() == false )
	{
		delete m_Sightings.front();
		m_Sightings.pop_front();
	}
}


// Citres are most significant, 
//		ground units significant, 
//		transports somewhat significant, 
//		ships and subs less significant
// Include the time modifier (citres significance increases with time)
float CAIPoiObjectInvolvedInSighting::GetBaseSignificance()
{
	float fSignificance = 0.0f; 

	//
	if( m_iOwner == 0 )
	{
		if( dynamic_cast< CEOSAICity* >( m_pAIPoiObject ) )
		{
			fSignificance = 1.0f;
		}
		if( dynamic_cast< CEOSAIResource* >( m_pAIPoiObject ) )
		{
			fSignificance = 1.0f;
		}
	}
	else // if( m_iOwner != 0 )
	{
		if( dynamic_cast< CEOSAICity* >( m_pAIPoiObject ) )
		{
			fSignificance = 1.0f;
		}
		if( dynamic_cast< CEOSAIResource* >( m_pAIPoiObject ) )
		{
			fSignificance = 1.0f;
		}
		if( dynamic_cast< CEOSAIUnit* >( m_pAIPoiObject ) )
		{
			CEOSAIUnit* pUnit = dynamic_cast< CEOSAIUnit* >( m_pAIPoiObject );
			if( pUnit->GetNumberOfGroundUnitsThatICanContain() > 0 )
			{
				fSignificance = 0.6f;
			}
			else if( pUnit->IsGroundUnit() )
			{
				fSignificance = 0.8f;
			}
			else if( pUnit->IsSeaUnit() )
			{
				fSignificance = 0.4f;
			}
			else if( pUnit->IsAirUnit() )
			{
				fSignificance = 0.3f;
			}
			/-*
			else if( pUnit->IsMissile() )
			{
				fSignificance = 0.3f;
			}
			*-/
			else
			{
				ASSERT( false ); // this can't happen
				fSignificance = 0.3f;
			}
		}
	}
	return fSignificance;
}

void  CAIPoiObjectInvolvedInSighting::ConsiderAddingASighting( float fTime, CEOSAILocation location )
{
	if( this->m_bIsMobile )
	{
		// Add sightings for mobile units once per turn (no more than that)
		if( m_Sightings.empty() )
		{
			CAIPoiObjectSighting* pSighting = new CAIPoiObjectSighting( this, location, fTime );
			m_Sightings.push_back( pSighting );
		}
		else
		{
			CAIPoiObjectSighting* pSighting = m_Sightings.back();
			float fTimeGap = fTime - pSighting->m_TimeOfSighting;
			if( fTimeGap >= 1.0f )
			{
				CAIPoiObjectSighting* pSighting = new CAIPoiObjectSighting( this, location, fTime );
				m_Sightings.push_back( pSighting );
			}
		}
	}
	else
	{
		// if this is non-mobile, create a sighting only if the owner has changed
		if( m_Sightings.empty() )
		{
			CAIPoiObjectSighting* pSighting = new CAIPoiObjectSighting( this, location, fTime );
			m_Sightings.push_back( pSighting );
		}
		else if( m_iOwner != m_pAIPoiObject->GetOwner() )
		{
			CAIPoiObjectSighting* pSighting = m_Sightings.front();
			pSighting->m_TimeOfSighting = fTime;
		}
	}
}
*/

/*
CAIHistoricalSightings::~CAIHistoricalSightings()
{
	DeleteObjects();
}

void  CAIHistoricalSightings::DeleteObjects()
{
	while( m_PoiObjectIsSighting.empty() == false )
	{
		delete m_PoiObjectIsSighting.front();
		m_PoiObjectIsSighting.pop_front();
	}
}

void  CAIHistoricalSightings::AddSighting( CPoiObject* pPoiObject, float fTime, CEOSAILocation location )
{
	CAIPoiObjectInvolvedInSighting* pPoiObjectInSighting = InvokePoiObject( pPoiObject );
	ASSERT( pPoiObjectInSighting );
	if( dynamic_cast< CResourceSource* >( pPoiObject ) )
	{
		int z=0;
	}
	pPoiObjectInSighting->ConsiderAddingASighting( fTime, location );
}

// Need to update periodically (every turn or so)
void  CAIHistoricalSightings::UpdateSignificanceOfSightings( float fCurrentTime )
{
	std::list< CAIPoiObjectInvolvedInSighting* >::iterator iter1;
    for( iter1 =  m_PoiObjectIsSighting.begin(); iter1 != m_PoiObjectIsSighting.end(); ++iter1 )
	{
		CAIPoiObjectInvolvedInSighting* pPoiObject = *iter1;

		float fBaseSignificance = pPoiObject->GetBaseSignificance();

		long iSightingNumber = 0;
		std::list< CAIPoiObjectSighting* >::iterator iter2;
		for( iter2 =  pPoiObject->m_Sightings.begin(); iter2 != pPoiObject->m_Sightings.end(); ++iter2 )
		{
			CAIPoiObjectSighting* pSighting = *iter2;
			//
			iSightingNumber++;
			float fTimeGap = fCurrentTime - pSighting->m_TimeOfSighting.GetTime();
			//
			float fSignificanceOfThisSighting = fBaseSignificance;
			fSignificanceOfThisSighting = fSignificanceOfThisSighting * 3 / (iSightingNumber+2);
			//
			if( pPoiObject->m_bIsMobile )
			{
				// Significance declines over time
				fSignificanceOfThisSighting /= sqrt(fTimeGap/10.0f+1.0f);
			}
			if( pPoiObject->m_bIsMobile == false )
			{
				if( pPoiObject->m_iOwner == 0 ) // not owned?
				{
					// Significance declines over time
					fSignificanceOfThisSighting /= sqrt(fTimeGap/10.0f+1.0f);
				}
				else // it's owned
				{
					// Significance increases over time
					fSignificanceOfThisSighting *= sqrt(fTimeGap/20.0f+1.0f);
				}
			}
			//
			pSighting->m_fSignificance = fSignificanceOfThisSighting;

			// Non-mobile units should only have one sighting
			//   (I don't attach new sightings of non-mobile poi)
			if( pPoiObject->m_bIsMobile == false )
			{
				ASSERT( iSightingNumber == 1 );
			}
		}
	}
}

// Figure out the likihood of player presence at a location
float CAIHistoricalSightings::GetAssumedPlayerPresence( CEOSAILocation location, long iPlayer )
{
	ASSERT( m_pWorldDesc );

	float fAssumedPresence = 0.0f;

	std::list< CAIPoiObjectInvolvedInSighting* >::iterator iter1;
    for( iter1 =  m_PoiObjectIsSighting.begin(); iter1 != m_PoiObjectIsSighting.end(); ++iter1 )
	{
		CAIPoiObjectInvolvedInSighting* pPoiObject = *iter1;

		float fBaseSignificance = pPoiObject->GetBaseSignificance();

		long iSightingNumber = 0;
		std::list< CAIPoiObjectSighting* >::iterator iter2;
		for( iter2 =  pPoiObject->m_Sightings.begin(); iter2 != pPoiObject->m_Sightings.end(); ++iter2 )
		{
			CAIPoiObjectSighting* pSighting = *iter2;

			// Get distance
			float fDistance = m_pWorldDesc->GetPixelDistance( pSighting->m_Location, location );
			if( fDistance < 500.0f )
			{
				// fMult = 3.0 at 0, 0.0 at 500
				float fMult = 3.0f * (500.0f-fDistance)/500.0f;
				if( pPoiObject->m_iOwner == iPlayer )
				{
					fAssumedPresence += pSighting->m_fSignificance * fMult;
				}
				else
				{
					fAssumedPresence -= pSighting->m_fSignificance * fMult * 0.5;
				}
			}
		}
	}

	if( fAssumedPresence < 0.0f ){ fAssumedPresence = 0.0f; }
	return sqrt( fAssumedPresence );
}

float CAIHistoricalSightings::GetAssumedOtherPlayerPresence( CEOSAILocation location, long iPlayer )
{
	ASSERT( m_pWorldDesc );

	float fOtherPlayerPresence = 0.0f;
	float fNonPresence = 0.0f;

	std::list< CAIPoiObjectInvolvedInSighting* >::iterator iter1;
    for( iter1 =  m_PoiObjectIsSighting.begin(); iter1 != m_PoiObjectIsSighting.end(); ++iter1 )
	{
		CAIPoiObjectInvolvedInSighting* pPoiObject = *iter1;

		float fBaseSignificance = pPoiObject->GetBaseSignificance();

		long iSightingNumber = 0;
		std::list< CAIPoiObjectSighting* >::iterator iter2;
		for( iter2 =  pPoiObject->m_Sightings.begin(); iter2 != pPoiObject->m_Sightings.end(); ++iter2 )
		{
			CAIPoiObjectSighting* pSighting = *iter2;

			// Get distance
			float fDistance = m_pWorldDesc->GetPixelDistance( pSighting->m_Location, location );
			if( fDistance < 500.0f )
			{
				// fMult = 3.0 at 0, 0.0 at 500
				float fMult = 3.0f * (500.0f-fDistance)/500.0f;

				// if this is mine or unowned, subtract a bit from the OtherPlayer presence
				if( pPoiObject->m_iOwner == 0 || pPoiObject->m_iOwner == iPlayer )
				{
					fNonPresence += pSighting->m_fSignificance * fMult;// * 0.5;
				}
				else
				{
					fOtherPlayerPresence += pSighting->m_fSignificance * fMult;
				}
			}
		}
	}

	if( fOtherPlayerPresence < 0.0f ){ fOtherPlayerPresence = 0.0f; }
	if( fNonPresence < 0.0f ){ fNonPresence = 0.0f; }
	return sqrt( fOtherPlayerPresence ) - sqrt( fNonPresence );
}

float CAIHistoricalSightings::EstimatePlayerProduction( long iPlayer, CAISpatialMap* pSpatialMap, long iTurn )
{
	// There are two ways to estimate player size:
	//   1. Assume that the player grows by a certain amount each turn (requires that we know the initial size)
	//   2. If we have seen the enemy player, we might be able to estimate the nation's size
	// I use method 1 unless method 2 estimates a larger number.
/*
	// Estimate the city production per region ratio
	float fKnownTotalProduction = 0.0f;
	std::list< CAIPoiObjectInvolvedInSighting* >::iterator iter1;
    for( iter1 =  m_PoiObjectIsSighting.begin(); iter1 != m_PoiObjectIsSighting.end(); ++iter1 )
	{
		CAIPoiObjectInvolvedInSighting* pPoiObject = *iter1;
		CCity* pCity = dynamic_cast< CCity* >( pPoiObject->m_pPoiObject );
		if( pCity )
		{
			fKnownTotalProduction += pCity->GetProduction();
		}
	}
	// Step 2: add up all the explored area you know about
	float fExploredArea = 0.0f;
	float fUnexploredArea = 0.0f;
	for( long i=1; i<pSpatialMap->m_RegionArray.GetUpperBound(); i++ )
	{
		CEOSAIRegion2* pRegion = pSpatialMap->m_RegionArray.GetAt( i );
		ASSERT( pRegion );
		if( pRegion->HasBeenExplored() ){ fExploredArea += pRegion->GetSizeOfRegion(); }
		else{ fUnexploredArea += pRegion->GetSizeOfRegion(); }
	}
	float fProductionPerArea = fKnownTotalProduction / fExploredArea;

	// If there is only a small amount of area explored, my calculations will be a bit off.  Adjust them.
	// Based on "map is known" calculations (on a small map) the ratio is around 0.00375 production per area
	if( fExploredArea <  5000.0f )
	{
		fProductionPerArea = 0.00375f;
	}
	else if( fExploredArea < 20000.0f )
	{
		float fAdjustedExploredArea = fExploredArea - 5000.0f;
		float fWeight = ( 15000.0f - fAdjustedExploredArea )/15000.0f;
		fProductionPerArea = (     fWeight) * 0.00375f +
							 (1.0f-fWeight) * fProductionPerArea;
	}
*-/
	float fUnexploredArea = 0.0f;
	for( long i=1; i<pSpatialMap->m_RegionArray.GetUpperBound(); i++ )
	{
		CEOSAIRegion2* pRegion = pSpatialMap->m_RegionArray.GetAt( i );
		ASSERT( pRegion );
		if( pRegion->HasBeenExplored() == false ){ fUnexploredArea += pRegion->GetSizeOfRegion(); }
	}

	float fProductionPerArea = m_pAIPlayer->GetAIBrain()->GetEstimatedCitResProductionPerArea();
	ASSERT( fProductionPerArea != 0.0f );

	//
	// Blindly estimate the player's production
	//
	float fAssumedStartCityProduction = 20.0f;
	float fModTurn = (iTurn+24.0f)/25.0f;
	float fBlindEstimateBasedOnGrowth = fAssumedStartCityProduction*fModTurn*sqrt( fModTurn );

	// Look at the total unknown area, if the fBlindEstimateBasedOnGrowth is too large, limit it.
	long iNumberOfOtherPlayers = m_pWorldDesc->GetWorldBuildDesc()->GetNumberOfPlayers() - 1;
	if( iNumberOfOtherPlayers < 1 ){ iNumberOfOtherPlayers = 1; }
	float fBlindEstimateBasedOnUnexploredArea = fUnexploredArea*fProductionPerArea/iNumberOfOtherPlayers;
	if( fBlindEstimateBasedOnGrowth > fBlindEstimateBasedOnUnexploredArea )
	{
		fBlindEstimateBasedOnGrowth = fBlindEstimateBasedOnUnexploredArea;
	}

	//
	// Method #2: Now look at the known player production and unknown area where we think he might be
	//
	float fKnownPlayerProduction = 0.0f;

	// Step 1: add-up all the places you know the player owns,
	//		   add-up all the cities you know about
	std::list< CAIPoiObjectInvolvedInSighting* >::iterator iter1;
    for( iter1 =  m_PoiObjectIsSighting.begin(); iter1 != m_PoiObjectIsSighting.end(); ++iter1 )
	{
		CAIPoiObjectInvolvedInSighting* pPoiObject = *iter1;
		CCity* pCity = dynamic_cast< CCity* >( pPoiObject->m_pPoiObject );
		if( pCity )
		{
			if( pCity->GetOwner() == iPlayer )
			{
				fKnownPlayerProduction += pCity->GetTotalProduction();
			}
		}
	}

	// Look at regions where you suspect the player might be, add some assumed production
	float fAssumedAreaOccupied = 0.0f;
	float fAssumedPlayerProduction = 0.0f;
	for( long i=1; i<pSpatialMap->m_RegionArray.GetUpperBound(); i++ )
	{
		CEOSAIRegion2* pRegion = pSpatialMap->m_RegionArray.GetAt( i );
		ASSERT( pRegion );
		if( pRegion->HasBeenExplored() == false )
		{
			float fAssumedPresence = GetAssumedPlayerPresence( pRegion->GetLocation(), iPlayer );
			if( fAssumedPresence > 1.0f )
			{
				fAssumedPresence = 1.0f;
			}
			if( fAssumedPresence == 1.0f )
			{
				fAssumedAreaOccupied += fAssumedPresence * pRegion->GetSizeOfRegion();
			}
		}
		//
	}
	fAssumedPlayerProduction += fAssumedAreaOccupied * fProductionPerArea;

	// If we have some "known" data, favor that
	float fRelyOnRoughEstimate = 1.0f; // 1.0 = favor known information, 0.0 = favor blind information
	fRelyOnRoughEstimate = ( fKnownPlayerProduction/100.0f );
	if( fRelyOnRoughEstimate > 1.0f ){ fRelyOnRoughEstimate = 1.0f; }
	return (     fRelyOnRoughEstimate) * (fKnownPlayerProduction + fAssumedPlayerProduction) +
		   (1.0f-fRelyOnRoughEstimate) * fBlindEstimateBasedOnGrowth;
}

CAIPoiObjectInvolvedInSighting* CAIHistoricalSightings::InvokePoiObject( CPoiObject* pPoiObject )
{
	std::list< CAIPoiObjectInvolvedInSighting* >::iterator iter;
    for( iter =  m_PoiObjectIsSighting.begin(); iter != m_PoiObjectIsSighting.end(); ++iter )
	{
		CAIPoiObjectInvolvedInSighting* pPoiObjectInList = *iter;
		if( pPoiObject == pPoiObjectInList->m_pPoiObject )
		{
			return pPoiObjectInList;
		}
	}
	// Create one
	CAIPoiObjectInvolvedInSighting* pPoiObjectInvolved = new CAIPoiObjectInvolvedInSighting( pPoiObject );
	m_PoiObjectIsSighting.push_back( pPoiObjectInvolved );
	return pPoiObjectInvolved;
}
*/
