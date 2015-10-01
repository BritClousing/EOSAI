
#pragma once

#include "EOSAIEnumGeoPlan.h"
#include "EOSAIEnumGeoSituation.h"
//#include "EOSAIPlayer2.h"
namespace EOSAI
{
	class AIPlayer;
};

// The CEOSAIGeoView stores per-player information about the AIGeos

class CEOSAIGeoView
{
	public:
		CEOSAIGeoView( EOSAI::AIPlayer* pAIPlayer, CEOSAIGeo* pEOSAIGeo );//long iAIPlayer );
		void CalculateAIGeoViewValues();

		//
		// Poi Object count
		//
			long  GetUnownedPoiObjects(){ return m_iUnownedPoiObjects; }
			long  GetSATPoiObjects(){ return m_iMyPoiObjects + m_iTeamPoiObjects + m_iAlliesPoiObjects; }
			long  GetMyPoiObjects(){ return m_iMyPoiObjects; }
			long  GetATPoiObjects(){ return m_iAlliesPoiObjects + m_iTeamPoiObjects; }
			long  GetTeamPoiObjects(){ return m_iTeamPoiObjects; }
			long  GetAlliesPoiObjects(){ return m_iAlliesPoiObjects; }
			long  GetNeutralPoiObjects(){ return m_iNeutralPoiObjects; }
			long  GetEnemyPoiObjects(){ return m_iEnemyPoiObjects; }
			long  GetNeutralOrEnemyPoiObjects(){ return m_iNeutralPoiObjects + m_iEnemyPoiObjects; }


		void  SetMyGroundSeaUnitAccess01( float fAccess ){ m_fMyGroundSeaUnitAccess01 = fAccess; }
		float GetMyGroundSeaUnitAccess01(){ return m_fMyGroundSeaUnitAccess01; }
		void  SetMyAirUnitAccess01( float fAccess ){ m_fMyAirUnitAccess01 = fAccess; }
		float GetMyAirUnitAccess01(){ return m_fMyAirUnitAccess01; }
		//
		float GetMyCitResDistance(){ return m_fMyCitResDistance; }
		float GetMyCitResUnitDistance(){ return m_fMyCitResUnitDistance; }
		float GetNeutralEnemyCitResDistance(){ return m_fNeutralEnemyCitResDistance; }
		float GetNeutralEnemyCitResUnitDistance(){ return m_fNeutralEnemyCitResUnitDistance; }
		float GetHumanNeutralEnemyCitResDistance(){ return m_fHumanNeutralEnemyCitResDistance; }
		float GetHumanNeutralEnemyCitResUnitDistance(){ return m_fHumanNeutralEnemyCitResUnitDistance; }

		// DistanceMult gives higher values to closer geos, and higher values to geos closer to neutral/enemies
		float GetDistanceMult(); // Typically varies from 1.5 to 0.0, based on AI CitRes Distance and other player CitRes Distance
		//float GetCitResValue();
		float GetSubjectiveCitResValue();
		void  CalculateGeoDistanceCitResScore();//{ m_fGeoDistanceCitResScore = GetDistanceMult() * sqrt( GetCitResValue() ); }
		float GetGeoDistanceCitResScore(){ return m_fGeoDistanceCitResScore; }

		void  SetGeoDistanceCitResScoreRank( long iRank ){ m_iGeoDistanceCitResScoreRank = iRank; }
		long  GetGeoDistanceCitResScoreRank(){ return m_iGeoDistanceCitResScoreRank; }

		long  GetNumberOfUnownedAndEnemyPoiObjects(); // Number of tasks

		//
		void                   SetGeoSituation( EOSAIEnumGeoSituation e ){ m_eGeoSituation = e; }
		EOSAIEnumGeoSituation  GetGeoSituation(){ return m_eGeoSituation; }
		//
		void              SetPlan( EOSAIEnumGeoPlan e ){ m_eGeoPlan = e; }
		EOSAIEnumGeoPlan  GetPlan(){ return m_eGeoPlan; }

		//
		// The AIPlayer who uses this data
		//
			EOSAI::AIPlayer* m_pAIPlayer;
			CEOSAIGeo*         m_pAIGeo;

		// Per player data
		//
			EOSAIEnumGeoSituation  m_eGeoSituation;//[EOSAI_MAX_NUMBER_OF_PLAYERS];
			EOSAIEnumGeoPlan       m_eGeoPlan;//[EOSAI_MAX_NUMBER_OF_PLAYERS];

		//
		// Poi Object count
		//
			long  m_iUnownedPoiObjects;
			long  m_iMyPoiObjects;
			long  m_iTeamPoiObjects;
			long  m_iAlliesPoiObjects;
			long  m_iNeutralPoiObjects;
			long  m_iEnemyPoiObjects;
		//
		// Total
		//
			// Based on my CitRes Distance, other player's CitRes Distance, and the CitRes values
			float m_fGeoDistanceCitResScore;
			long  m_iGeoDistanceCitResScoreRank; // 1 = is the top scoring AIGeo

		//
		// My Access
		//
			float m_fMyGroundSeaUnitAccess01;
			float m_fMyAirUnitAccess01;

			float m_fMyCitResDistance;
			float m_fMyCitResUnitDistance;
			float m_fNeutralEnemyCitResDistance;
			float m_fNeutralEnemyCitResUnitDistance;
			float m_fHumanNeutralEnemyCitResDistance;
			float m_fHumanNeutralEnemyCitResUnitDistance;
		//
			float m_fDangerOfEnemyInvasion01; // Not used yet

			// Power needed. Invade=Combat, Expand=CaptureUnowned, ControlArea=Destroy/Block Enemies
			float m_fDesire_InvadeExpandAndControlArea_PowerNeeded; 
			float m_fDesire_DegradeArea_PowerNeeded; // Bomb area.  Based on the enemy units, cities, in the area
			float m_fDesire_ProtectArea_PowerNeeded; // Units here for defense. Based on nearby enemy units, pathways
			float m_fArrivalTimePriority01; // Used when a geo is in dispute or near an enemy, need to get units there immediately
};
