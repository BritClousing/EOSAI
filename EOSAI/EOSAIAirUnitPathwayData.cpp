
#include "stdafx.h"
#include "EOSAIAirUnitPathwayData.h"

//

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAirPathfinderSubPointsList::~CAirPathfinderSubPointsList()
{
	while( m_List.IsEmpty() == FALSE ){ delete m_List.RemoveHead(); }
}

CEOSAIAirPathfinderMapPoint::~CEOSAIAirPathfinderMapPoint()
{
	for( long i=0; i<m_SubPointsArray.m_iSize; i++ )
	{
		delete m_SubPointsArray[i];
		m_SubPointsArray[i] = NULL;
	}
}

//void  CEOSAIAirPathfinderMapPoint::SetNumberOfLevels( long i )
void  CEOSAIAirPathfinderMapPoint::SetNumberOfPredefinedSteps( long iSteps )
{
	if( iSteps != m_SubPointsArray.m_iSize )
	{
		for( long i=0; i<m_SubPointsArray.m_iSize; i++ )
		{
			delete m_SubPointsArray[i];
			m_SubPointsArray[i] = NULL;
		}
		m_SubPointsArray.Reset();
	}

	m_SubPointsArray.SetSize( iSteps );
	for( long j=0; j<iSteps; j++ )
	{
		//m_SubPointsArray.Value(j) = new CAirPathfinderSubPointsList;
		m_SubPointsArray[j] = new CAirPathfinderSubPointsList;
	}
}

CAirPathfinderSubPoint*  CEOSAIAirPathfinderMapPoint::InvokeSubPoint( long iStepNumber )
{
	CAirPathfinderSubPointsList* pSubPoints = m_SubPointsArray.Value( iStepNumber );
	ASSERT( pSubPoints );
	ASSERT( pSubPoints->m_List.IsEmpty() );
	//
	CAirPathfinderSubPoint* pSubPoint = new CAirPathfinderSubPoint();
	pSubPoint->m_pMapPoint = this;
	pSubPoints->m_List.AddTail( pSubPoint );
	return pSubPoint;
}

CAirPathfinderSubPoint*  CEOSAIAirPathfinderMapPoint::ConsiderNewSubPointObject( 
	CList< CAirPathfinderSubPoint* >* pSubPointsList,
	long iStepNumber, float fPathfindingValue, float fDistanceSinceLastAirbase )
{
	CAirPathfinderSubPointsList* pSubPoints = m_SubPointsArray.Value( iStepNumber );
	ASSERT( pSubPoints );
	ASSERT( pSubPoints->m_List.GetCount() < 100 );

	// Simple case: there are no sub points yet
	if( pSubPoints->m_List.IsEmpty() )
	{
		CAirPathfinderSubPoint* pSubPoint = new CAirPathfinderSubPoint();
		pSubPoint->m_pMapPoint = this;
		pSubPoints->m_List.AddTail( pSubPoint );
		return pSubPoint;
	}
	// Next: there is a sub-point, and this is a Single sub-point object
	if( CanContainMultipleSubPoints() == false )
	{
		CAirPathfinderSubPoint* pSubPoint = pSubPoints->m_List.GetHead();
		if( pSubPoint->m_fPathfindingValue > fPathfindingValue ) return pSubPoint;
		return NULL;
	}
	else // can contain multiple sub-points
	{
		POSITION pos = pSubPoints->m_List.GetHeadPosition();
		while( pos )
		{
			CAirPathfinderSubPoint* pSubPointInList = pSubPoints->m_List.GetNext( pos );
			if( pSubPointInList->m_fPathfindingValue < fPathfindingValue &&
				pSubPointInList->m_fDistanceSinceLastAirbase < fDistanceSinceLastAirbase )
			{
				// This existing sub-point is better than the new one would be
				return NULL;
			}
		}
		// Create a new sub-point
		{
			CAirPathfinderSubPoint* pNewSubPoint = new CAirPathfinderSubPoint();
			pNewSubPoint->m_pMapPoint = this;
			//
			pos = pSubPoints->m_List.GetHeadPosition();
			while( pos )
			{
				POSITION prevPos = pos;
				CAirPathfinderSubPoint* pSubPointInList = pSubPoints->m_List.GetNext( pos );
				if( pSubPointInList->m_fPathfindingValue > fPathfindingValue &&
					pSubPointInList->m_fDistanceSinceLastAirbase > fDistanceSinceLastAirbase )
				{
					// The new sub-point is better than this existing one
					//   Collapse the sub-point into the new one
					if( pSubPointInList->m_pNextStep )
					{
						pSubPointInList->m_pNextStep->m_pPrevStep = pNewSubPoint;
						pNewSubPoint->m_pNextStep = pSubPointInList->m_pNextStep;
					}
					if( pSubPointInList->m_pPrevStep )
					{
						pSubPointInList->m_pPrevStep->m_pNextStep = pNewSubPoint;
						pNewSubPoint->m_pPrevStep = pSubPointInList->m_pPrevStep;
					}
					//
					if( pSubPointInList->m_bTemp_IsInsideCurrentSubPointsList )//m_pCurrentList )
					{
						// Remove the pSubPointInList from the CurrentList
						POSITION pos2 = pSubPointsList->GetHeadPosition();
						while( pos2 )
						{
							POSITION prevPos2 = pos2;
							CAirPathfinderSubPoint* pSubPoint2 = pSubPointsList->GetNext( pos2 );
							if( pSubPoint2 == pSubPointInList )
							{
								pSubPointsList->RemoveAt( prevPos2 );
							}
						}
						/*
						POSITION pos2 = pSubPointInList->m_pCurrentList->GetHeadPosition();
						while( pos2 )
						{
							POSITION prevPos2 = pos2;
							CAirPathfinderSubPoint* pSubPoint2 = pSubPointInList->m_pCurrentList->GetNext( pos2 );
							if( pSubPoint2 == pSubPointInList )
							{
								pSubPointInList->m_pCurrentList->RemoveAt( prevPos2 );
							}
						}
						*/
					}
					//
					delete pSubPointInList;
					pSubPoints->m_List.RemoveAt( prevPos );
				}
			}
			ASSERT( pSubPoints->m_List.GetCount() < 100 );
			pSubPoints->m_List.AddTail( pNewSubPoint );
			return pNewSubPoint;
		}
	}

	return NULL;
}


// Collect all the units that can fire-on the AirUnit
/*
void  CAIAirUnitPathway::CalculateAirPathwayResistance()
{
	m_Resistance.Clear();
	//
	CAIAirPathPoint* pCurrentPoint = NULL;
	CAIAirPathPoint* pLastPoint = NULL;
	POSITION pos = m_PreDefinedPath.GetHeadPosition();
	while( pos )
	{
		pCurrentPoint = m_PreDefinedPath.GetNext( pos );
		if( pLastPoint )
		{
			AddToResistance( pLastPoint->m_Location, pCurrentPoint->m_Location, &m_Resistance );
		}
		pLastPoint = pCurrentPoint;
	}

	// What are the survival odds?
	m_fSurvivalOdds = 1.0f;
	pos = m_Resistance.m_Resistance.GetHeadPosition();
	while( pos )
	{
		CAIAirResistance* pResistance = m_Resistance.m_Resistance.GetNext( pos );
		//CEOSAIAttackVs2* pAttackVs = pResistance->m_pAIUnit->GetPoiObject()->GetAttackVs( m_pAIAirUnit->GetPoiObject(), false );
		CEOSAIAttackVs2* pAttackVs = pResistance->m_pAIUnit->GetPoiObject()->GetAttackVs( EnumPoiObjectType::enum_Unit, m_pUnitTemplate, false );
		if( pAttackVs )
		{
			float fHitOdds = 
				CCombatManager::GetAntiAircraftHitOdds(
					pResistance->m_fTimeWithinCombatRange,
					pResistance->m_pAIUnit->GetUnit(),
					m_pUnitTemplate );
					//m_pAIAirUnit->GetUnit() );
			m_fSurvivalOdds *= ( 1.0-fHitOdds );

			/-*
			CPoiObjectCombatState  EnemyUnitState( pResistance->m_pAIUnit->GetPoiObject() );
			CPoiObjectCombatState  AirUnitState( m_pAIAirUnit->GetUnit() );

			The combat planner isn't currently being used.
			The PoiObjectCombatState stores all kinds of information - HP, location, etc
			I should setup the CombatPlanner to use PoiObjectCombatState.
			The CombatPlanner needs to move units over time - how do I setup those orders?
			  Do I use the AIAirPathway?
			  I will need a corresponding class for land/sea units
				It should be made more sophisticated so that it can reference the WorldDesc terrain
				for combat bonus/penalty and movement rate effects
			It also needs to handle probablistic HP's (if a unit has a 50% chance of being dead
				by the time it reaches the target, then this affects the damage it does)
			*-/
		}
	}
}


void CAIAirUnitPathway::AddToResistance( CEOSAILocation Start, CEOSAILocation End, CAIAirResistanceSet* pResistance )
{
	CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	// First, find all the units that can fire on my air unit
	CList< CEOSAIUnit2* >  m_EnemyUnits;
	POSITION pos = m_pAIBrain->GetAIPoiObjectList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIPoiObject* pAIPoiObject = m_pAIBrain->GetAIPoiObjectList()->GetNext( pos );
		if( pAIPoiObject->GetForeignRelations().IsEnemy() )
		{
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
			if( pAIUnit )
			{
				//CEOSAIAttackVs2* pAttackVs = pAIPoiObject->GetPoiObject()->GetAttackVs( m_pAIAirUnit->GetUnit(), false );
				CEOSAIAttackVs2* pAttackVs = pAIPoiObject->GetPoiObject()->GetAttackVs( EnumPoiObjectType::enum_Unit, m_pUnitTemplate, false );
				if( pAttackVs && pAttackVs->CanTarget() )
				{
					m_EnemyUnits.AddTail( pAIUnit );
				}
			}
		}
	}

	//float fAirUnitMovementRate = m_pAIAirUnit->GetMovementRate();
	float fAirUnitMovementRate = m_pUnitTemplate->GetMovementRate();

	// DEBUG
	// /-*
	pos = m_EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pEnemy = m_EnemyUnits.GetNext( pos );
		CEOSAILocation EnemyLocation = pEnemy->GetInitialState()->GetLocation();

		float fCombatRange = 0.0f;
		//CEOSAIAttackVs2* pAttackVs = pAIPoiObject->GetPoiObject()->GetAttackVs( m_pAIAirUnit->GetUnit(), false );
		//CEOSAIAttackVs2* pAttackVs = pEnemy->GetPoiObject()->GetAttackVs( m_pAIAirUnit->GetUnit(), false );
		CEOSAIAttackVs2* pAttackVs = pEnemy->GetPoiObject()->GetAttackVs( EnumPoiObjectType::enum_Unit, m_pUnitTemplate, false );
		if( pAttackVs )
		{
			fCombatRange = pAttackVs->GetAttackRange();
		}
	// *-/
	/-*
	{
		float fCombatRange = 100.0f;
		CEOSAILocation EnemyLocation( Pixel, 150,150 );
	*-/

		// Create vectors out of the Start->End, Start->Enemy lines
		float fStartToEndX, fStartToEndY;
		pWorldBuildDesc->GetPixelDirection( Start.GetRealX(),Start.GetRealY(), End.GetRealX(),End.GetRealY(), &fStartToEndX,&fStartToEndY );
		float fStartToEnemyX, fStartToEnemyY;
		pWorldBuildDesc->GetPixelDirection( Start.GetRealX(),Start.GetRealY(), EnemyLocation.GetRealX(),EnemyLocation.GetRealY(), &fStartToEnemyX,&fStartToEnemyY );

		// Flip the sign
		fStartToEndX *= -1.0f;
		fStartToEndY *= -1.0f;
		fStartToEnemyX *= -1.0f;
		fStartToEnemyY *= -1.0f;

		// Normalize the vectors
		float fSize = sqrt( fStartToEndX*fStartToEndX + fStartToEndY*fStartToEndY );
		fStartToEndX /= fSize;
		fStartToEndY /= fSize;
		fSize = sqrt( fStartToEnemyX*fStartToEnemyX + fStartToEnemyY*fStartToEnemyY );
		fStartToEnemyX /= fSize;
		fStartToEnemyY /= fSize;

		// Get the dot product
		float fDotProduct = fStartToEndX*fStartToEnemyX + fStartToEndY*fStartToEnemyY;

		// Find the distance from Start to Enemy
		float fDistanceFromStartToEnemy = pWorldBuildDesc->GetPixelDistance( Start, EnemyLocation );
		float fDistanceFromStartToEnd = pWorldBuildDesc->GetPixelDistance( Start, End );

		// Find the location where Start->End is closest to EnemyUnit (called the ClosestPoint)
		float fDistanceFromStartToClosestPoint = fDistanceFromStartToEnemy * fDotProduct;
		//float fDistanceFromStartToClosestPoint = fDotProduct;

		CEOSAILocation ClosestPoint;
		ClosestPoint.SetReal( 
			Start.GetRealX() + fStartToEndX*fDistanceFromStartToClosestPoint,
			Start.GetRealY() + fStartToEndY*fDistanceFromStartToClosestPoint );

		// Now that I have the closest point, I can figure out how long it exists inside the combat circle
		float fDistanceFromEnemyToClosestPoint = pWorldBuildDesc->GetPixelDistance( ClosestPoint, EnemyLocation );

		if( fDistanceFromEnemyToClosestPoint > fCombatRange )
		{
			// If the distance is larger than the combat range, then the AirUnit doesn't enter
			//   the combat range circle
			int x=0; // continue;
		}

		// The AirUnit might enter the combat circle - but that depends on the start and end points
		//float fDistanceFromStartToClosestPoint2 = pWorldBuildDesc->GetPixelDistance( Start, ClosestPoint );
		//ASSERT( fabs( fDistanceFromStartToClosestPoint - fDistanceFromStartToClosestPoint2 ) < 0.01f );

		float fInsideCombatRangeSpan = sqrt( fCombatRange*fCombatRange - fDistanceFromEnemyToClosestPoint*fDistanceFromEnemyToClosestPoint );

		//
		float fEnterDistance = fDistanceFromStartToClosestPoint - fInsideCombatRangeSpan;
		float fExitDistance = fDistanceFromStartToClosestPoint + fInsideCombatRangeSpan;

		if( fEnterDistance < 0.0f ) fEnterDistance = 0.0f;
		if( fEnterDistance > fDistanceFromStartToEnd ) fEnterDistance = fDistanceFromStartToEnd;
		if( fExitDistance < 0.0f ) fExitDistance = 0.0f;
		if( fExitDistance > fDistanceFromStartToEnd ) fExitDistance = fDistanceFromStartToEnd;

		float fDistanceInsideCombatRange = fExitDistance - fEnterDistance;

		if( fDistanceInsideCombatRange > 0.0f )
		{
			float fTimeInsideCombatRange = fDistanceInsideCombatRange / (fAirUnitMovementRate + 0.0001f);// the 0.0001 is to avoid divide by zero
			pResistance->Add( pEnemy, fTimeInsideCombatRange );
		}
	}
}
*/