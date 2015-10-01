
#include "stdafx.h"
#include "EOSAIUnitActionIdea.h"
//#include "AIActionOutline.h"
#include "EOSAIDesire2.h"
//#include "9MovementPathwaysFromEverywhereToMRX.h"
#include "EOSAIMultiRegion2.h"
#include "EOSAIUnit2.h"
#include "EOSAIBrain.h"
//#include "CombatManager.h"
#include "EOSAIUnitTemplate.h"
//#include "MovementPath.h"
//#include "WorldDesc.h"
//#include "AIUnitProximity.h"
//#include "AIPoiObjectProximity2.h"
#include "EOSAIUnitAction.h"
#include "EOSAITacticalProject2.h"
//#include "MovementPathIdeasForAirUnit.h"
//#include "MovementPathForGroundSeaUnit.h"
#include "City.h"
//#include "City.h"
#include "EOSAIUnit2.h"
#include "EOSAIAirbasesSet.h"
//#include "AITacticalProject.h"
#include "EOSAITaskForce3.h"
#include "EOSAIDesireSpatial.h"
#include "EOSAIUnitPathwayPredefinedStep.h"

#include "EOSAIGroundUnitPathwayFinder.h"
#include "EOSAISeaUnitPathwayFinder.h"
#include "EOSAIAirUnitPathwayFinder.h"
//#include "AICityActionIdeaNode.h"
#include "EOSAITransportAssistedPath.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"
#include "EOSAICommonData2.h"
//#include "Checkpoint.h"
#ifdef _DEBUG
#include "EOSAIInterface.h"
#endif _DEBUG

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DO_TRANSPORT_COMBO_CALCULATION TRUE
#define CREATE_TRANSPORT_MOVEMENT_DESC TRUE

// Should include pathway information (for bombardment, anti-air), combat difficultly, etc
void  CEOSAIUnit2ActionIdeaTargetValue::CalculateValue()
{
	// Value is based on:
	//   Am I taking ranged fire along the path?
	//   Is this a good unit to fight the enemy units?
	//   What's the value of the target?  (High-value cities > Low-value cities)
	//   What's the time-to-target?

	CList< CEOSAIUnit2* >  EnemyUnits;
	m_pDesire->GetUnitsInTheImmediateArea( &EnemyUnits );
	//
	float fEnemyCombatSignificance = 0.0;
	POSITION pos = EnemyUnits.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2* pAIUnit = EnemyUnits.GetNext( pos );
		fEnemyCombatSignificance += 
			pAIUnit->GetCurrentHP01() *
			pAIUnit->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetCombatSignificance();
	}

	float fMyCombatSignificance = 0.0f;
	if( m_pAIUnitActionIdea->GetAIUnitActor() )
	{
		fMyCombatSignificance = 
			m_pAIUnitActionIdea->GetAIUnitActor()->GetCurrentHP01() *
			m_pAIUnitActionIdea->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	else
	{
		fMyCombatSignificance = m_pAIUnitActionIdea->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetCombatSignificance();
	}
	float fCombatBalance = fMyCombatSignificance / (2.0f*fEnemyCombatSignificance+5.0f);
	if( fCombatBalance > 1.0f ) fCombatBalance = 1.0f;
	if( fEnemyCombatSignificance == 0.0f ){ fCombatBalance = 1.0f; }

	// Give extra value to the closest desire
	float fTimeToTargetMult = m_pAIUnitActionIdea->m_fBestTimeToTargetDesire / m_fTimeToTarget;

	m_fTargetValue = ( m_pDesire->GetSimpleInterest() ) *
					 ( fCombatBalance ) *
					 ( fTimeToTargetMult ) *
					 ( 2.0f / (m_fTimeToTarget+2.0f) );
}

//////////////////////////////////////////

CEOSAIUnit2TransportMovementDesc::CEOSAIUnit2TransportMovementDesc( CEOSAIUnit2* pAITransportee, CEOSAIUnit2ActionIdea* pAITransporteeUnitActionIdea )
{
	m_pAITransportee = pAITransportee;
	m_pAITransporteeUnitActionIdea = pAITransporteeUnitActionIdea;
	m_pAITransportUnitActionIdea = NULL;
//	m_pHypotheticalAITransportUnit = NULL;
	m_pAITransport = NULL;
//	m_pAICity = NULL;// 30498234
	m_pTransportAIUnitTemplate = NULL;
	//
	m_fBuildTime = 0.0f;
	m_fTotalTime = 0.0f;
}

CEOSAIUnit2TransportMovementDesc::~CEOSAIUnit2TransportMovementDesc()
{
	delete m_pAITransportUnitActionIdea;
//	delete m_pHypotheticalAITransportUnit;
}

CEOSAIPoiObject* CEOSAIUnit2TransportMovementDesc::GetAIPoiActor()
{
	if( m_pAITransport ){ return m_pAITransport; }
//	if( m_pAICity ){ return m_pAICity; } // 30498234
	//if( m_pAICityActionIdeaNode ){ return m_pAICityActionIdeaNode->m_pAICity; }
	ASSERT( false );
	return NULL;
}
/*
CEOSAIUnit2ActionIdea* CEOSAIUnit2TransportMovementDesc::CreateAIUnitActionIdea( CAITacticalProject* pAITacticalProject )
{
	//ASSERT( m_pAITransportUnit || m_pAICityActionIdeaNode );
	ASSERT( m_pAITransport );// || m_pAICity );
	CEOSAIUnit2ActionIdea* pAIUnitActionIdea = NULL;
	if( m_pAITransport )
	{
		pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject, m_pAITransport );
		//pAIUnitActionIdea->SetActionType( CEOSAIUnit2ActionIdea::EnumActionType_Transport );
		//pAIUnitActionIdea->m_pTransportee = m_pAITransportee;
		pAIUnitActionIdea->Transport( m_pAITransportee, m_pAITransporteeUnitActionIdea );

		m_pAITransportUnitActionIdea = pAIUnitActionIdea;
	}
	else
	{
		ASSERT( false ); // 30498234
/-*
		// Create a hypothetical CEOSAIUnit2 which is a transport
		CEOSAIBrain* pAIBrain = pAITacticalProject->GetAIBrain();
		CEOSAIUnit2* pHypotheticalAITransportUnit = new CEOSAIUnit2( pAIBrain );
		//pHypotheticalAITransportUnit->CreateHypotheticalUnit( m_pTransportUnitTemplate, m_pAICity->GetOwner(), m_pAICity->GetInitialState()->GetLocation(), m_fBuildTime );
		pHypotheticalAITransportUnit->CreateHypotheticalUnit( m_pAICity, m_pTransportUnitTemplate, m_fBuildTime );

		pAIBrain->AddAIObject( pHypotheticalAITransportUnit );

		pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject, pHypotheticalAITransportUnit );
	//	pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject, m_pAICity, m_pTransportUnitTemplate );
		//pAIUnitActionIdea->SetActionType( CEOSAIUnit2ActionIdea::EnumActionType_Transport );
		//pAIUnitActionIdea->m_pTransportee = m_pAITransportee;
		pAIUnitActionIdea->Transport( m_pAITransportee, m_pAITransporteeUnitActionIdea );
		m_pAITransportUnitActionIdea = pAIUnitActionIdea;
		//m_pHypotheticalAITransportUnit = pHypotheticalAITransportUnit;
*-/
	}
	return pAIUnitActionIdea;
}
*/
CEOSAIUnit2ActionIdea* CEOSAIUnit2TransportMovementDesc::CreateAIUnitActionIdea( CEOSAITacticalProject2* pAITacticalProject2 )
{
	//ASSERT( m_pAITransportUnit || m_pAICityActionIdeaNode );
	ASSERT( m_pAITransport );// || m_pAICity );
	CEOSAIUnit2ActionIdea* pAIUnitActionIdea = NULL;
	if( m_pAITransport )
	{
		pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject2, m_pAITransport );
		//pAIUnitActionIdea->SetActionType( CEOSAIUnit2ActionIdea::EnumActionType_Transport );
		//pAIUnitActionIdea->m_pTransportee = m_pAITransportee;
		pAIUnitActionIdea->Transport( m_pAITransportee, m_pAITransporteeUnitActionIdea );

		m_pAITransportUnitActionIdea = pAIUnitActionIdea;
	}
	else
	{
		ASSERT( false ); // 30498234
/*
		// Create a hypothetical CEOSAIUnit2 which is a transport
		CEOSAIBrain* pAIBrain = pAITacticalProject->GetAIBrain();
		CEOSAIUnit2* pHypotheticalAITransportUnit = new CEOSAIUnit2( pAIBrain );
		//pHypotheticalAITransportUnit->CreateHypotheticalUnit( m_pTransportUnitTemplate, m_pAICity->GetOwner(), m_pAICity->GetInitialState()->GetLocation(), m_fBuildTime );
		pHypotheticalAITransportUnit->CreateHypotheticalUnit( m_pAICity, m_pTransportUnitTemplate, m_fBuildTime );

		pAIBrain->AddAIObject( pHypotheticalAITransportUnit );

		pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject, pHypotheticalAITransportUnit );
	//	pAIUnitActionIdea = new CEOSAIUnit2ActionIdea( pAITacticalProject, m_pAICity, m_pTransportUnitTemplate );
		//pAIUnitActionIdea->SetActionType( CEOSAIUnit2ActionIdea::EnumActionType_Transport );
		//pAIUnitActionIdea->m_pTransportee = m_pAITransportee;
		pAIUnitActionIdea->Transport( m_pAITransportee, m_pAITransporteeUnitActionIdea );
		m_pAITransportUnitActionIdea = pAIUnitActionIdea;
		//m_pHypotheticalAITransportUnit = pHypotheticalAITransportUnit;
*/
	}
	return pAIUnitActionIdea;
}

//

CEOSAIUnit2ActionIdea::CEOSAIUnit2ActionIdea()
{
	m_pAIBrain = NULL;
	//m_pTacticalProject = NULL;
	m_pTacticalProject2 = NULL;
	Clear();
}
/*
CEOSAIUnit2ActionIdea::CEOSAIUnit2ActionIdea( CAITacticalProject* pTacticalProject, CEOSAIUnit2* pAIUnit ) 
	//: CAIPoiObjectActionIdea( pTacticalProject )
{
	Clear();
	m_pAIBrain = pTacticalProject->GetAIBrain();
	m_pTacticalProject = pTacticalProject;
	m_pTacticalProject2 = NULL;
	//SetAITaskForce( pTaskForce );
	//SetActor( pActor );
	m_pAIUnitActor = pAIUnit;
	m_pAICityActor = NULL;
	//m_pAICityActionIdeaNode = NULL;
	m_pUnitTemplate = pAIUnit->GetUnitTemplate();
	//
	//m_JobRole.m_bCombat = 
	//
	m_pAITransportUnitActionIdea = NULL;
	m_pAITransportAssistedPath = NULL;
	m_pAITransportee = NULL;
	m_pAITransporteeUnitActionIdea = NULL;
	//
	m_fDistance = 0.0f;
	m_fBuildTime = 0.0f;
	m_fDirectMoveToTargetTime = 0.0f;
	//m_fTransportAssistedMoveToTargetTime = 0.0f;
	//
	m_bTransportRequired = false;
	//m_fTransportArrivalTime = false;
	m_fTransportComboTime = 0.0f;
	//
	m_fBestTimeToTargetDesire = 1000000.0f;
	//
	m_bMultiRegionTransportMovementDescs_Exist = false;
	m_bAIRegionTransportMovementDescs_Exist = false;
}
*/
CEOSAIUnit2ActionIdea::CEOSAIUnit2ActionIdea( CEOSAITacticalProject2* pTacticalProject2, CEOSAIUnit2* pAIUnit )
{
	Clear();
	m_pAIBrain = pTacticalProject2->GetAIBrain();
	//m_pTacticalProject  = NULL;
	m_pTacticalProject2 = pTacticalProject2;
	//SetAITaskForce( pTaskForce );
	//SetActor( pActor );
	m_pAIUnitActor = pAIUnit;
	m_pAICityActor = NULL;
	//m_pAICityActionIdeaNode = NULL;
	m_pAIUnitTemplate = pAIUnit->GetAIUnitTemplate();
	//
	//m_JobRole.m_bCombat = 
	//
	m_pAITransportUnitActionIdea = NULL;
	m_pAITransportAssistedPath = NULL;
	m_pAITransportee = NULL;
	m_pAITransporteeUnitActionIdea = NULL;
	//
	m_fDistance = 0.0f;
	m_fBuildTime = 0.0f;
	m_fDirectMoveToTargetTime = 0.0f;
	//m_fTransportAssistedMoveToTargetTime = 0.0f;
	//
	m_bTransportRequired = false;
	//m_fTransportArrivalTime = false;
	m_fTransportComboTime = 0.0f;
	//
	m_fBestTimeToTargetDesire = 1000000.0f;
	//
	m_bMultiRegionTransportMovementDescs_Exist = false;
	m_bAIRegionTransportMovementDescs_Exist = false;
}

/*
CEOSAIUnit2ActionIdea::CEOSAIUnit2ActionIdea( CAITacticalProject* pTacticalProject, CEOSAICityActionIdeaNode* pAICityActionIdeaNode, CUnitTemplate* pUnitTemplate )
{
	Clear();
	m_pTacticalProject = pTacticalProject;
	m_pAIUnitActor = NULL;
	//m_pAICityActor = pAICityActionIdeaNode->m_pAICity;
	m_pAICityActionIdeaNode = pAICityActionIdeaNode;
	m_pUnitTemplate = pUnitTemplate;
	//
	m_pAITransportActionIdea = NULL;
}
*/
/*
CEOSAIUnit2ActionIdea::CEOSAIUnit2ActionIdea( CAITacticalProject* pTacticalProject, CEOSAICity* pAICity, CUnitTemplate* pUnitTemplate )
	//: CAIPoiObjectActionIdea( pTacticalProject )
{
	Clear();
	m_pAIBrain = pTacticalProject->GetAIBrain();
	m_pTacticalProject = pTacticalProject;
	m_pTacticalProject2 = NULL;
	m_pAIUnitActor = NULL;
	m_pAICityActor = pAICity;
	//m_pAICityActionIdeaNode = NULL;
	m_pUnitTemplate = pUnitTemplate;
	//
	m_pAITransportUnitActionIdea = NULL;
}
*/
CEOSAIUnit2ActionIdea::CEOSAIUnit2ActionIdea( CEOSAITacticalProject2* pTacticalProject2, CEOSAICity* pAICity, CEOSAIUnitTemplate* pAIUnitTemplate )
{
	Clear();
	m_pAIBrain = pTacticalProject2->GetAIBrain();
	//m_pTacticalProject = NULL;
	m_pTacticalProject2 = pTacticalProject2;
	m_pAIUnitActor = NULL;
	m_pAICityActor = pAICity;
	//m_pAICityActionIdeaNode = NULL;
	m_pAIUnitTemplate = pAIUnitTemplate;
	//
	m_pAITransportUnitActionIdea = NULL;
}

CEOSAIUnit2ActionIdea::~CEOSAIUnit2ActionIdea()
{
	while( m_UnitTransportMovementDescs.IsEmpty() == FALSE ){ delete m_UnitTransportMovementDescs.RemoveHead(); }
	while( m_DesireValues.IsEmpty() == FALSE ){ delete m_DesireValues.RemoveHead(); }
}

void CEOSAIUnit2ActionIdea::Clear()
{
	//m_pTaskForce = NULL;
	m_eActionType = EnumActionType_Undefined;
	//m_pActor = NULL;
	m_pAIUnitActor = NULL;
	m_pAICityActor = NULL;
	//m_pAICityActionIdeaNode = NULL;
	m_pAIUnitTemplate = NULL;

	m_pAITransportUnitActionIdea = NULL;

	//m_pUnitPathway = NULL;
	//

	//m_bIsValid = false;

	//m_pScheduledAIUnitAction = NULL;
	m_pPredefinedStep = NULL;

	//m_fProximityToClosestTransport = 1000000.0f;

//	m_fAccessibility = 0.0f;
//	m_fRisk = 0.0f;

	//m_fDistance = 0.0f;
	//m_fBuildTime = 0.0f;
	//m_fMoveToTargetTime = 0.0f;
	//m_fDirectMoveToTargetTime = 1000000.0f;
	//m_fTransportComboTime = 1000000.0f;
	//m_fTransportAssistedMoveToTargetTime = 1000000.0f;
	//m_bTransportRequired = false;
	//m_fTransportArrivalTime = 0.0f; // 0.0 means use an imaginary transport
	m_bFailureCondition_TargetIsNotWithinAirRange = false;

	//m_fRelativeScoreAccordingToAIActionDefinition = 0.0f;
	//m_fRelativeScoreAccordingToAIPoiObject = 0.0f;
	//m_fTTTScoreAndAIPoiObjectRelativeScore = 0.0f;

	// Air Unit information
	//m_bTargetIsWithinMyMovementRangeFromOneOfMyAirbases = false;
	// Ground Unit information
	m_fImaginarySeaTransportDividedByGroundRoute = 0.0f;
	m_fImaginaryAirTransportDividedByGroundRoute = 0.0f;
	m_fImaginaryAirTransportDividedByImaginarySeaTransport = 0.0f;


	m_pAITransportUnitActionIdea = NULL;
	m_pAITransportAssistedPath = NULL;
	m_pAITransportee = NULL;
	m_pAITransporteeUnitActionIdea = NULL;
	//
	m_fDistance = 0.0f;
	m_fBuildTime = 0.0f;
	m_fDirectMoveToTargetTime = 1000000.0f;
	m_fTransportComboTime = 1000000.0f;
	//m_fTransportAssistedMoveToTargetTime = 0.0f;
	//
	m_bTransportRequired = false;
	//
	m_fBestTimeToTargetDesire = 1000000.0f;
	//
	m_bMultiRegionTransportMovementDescs_Exist = false;
	m_bAIRegionTransportMovementDescs_Exist = false;
	//
	while( m_DesireValues.IsEmpty() == FALSE ){ delete m_DesireValues.RemoveHead(); }
}

void CEOSAIUnit2ActionIdea::SetActor( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
	if( pAIUnit )
	{
		m_pAIUnitActor = pAIUnit;
		m_pAIUnitTemplate = pAIUnit->GetAIUnitTemplate();
		m_pAICityActor = NULL;
	}
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pAIPoiObject );
	if( pAICity )
	{
		m_pAIUnitActor = NULL;
		m_pAIUnitTemplate = NULL;
		m_pAICityActor = pAICity;
	}
}

/*
void CEOSAIUnit2ActionIdea::SetActor( CEOSAIPoiObject* pActor )
{
	m_pActor = pActor;
	m_pAIUnitActor = dynamic_cast< CEOSAIUnit2* >( pActor );
	m_pUnitTemplate = NULL;
	if( m_pAIUnitActor )
	{
		m_pUnitTemplate = m_pAIUnitActor->GetUnitTemplate();
	}
}
*/

//

CEOSAIPoiObject*  CEOSAIUnit2ActionIdea::GetAIPoiObjectActor()
{
	if( m_pAIUnitActor ) return m_pAIUnitActor; 
	if( m_pAICityActor ) return m_pAICityActor;
	//if( m_pAICityActionIdeaNode ) return GetAICityActor();
	ASSERT( false );
	return NULL;
}

CEOSAICity* CEOSAIUnit2ActionIdea::GetAICityActor()
{
	//if( m_pAICityActionIdeaNode )
	//{
	//	return m_pAICityActionIdeaNode->m_pAICity;
	//}
	//return NULL;
	return m_pAICityActor;
}

CEOSAILocation  CEOSAIUnit2ActionIdea::GetTargetLocation()
{
	//ASSERT( GetAIUnitAction() );
	//ASSERT( GetAIUnitAction()->GetTargetDesire() );

	//CEOSAILocation Location = GetAIUnitAction()->GetTargetDesire()->GetLocation();
	//ASSERT( Location.GetRealX() != 0.0f || Location.GetRealY() != 0.0f );
	if( m_DesireAllocations.IsEmpty() == FALSE )
	{
		CEOSAIDesireSpatial* pDesire = m_DesireAllocations.GetHead();
		return pDesire->GetLocation();
	}
	//return m_pTacticalProject2->GetFirstDesire()->GetLocation();
	return m_pTacticalProject2->GetLocation();//->GetFirstDesire()->GetLocation();
}

void CEOSAIUnit2ActionIdea::GetTransportIdeas2( 
		EnumMovementDescFilter eFilter, CEOSAITaskForce3* pAITaskForce, CEOSAIUnit2TransportMovementDesc** pBestMovementDesc )
{
	long iMyUnitSize = m_pAIUnitTemplate->GetMySizeInContainer();

	float fBestTime = 1000000.0f;
	*pBestMovementDesc = NULL;

	// If the unit is inside a transport, then use that one
	//CEOSAIUnit2* pAIUnit = pUnitStackItem->m_pUnitActionIdea->GetAIUnitActor();
	CEOSAIUnit2* pAIUnit = GetAIUnitActor();
	if( pAIUnit )
	{
		CEOSAIUnit2* pCurrentAIUnitContainer = pAIUnit->GetCurrentAIUnitContainer();
		if( pCurrentAIUnitContainer )
		{
			POSITION pos = m_UnitTransportMovementDescs.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnit2TransportMovementDesc* pMovementDesc = m_UnitTransportMovementDescs.GetNext( pos );
				if( pMovementDesc->m_pAITransport == pCurrentAIUnitContainer )
				{
					*pBestMovementDesc = pMovementDesc;
					return;
				}
			}
		}
	}

	POSITION pos = m_UnitTransportMovementDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TransportMovementDesc* pMovementDesc = m_UnitTransportMovementDescs.GetNext( pos );

		// Update BestPossibleTime
		if( eFilter == EnumMovementDescFilter_BestPossible )
		{
			if( pMovementDesc->m_fTotalTime < fBestTime )
			{
				if( pMovementDesc->m_pAITransport )
				{
					if( pMovementDesc->m_pAITransport->CanInsertTransportIdea( pAITaskForce, this ) )
					{
						fBestTime = pMovementDesc->m_fTotalTime;
						*pBestMovementDesc = pMovementDesc;
					}
				}
				/* // 30498234
				if( pMovementDesc->m_pAICity )
				{
					fBestTime = pMovementDesc->m_fTotalTime;
					*pBestMovementDesc = pMovementDesc;
				}
				*/
			}
		}
		if( eFilter == EnumMovementDescFilter_BestAvailable )
		{
			// Update BestAvailableTime
			if( pMovementDesc->m_fTotalTime < fBestTime )
			{
				if( pMovementDesc->m_pAITransport )
				{
					if( pMovementDesc->m_pAITransport->IsAllocated() == false )
					{
						#ifdef _DEBUG
						if( g_pEOSAIInterface->GetCurrentTurn() == 11 )
						{
							int j=0;
						}
						#endif
						if( pMovementDesc->m_pAITransport->CanInsertTransportIdea( pAITaskForce, this ) )
						{
							fBestTime = pMovementDesc->m_fTotalTime;
							*pBestMovementDesc = pMovementDesc;
						}
					}
				}
				/* // 30498234
				if( pMovementDesc->m_pAICity )
				{
					if( pMovementDesc->m_pAICity->IsAllocated() == false )
					{
						fBestTime = pMovementDesc->m_fTotalTime;
						*pBestMovementDesc = pMovementDesc;
					}
				}
				*/
			}
		}
	}
}

/*
CEOSAIUnit2TransportMovementDesc*  CEOSAIUnit2ActionIdea::GetBestTransport()
{
	float fBestTime = 1000000.0f;
	CEOSAIUnit2TransportMovementDesc*  pBestMovementDesc = NULL;
	POSITION pos = m_UnitTransportMovementDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TransportMovementDesc* pMovementDesc = m_UnitTransportMovementDescs.GetNext( pos );
		if( pMovementDesc->m_fTotalTime < fBestTime )
		{
			if( pMovementDesc->m_pAITransportUnit )
			{
				fBestTime = pMovementDesc->m_fTotalTime;
				pBestMovementDesc = pMovementDesc;
			}
			if( pMovementDesc->m_pAICity )
			{
				fBestTime = pMovementDesc->m_fTotalTime;
				pBestMovementDesc = pMovementDesc;
			}
		}
	}
	return pBestMovementDesc;
}

CEOSAIUnit2TransportMovementDesc*  CEOSAIUnit2ActionIdea::GetBestAvailableTransport()
{
	long iMySize = m_pUnitTemplate->GetMySizeInContainer();

	float fBestTime = 1000000.0f;
	CEOSAIUnit2TransportMovementDesc*  pBestMovementDesc = NULL;
	POSITION pos = m_UnitTransportMovementDescs.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TransportMovementDesc* pMovementDesc = m_UnitTransportMovementDescs.GetNext( pos );
		if( pMovementDesc->m_fTotalTime < fBestTime )
		{
			if( pMovementDesc->m_pAITransportUnit )
			{
				if( pMovementDesc->m_pAITransportUnit->IsAllocated() == false ||
					pMovementDesc->m_pAITransportUnit->GetAllocatedTacticalProject() == m_pTacticalProject )
				{
					//long iContainerSize = pMovementDesc->m_pAITransportUnit->GetContainerSize();
					//long iAvailableSize = pMovementDesc->m_pAITransportUnit->GetUnallocatedContainerSpace();
					//if( iAvailableSize 

					fBestTime = pMovementDesc->m_fTotalTime;
					pBestMovementDesc = pMovementDesc;
				}
			}
			if( pMovementDesc->m_pAICity )
			{
				//if( pMovementDesc->GetAIPoiActor()>m_pAICity->IsAllocated() == false ||
				//	pMovementDesc->m_pAICity->GetAllocatedTacticalProject() == pAITacticalProject )
			//	if( pMovementDesc->GetAIPoiActor()->IsAllocated() == false ||
			//		pMovementDesc->GetAIPoiActor()->GetAllocatedTacticalProject() == m_pAITacticalProject )
				if( pMovementDesc->m_pAICity->IsAllocated() == false ||
					pMovementDesc->m_pAICity->GetAllocatedTacticalProject() == m_pTacticalProject )
				{
					fBestTime = pMovementDesc->m_fTotalTime;
					pBestMovementDesc = pMovementDesc;
				}
			}
		}
	}
	return pBestMovementDesc;
}
*/

void CEOSAIUnit2ActionIdea::SetAITransportUnitActionIdea( CEOSAIUnit2ActionIdea* pTransportUnitActionIdea )
{
	m_pAITransportUnitActionIdea = pTransportUnitActionIdea;
}

/*
void CEOSAIUnit2ActionIdea::UseTransport( CEOSAIPoiObject* pAITransportUnit )
{
	m_pUseAITransport = pAITransportUnit;
}
*/
/*
void CEOSAIUnit2ActionIdea::CreateMovementRoutes()
{
	ASSERT( m_pLandRouteActionOutline == NULL );
	ASSERT( m_pSeaRouteActionOutline == NULL );
	ASSERT( m_pSeaTransportActionOutline == NULL );
	ASSERT( m_pAirActionOutline == NULL );

	CWorldDesc* pWorldDesc = m_pActionDefinition->GetAIBrain()->GetWorldDesc();
	m_fDirectAirDistance = pWorldDesc->GetPixelDistance( m_pActor->GetInitialState()->GetLocation(), m_pTargetMultiRegion->GetLocation() );
	m_fDirectAirDistance -= m_pTargetMultiRegion->m_fRadiusOfRegions;
/*
	if( m_eMovementType == EOSAIEnumMovementType3_Land )
	{
		m_bLandRouteExists = true;
		m_bSeaTransportRouteExists = true;
		m_bAirRouteExists = true;
		m_pLandRouteActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
		m_pSeaTransportActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
		m_pAirActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
	}
	if( eMovementType == EOSAIEnumMovementType3_Water )
	{
		m_bSeaRouteExists = true;
		m_pSeaRouteActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
	}
	if( eMovementType == EOSAIEnumMovementType3_Air )
	{
		m_bAirRouteExists = true;
		m_pAirActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
	}
*-/
	//
	C9MovementPathwaysFromEverywhereToMRX* pAllPathways = m_pTargetMultiRegion->InvokePathwaysToMultiRegion();
	//long iTargetMultiRegion = pMoveToMultiRegion->m_iId;

	if( m_pLandRouteActionOutline ){ m_pLandRouteActionOutline->Clear(); }
	if( m_pSeaRouteActionOutline ){ m_pSeaRouteActionOutline->Clear(); }
	if( m_pSeaTransportActionOutline  ){ m_pSeaTransportActionOutline->Clear(); }
	if( m_pAirActionOutline  ){ m_pAirActionOutline->Clear(); }

	// If this is a ground unit, try to find a NoGeoSwitch route, a SeaTransport, and
	//   an Air route
	CMultiRegionResult* pMultiRegionResult;
	CMultiRegionPathwayResult* pMultiRegionPathwayResult;
	long iActorMultiRegion = m_pActor->GetFinalState()->GetMultiRegion()->m_iId;
	if( m_eMovementType == EOSAIEnumMovementType3_Land )
	{
		// Is there a NonTransport route?
		pMultiRegionPathwayResult = &pAllPathways->m_LandPathMinimizeDistancePathway;
		pMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iActorMultiRegion );
		if( pMultiRegionResult )
		{
			//m_bLandRouteExists = true;
			m_pLandRouteActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
			m_pLandRouteActionOutline->CreateMovementRouteToMultiRegion( pMultiRegionPathwayResult, m_pTargetMultiRegion );
		}
		//else{ m_bLandRouteExists = false; }
		//
		pMultiRegionPathwayResult = &pAllPathways->m_AllowOneTransportMinimizeDistancePathway;
		pMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iActorMultiRegion );
		if( pMultiRegionResult )
		{
			if( pMultiRegionResult->m_iGeoSwitches > 0 )
			{
				//m_bSeaTransportRouteExists = true;
				m_pSeaTransportActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
				m_pSeaTransportActionOutline->CreateMovementRouteToMultiRegion( pMultiRegionPathwayResult, m_pTargetMultiRegion );
			}
			//else{ m_bSeaTransportRouteExists = false; }
		}
		//else{ m_bSeaTransportRouteExists = false; }
		//
		pMultiRegionPathwayResult = &pAllPathways->m_AirMinimizeDistancePathway;
		pMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iActorMultiRegion );
		if( pMultiRegionResult )
		{
			//m_bAirRouteExists = true;
			m_pAirActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
			m_pAirActionOutline->CreateMovementRouteToMultiRegion( pMultiRegionPathwayResult, m_pTargetMultiRegion );
		}
		//else{ ASSERT( false ); m_bAirRouteExists = false; }
	}
	// If this is a sea unit, find a NoTransport route
	if( m_eMovementType == EOSAIEnumMovementType3_Water )
	{
		pMultiRegionPathwayResult = &pAllPathways->m_WaterPathMinimizeDistancePathway;
		pMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iActorMultiRegion );
		if( pMultiRegionResult )
		{
			//m_bSeaRouteExists = true;
			m_pSeaRouteActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
			m_pSeaRouteActionOutline->CreateMovementRouteToMultiRegion( pMultiRegionPathwayResult, m_pTargetMultiRegion );
		}
		//else{ m_bSeaRouteExists = false; }
	}
	// If this is an air unit, use the AirTransport information as the NoTransport route
	if( m_eMovementType == EOSAIEnumMovementType3_Air )
	{
		pMultiRegionPathwayResult = &pAllPathways->m_AirMinimizeDistancePathway;
		pMultiRegionResult = pMultiRegionPathwayResult->GetMultiRegionResult( iActorMultiRegion );
		if( pMultiRegionResult )
		{
			//m_bAirRouteExists = true;
			m_pAirActionOutline = new CEOSAIActionOutline( m_pActionDefinition, m_pActor );
			m_pAirActionOutline->CreateMovementRouteToMultiRegion( pMultiRegionPathwayResult, m_pTargetMultiRegion );
		}
		//else{ ASSERT( false ); m_bAirRouteExists = false; }
	}

	//m_bIsValid = RouteExists();

	// Do some analysis on the movement routes
	if( SeaRouteExists() || LandRouteExists() || SeaTransportRouteExists() || AirRouteExists() )
	{
		// Air or Water route (this is the best case scenario)
		//float fSimpleDistance = GetSimpleAirOrWaterDistance();
		/*
		INCOMPLETE:
		just get things working with transports.  i'll worry about the danger-avoidance
			versus direct route stuff later (which influences whether to attack intermediate
			enemy units).  also, i need to keep "polite" information in mind, especially
			when i'm about to invade a neutral player's territory (don't want my transport
			to be completely obvious on the way there).
		*-/
		// Look at the need for a transport, danger, etc
		if( m_eMovementType == EOSAIEnumMovementType3_Air )
		{
			m_bTargetIsWithinMyMovementRangeFromOneOfMyAirbases = GetAIUnitActor()->GetMovementLimit() > m_pActionDefinition->GetDistanceFromMyClosestAirbase();
		}
		else if( m_eMovementType == EOSAIEnumMovementType3_Water )
		{
			// I should check the SimpleRoute versus Danger/Sighting avoidance, polite pathways
		}
		else if( m_eMovementType == EOSAIEnumMovementType3_Land )
		{
			// Ground Unit
			m_fImaginarySeaTransportDividedByGroundRoute = 0.0f;
			m_fImaginaryAirTransportDividedByGroundRoute = 0.0f;
			m_fImaginaryAirTransportDividedByImaginarySeaTransport = 0.0f;

			/*
			if( GetRequiresATransport() )
			{
				// ImaginaryAirTransport versus ImaginarySeaTransport?
				if( SeaTransportRouteExists() )
				{
					m_fImaginaryAirTransportDividedByImaginarySeaTransport =
						m_pAirActionOutline->GetMovementDistance() / m_pSeaTransportActionOutline->GetMovementDistance();
				}
				else{} // Only an air-route exists
			}
			else
			{
			*-/
				// ImaginaryAirTransport versus ImaginarySeaTransport versus NoTransport
				if( SeaTransportRouteExists() && LandRouteExists() )
				{
					m_fImaginarySeaTransportDividedByGroundRoute =
						m_pSeaTransportActionOutline->GetMovementDistance() / m_pLandRouteActionOutline->GetMovementDistance();
				}
				if( AirRouteExists() && LandRouteExists() )
				{
					m_fImaginaryAirTransportDividedByGroundRoute =
						m_pAirActionOutline->GetMovementDistance() / m_pLandRouteActionOutline->GetMovementDistance();
				}
				if( AirRouteExists() && SeaTransportRouteExists() )
				{
					m_fImaginaryAirTransportDividedByImaginarySeaTransport =
						m_pAirActionOutline->GetMovementDistance() / m_pSeaTransportActionOutline->GetMovementDistance();
				}
			//}
			//bool bNonTransportRouteExists = pAIUnitActionIdea->GetRequiresATransport();
		}
	}
}
*/

/*
void  CEOSAIUnit2ActionIdea::CalculatePathway( EnumPathwayCalculationLevel ePathwayLevel )
{
	if( ePathwayLevel == EnumPathwayCalculationLevel_SimplePath )
	{
		ASSERT( m_pUnitPathway == NULL );
		// Create a UnitPathway object specific to this type of unit
		m_pUnitPathway = CEOSAIUnit2Pathway::CreatePathway( GetAIPoiObjectActor(), m_pUnitTemplate );
		m_pUnitPathway->CalculateInitialEvaluation();
	}
}
*/

void  CEOSAIUnit2ActionIdea::CalculateTimeToTarget()
{
	m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePathwaysToEverywhere.Continue();
	GetAIPoiObjectActor()->InvokeMultiRegionPathwaysToEverywhere();
	m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePathwaysToEverywhere.Stop();

	CEOSAILocation UnitStart = GetAIPoiObjectActor()->GetInitialState()->GetLocation();
	long iUnitGeo = GetAIPoiObjectActor()->GetInitialState()->GetGeo();

	CEOSAILocation TargetLocation;// = pTargetDesire->GetLocation();
	CEOSAIDesireSpatial* pTargetDesire = NULL; // may or may not be valid (submarine-warfare has no 'desire')
	/*
	if( GetTacticalProject() )
	{
		GetTacticalProject()->GetClosestPrimaryGoal( UnitStart, &pTargetDesire );
	}
	*/
	if( GetTacticalProject2() )
	{
		GetTacticalProject2()->GetClosestPrimaryGoal( UnitStart, &pTargetDesire );
		TargetLocation = GetTacticalProject2()->GetClosestPrimaryTargetLocation( UnitStart );
	}
	//ASSERT( pTargetDesire );

	//CWorldDescServer* pWorldDescServer = m_pAIBrain->GetWorldDescServer();

	//CEOSAILocation TargetLocation = pTargetDesire->GetLocation();
	//CGeo* pGeo = pWorldDescServer->GetGeo( TargetLocation );
	CEOSAIGeo* pGeo = g_pEOSAICommonData->GetAIGeo( TargetLocation );
	long iTargetGeo = 0; if( pGeo ){ iTargetGeo = pGeo->GetId(); }
	//float fAirDistance = GetCommonState()->GetWorldBuildDesc()->GetPixelDistance( UnitStart, TargetLocation );
	float fAirDistance = g_pWorldDistanceTool->GetDistance( UnitStart, TargetLocation );

	m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TargetDesire_InvokePathToEverywhere.Continue();
	//CEOSAIRegionPathwayMap* pPathway = pTargetDesire->InvokePathToEverywhere();//GetPathwayResult();
	CEOSAIMultiRegionManager2* pMultiRegionManager = g_pEOSAICommonData->GetMultiRegionManager();
	CEOSAIMultiRegion2* pTargetMultiRegion = pMultiRegionManager->GetMultiRegion( TargetLocation );
	CEOSAIRegionPathwayMap* pPathway = pTargetMultiRegion->InvokePathToEverywhere();
	m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TargetDesire_InvokePathToEverywhere.Stop();

	CEOSAIRegionPathwayItem* pUnitPathResult = pPathway->GetRegionResult( UnitStart );

	float fUnitMovementRate = m_pAIUnitTemplate->GetMovementRate();
	if( m_pAIUnitActor->IsAirUnit() )
	{
		// If this is an air unit, check the air-distance
		//if( fAirDistance <= 0.0f ){ ASSERT( false ); return 0.0f; }

		EnumMovementMotive eMovementMotive;
		if( pTargetDesire && pTargetDesire->GetInvolvesAnAttack() )
		{
			eMovementMotive = MovementMotive_Attack;
		}
		else
		{
			eMovementMotive = MovementMotive_RoundTrip;
		}

		// what about transports?  air-transports, sea-transports, etc
		float fTimeToTarget = GetAirPathwayTimeToTarget(
					GetAIUnitActor()->GetOwner(),
					m_pAIBrain,
					m_pAIUnitTemplate,
					UnitStart,
					TargetLocation,
					eMovementMotive ); // return or not return?

		m_bTransportRequired = false;
		m_fDistance = fAirDistance;
		m_fDirectMoveToTargetTime = fTimeToTarget;
		m_fTransportComboTime = 1000000.0f;
	}
	else
	{
		// INCOMPLETE: What about transports?
		//   How do I include transport movement rates?
		//   What about air-transports?  movement rates and range?
		float fPathwayDistance = fAirDistance;
		if( pUnitPathResult )
		{
			fPathwayDistance = pUnitPathResult->m_fDistance;
		}

		/*
		if( GetTacticalProject() )
		{
			ASSERT( GetTacticalProject()->GetPrimaryGoals()->IsEmpty() == FALSE );
		}
		*/
		//if( GetTacticalProject2() )
		//{
		//	ASSERT( GetTacticalProject2()->GetPrimaryGoals()->IsEmpty() == FALSE );
		//}
		//CEOSAIMultiRegionManager2* pMultiRegionManager = m_pAIBrain->GetMultiRegionManager();

		//CAITacticalProject* pTacticalProject = m_pTacticalProject;
		//
		float fMovementRate = m_pAIUnitActor->GetMovementRate();

		m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePath2.Continue();
		m_bTransportRequired = false;
		CEOSAIRegionPathwayMap* pPathway = NULL;
		CEOSAIRegionPathwayItem* pPathItem = NULL;
		if( GetAIUnitActor() )
		{
			CEOSAIUnit2* pAIUnit = GetAIUnitActor();
			pPathway = pAIUnit->InvokeDirectAIRegionMapToEverywhere();
			//pPathItem = pAIUnit->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( pTargetDesire->GetLocation() );
			pPathItem = pAIUnit->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );

#ifdef _DEBUG
			CEOSAIIntSet AccessibleRegionsList;
			for( long i=0; i<pPathway->GetArraySize(); i++ )
			{
				CEOSAIRegionPathwayItem* pPathItemX = pPathway->GetRegionResult( i );
				if( pPathItemX )
				{
					AccessibleRegionsList.Add( i );
				}
			}
			int gsdfd=0;

			//CEOSAIUnit2* pAIUnit = GetAIUnitActor();
			int hsdfsdf=0;
/*
			CEOSAIRegionPathwayItem* pPathItem3 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 3 );
			CEOSAIRegionPathwayItem* pPathItem386 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 386 );

			CEOSAIRegionPathwayItem* pPathItem4 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 4 );
			CEOSAIRegionPathwayItem* pPathItem118 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 118 );

			CEOSAIRegionPathwayItem* pPathItem384 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 384 );
			CEOSAIRegionPathwayItem* pPathItem407 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 407 );
			CEOSAIRegionPathwayItem* pPathItem406 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 406 );

			CEOSAIRegionPathwayItem* pPathItem390 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 390 );
			CEOSAIRegionPathwayItem* pPathItem401 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 401 );
			CEOSAIRegionPathwayItem* pPathItem400 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 400 );

			int h=0;
			CEOSAIRegionPathwayItem* pPathItem119 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 119 );

			CEOSAIRegionPathwayItem* pPathItem393 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 393 );
			CEOSAIRegionPathwayItem* pPathItem392 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 392 );
			CEOSAIRegionPathwayItem* pPathItem391 = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( 391 );
			int hew3=0;
			
			//pPathway = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere();
			AccessibleRegionsList.Clear();
			CEOSAIRegionPathwayMap* pPathway3 = GetAIUnitActor()->GetAIBrain()->GetAIRegionMapToEverywhere()->InvokeDirectAIRegionMapToEverywhere( 3, GetUnitTemplate() );
			for( long i=0; i<pPathway3->GetArraySize(); i++ )
			{
				CEOSAIRegionPathwayItem* pPathItemX = pPathway3->GetRegionResult( i );
				if( pPathItemX )
				{
					AccessibleRegionsList.Add( i );
					//CEOSAIRegionPathwayItem* pPathItemX = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( i );
					int hsfda3t=0;
				}
			}
			CEOSAIPathfinderPoint* pAIRegion387 = GetAIUnitActor()->GetAIBrain()->GetAIRegionManager()->GetPathfinderPoint( 387 );
			CEOSAIPathfinderPoint* pAIRegion399 = GetAIUnitActor()->GetAIBrain()->GetAIRegionManager()->GetPathfinderPoint( 399 );
			int h34234=0;
*/
#endif _DEBUG

			if( pPathItem )
			{
				m_bTransportRequired = false;
				m_fDistance = pPathItem->m_fDistance;
				if( m_fDistance < 100.0f ){ m_fDistance = fAirDistance; }
			}
			else
			{
				m_bTransportRequired = true;
				m_fDistance = 1000000.0f;
			}
		}
		if( GetAICityActor() )
		{
			ASSERT( false );
			pPathway = GetAICityActor()->InvokeLandAIRegionMapToEverywhere();
			//pPathItem = GetAICityActor()->InvokeLandAIRegionMapToEverywhere()->GetRegionResult( pTargetDesire->GetLocation() );
			pPathItem = GetAICityActor()->InvokeLandAIRegionMapToEverywhere()->GetRegionResult( TargetLocation );
			if( pPathItem == NULL ){ m_bTransportRequired = true; }
			m_fDistance = fAirDistance;
			//m_iTransportsRequired = (pPathItem->m_iGeoSwitches+1) / 2;
		}
		m_pAIBrain->m_MyUnits_CalculateTimeToTarget_InvokePath2.Stop();

		// If the start and end are close together, just use the air-distance, because it's more accurate
		//if( pPathItem->GetPathwayStepsCount( pPathway ) <= 1 ){ m_fDistance = fAirDistance; }
		//if( m_fDistance < 80.0f ){ m_fDistance = fAirDistance; }
		m_fTransportComboTime = 1000000.0f;
		if( m_bTransportRequired )
		{
			m_fDirectMoveToTargetTime = 1000000.0f;
		}
		else
		{
			//m_fDirectMoveToTargetTime = m_fDistance / fMovementRate;
			m_fDirectMoveToTargetTime = ( m_fDistance - 25.0f ) / fMovementRate;
			if( m_fDirectMoveToTargetTime < 0.0f ) m_fDirectMoveToTargetTime = 0.0f;
			//if( iTargetGeo != iUnitGeo ){ m_fDirectMoveToTargetTime += 0.5f; }
		}

		// Ships can't do geo-switches
		if( m_pAIUnitTemplate->IsSeaUnit() && m_bTransportRequired )
		{
			m_fDistance = 1000000.0f;
			m_fDirectMoveToTargetTime = 1000000.0f;
		}
		if( m_pAIUnitTemplate->IsGroundUnit() && 
			m_bTransportRequired &&
			//pTargetDesire->GetMultiRegion()->IsWater() == false )
			pTargetMultiRegion->IsWater() == false )
		{
			m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TransportComboAssistedMovementTime.Continue();
			m_fTransportComboTime = m_pAIUnitActor->GetTransportCombinedAssistedMovementTime( TargetLocation );
			m_pAIBrain->m_MyUnits_CalculateTimeToTarget_TransportComboAssistedMovementTime.Stop();
		}
	}
}

void  CEOSAIUnit2ActionIdea::InvokeMultiRegionTransportMovementDescs()
{
	if( m_bMultiRegionTransportMovementDescs_Exist ) return;

	GetAIPoiObjectActor()->InvokeMultiRegionPathwaysToEverywhere();

	CEOSAILocation UnitStart = GetAIPoiObjectActor()->GetInitialState()->GetLocation();

	//CEOSAIDesireSpatial* pTargetDesire = NULL;
	//GetTacticalProject()->GetClosestPrimaryGoal( UnitStart, &pTargetDesire );
	//ASSERT( pTargetDesire );

	//CWorldDescPlayer* pWorldDescPlayer = GetTacticalProject()->GetAIBrain()->GetWorldDescPlayer();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	//CEOSAILocation TargetLocation = pTargetDesire->GetLocation();
	//CEOSAILocation TargetLocation = GetTacticalProject()->GetClosestPrimaryTargetLocation( UnitStart );
	CEOSAILocation TargetLocation;
	/*
	if( GetTacticalProject() )
	{
		TargetLocation = GetTacticalProject()->GetClosestPrimaryTargetLocation( UnitStart );
	}
	*/
	if( GetTacticalProject2() )
	{
		TargetLocation = GetTacticalProject2()->GetClosestPrimaryTargetLocation( UnitStart );
	}
	CEOSAIMultiRegion2* pTargetMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( TargetLocation );
	//CGeo* pTargetGeo = pWorldDescServer->GetGeo( TargetLocation );
	CEOSAIGeo* pTargetGeo = g_pEOSAICommonData->GetAIGeo( TargetLocation );
	long iTargetGeo = 0; if( pTargetGeo ){ iTargetGeo = pTargetGeo->GetId(); }
	float fAirDistance = g_pWorldDistanceTool->GetDistance( UnitStart, TargetLocation );

	ASSERT( pTargetMultiRegion );
	if( m_pAIUnitTemplate->IsGroundUnit() &&
		m_bTransportRequired &&
		pTargetMultiRegion->IsWater() == false )
		//pTargetDesire->GetMultiRegion()->IsWater() == false )
	{
		ASSERT( m_UnitTransportMovementDescs.IsEmpty() );

		// Lookup some transport estimates
		//   We do a simple calculation here: air-distance to the ground unit
		m_pAIBrain->m_TransportCalculationsTime.Continue();

		//CEOSAILocation GroundUnitLocation = GetAIPoiObjectActor()->GetInitialState()->GetLocation();

		// I don't negotiate for transports with this code, I just take whatever transport can
		//   get here first (including the possibility of building one) and use it.  This is 
		//   simplistic, but it provides an estimate.
		//CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitTemplate );
		CEOSAITransportsSet  TransportsSet( m_pAIBrain->GetAIThoughtDatabase(), m_pAIUnitTemplate );
		//m_fTransportArrivalTime = TransportsSet.GetEarliestArrivalTimeToLocation_UseCrowsFlight( GroundUnitLocation );
		//m_fMoveToTargetTime += m_fTransportArrivalTime;

		if( m_pAIUnitActor )
		{
			float fBestTime = 1000000.0f;
			long iActorGeo = GetAIPoiObjectActor()->GetInitialState()->GetGeo();
			if( m_pAIUnitActor->IsInsideTransport() )
			{
				CEOSAIUnit2* pTransportAIUnit = m_pAIUnitActor->GetCurrentAITransport();
				CEOSAIUnitTemplate* pTransportUnitTemplate = pTransportAIUnit->GetAIUnitTemplate();

				bool bTransportCanReachTarget = false;
				if( pTransportUnitTemplate->IsAirUnit() )
				{
					// Actually, this should test the distance
					bTransportCanReachTarget = true;
				}
				if( pTransportUnitTemplate->IsSeaUnit() )
				{
					CEOSAIIntSet  TransportAccessibleGeos;
					CEOSAIRegion2* pAIRegion = pTransportAIUnit->GetInitialState()->GetAIRegion();
					ASSERT( pAIRegion->GetGeo() );
					if( pAIRegion->GetGeo()->GetGeoType() == CEOSAIGeo::Water )
					{
						TransportAccessibleGeos.Add( pAIRegion->GetGeo()->GetId() );
					}
					else // this could be a city (but cities are part of a larger land-geo)
					{
						POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
						while( pos )
						{
							CEOSAIAdjacentRegionDesc* pAdjDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
							CEOSAIGeo* pGeo = pAdjDesc->m_pRegion->GetGeo();
							if( pGeo->GetGeoType() == CEOSAIGeo::Water )
							{
								TransportAccessibleGeos.Add( pGeo->GetId() );
							}
						}
					}

					long iGroundUnitGeo = m_pAIUnitActor->GetInitialState()->GetGeo();
					g_pEOSAICommonData->AddAdjacentLandGeos( TransportAccessibleGeos );
					//
					if( TransportAccessibleGeos.Exists( iGroundUnitGeo ) &&
						TransportAccessibleGeos.Exists( iTargetGeo ) )
					{
						bTransportCanReachTarget = true;
					}
					int h=0;
				}
				if( bTransportCanReachTarget )
				{
					CEOSAITransportAssistedPath  DEBUG_TransportAssistedPath; // DEBUG
					float fTime = m_pAIUnitActor->GetTransportAssistedMultiRegionMovementPath( pTransportAIUnit, TargetLocation, &DEBUG_TransportAssistedPath );

					m_pAIBrain->m_iAIUnitTransportMovementDescCount++;

					CEOSAIUnit2TransportMovementDesc* pDesc = new CEOSAIUnit2TransportMovementDesc( GetAIUnitActor(), this );
					pDesc->m_pAITransport = pTransportAIUnit;
					pDesc->m_pTransportAIUnitTemplate = pTransportAIUnit->GetAIUnitTemplate();
					pDesc->m_fTotalTime = fTime;
					fBestTime = min( fBestTime, pDesc->m_fTotalTime );
					m_UnitTransportMovementDescs.AddTail( pDesc );
					int h=0;
				}
			}
			else
			{
				POSITION pos = TransportsSet.m_ExistingTransports.GetHeadPosition();
				while( pos )
				{
					CEOSAIUnit2* pTransportAIUnit = TransportsSet.m_ExistingTransports.GetNext( pos );
					CEOSAIUnitTemplate* pTransportUnitTemplate = pTransportAIUnit->GetAIUnitTemplate();

					bool bTransportCanReachTarget = false;
					if( pTransportUnitTemplate->IsAirUnit() )
					{
						// Actually, this should test the distance
						bTransportCanReachTarget = true;
					}
					if( pTransportUnitTemplate->IsSeaUnit() )
					{
						CEOSAIIntSet  TransportAccessibleGeos;
						CEOSAIRegion2* pAIRegion = pTransportAIUnit->GetInitialState()->GetAIRegion();
						ASSERT( pAIRegion->GetGeo() );
						if( pAIRegion->GetGeo()->GetGeoType() == CEOSAIGeo::Water )
						{
							TransportAccessibleGeos.Add( pAIRegion->GetGeo()->GetId() );
						}
						else // this could be a city (but cities are part of a larger land-geo)
						{
							POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
							while( pos )
							{
								CEOSAIAdjacentRegionDesc* pAdjDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
								CEOSAIGeo* pGeo = pAdjDesc->m_pRegion->GetGeo();
								if( pGeo->GetGeoType() == CEOSAIGeo::Water )
								{
									TransportAccessibleGeos.Add( pGeo->GetId() );
								}
							}
						}

						long iGroundUnitGeo = m_pAIUnitActor->GetInitialState()->GetGeo();

						g_pEOSAICommonData->AddAdjacentLandGeos( TransportAccessibleGeos );
						//
						if( TransportAccessibleGeos.Exists( iGroundUnitGeo ) &&
							TransportAccessibleGeos.Exists( iTargetGeo ) )
						{
							bTransportCanReachTarget = true;
						}
						int h=0;
					}
					if( bTransportCanReachTarget )
					{
						CEOSAITransportAssistedPath  DEBUG_TransportAssistedPath; // DEBUG
						float fTime = m_pAIUnitActor->GetTransportAssistedMultiRegionMovementPath( pTransportAIUnit, TargetLocation, &DEBUG_TransportAssistedPath );

						m_pAIBrain->m_iAIUnitTransportMovementDescCount++;

						CEOSAIUnit2TransportMovementDesc* pDesc = new CEOSAIUnit2TransportMovementDesc( GetAIUnitActor(), this );
						pDesc->m_pAITransport = pTransportAIUnit;
						pDesc->m_pTransportAIUnitTemplate = pTransportAIUnit->GetAIUnitTemplate();
						pDesc->m_fTotalTime = fTime;
						fBestTime = min( fBestTime, pDesc->m_fTotalTime );
						m_UnitTransportMovementDescs.AddTail( pDesc );
						int h=0;
					}
				}
			}
		}
		m_pAIBrain->m_TransportCalculationsTime.Stop();
	}
	m_bMultiRegionTransportMovementDescs_Exist = true;
}
/*
void  CEOSAIUnit2ActionIdea::InvokeAIRegionTransportMovementDescs()
{
	if( m_bAIRegionTransportMovementDescs_Exist ) return;

	GetAIPoiObjectActor()->InvokeMultiRegionPathwaysToEverywhere();

	CEOSAILocation UnitStart = GetAIPoiObjectActor()->GetInitialState()->GetLocation();

	CEOSAIDesireSpatial* pTargetDesire = NULL;
	GetTacticalProject()->GetClosestPrimaryGoal( UnitStart, &pTargetDesire );
	ASSERT( pTargetDesire );

	//CWorldDescPlayer* pWorldDescPlayer = GetTacticalProject()->GetAIBrain()->GetWorldDescPlayer();
	CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAILocation TargetLocation = pTargetDesire->GetLocation();
	CGeo* pGeo = pWorldDescServer->GetGeo( TargetLocation );
	long iTargetGeo = 0; if( pGeo ){ iTargetGeo = pGeo->GetGeoNumber(); }
	float fAirDistance = GetCommonState()->GetWorldBuildDesc()->GetPixelDistance( UnitStart, TargetLocation );

	if( m_pUnitTemplate->IsGroundUnit() &&
		m_bTransportRequired &&
		pTargetDesire->GetMultiRegion()->IsWater() == false )
	{
		ASSERT( m_UnitTransportMovementDescs.IsEmpty() );

		// Lookup some transport estimates
		//   We do a simple calculation here: air-distance to the ground unit
		GetTacticalProject()->GetAIBrain()->m_TransportCalculationsTime.Continue();

		//CEOSAILocation GroundUnitLocation = GetAIPoiObjectActor()->GetInitialState()->GetLocation();

		// I don't negotiate for transports with this code, I just take whatever transport can
		//   get here first (including the possibility of building one) and use it.  This is 
		//   simplistic, but it provides an estimate.
		//CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitTemplate );
		CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitTemplate );
		//m_fTransportArrivalTime = TransportsSet.GetEarliestArrivalTimeToLocation_UseCrowsFlight( GroundUnitLocation );
		//m_fMoveToTargetTime += m_fTransportArrivalTime;

		if( m_pAIUnitActor )
		{
			float fBestTime = 1000000.0f;
			long iActorGeo = GetAIPoiObjectActor()->GetInitialState()->GetGeo();

			POSITION pos;
			//Checkpoint::Write( "r8" );
			pos = TransportsSet.m_ExistingTransports.GetHeadPosition();
			while( pos )
			{
				CEOSAIUnit2* pTransportAIUnit = TransportsSet.m_ExistingTransports.GetNext( pos );
				CUnitTemplate* pTransportUnitTemplate = pTransportAIUnit->GetUnitTemplate();

				bool bTransportCanReachTarget = false;
				if( pTransportUnitTemplate->IsAirUnit() ){ bTransportCanReachTarget = true; }
				if( pTransportUnitTemplate->IsSeaUnit() )
				{
					CEOSAIIntSet  TransportAccessibleGeos;
					CEOSAIRegion2* pAIRegion = pTransportAIUnit->GetInitialState()->GetAIRegion();
					ASSERT( pAIRegion->GetGeo() );
					if( pAIRegion->GetGeo()->GetGeoType() == CGeo::Water )
					{
						TransportAccessibleGeos.Add( pAIRegion->GetGeo()->GetGeoNumber() );
					}
					else // this could be a city (but cities are part of a larger land-geo)
					{
						POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
						while( pos )
						{
							CEOSAIAdjacentRegionDesc* pAdjDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
							CGeo* pGeo = pAdjDesc->m_pRegion->GetGeo();
							if( pGeo->GetGeoType() == CGeo::Water )
							{
								TransportAccessibleGeos.Add( pGeo->GetGeoNumber() );
							}
						}
					}

					//long iTransportWaterStartGeo = pWorldDescPlayer->GetWaterGeoNumber7c( pTransportAIUnit->GetInitialState()->GetLocation() );
					//long iTransportStartGeo = pTransportAIUnit->GetInitialState()->GetGeo();
					long iGroundUnitGeo = m_pAIUnitActor->GetInitialState()->GetGeo();

					//CEOSAIIntSet  TransportAccessibleGeos;
					//if( iTransportWaterStartGeo > 0 ) TransportAccessibleGeos.Add( iTransportWaterStartGeo );
					//TransportAccessibleGeos.Add( iTransportStartGeo );
					//pWorldDescPlayer->AddAdjacentWaterGeos( TransportAccessibleGeos );
					//pWorldDescPlayer->AddAdjacentLandGeos( TransportAccessibleGeos );
					pWorldDescServer->AddAdjacentLandGeos( TransportAccessibleGeos );
					//
					if( TransportAccessibleGeos.Exists( iGroundUnitGeo ) &&
						TransportAccessibleGeos.Exists( iTargetGeo ) )
					{
						bTransportCanReachTarget = true;
					}
					int h=0;
				}
				if( bTransportCanReachTarget )
				{
					CEOSAITransportAssistedPath  DEBUG_TransportAssistedPath; // DEBUG
					float fTime = m_pAIUnitActor->GetTransportAssistedAIRegionMovementPath( pTransportAIUnit, TargetLocation, &DEBUG_TransportAssistedPath );

					GetTacticalProject()->GetAIBrain()->m_iAIUnitTransportMovementDescCount++;

					CEOSAIUnit2TransportMovementDesc* pDesc = new CEOSAIUnit2TransportMovementDesc( GetAIUnitActor(), this );
					pDesc->m_pAITransport = pTransportAIUnit;
					pDesc->m_pTransportUnitTemplate = pTransportAIUnit->GetUnitTemplate();
					pDesc->m_fTotalTime = fTime;
					fBestTime = min( fBestTime, pDesc->m_fTotalTime );
					m_UnitTransportMovementDescs.AddTail( pDesc );
					int h=0;
				}
			}
			//Checkpoint::Write( "r9" );
			/* // 30498234
			pos = TransportsSet.m_Cities.GetHeadPosition();
			while( pos )
			{
				CEOSAICity* pAICity = TransportsSet.m_Cities.GetNext( pos );
				long iCityWaterGeo = pAICity->GetServerCity()->GetWaterGeoNumber3();
				//CEOSAICityActionIdeaNode* pAICity = this->m_pTacticalProject->GetA
				POSITION pos2 = TransportsSet.m_BuildableTransports.GetHeadPosition();
				while( pos2 )
				{
					CUnitTemplate* pTransportUnitTemplate = TransportsSet.m_BuildableTransports.GetNext( pos2 );

					if( pAICity->GetObjectId() == 17 )
					{
						int g=0;
					}

					bool bTransportCanReachTarget = false;
					if( pTransportUnitTemplate->IsAirUnit() ){ bTransportCanReachTarget = true; }
					if( pTransportUnitTemplate->IsSeaUnit() && iCityWaterGeo > 0 )
					{
						CEOSAIIntSet  TransportAccessibleGeos;
						if( iCityWaterGeo > 0 ) TransportAccessibleGeos.Add( iCityWaterGeo );
						pWorldDescPlayer->AddAdjacentLandGeos( TransportAccessibleGeos );
						//
						if( TransportAccessibleGeos.Exists( m_pAIUnitActor->GetInitialState()->GetGeo() ) &&
							TransportAccessibleGeos.Exists( iTargetGeo ) )
						{
							bTransportCanReachTarget = true;
						}
					}
					if( bTransportCanReachTarget )
					{
						float fTransportBuildTime = pAICity->GetServerCity()->GetTimeToBuild( pTransportUnitTemplate, true );
						float fTime = m_pAIUnitActor->GetTransportAssistedAIRegionMovementPath( pAICity, pTransportUnitTemplate, fTransportBuildTime, TargetLocation );

						GetTacticalProject()->GetAIBrain()->m_iAIUnitTransportMovementDescCount++;

						CEOSAIUnit2TransportMovementDesc* pDesc = new CEOSAIUnit2TransportMovementDesc( GetAIUnitActor(), this );
						pDesc->m_pAITransport = NULL;
						pDesc->m_pAICity = pAICity;
						//pDesc->m_pAICityActionIdeaNode = pAICity;
						pDesc->m_pTransportUnitTemplate = pTransportUnitTemplate;
						pDesc->m_fBuildTime = fTransportBuildTime;
						pDesc->m_fTotalTime = fTime;// + pAICity->GetCity()->GetTimeToBuild( pTransportUnitTemplate, true );
						fBestTime = min( fBestTime, pDesc->m_fTotalTime );
						m_UnitTransportMovementDescs.AddTail( pDesc );
						int h=0;
					}
				}
			}
			*-/
		}
		GetTacticalProject()->GetAIBrain()->m_TransportCalculationsTime.Stop();
	}
	m_bAIRegionTransportMovementDescs_Exist = true;
}
*/
/*
void  CEOSAIUnit2ActionIdea::CalculatePathwayAndTimeToTarget()
{
	//Checkpoint::Write( "r1" );
	GetAIPoiObjectActor()->InvokeMultiRegionPathwaysToEverywhere();
	//Checkpoint::Write( "r2" );

	// This is an oversimplification
	//CEOSAIDesireSpatial* pDesireSpatial = GetTaskForce()->GetTacticalProject()->GetPrimaryDesires()->GetHead();
	//CEOSAIDesireSpatial* pDesireSpatial = GetTaskForce()->GetTacticalProject()->GetFirstDesire();//>GetPrimaryDesires()->GetHead();
	//CEOSAIDesireSpatial* pDesireSpatial = GetTacticalProject()->GetFirstDesire();//>GetPrimaryDesires()->GetHead();
	//CEOSAIDesireSpatial* pDesireSpatial = GetTacticalProject()->GetFirstDesire();//>GetPrimaryDesires()->GetHead();

	// INCOMPLETE:
	// This section needs to be customized for AirUnits versus Ground/Sea units
	//   INCOMPLETE: The air units need to have a specific target, and need to target different desires.

	CEOSAILocation UnitStart = GetAIPoiObjectActor()->GetInitialState()->GetLocation();

	CEOSAIDesireSpatial* pTargetDesire = NULL;
	GetTacticalProject()->GetClosestPrimaryGoal( UnitStart, &pTargetDesire );
	ASSERT( pTargetDesire );
	//Checkpoint::Write( "r2.a" );

	CWorldDescPlayer* pWorldDescPlayer = GetTacticalProject()->GetAIBrain()->GetWorldDescPlayer();
	//Checkpoint::Write( "r2.b" );

	// This isn't necessary for aircraft (maybe for danger), but all desires will have some
	//   ground or sea units as potential assets.
//	pTargetDesire->InvokeProcessingLevel( CEOSAIDesireSpatial::EnumCustomPathwayCalculation );
	//pTargetDesire->CreatePathwayFromEverywhereToHere();
	//pTargetDesire->InvokePathToEverywhere();
	//Checkpoint::Write( "r2.c" );

	CEOSAILocation TargetLocation = pTargetDesire->GetLocation();
	CGeo* pGeo = pWorldDescPlayer->GetGeo( TargetLocation );
	long iTargetGeo = 0; if( pGeo ){ iTargetGeo = pGeo->GetGeoNumber(); }
	float fAirDistance = GetCommonState()->GetWorldBuildDesc()->GetPixelDistance( UnitStart, TargetLocation );
	//Checkpoint::Write( "r2.d" );

	CEOSAIRegionPathwayMap* pPathway = pTargetDesire->InvokePathToEverywhere();//GetPathwayResult();

	//CMultiRegion* pUnitStartMultiRegion = m_pAIBrain->GetMultiRegionManager()->GetMultiRegion( UnitStart );
	//CMultiRegionResult* pUnitPathResult = pPathway->GetMultiRegionResult( pUnitStartMultiRegion->m_iId );
	CEOSAIRegionPathwayItem* pUnitPathResult = pPathway->GetRegionResult( UnitStart );

	//Checkpoint::Write( "r3" );
	float fUnitMovementRate = m_pUnitTemplate->GetMovementRate();
	if( m_pAIUnitActor->IsAirUnit() )
	{
		// If this is an air unit, check the air-distance
		//if( fAirDistance <= 0.0f ){ ASSERT( false ); return 0.0f; }

		EnumMovementMotive eMovementMotive;
		if( pTargetDesire->GetInvolvesAnAttack() )
		{
			eMovementMotive = MovementMotive_Attack;
		}
		else
		{
			eMovementMotive = MovementMotive_RoundTrip;
		}

		//Checkpoint::Write( "r4" );
		// what about transports?  air-transports, sea-transports, etc
		float fTimeToTarget = GetAirPathwayTimeToTarget(
					GetAIUnitActor()->GetOwner(),
					GetTacticalProject()->GetAIBrain(),
					m_pUnitTemplate,
					UnitStart,
					TargetLocation,
					eMovementMotive ); // return or not return?

		m_bTransportRequired = false;
		m_fDistance = fAirDistance;
		//m_fMoveToTargetTime = fTimeToTarget;
		m_fDirectMoveToTargetTime = fTimeToTarget;
		//m_fTransportAssistedMoveToTargetTime = 1000000.0f;
	}
	else
	{
		// INCOMPLETE: What about transports?
		//   How do I include transport movement rates?
		//   What about air-transports?  movement rates and range?
		float fPathwayDistance = fAirDistance;
		if( pUnitPathResult )
		{
			fPathwayDistance = pUnitPathResult->m_fDistance;
		}

		//Checkpoint::Write( "r5" );
		ASSERT( GetTacticalProject()->GetPrimaryGoals()->IsEmpty() == FALSE );
		//CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIActionDefinition()->GetAIBrain()->GetAIThoughtDatabase();
		CEOSAIMultiRegionManager2* pMultiRegionManager = GetTacticalProject()->GetAIBrain()->GetMultiRegionManager();

		CAITacticalProject* pTacticalProject = m_pTacticalProject;//GetTaskForce()->GetTacticalProject();
		//
		float fMovementRate = m_pAIUnitActor->GetMovementRate();

		//Checkpoint::Write( "r6" );
		m_bTransportRequired = false;
		CEOSAIRegionPathwayMap* pPathway = NULL;
		CEOSAIRegionPathwayItem* pPathItem = NULL;
		if( GetAIUnitActor() )
		{
			pPathway = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere();
			pPathItem = GetAIUnitActor()->InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( pTargetDesire->GetLocation() );
			if( pPathItem )
			{
				m_bTransportRequired = false;
				m_fDistance = pPathItem->m_fDistance;
				if( m_fDistance < 80.0f ){ m_fDistance = fAirDistance; }
			}
			else
			{
				m_bTransportRequired = true;
				m_fDistance = 1000000.0f;
			}
		}
		if( GetAICityActor() )
		{
			ASSERT( false );
			pPathway = GetAICityActor()->InvokeLandAIRegionMapToEverywhere();
			pPathItem = GetAICityActor()->InvokeLandAIRegionMapToEverywhere()->GetRegionResult( pTargetDesire->GetLocation() );
			if( pPathItem == NULL ){ m_bTransportRequired = true; }
			m_fDistance = fAirDistance;
			//m_iTransportsRequired = (pPathItem->m_iGeoSwitches+1) / 2;
		}

		// If the start and end are close together, just use the air-distance, because it's more accurate
		//if( pPathItem->GetPathwayStepsCount( pPathway ) <= 1 ){ m_fDistance = fAirDistance; }
		//if( m_fDistance < 80.0f ){ m_fDistance = fAirDistance; }
		m_fDirectMoveToTargetTime = m_fDistance / fMovementRate;
		if( m_fDistance == 1000000.0f ){ m_fDirectMoveToTargetTime = 1000000.0f; }
		m_fTransportComboTime = 1000000.0f;
		//m_fTransportAssistedMoveToTargetTime = 1000000.0f;

		//Checkpoint::Write( "r7" );
		// Ships can't do geo-switches
		if( m_pUnitTemplate->IsSeaUnit() && m_bTransportRequired )
		{
			m_fDistance = 1000000.0f;
			m_fDirectMoveToTargetTime = 1000000.0f;
			//m_fMoveToTargetTime = 1000000.0f;
		}
		if( m_pUnitTemplate->IsGroundUnit() && m_bTransportRequired &&
			pTargetDesire->GetMultiRegion()->IsWater() == false )
		{
			if( DO_TRANSPORT_COMBO_CALCULATION )
			{
				m_fTransportComboTime = m_pAIUnitActor->GetTransportComboAssistedMovementTime( TargetLocation );
				//m_fTransportAssistedMoveToTargetTime = m_fTransportComboTime;
			}
			if( CREATE_TRANSPORT_MOVEMENT_DESC )
			{
				// Lookup some transport estimates
				//   We do a simple calculation here: air-distance to the ground unit
				GetTacticalProject()->GetAIBrain()->m_TransportCalculationsTime.Continue();

				//CEOSAILocation GroundUnitLocation = GetAIPoiObjectActor()->GetInitialState()->GetLocation();

				// I don't negotiate for transports with this code, I just take whatever transport can
				//   get here first (including the possibility of building one) and use it.  This is 
				//   simplistic, but it provides an estimate.
				//CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitTemplate );
				CEOSAITransportsSet  TransportsSet( GetTacticalProject()->GetAIBrain()->GetAIThoughtDatabase(), m_pUnitTemplate );
				//m_fTransportArrivalTime = TransportsSet.GetEarliestArrivalTimeToLocation_UseCrowsFlight( GroundUnitLocation );
				//m_fMoveToTargetTime += m_fTransportArrivalTime;

				while( m_UnitTransportMovementDescs.IsEmpty() == FALSE ){ delete m_UnitTransportMovementDescs.RemoveHead(); }
				if( m_pAIUnitActor )
				{
					float fBestTime = 1000000.0f;
					long iActorGeo = GetAIPoiObjectActor()->GetInitialState()->GetGeo();

					POSITION pos;
					//Checkpoint::Write( "r8" );
					pos = TransportsSet.m_ExistingTransports.GetHeadPosition();
					while( pos )
					{
						CEOSAIUnit2* pTransportAIUnit = TransportsSet.m_ExistingTransports.GetNext( pos );
						CUnitTemplate* pTransportUnitTemplate = pTransportAIUnit->GetUnitTemplate();

						bool bTransportCanReachTarget = false;
						if( pTransportUnitTemplate->IsAirUnit() ){ bTransportCanReachTarget = true; }
						if( pTransportUnitTemplate->IsSeaUnit() )
						{
							CEOSAIIntSet  TransportAccessibleGeos;
							CEOSAIRegion2* pAIRegion = pTransportAIUnit->GetInitialState()->GetAIRegion();
							ASSERT( pAIRegion->GetGeo() );
							if( pAIRegion->GetGeo()->GetGeoType() == CGeo::Water )
							{
								TransportAccessibleGeos.Add( pAIRegion->GetGeo()->GetGeoNumber() );
							}
							else // this could be a city (but cities are part of a larger land-geo)
							{
								POSITION pos = pAIRegion->m_AdjacentRegions.GetHeadPosition();
								while( pos )
								{
									CEOSAIAdjacentRegionDesc* pAdjDesc = pAIRegion->m_AdjacentRegions.GetNext( pos );
									if( pAdjDesc->m_pRegion->GetGeo()->GetGeoType() == CGeo::Water )
									{
										TransportAccessibleGeos.Add( pAdjDesc->m_pRegion->GetGeo()->GetGeoNumber() );
									}
								}
							}

							//long iTransportWaterStartGeo = pWorldDescPlayer->GetWaterGeoNumber7c( pTransportAIUnit->GetInitialState()->GetLocation() );
							//long iTransportStartGeo = pTransportAIUnit->GetInitialState()->GetGeo();
							long iGroundUnitGeo = m_pAIUnitActor->GetInitialState()->GetGeo();

							//CEOSAIIntSet  TransportAccessibleGeos;
							//if( iTransportWaterStartGeo > 0 ) TransportAccessibleGeos.Add( iTransportWaterStartGeo );
							//TransportAccessibleGeos.Add( iTransportStartGeo );
							//pWorldDescPlayer->AddAdjacentWaterGeos( TransportAccessibleGeos );
							pWorldDescPlayer->AddAdjacentLandGeos( TransportAccessibleGeos );
							//
							if( TransportAccessibleGeos.Exists( iGroundUnitGeo ) &&
								TransportAccessibleGeos.Exists( iTargetGeo ) )
							{
								bTransportCanReachTarget = true;
							}
							/-*
							CEOSAIRegionPathwayMap* pTransportPathwayMap = pTransportAIUnit->InvokeDirectAIRegionMapToEverywhere();
							CEOSAIRegionPathwayMap* pGroundUnitPathwayMap = m_pAIUnitActor->InvokeDirectAIRegionMapToCoasts();

							long iCount1 = 0;
							long iCount2 = 0;

							// Is there an overlap?
							long iArraySize = pTransportPathwayMap->GetArraySize();
							for( long iRegion=0; iRegion<iArraySize; iRegion++ )
							{
								if( pTransportPathwayMap->GetRegionResult( iRegion ) ) iCount1++;
								if( pGroundUnitPathwayMap->GetRegionResult( iRegion ) ) iCount2++;

								if( pTransportPathwayMap->GetRegionResult( iRegion ) &&
									pGroundUnitPathwayMap->GetRegionResult( iRegion ) )
								{
									bTransportCanReachTarget = true;
									break;
								}
							}
							*-/
							int h=0;
						}
						if( bTransportCanReachTarget )
						{
							CEOSAITransportAssistedPath  DEBUG_TransportAssistedPath; // DEBUG
							float fTime = m_pAIUnitActor->GetTransportAssistedMovementPath( pTransportAIUnit, TargetLocation, &DEBUG_TransportAssistedPath );

							GetTacticalProject()->GetAIBrain()->m_iAIUnitTransportMovementDescCount++;

							CEOSAIUnit2TransportMovementDesc* pDesc = new CEOSAIUnit2TransportMovementDesc( GetAIUnitActor(), this );
							pDesc->m_pAITransport = pTransportAIUnit;
							pDesc->m_pTransportUnitTemplate = pTransportAIUnit->GetUnitTemplate();
							pDesc->m_fTotalTime = fTime;
							fBestTime = min( fBestTime, pDesc->m_fTotalTime );
							m_UnitTransportMovementDescs.AddTail( pDesc );
							int h=0;
						}
					}
					//Checkpoint::Write( "r9" );
					pos = TransportsSet.m_Cities.GetHeadPosition();
					while( pos )
					{
						CEOSAICity* pAICity = TransportsSet.m_Cities.GetNext( pos );
						long iCityWaterGeo = pAICity->GetCity()->GetWaterGeoNumber3();
						//CEOSAICityActionIdeaNode* pAICity = this->m_pTacticalProject->GetA
						POSITION pos2 = TransportsSet.m_BuildableTransports.GetHeadPosition();
						while( pos2 )
						{
							CUnitTemplate* pTransportUnitTemplate = TransportsSet.m_BuildableTransports.GetNext( pos2 );

							if( pAICity->GetObjectId() == 17 )
							{
								int g=0;
							}

							bool bTransportCanReachTarget = false;
							if( pTransportUnitTemplate->IsAirUnit() ){ bTransportCanReachTarget = true; }
							if( pTransportUnitTemplate->IsSeaUnit() && iCityWaterGeo > 0 )
							{
								CEOSAIIntSet  TransportAccessibleGeos;
								if( iCityWaterGeo > 0 ) TransportAccessibleGeos.Add( iCityWaterGeo );
								pWorldDescPlayer->AddAdjacentLandGeos( TransportAccessibleGeos );
								//
								if( TransportAccessibleGeos.Exists( m_pAIUnitActor->GetInitialState()->GetGeo() ) &&
									TransportAccessibleGeos.Exists( iTargetGeo ) )
								{
									bTransportCanReachTarget = true;
								}
								/-*
								CEOSAIRegionPathwayMap* pTransportPathwayMap = pAICity->InvokeWaterAIRegionMapToEverywhere();
								CEOSAIRegionPathwayMap* pGroundUnitPathwayMap = m_pAIUnitActor->InvokeDirectAIRegionMapToCoasts();

								long iCount2 = 0;
								long iCount3 = 0;

								// Is there an overlap?
								long iArraySize = pTransportPathwayMap->GetArraySize();
								for( long iRegion=0; iRegion<iArraySize; iRegion++ )
								{
									if( pTransportPathwayMap->GetRegionResult( iRegion ) ) iCount2++;
									if( pGroundUnitPathwayMap->GetRegionResult( iRegion ) ) iCount3++;

									if( pTransportPathwayMap->GetRegionResult( iRegion ) &&
										pGroundUnitPathwayMap->GetRegionResult( iRegion ) )
									{
										bTransportCanReachTarget = true;
										break;
									}
								}
								*-/
							}
							if( bTransportCanReachTarget )
							{
								float fTransportBuildTime = pAICity->GetCity()->GetTimeToBuild( pTransportUnitTemplate, true );
								float fTime = m_pAIUnitActor->GetTransportAssistedMovementPath( pAICity, pTransportUnitTemplate, fTransportBuildTime, TargetLocation );

								GetTacticalProject()->GetAIBrain()->m_iAIUnitTransportMovementDescCount++;

								CEOSAIUnit2TransportMovementDesc* pDesc = new CEOSAIUnit2TransportMovementDesc( GetAIUnitActor(), this );
								pDesc->m_pAITransport = NULL;
								pDesc->m_pAICity = pAICity;
								//pDesc->m_pAICityActionIdeaNode = pAICity;
								pDesc->m_pTransportUnitTemplate = pTransportUnitTemplate;
								pDesc->m_fBuildTime = fTransportBuildTime;
								pDesc->m_fTotalTime = fTime;// + pAICity->GetCity()->GetTimeToBuild( pTransportUnitTemplate, true );
								fBestTime = min( fBestTime, pDesc->m_fTotalTime );
								m_UnitTransportMovementDescs.AddTail( pDesc );
								int h=0;
							}
						}
					}
					//Checkpoint::Write( "r10" );
					//m_fTransportAssistedMoveToTargetTime = fBestTime;
				}
				GetTacticalProject()->GetAIBrain()->m_TransportCalculationsTime.Stop();
				int g=0;
			}

			// actually, the transport can change the "MoveToTarget" time based on it's MovementRate

			/-*
			lookup transports in the buildable/exists list?
			lookup transports in the nearby area?
			set "needs transport" in the AIJobs section?  (that doesn't specify which units need transport,
			  and that means two ground units on opposite islands might use that transport)
			if I have no transports, then the build-time will severely impact the value of this UnitActionIdea
			*-/
		}
	}
}
*/

/*
	float fTimeToTarget = GetAIThoughtDatabase()->GetTimeToTarget(
		pAIUnit->GetInitialState()->GetLocation(), GetLocation(),
		CEOSAIThoughtDatabase::MovementMotive_Recon,
		pAIUnit->GetUnitTemplate(), m_PathwayResult,
		GetAIThoughtDatabase()->GetUnitsICanBuildOrHave() );
*/
/*
float CEOSAIUnit2ActionIdea::GetBestCaseTotalTimeToTarget()
{
	float fDirectTime = m_fBuildTime + m_fDirectMoveToTargetTime;
	float fTransportTime = max( m_fBuildTime, m_fTransportArrivalTime ) + m_fTransportAssistedMoveToTargetTime;
	return min( fDirectTime, fTransportTime );
}
*/
/*
float CEOSAIUnit2ActionIdea::GetBuildAndTransportAndMoveToTargetTime()
{
	return max( m_fBuildTime, m_fTransportArrivalTime ) + m_fTransportAssistedMoveToTargetTime;
}
*/
/*
float CEOSAIUnit2ActionIdea::GetBuildAndDirectMoveToTargetTime()
{
	return m_fBuildTime + m_fDirectMoveToTargetTime;
	//	m_pAIUnitActor->GetInitialState()->GetTime() + GetTotalTimeToTarget();
}
*/
// Used for calculating air-pathways from Point A to Point B
float CEOSAIUnit2ActionIdea::GetAirPathwayTimeToTarget(
			long iAirUnitOwner,
			CEOSAIBrain* pAIBrain,
			CEOSAIUnitTemplate* pAirUnit,
			CEOSAILocation UnitStart,
			CEOSAILocation UnitEnd,
			EnumMovementMotive eMovementMotive ) // return or not return?
{
	ASSERT( pAirUnit->IsAirUnit() );
	if( pAirUnit->IsAirUnit() )
	{
		//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
		//float fAirDistance = pWorldBuildDesc->GetPixelDistance( UnitStart, UnitEnd );
		float fAirDistance = g_pWorldDistanceTool->GetDistance( UnitStart, UnitEnd );

		// If this is an air unit, check the air-distance
		float fAirUnitMovementRate = pAirUnit->GetMovementRate();
		if( fAirUnitMovementRate <= 0.0f ){ return 0.0f; }
		float fAirUnitMovementLimit = pAirUnit->GetMovementLimit();

		float fMaxDistance = 0.0f;
		if( eMovementMotive == MovementMotive_Attack    ){ fMaxDistance = pAirUnit->GetMovementRangeForAttack(); }
		if( eMovementMotive == MovementMotive_RoundTrip ){ fMaxDistance = pAirUnit->GetMovementLimit()/2.0f; }

		if( fAirUnitMovementLimit < fMaxDistance ) // Direct flight is possible
		{
			return fAirDistance / fAirUnitMovementRate;
		}
		else // Need to stop somewhere first.  Lookup the other airbases on the map
		{
			// INCOMPLETE: Can the unit fly to the carriers, and land on them?  (missiles can't)
			// INCOMPLETE: Can the aircraft fly directly to the airbase, or does it need intermediates?
			CEOSAIAirbasesSet  Airbases;
			pAIBrain->GetAIThoughtDatabase()->GetAirbases( iAirUnitOwner, pAirUnit, &Airbases );
			float fAirbaseDistance = Airbases.GetClosestAirbaseDistance( UnitEnd );
			if( fAirbaseDistance < fMaxDistance )
			{
				return 2.0f + ( fAirDistance / fAirUnitMovementRate );
			}
			else // all of the airbases are out-of-range
			{
				return 1000000.0f;
			}
		}
	}
	return 1000000.0f;
}

// static function
/*
float CEOSAIUnit2ActionIdea::GetTimeToTarget(
			CUnitTemplate* pTraveller,
			CEOSAILocation UnitStart,
			CEOSAILocation UnitEnd,
			EnumMovementMotive eMovementMotive,
			CMultiRegionPathwayResult& PathwayResult,
			CEOSAIUnitTemplateSet* pAssistUnits )
{
	CWorldDesc* pWorldDesc = m_pAIBrain->GetWorldDesc();

	CMultiRegion* pUnitStartMultiRegion = m_pAIBrain->GetMultiRegionManager()->GetMultiRegion( UnitStart );
	CMultiRegionResult* pUnitPathResult = PathwayResult.GetMultiRegionResult( pUnitStartMultiRegion->m_iId );

	// If this is an air unit, check the air-distance
	float fDistance = pUnitPathResult->m_fDistance;
	float fTravellerMovementRate = pTraveller->GetMovementRate();
	if( fTravellerMovementRate <= 0.0f ){ fTravellerMovementRate = 0.001f; } // to prevent divide by zero
	if( pTraveller->IsAirUnit() )
	{
		fDistance = pWorldDesc->GetPixelDistance( UnitStart, UnitEnd );
		if( fDistance <= 0.0f ) return 0.0f;

		if( eMovementMotive == MovementMotive_Attack )
		{
			if( fDistance < pTraveller->GetMovementRangeForAttack() ){ return fDistance / fTravellerMovementRate; }
			else // This air unit need to land somewhere first
			{
				return 2.0f + fDistance / fTravellerMovementRate;
			}
		}
		if( eMovementMotive == MovementMotive_Recon )
		{
			if( fDistance < pTraveller->GetMovementLimit()/2.0f ){ return fDistance / fTravellerMovementRate; }
			else // This air unit need to land somewhere first
			{
				return 2.0f + fDistance / fTravellerMovementRate;
			}
		}
	}
	else // This is a ground/sea unit
	{
		if( fDistance <= 0.0f ) return 0.0f;
		float fTravellerMovementRate = pTraveller->GetMovementRate();
		float fTimeToTarget = 1000000.0f;
		if( pTraveller->GetMovementRate() > 0.0f ){ fTimeToTarget = fDistance / fTravellerMovementRate; }

		CEOSAIUnitTemplateSet Transports;
		pAssistUnits->GetTransports( pTraveller, &Transports );
		POSITION pos = Transports.m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pTransport = Transports.m_List.GetNext( pos );
			if( pTransport->GetMovementRate() > 0.0f &&
				pTransport->GetMovementRate() > fTravellerMovementRate )
			{
				if( pTransport->IsAirUnit() )
				{
					float fDistance2 = pWorldDesc->GetPixelDistance( UnitStart, UnitEnd );
					if( fDistance2 <= 0.0f ) return 0.0f;
					if( fDistance2 < pTransport->GetMovementLimit()/2.0f )
					{
						fTimeToTarget = fDistance2 / pTransport->GetMovementRate();
					}
					else // This air unit need to land somewhere first
					{
						fTimeToTarget = 2.0f + fDistance2 / pTransport->GetMovementRate();
					}
				}
				else
				{
					fTimeToTarget = fDistance / pTransport->GetMovementRate();
				}
			}
		}
		return fTimeToTarget;

		/-*
		// What about if the traveller took a transport there?
		//   INCOMPLETE: if I'm dealing with air-units, I should take MovementLimit into account
		//   Ait-units can be either the Traveller or the Transport
		//   INCOMPLETE: I should take into account the existing cities and airfields (and allies)
		float fFastestTransport = GetMovementRate_FastestTransport( pTraveller, pAssistUnits );
		float fMovementRate = max( pTraveller->GetMovementRate(), fFastestTransport );
		if( fMovementRate <= 0.0f ) return 0.0f;

		return fDistance / fMovementRate;
		*-/
	}
	return 1000000.0f;
}
*/

/*
float CEOSAIUnit2ActionIdea::GetTimeToTargetUsingNoTransports()
{
	ASSERT( m_pMovementPathIdeas );
	if( m_pMovementPathIdeas->GetShortestPath() == NULL )
	{
		ASSERT( false );
		return 1000000.0f;
	}
	CMovementPath* pShortestNoTransportPath = m_pMovementPathIdeas->GetShortestNoTransportRoute();
	if( pShortestNoTransportPath )
	{
		float fUnitMovementRate = m_pUnitTemplate->GetMovementRate();
		if( fUnitMovementRate == 0.0f ){ return 1000000.0f; }
		return pShortestNoTransportPath->m_fTotalDistance / fUnitMovementRate;
	}
	// There are no "No Transport" routes
	return 1000000.0f;
}

float CEOSAIUnit2ActionIdea::GetTimeToTargetUsingTransport( CEOSAIPoiObject* pAITransportOrCity )
{
	CMovementPathForGroundSeaUnit* pShortestSeaTransportPath = m_pMovementPathIdeas->GetShortestSeaTransportPath();
	if( pShortestSeaTransportPath )
	{
		CEOSAIPoiObjectProximity2* pProx = pShortestSeaTransportPath->GetTransportsProximityObjectToPickup( pAITransportOrCity );
		ASSERT( pProx );
		return GetTimeToTargetUsingTransport( pProx );
	}
	return 1000000.0f;
}

float CEOSAIUnit2ActionIdea::GetTimeToTargetUsingTransport( CEOSAIPoiObjectProximity2* pAITransportOrCityProx )
{
	ASSERT( m_pMovementPathIdeas );
	if( m_pMovementPathIdeas->GetShortestPath() == NULL )
	{
		ASSERT( false );
		return 1000000.0f;
	}
	CMovementPathForGroundSeaUnit* pShortestSeaTransportPath = m_pMovementPathIdeas->GetShortestSeaTransportPath();
	if( pShortestSeaTransportPath )
	{
		float fUnitMovementRate = m_pUnitTemplate->GetMovementRate();
		if( fUnitMovementRate == 0.0f ){ return 1000000.0f; }

		float fGroundDistanceToPickup = pShortestSeaTransportPath->GetDistanceToFirstWater();
		float fGroundTimeToPickup = fGroundDistanceToPickup / fUnitMovementRate;
		float fDistanceFromPickupToDropoff = pShortestSeaTransportPath->GetDistanceOverWater();
		float fTimeFromPickupToDropoff = 0.0f; // Depends on transport rate
		float fGroundDistanceFromDropoffToTarget = pShortestSeaTransportPath->GetDistanceOverLand() - pShortestSeaTransportPath->GetDistanceToFirstWater();
		float fGroundTimeFromDropoffToTarget = fGroundDistanceFromDropoffToTarget / fUnitMovementRate;

		//float fUnitMovementRate = m_pAIUnitActorTemplate->GetUnitTemplate()->GetMovementRate();
		//if( fUnitMovementRate == 0.0f ){ return 1000000.0f; }
		//fBestRouteTime = pMovementPath->m_fDistance / fUnitMovementRate;
		float fTransportTimeToPickup = 1000000.0f;
		CEOSAIUnit2* pAITransport = dynamic_cast< CEOSAIUnit2* >( pAITransportOrCityProx->m_pAIPoiObject );
		ASSERT( pAITransport );
		if( pAITransport &&
			pAITransport->GetUnit()->CanContain( m_pUnitTemplate ) )
		{
			float fTransportMovementRate = pAITransport->GetMovementRate();
			if( fTransportMovementRate > 0.0f )
			{
				fTransportTimeToPickup = pAITransportOrCityProx->m_fDistance / fTransportMovementRate;
				fTimeFromPickupToDropoff = fDistanceFromPickupToDropoff / fTransportMovementRate;
			}
			float fTimeForThisOption =
				max( fGroundTimeToPickup, fTransportTimeToPickup ) +
				fTimeFromPickupToDropoff +
				fGroundTimeFromDropoffToTarget;
			return fTimeForThisOption;
		}

		return 1000000.0f;//fTimeForThisOption;
	}
	// There are no "Sea Transport" routes
	return 1000000.0f;
}

float CEOSAIUnit2ActionIdea::GetTimeToTargetForHypotheticalTaskForce_AllowTransport()
{
	ASSERT( m_pMovementPathIdeas );
	if( m_pMovementPathIdeas->GetShortestPath() == NULL )
	{
		ASSERT( false );
		return 1000000.0f;
	}

	bool bGetDirectRoutes = false;
	bool bGetSeaTransportRoutes = false;

	if( m_pMovementPathIdeas->GetTransportValueBasedOnDistance() == 0.0f ){ bGetDirectRoutes = true; }
	else if( m_pMovementPathIdeas->GetTransportValueBasedOnDistance() == 1.0f ){ bGetSeaTransportRoutes = true; }
	else{ bGetDirectRoutes = true; bGetSeaTransportRoutes = true; }

	float fBestRouteTime = 1000000.0f;

	if( bGetDirectRoutes )
	{
		CMovementPath* pMovementPath = m_pMovementPathIdeas->GetShortestPath();
		float fUnitMovementRate = m_pUnitTemplate->GetMovementRate();
		if( fUnitMovementRate == 0.0f ){ return 1000000.0f; }
		fBestRouteTime = pMovementPath->m_fTotalDistance / fUnitMovementRate;
	}
	if( bGetSeaTransportRoutes ) // routes involving one sea transport
	{
		float fUnitMovementRate = m_pUnitTemplate->GetMovementRate();
		if( fUnitMovementRate == 0.0f ){ return 1000000.0f; }

		// Transport time to pickup
		// GroundUnit time to pickup
		// Sea route from pickup to dropoff
		// Dropoff to target

		CMovementPathForGroundSeaUnit* pShortestSeaTransportPath = m_pMovementPathIdeas->GetShortestSeaTransportPath();
		pShortestSeaTransportPath->InvokeTransportAndCityProximityToPickupList();
		//
		float fGroundDistanceToPickup = pShortestSeaTransportPath->GetDistanceToFirstWater();
		float fGroundTimeToPickup = fGroundDistanceToPickup / fUnitMovementRate;
		float fDistanceFromPickupToDropoff = pShortestSeaTransportPath->GetDistanceOverWater();
		float fTimeFromPickupToDropoff = 0.0f; // Depends on transport rate
		float fGroundDistanceFromDropoffToTarget = pShortestSeaTransportPath->GetDistanceOverLand() - pShortestSeaTransportPath->GetDistanceToFirstWater();
		float fGroundTimeFromDropoffToTarget = fGroundDistanceFromDropoffToTarget / fUnitMovementRate;

		// Construct a list of transports that can carry this unit
		CList< CUnitTemplate* >  PotentialTransports;
		CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pTaskForce->GetAIBrain()->GetAIThoughtDatabase();
		CEOSAIUnitTemplateSet* pBuildableSet = pAIThoughtDatabase->GetUnitsICanBuild();
		CUnitTemplate* pThisUnitTemplate = m_pUnitTemplate;
		// Iterate over the possible built targets
		POSITION pos = pBuildableSet->m_List.GetHeadPosition();
		while( pos )
		{
			CUnitTemplate* pTransportTemplate = pBuildableSet->m_List.GetNext( pos );
			if( pTransportTemplate->CanContainUnits() &&
				pTransportTemplate->CanContain( pThisUnitTemplate ) )
			{
				//CString strTransport = pTransportTemplate->GetUnitTemplate()->GetInternalName();
				//PotentialTransports.AddTail( strTransport );
				PotentialTransports.AddTail( pTransportTemplate );
			}
		}

		// Get different times over water
		pos = pShortestSeaTransportPath->GetTransportAndCityProximityToPickupList()->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObjectProximity2* pProx = pShortestSeaTransportPath->GetTransportAndCityProximityToPickupList()->GetNext( pos );

			float fTransportTimeToPickup = 1000000.0f;
			CEOSAIUnit2* pAITransport = dynamic_cast< CEOSAIUnit2* >( pProx->m_pAIPoiObject );
			CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( pProx->m_pAIPoiObject );
			ASSERT( pAITransport || pAICity );
			if( pAITransport )
			{
				if( pAITransport->GetUnit()->CanContain( m_pUnitTemplate ) )
				{
					float fTransportMovementRate = pAITransport->GetMovementRate();
					if( fTransportMovementRate > 0.0f )
					{
						fTransportTimeToPickup = pProx->m_fDistance / fTransportMovementRate;
						fTimeFromPickupToDropoff = fDistanceFromPickupToDropoff / fTransportMovementRate;
					}
				}
				float fTimeForThisOption =
					max( fGroundTimeToPickup, fTransportTimeToPickup ) +
					fTimeFromPickupToDropoff +
					fGroundTimeFromDropoffToTarget;

				if( fBestRouteTime > fTimeForThisOption ){ fBestRouteTime = fTimeForThisOption; }
			}
			if( pAICity )
			{
				POSITION pos = PotentialTransports.GetHeadPosition();
				while( pos )
				{
					CUnitTemplate* pTransportTemplate = PotentialTransports.GetNext( pos );
					//CString strTransport = pTransportTemplate->GetInternalName();
					float fBuildTime = pAICity->GetCity()->GetTimeToBuild( pTransportTemplate ); // include partial build info
					// Movement time?
					float fTransportMovementRate = pTransportTemplate->GetMovementRate();
					if( fTransportMovementRate > 0.0f )
					{
						fTransportTimeToPickup = pProx->m_fDistance / fTransportMovementRate;
						fTimeFromPickupToDropoff = fDistanceFromPickupToDropoff / fTransportMovementRate;

						float fTimeForThisOption =
							max( fGroundTimeToPickup, fBuildTime + fTransportTimeToPickup ) +
							fTimeFromPickupToDropoff +
							fGroundTimeFromDropoffToTarget;

						if( fBestRouteTime > fTimeForThisOption ){ fBestRouteTime = fTimeForThisOption; }
					}
				}
				/*
				CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pActionDefinition->GetAIBrain()->GetAIThoughtDatabase();
				CEOSAIUnitTemplateSet* pBuildableSet = pAIThoughtDatabase->GetUnitsICanBuild();
				CUnitTemplate* pThisUnitTemplate = m_pAIUnitActorTemplate->GetUnitTemplate();
				// Iterate over the possible built targets
				POSITION pos = pBuildableSet->m_List.GetHeadPosition();
				while( pos )
				{
					CEOSAIUnitTemplate* pTransportTemplate = pBuildableSet->m_List.GetNext( pos );
					if( pTransportTemplate->GetUnitTemplate()->CanContainUnits() &&
						pTransportTemplate->GetUnitTemplate()->CanContain( pThisUnitTemplate ) )
					{
						// Build time?
						CString strTransport = pTransportTemplate->GetUnitTemplate()->GetInternalName();
						float fBuildTime = pAICity->GetCity()->GetTimeToProduceBuildOption( strTransport ); // include partial build info
						// Movement time?
						float fTransportMovementRate = pTransportTemplate->GetUnitTemplate()->GetMovementRate();
						if( fTransportMovementRate > 0.0f )
						{
							fTransportTimeToPickup = fBuildTime + pProx->m_fDistance / fTransportMovementRate;
							fTimeFromPickupToDropoff = fDistanceFromPickupToDropoff / fTransportMovementRate;

							float fTimeForThisOption =
								max( fGroundTimeToPickup, fTransportTimeToPickup ) +
								fTimeFromPickupToDropoff +
								fGroundTimeFromDropoffToTarget;

							if( fBestRouteTime > fTimeForThisOption ){ fBestRouteTime = fTimeForThisOption; }
						}
					}
				}
				*-/
			}
			/*
			float fTimeForThisOption =
				max( fGroundTimeToPickup, fTransportTimeToPickup ) +
				fTimeFromPickupToDropoff +
				fTimeFromPickupToDropoff;

			if( fBestRouteTime > fTimeForThisOption )
			{
				fBestRouteTime = fTimeForThisOption;
			}
			*-/
		}
	}
	return fBestRouteTime;
}
*/
float CEOSAIUnit2ActionIdea::GetUnitProductionIronCost1()
{
	return GetAIUnitTemplate()->GetProductionAndIronCost1();
	/*
	float fIronMult = m_pTacticalProject->GetAIBrain()->GetCitResValueMultiplier()->m_fIronValueMultiplier;
	return
		( m_pUnitTemplate->GetProductionCost() ) +
		( fIronMult * m_pUnitTemplate->GetIronCost() );
	*/
}

/*
bool  CEOSAIUnit2ActionIdea::RouteExists() // Can return false in the case of sea units
{
	return ( SeaRouteExists() || LandRouteExists() || SeaTransportRouteExists() || AirRouteExists() );
}
*/
/*
float CEOSAIUnit2ActionIdea::GetSimpleAirOrWaterDistance() // Air/Ground Units: Air distance, SeaUnits: Water distance
{
	if( m_bAirRouteExists ){ return m_pAirActionOutline->GetMovementDistance(); }
	if( m_bSingleGeoRouteExists ){ return m_pSingleGeoActionOutline->GetMovementDistance(); }
	return 1000000.0f;
}
*/
/*
float CEOSAIUnit2ActionIdea::GetShortestPathDistance()
{
	if( m_pMovementPathIdeas &&
		m_pMovementPathIdeas->GetShortestPath() )
	{
		return m_pMovementPathIdeas->GetShortestPath()->GetTotalDistance();
	}
	return 1000000.0f;
}
*/
/*
float CEOSAIUnit2ActionIdea::GetShortestActionOutlineDistance()
{
	float fMinimumDistance = 1000000.0f;
	if( AirRouteExists() )
	{
		float fDistance = m_pAirActionOutline->GetMovementDistance();
		if( fMinimumDistance > fDistance ){ fMinimumDistance = fDistance; }
	}
	if( SeaTransportRouteExists() )
	{
		float fDistance = m_pSeaTransportActionOutline->GetMovementDistance();
		if( fMinimumDistance > fDistance ){ fMinimumDistance = fDistance; }
	}
	if( LandRouteExists() )
	{
		float fDistance = m_pLandRouteActionOutline->GetMovementDistance();
		if( fMinimumDistance > fDistance ){ fMinimumDistance = fDistance; }
	}
	if( SeaRouteExists() )
	{
		float fDistance = m_pSeaRouteActionOutline->GetMovementDistance();
		if( fMinimumDistance > fDistance ){ fMinimumDistance = fDistance; }
	}
	return fMinimumDistance;
}

float CEOSAIUnit2ActionIdea::GetShortestActionOutlineTimeUsingTransportMovementRates()
{
	if( m_pAIUnitActor->IsGroundUnit() )
	{
		float fUnitMovementRate = m_pAIUnitActorTemplate->GetUnitTemplate()->GetMovementRate();
		float fFastestAirTransportMovementRate = m_pActor->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestAirTransportMovementRate();
		float fFastestSeaTransportMovementRate = m_pActor->GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave()->GetFastestSeaTransportMovementRate();

		float fMinimumTime = 1000000.0f;
		if( AirRouteExists() && fFastestAirTransportMovementRate > 0.0f )
		{
			float fTime = m_pAirActionOutline->GetMovementDistance() / max( fUnitMovementRate, fFastestAirTransportMovementRate );
			if( fMinimumTime > fTime ){ fMinimumTime = fTime; }
		}
		if( SeaTransportRouteExists() && fFastestSeaTransportMovementRate > 0.0f )
		{
			float fTime = m_pSeaTransportActionOutline->GetMovementDistance() / max( fUnitMovementRate, fFastestSeaTransportMovementRate );
			if( fMinimumTime > fTime ){ fMinimumTime = fTime; }
		}
		if( LandRouteExists() )
		{
			float fTime = m_pLandRouteActionOutline->GetMovementDistance() / fUnitMovementRate;
			if( fMinimumTime > fTime ){ fMinimumTime = fTime; }
		}
		return fMinimumTime;
	}
	else
	{
		return GetShortestActionOutlineDistance() / m_pAIUnitActor->GetMovementRate();
	}
}
*/
/*
void CEOSAIUnit2ActionIdea::FigureOutRoleInTheActionDefinition()
{
	/-*
	if( pUnitActionIdea->GetAIUnitActor() )
	{
		if( pUnitActionIdea->GetAIUnitActor()->GetPoiMobile()->GetName() == "Bomber (Class 1)" )
		{
			int z=0;
		}
	}
	*-/

	/-*
	m_bActionRole_TransportOrCarrier = false;
	m_bActionRole_CaptureCitRes = false;
	m_bActionRole_AirfieldBuilder = false;
	m_bActionRole_Combat = false;
	*-/

	/-*
	CAITacticalProjectUnitNeeds2* pActionUnitNeeds = m_pActionDefinition->GetUnitNeeds();

	if( pActionUnitNeeds->m_iNeed_GroundCitResHunter > 0 )
	{
		m_bActionRole_CaptureCitRes = true;
		m_bGroundCitResHunter = m_pUnitTemplate->IsGroundCitResHunter();
	}
	if( pActionUnitNeeds->m_iNeed_SeaResHunter > 0 )
	{
		m_bActionRole_CaptureCitRes = true;
		m_bSeaResHunter = m_pUnitTemplate->IsSeaResHunter();
	}
	if( pActionUnitNeeds->m_iNeed_AirfieldBuilder > 0 )
	{
		m_bActionRole_AirfieldBuilder = true;
		m_bAirfieldBuilder = m_pUnitTemplate->CanBuildAirfields();
	}
	if( pActionUnitNeeds->m_iNeed_TransportCapacity > 0 )
	{
		m_bActionRole_TransportOrCarrier = true;
		m_iTransportCapability = m_pUnitTemplate->GetNumberOfGroundUnitsThatICanContain();
		m_iCarrierCapability = m_pUnitTemplate->GetNumberOfAirUnitsThatICanContain();
	}

	if( m_pAIUnitActor )
	{
		m_fCombatValue = m_pActionDefinition->GetCombatValueAgainstEnemy( GetAIUnitActor() );
	}
	else
	{
		m_fCombatValue = m_pActionDefinition->GetCombatValueAgainstEnemy( GetUnitActorTemplate() );
	}
	*-/
	//
	/-*
	if( m_fCombatValue > 0.0f )
	{
		m_bActionRole_Combat = true;
		m_fCombatEfficiency = m_fCombatValue / m_pUnitTemplate->GetProductionCost();
	}
	*-/
}
*/
/*
void CEOSAIUnit2ActionIdea::CalculateActionRoleScores()
{
	// How do I handle this?
	//   Do I create a score for each Action-Role (Combat, Transport, AirfieldBuilder, CitResCapture)?
	//   Then I have the ActionDefinition find the UnitActionIdeas that fulfill each individual Action-Role,
	//     and allow one UnitActionIdea to fulfill multiple roles - e.g. Combat + CitResCapture?
	//   Should I create one score value, and have the ActionDefinition search the lists for UnitActionIdeas
	//     to fulfill each of the roles?
	//
	//   How would/should the AI handle a situation like this?
	//
	//   UnitActionIdea    CombatEfficiency   CitResCapture   Transport   TimeFunction(TimeToTarget)
	//         #1                8.0              false         false                2.0
	//         #2                7.0               true         false                2.7
	//         #3                6.0               true         false                1.2
	//         #4                2.0              false          true                4.2
	//         #5                2.1              false          true                5.2
	//
	//m_fActionRole_Combat_Value = m_fCombatEfficiency * m_pActionDefinition->GetTimeValueFunction( m_fSimpleTimeToTarget );
	//m_fActionRole_Combat_Value = m_fCombatEfficiency * m_pTaskForce->GetTimeValueFunction( m_fSimpleTimeToTarget );
	//m_fActionRole_Combat_Value = m_fCombatEfficiency * m_pTaskForce->GetTimeValueFunction( m_fMoveToTargetTime );
}
*/
// Task Values
//
/*
float CEOSAIUnit2ActionIdea::GetGroundCitResHunterCapacity01()
{
	return m_pUnitTemplate->GetGroundCitResHunterCapacity();
}

float CEOSAIUnit2ActionIdea::GetSeaResHunterCapacity01()
{
	return m_pUnitTemplate->GetSeaResourceHunterCapability();
}

float CEOSAIUnit2ActionIdea::GetAirfieldBuilderCapacity01()
{
	return m_pUnitTemplate->GetAirfieldBuilderCapacity();
}
*/


//
/*
void  CEOSAIUnit2ActionIdea::ClearMovementPathIdeas()
{
	ASSERT( false ); // don't know if I actually need this method
	delete m_pMovementPathIdeas;
	m_pMovementPathIdeas = NULL;
}

CMovementPathIdeas* CEOSAIUnit2ActionIdea::CreateMovementPathIdeas()
{
/-*
	ASSERT( m_pMovementPathIdeas == NULL );
	//
	CEOSAIBrain* pAIBrain = m_pAIDesire->GetAIBrain();
	CMultiRegion* pMultiRegion = m_pActor->GetFinalState()->GetMultiRegion();
	/-*
	//CMovementManager* pMovementManager = pAIBrain->GetMovementManager();
	CMovementManager MovementManager( pAIBrain, pAIBrain->GetAIPlayerNumber() );
	//MovementManager.Clear();
	//MovementManager.SetPlayer( pAIBrain->GetAIPlayerNumber() );
	CMultiRegion* pMultiRegion = m_pActor->GetFinalState()->GetMultiRegion();
	MovementManager.SetStartMultiRegion( pMultiRegion->m_iId );
	if( m_pTargetMultiRegion )
	{
		MovementManager.SetEndMultiRegion( m_pTargetMultiRegion->m_iId );
	}
	if( m_pTargetAIPoiObject )
	{
		MovementManager.SetEndPoiObject( m_pTargetAIPoiObject->GetPoiObject() );
	}
	m_pMovementPathIdeas = MovementManager.CreateMovementPathIdeas( m_eMovementType );
	*-/

	m_pMovementPathIdeas = CMovementPathIdeas::Create( pAIBrain, pAIBrain->GetAIPlayerNumber(), m_eMovementType );
	m_pMovementPathIdeas->SetStartMultiRegion( pMultiRegion->m_iId );
	if( m_pTargetMultiRegion ){ m_pMovementPathIdeas->SetEndMultiRegion( m_pTargetMultiRegion->m_iId ); }
	if( m_pTargetAIPoiObject ){ m_pMovementPathIdeas->SetEndPoiObject( m_pTargetAIPoiObject->GetPoiObject() ); }
	m_pMovementPathIdeas->CreateMovementPaths();
*-/
	/-*
	if( m_eMovementType == EOSAIEnumMovementType3_Land ||
		m_eMovementType == EOSAIEnumMovementType3_Water )
	{
		m_pMovementPathIdeas = new CMovementPathIdeas( pAIBrain, pAIBrain->GetAIPlayerNumber(), m_eMovementType );
		m_pMovementPathIdeas->SetStartMultiRegion( pMultiRegion->m_iId );
		if( m_pTargetMultiRegion ){ m_pMovementPathIdeas->SetEndMultiRegion( m_pTargetMultiRegion->m_iId ); }
		if( m_pTargetAIPoiObject ){ m_pMovementPathIdeas->SetEndPoiObject( m_pTargetAIPoiObject->GetPoiObject() ); }
		m_pMovementPathIdeas->CreateMovementPaths();
	}
	if( m_eMovementType == EOSAIEnumMovementType3_Air )
	{
		m_pMovementPathIdeas = new CMovementPathIdeasForAirUnit( pAIBrain, pAIBrain->GetAIPlayerNumber() );
		m_pMovementPathIdeas->SetStartMultiRegion( pMultiRegion->m_iId );
		if( m_pTargetMultiRegion ){ m_pMovementPathIdeas->SetEndMultiRegion( m_pTargetMultiRegion->m_iId ); }
		if( m_pTargetAIPoiObject ){ m_pMovementPathIdeas->SetEndPoiObject( m_pTargetAIPoiObject->GetPoiObject() ); }
		m_pMovementPathIdeas->CreateMovementPaths();
	}
	*-/
	//FindPotentialTransports(); // DEBUG

	return m_pMovementPathIdeas;
}

bool CEOSAIUnit2ActionIdea::PathwayExists()
{
	if( m_pMovementPathIdeas ){ return m_pMovementPathIdeas->PathwayExists(); }
	return false;
}

bool  CEOSAIUnit2ActionIdea::RequiresATransport()
{
	if( m_pMovementPathIdeas ){ m_pMovementPathIdeas->RequiresATransport(); }
	//if( m_eMovementType == EOSAIEnumMovementType3_Land ){ return !LandRouteExists(); }
	return false; 
}

float CEOSAIUnit2ActionIdea::GetTransportValueBasedOnDistance()
{
	return m_pMovementPathIdeas->GetTransportValueBasedOnDistance();
}

CEOSAIUnit2* CEOSAIUnit2ActionIdea::IfINeedATransportPickOne()
{
	ASSERT( m_pMovementPathIdeas );
	CMovementPath* pMovementPath = m_pAllocatedMovementPath;
	CMovementPathForGroundSeaUnit* pGroundSeaMovementPath = dynamic_cast< CMovementPathForGroundSeaUnit* >( m_pAllocatedMovementPath );
	if( pMovementPath == NULL || pGroundSeaMovementPath == NULL )
	{
		pMovementPath = m_pMovementPathIdeas->GetShortestPath();
		pGroundSeaMovementPath = dynamic_cast< CMovementPathForGroundSeaUnit* >( pMovementPath );
	}
	ASSERT( pMovementPath );
	//if( m_pAllocatedMovementPath == NULL ) return NULL;
	if( m_pMovementPathIdeas->GetTransportValueBasedOnDistance() > 0.2f )
	{
		CEOSAIUnit2* pAIBestUnit = NULL;
		float fBestTime = 1000000.0f;
		POSITION pos = pGroundSeaMovementPath->m_TransportsProximity.GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObjectProximity2* pProx = pGroundSeaMovementPath->m_TransportsProximity.GetNext( pos );
			CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pProx->m_pAIPoiObject );
			if( pAIUnit )
			{
				float fTime = GetTimeToTargetUsingTransport( pProx );
				if( fTime < fBestTime )
				{
					fBestTime = fTime;
					pAIBestUnit = pAIUnit;
				}
			}
		}
		return pAIBestUnit;
	}
	return NULL;
}
*/
/*
float CEOSAIUnit2ActionIdea::GetImaginaryTransportValue()
{
	if( m_eMovementType == EOSAIEnumMovementType3_Land )
	{
		if( LandRouteExists() )
		{
			if( AirRouteExists() )
			{
				return 1.0f - m_fImaginaryAirTransportDividedByGroundRoute;
			}
			if( SeaTransportRouteExists() )
			{
				return 1.0f - m_fImaginarySeaTransportDividedByGroundRoute;
			}
		}
		else // a SingleGeoRoute does not exist
		{
			if( AirRouteExists() || SeaTransportRouteExists() )
			{
				return 1.0f;
			}
		}
	}
	// Air and Sea units don't benefit from transports
	return 0.0f;
}
*/


//void CEOSAIUnit2ActionIdea::InvokePotentialTransports() // compile a list of transports for this unit
/*
void CEOSAIUnit2ActionIdea::GetPotentialTransports( CList< CEOSAIUnit2Proximity* >* pList ) // compile a list of transports for this unit
{
	// Get AIUnitProximity values for all units that can contain units
	//   The Proximity value is based on distance from the first water MultiRegion
	CList< CEOSAIUnit2Proximity* >* pTransportList = m_pMovementPathIdeas->GetTransportProximityToPickupList();

	POSITION pos = pTransportList->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2Proximity* pUnitProximity = pTransportList->GetNext( pos );
		//CUnitTemplate* pTransportUnit = pUnitProximity->m_pAIUnit->GetUnit();
		CUnit* pTransportUnit = pUnitProximity->m_pAIUnit->GetUnit();
		if( pTransportUnit->CanContain( m_pAIUnitActorTemplate->GetUnitTemplate() ) )
		{
			m_PotentialTransports.AddTail( pUnitProximity );
		}
	}
}
*/
/*
bool CEOSAIUnit2ActionIdea::TargetIsWithinMyAirRange()
{
	ASSERT( m_pMovementPathIdeas );
	//ASSERT( m_eMovementType == EOSAIEnumMovementType3_Air );
	return m_pMovementPathIdeas->PathwayExists();
	/-*
	float fRange = 0.0f;
	if( m_pAIUnitActor )
	{
		fRange = m_pAIUnitActor->GetMovementLimit();
	}
	if( m_pAIUnitActorTemplate )
	{
		fRange = m_pAIUnitActorTemplate->GetUnitTemplate()->GetMovementLimit();
	}
	return fRange > m_pMovementPathIdeas->GetDirectAirDistance();
	*-/
}
*/

/*
void CEOSAIUnit2ActionIdea::ReCalculateAndReScore()
{
	// Re-generate the pathways, re-score
	//   I may need to update the AIActionDefinition's relative scores
	//   I will need to update the AIObject's relative scores
	//
	//CreateMovementRoutes();
	ASSERT( false ); // don't know if I actually need this method
	//ClearMovementPathIdeas();
	//CreateMovementPathIdeas();
	//m_pActionDefinition->CalculateTTTScore( this );

//	RemoveFromLists();
//	AddToLists();

	// I need to resort a variety of lists
	/-*
		m_AllMyUnitActionValues.InsertSorted( pUnitActionValue );
		m_PrimaryUnits.InsertSorted( pUnitActionValue );
		pUnitActionValue->m_bIsAPrimaryUnit = true;

		// Insert this ActionValue into AIPoiObject
		pAIUnit->AddUnitActionValue( pUnitActionValue );

		// Insert this ActionValue into AIThoughtDatabase
		pAIThoughtDatabase->AddUnitActionValue( pUnitActionValue );
	*-/
}
*/
bool CEOSAIUnit2ActionIdea::ActorIsAllocated()
{
	return GetAIPoiObjectActor()->IsAllocated();
}
/*
void CEOSAIUnit2ActionIdea::AllocateActorToTacticalProject()
{
	GetAIPoiObjectActor()->AllocateToTacticalProject( this );
}
*/
/*
void CEOSAIUnit2ActionIdea::AllocateShallow( bool bAllocate )
{
	//m_pActor->UnitActionIdeaWasAllocated( this, bAllocate );
	m_bAllocated = bAllocate;
	//ASSERT( m_pMovementPathIdeas );
	//m_pAllocatedMovementPath = m_pMovementPathIdeas->GetShortestPath();
}

void CEOSAIUnit2ActionIdea::AllocateDeep( bool bAllocate )
{
	m_pActor->UnitActionIdeaWasAllocated( this, bAllocate );
	m_bAllocated = bAllocate;
	//ASSERT( m_pMovementPathIdeas );
	//m_pAllocatedMovementPath = m_pMovementPathIdeas->GetShortestPath();
}
*/
/*
void CEOSAIUnit2ActionIdea::RemoveFromLists()
{
	//m_pActionDefinition->RemoveUnitActionIdea( this );

	// Insert this ActionValue into AIPoiObject
	GetAIPoiObjectActor()->RemoveUnitActionIdea( this );

	// Insert this ActionValue into AIThoughtDatabase
	//m_pActionDefinition->GetAIBrain()->GetAIThoughtDatabase()->RemoveUnitActionValue( this );
}
*/
/*
void CEOSAIUnit2ActionIdea::AddToLists()
{
	/-*
	i need to update the UnitActionValues
		i need to update the AIObject's relative scores - but right now i don't use it for anything
		i think I need to sort them by the SimpleScoreAIObjectRelativeScore
		how do I calculate this?
	check the best ActionDefinition score
	check the best Actor score
	if the best score changes, i have to recalculate the relative score
	*-/

	//m_pActionDefinition->AddUnitActionIdea( this );

	// Insert this ActionValue into AIPoiObject
	GetAIPoiObjectActor()->AddUnitActionIdea( this );

	// Insert this ActionValue into AIThoughtDatabase
	//m_pActionDefinition->GetAIBrain()->GetAIThoughtDatabase()->AddUnitActionValue( this );
}
*/

// Compile available targets, and values
//void CEOSAIUnit2ActionIdea::CompileTargetDesires()
void CEOSAIUnit2ActionIdea::CompileDesireValues()
{
	ASSERT( m_pPredefinedStep == NULL );
	ASSERT( m_DesireValues.IsEmpty() );
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();

	// Iterate over the desires, attach a proximity value to each
	//   This will help me sort-out which units I'll send after which targets
	//CEOSAIUnit2* pAIUnit = GetAIUnitActor();
	//if( pAIUnit )
	CEOSAIUnitTemplate* pActorAIUnitTemplate = GetAIUnitTemplate();
	CEOSAIPoiObject* pAIPoiObjectActor = GetAIPoiObjectActor();

	// Compile a list of all the desires
	CList< CEOSAIDesireSpatial* >  Desires;
	/*
	if( m_pTacticalProject )
	{
		CAITacticalProjectStrategy* pAllocatedStrategy = m_pTacticalProject->GetAllocatedStrategy();
		pAllocatedStrategy->GetDesires( &Desires );
	}
	*/
	if( m_pTacticalProject2 )
	{
		CEOSAITacticalProject2Strategy* pAllocatedStrategy = m_pTacticalProject2->GetAllocatedStrategy();
		pAllocatedStrategy->GetDesires( &Desires );
	}

	m_fBestTimeToTargetDesire = 1000000.0f;

	// What's the target?
	CEOSAIDesireSpatial*  pTargetDesire = NULL;
	POSITION pos = Desires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = Desires.GetNext( pos );
		if( pDesire->GetJobsToDo()->CanFulfillAJob( pActorAIUnitTemplate ) &&
			pDesire->CurrentForeignRelationsPreventsPersuingThisDesire() == false )
		{
			// INCOMPLETE
			// Value is based on proximity and usefulness (e.g. Attrition)
			//float fDistance = pWorldBuildDesc->GetPixelDistance( pDesire->GetLocation(), pAIPoiObjectActor->GetInitialState()->GetLocation() );
			float fDistance = g_pWorldDistanceTool->GetDistance( pDesire->GetLocation(), pAIPoiObjectActor->GetInitialState()->GetLocation() );
			float fTimeToTarget = fDistance / ( pActorAIUnitTemplate->GetMovementRate()+1.0f );

			m_fBestTimeToTargetDesire = min( m_fBestTimeToTargetDesire, fTimeToTarget );

			CEOSAIUnit2ActionIdeaTargetValue* pValue = new CEOSAIUnit2ActionIdeaTargetValue( this, pDesire );
			pValue->m_fTimeToTarget = fTimeToTarget;
			pValue->CalculateValue();
			m_DesireValues.AddTail( pValue );
		}
	}
}

long CEOSAIUnit2ActionIdea::GetNumberOfAvailableDesireTargets()
{
	long iCount = 0;
	POSITION pos = m_DesireValues.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2ActionIdeaTargetValue* pValue = m_DesireValues.GetNext( pos );
		CEOSAIDesireSpatial* pDesire = pValue->m_pDesire;
		float fDesireFulfillment01 = pDesire->GetAllocatedFulfillment();
		if( fDesireFulfillment01 < 1.0f ) iCount++;
	}
	return iCount;
}

void CEOSAIUnit2ActionIdea::CreatePredefinedStep()
{
	ASSERT( m_pPredefinedStep == NULL );
	ASSERT( GetNumberOfDesireAllocations() > 0 );
	CEOSAIUnit2* pAIUnit = GetAIUnitActor();
	if( pAIUnit )
	{
		CEOSAIUnit2PathwayFinder* pPathwayFinder = GetAIUnitActor()->GetAIUnitPathwayFinder();
		m_pPredefinedStep = new CEOSAIUnit2PathwayPredefinedStep( pPathwayFinder );
		m_pPredefinedStep->SetAIUnitActionIdea( this );

		if( this->GetTacticalProject2() &&
			this->GetTacticalProject2()->ProjectIncludesObjectIdAsPrimaryTask( 89 ) )
		{
			int h=0;
		}
		if( pAIUnit->GetObjectId() == 121 )
		{
			int h=0;
		}
		/*
		// Compile a list of all the desires
		CList< CEOSAIDesireSpatial* >  Desires;
		CAITacticalProjectStrategy* pAllocatedStrategy = m_pTacticalProject->GetAllocatedStrategy();
		pAllocatedStrategy->GetDesires( &Desires );

		// What's the target?
		CEOSAIDesireSpatial*  pTargetDesire = NULL;
		POSITION pos = Desires.GetHeadPosition();
		while( pos )
		{
			CEOSAIDesireSpatial* pDesire = Desires.GetNext( pos );
			if( pDesire->GetJobsToDo()->CanFulfillAJob( pAIUnit->GetUnitTemplate() ) &&
				pDesire->CurrentForeignRelationsPreventsPersuingThisDesire() == false )
			{
				pTargetDesire = pDesire;
				break;
			}
		}
		*/
		CEOSAIDesireSpatial* pTargetDesire = m_DesireAllocations.GetHead();

		//GetTacticalProject()->GetClosestPrimaryGoal( GetAIPoiObjectActor()->GetInitialState()->GetLocation(), &pDesire );
		if( pTargetDesire )
		{
			if( pTargetDesire->GetType() == EnumCaptureCity || 
				pTargetDesire->GetType() == EnumCaptureResource ||
				pTargetDesire->GetType() == EnumCaptureAirfield )
			{
				CEOSAIPoiObject* pTargetAIPoiObject = pTargetDesire->GetAIPoiObjectTarget();
				//CEOSAIPoiObject* pTargetAIPoiObject = m_pAIUnitActor->GetAIBrain()->GetAIPoiObject( pTargetPoiObject->GetObjectId() );

				m_pPredefinedStep->CaptureCitRes( pTargetAIPoiObject );
				pPathwayFinder->AppendStep( m_pPredefinedStep );
			}
			else if( pTargetDesire->GetType() == EnumDegradeCity )
			{
				CEOSAIPoiObject* pTargetAIPoiObject = pTargetDesire->GetAIPoiObjectTarget();
				//CEOSAIPoiObject* pTargetAIPoiObject = m_pAIUnitActor->GetAIBrain()->GetAIPoiObject( pTargetPoiObject->GetObjectId() );

				m_pPredefinedStep->DegradeTarget( pTargetAIPoiObject );
				pPathwayFinder->AppendStep( m_pPredefinedStep );
			}
			else if( pTargetDesire->GetType() == EnumDestroyUnit )
			{
				CEOSAIPoiObject* pTargetAIPoiObject = pTargetDesire->GetAIPoiObjectTarget();
				//CEOSAIPoiObject* pTargetAIPoiObject = m_pAIUnitActor->GetAIBrain()->GetAIPoiObject( pTargetPoiObject->GetObjectId() );

				CEOSAIUnit2* pTargetAIUnit = dynamic_cast< CEOSAIUnit2* >( pTargetAIPoiObject );
				ASSERT( pTargetAIUnit );
				m_pPredefinedStep->AttackUnit( pTargetAIUnit );
				pPathwayFinder->AppendStep( m_pPredefinedStep );
			}
			else{ ASSERT( false ); }

			bool bRequiresTransport = m_pPredefinedStep->RequiresTransport();
			if( m_pPredefinedStep && 
				m_pPredefinedStep->RequiresTransport() &&
				m_pAITransportUnitActionIdea )
			{
				CEOSAIUnit2* pAITransport = m_pAITransportUnitActionIdea->GetAIUnitActor();
				if( pAITransport )
				{
					m_pPredefinedStep->UseTransportAndCreatePickupDropoffSteps( pAITransport );
				}
			}
		}
	}
}
/*
CEOSAIUnit2Action* CEOSAIUnit2ActionIdea::CreateAction()
{
	ASSERT( GetAIUnitAction() == NULL );

	//
	// OLD METHOD ----------
	//
	CEOSAIUnit2Action* pAIUnitAction = new CEOSAIUnit2Action( this );
	SetAIUnitAction( pAIUnitAction );
	m_pAIUnitActor->GetAIObjectActionProgression()->AppendAIAction( pAIUnitAction );

		// Options:
		//   Place the actions in the middle of the sequence (look at distances, timing)
		//   Place the actions at the end of the sequence
		// If using a transport, what factors influence the placement of the pickup/dropoff tasks?
		//   Maximum transport capacity
		//   Distance
		//   Timing (transport, transportee both arriving at the pickup at the same time)
		//   Willingness/unwillingness to delay another important action (e.g. reinforcements)

		// The scheduling of an action causes the Unit's UnitActionDefinitions to change:
		//   The unit might be passing closer to another AIActionDefinition target (increased score),
		//   The unit is now tied up doing something, so its value for other AIActions decreases.
		//   The odds that this unit will survive this task lowers its value for other AIActions.
		// I need to recalculate my AIActionUnitValues, along with those of any assisting units
		//   (i.e. Transports), and any affected units (e.g. if I get picked up by a transport
		//   and cause that transport to be delayed, I also delay all the other units on that
		//   transport, which lowers their scores).

	return pAIUnitAction;
}
*/
/*
float CEOSAIUnit2ActionIdea::GetCombatValueAgainstTarget( CList< CEOSAIPoiObject* >* pTargetList )
{
	if( m_pAIUnitActor )
	{
		CCombatManager CombatManager;
		CombatManager.AddAttacker( m_pAIUnitActor->GetPoiObject() );
		POSITION pos = pTargetList->GetHeadPosition();
		while( pos )
		{
			CEOSAIPoiObject* pAIPoiObject = pTargetList->GetNext( pos );
			CombatManager.AddDefender( pAIPoiObject->GetPoiObject() );
		}
		float fCombatBalance;
		float fAttrition;
		bool bValid = CombatManager.CalculateCombatValues( &fCombatBalance, &fAttrition );
		if( bValid )
		{
			return fAttrition;
		}
		return 0.0f;
	}
	ASSERT( false );
	return 0.0f;
}
*/
/*
float CEOSAIUnit2ActionIdea::GetCombatEfficiency()
{
	return m_fCombatValue / m_pUnitTemplate->GetProductionCost();
}
*/
