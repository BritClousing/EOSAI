
#pragma once

//#include "stdafx.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAILocation.h"
#include "EOSAIIntSet.h"
//class CPoiObject;
class CEOSAIPoiObject;
class CAIAirPathPoint;
class CEOSAIUnit;

class CEOSAIUnitPathwayPredefinedStep;
class CAirPathfinderSubPoint;
class CAirPathfinderSubPointsList
{
	public:
		~CAirPathfinderSubPointsList();

		CList< CAirPathfinderSubPoint* >  m_List;
};

// A MainPoint is created for each Predefined Point and Airbase
//   The MainPoint contains many Subpoints
//     For each PredefinedStep, there is an array of Subpoints.
//     Normally, there would be just one Subpoint for each Predefined step, but there can be
//     multiple because of differential "Pathfinding Value", "Distance Since Last Landing" values
//     (In other words, I would normally keep just the top PathfindingValue and delete all other ones,
//      but the best pathfinding value might not be able to make it back to base.)
class CEOSAIAirPathfinderMapPoint
{
	public:
		CEOSAIAirPathfinderMapPoint()
		{
			m_pAIPoiObject = NULL;
			//m_pPoiObject = NULL;
			m_pPredefinedStep = NULL;

			m_bIsStartPoint = false;
			m_bIsAnAirbase = false;
			m_bIsAnAttack = false;
			m_bIsAWaypoint = false;
			//
			m_InitialEvaluation_bIsReachableAirbase = false;
			m_InitialEvaluation_fArrivalTime = 0.0f;
		}
		~CEOSAIAirPathfinderMapPoint();

		void  SetNumberOfPredefinedSteps( long i );
		CAirPathfinderSubPoint*  InvokeSubPoint( long iStepNumber );

		//
		CEOSAIPoiObject*     m_pAIPoiObject;
		//CPoiObject*       m_pPoiObject;
		CEOSAIUnitPathwayPredefinedStep*  m_pPredefinedStep;
		CEOSAILocation         m_Location;

		bool  CanContainMultipleSubPoints(){ return !m_bIsAnAirbase; }
		bool  m_bIsStartPoint;
		bool  m_bIsAnAirbase;
		bool  m_bIsAnAttack;
		bool  m_bIsAWaypoint;
		CEOSAIIntSet  m_EndPointSet; // Example: end point 0,3,4

		bool  m_InitialEvaluation_bIsReachableAirbase;
		float m_InitialEvaluation_fArrivalTime;

		// Consider adding a new SubPoint value to iStepNumber
		//   (Each iStepNumber can contain several SubPoints)
		CAirPathfinderSubPoint*  ConsiderNewSubPointObject( 
			CList< CAirPathfinderSubPoint* >* pSubPointsList, 
			long iStepNumber, float fPathfindingValue, float fDistanceSinceLastAirbase );

		CEOSAIBCDumbArray1D< CAirPathfinderSubPointsList* >  m_SubPointsArray;
};

class CAirPathfinderSubPoint
{
	public:
		CAirPathfinderSubPoint()
		{
			m_pPredefinedStep = NULL;
			m_pMapPoint = NULL;
			//
			m_iStepNumber = 0;
			m_bTemp_IsInsideCurrentSubPointsList = false;
			m_fDistanceSinceLastAirbase = 0.0f;

			m_fPathfindingValue = 0.0f;
			m_fTotalDistance = 0.0f;
			m_fTotalTime = 0.0f;
			m_fTotalDanger = 0.0f;
			m_fTotalPresence = 0.0f;
			m_fTotalSighting = 0.0f;
			m_pNextStep = NULL;
			m_pPrevStep = NULL;
		}

		CEOSAIUnitPathwayPredefinedStep*  m_pPredefinedStep;
		CEOSAIAirPathfinderMapPoint*  m_pMapPoint;

		long  m_iStepNumber; // example: 0

		bool  m_bTemp_IsInsideCurrentSubPointsList;
		float m_fDistanceSinceLastAirbase;

		float m_fPathfindingValue; // Weighted by Distance, Danger, etc
		float m_fTotalDistance;
		float m_fTotalTime;
		float m_fTotalDanger;
		float m_fTotalPresence;
		float m_fTotalSighting;

		CAirPathfinderSubPoint* m_pNextStep;
		CAirPathfinderSubPoint* m_pPrevStep;
};


class CAIAirResistance
{
	public:
		CAIAirResistance( CEOSAIUnit* pAIUnit, float fTimeSpan )
		{
			m_pAIUnit = pAIUnit;
			m_fTimeWithinCombatRange = fTimeSpan;
		}
		CEOSAIUnit*  m_pAIUnit;
		float     m_fTimeWithinCombatRange; // amount of time the air unit is within unit's attack range
};

class CAIAirResistanceSet
{
	public:
		CAIAirResistanceSet(){}
		~CAIAirResistanceSet(){ Clear(); }

		void  Clear(){ while( m_Resistance.IsEmpty() == FALSE ){ delete m_Resistance.RemoveHead(); } }
		void  Add( CEOSAIUnit* pAIUnit, float fTimeSpan )
		{
			POSITION pos = m_Resistance.GetHeadPosition();
			while( pos )
			{
				CAIAirResistance* pResistance = m_Resistance.GetNext( pos );
				if( pResistance->m_pAIUnit == pAIUnit )
				{
					pResistance->m_fTimeWithinCombatRange += fTimeSpan;
					return;
				}
			}
			m_Resistance.AddTail( new CAIAirResistance( pAIUnit, fTimeSpan ) );
		}

		CList< CAIAirResistance* >  m_Resistance;
};

// Final result
/*
class CAirPathfinderResult
{
	public:
		CAirPathfinderResult(){ Clear(); }
		~CAirPathfinderResult(){ Clear(); }
		void Clear()
		{
			m_Steps.RemoveAll();
			m_Resistance.Clear();
			m_fSurvivalOdds = 1.0f;
			m_fPathfindingValue = 1000000.0f;
			m_bResultsInDeath = false;
			m_fAttackTime = 0.0f;
			m_fCompletionTime = 0.0f;
			m_fDanger = 0.0f;
		}

		// Contains a series of waypoints (including the Predefined Steps and landings in-between)
		CList< CAirPathfinderSubPoint* >  m_Steps; // not owned

		CAIAirResistanceSet  m_Resistance;
		float m_fSurvivalOdds;

		float m_fPathfindingValue;
		bool  m_bResultsInDeath; // Self-Destruct or ran-out-of-fuel
		float m_fAttackTime;
		float m_fCompletionTime;
		float m_fDanger; // need to define this better (percent chance of survival? chance of survival before attack?)
};
*/
