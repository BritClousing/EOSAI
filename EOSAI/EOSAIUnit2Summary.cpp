
#include "stdafx.h"
#include "EOSAIUnit2Summary.h"
#include "AIPlayer.h"
//#include "WorldDescPlayer.h"
#include "City.h"
#include "EOSAIUnit2.h"
//#include "AINationalDesires.h"

//#include "BuildOptionEvaluation.h"
//#include "AIDesireToExplore.h"
//#include "AIDesireToExpand.h"
//#include "AIDesireToConquer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void  CEOSAIUnit2Summary::Clear()
{
	m_fProductionValue = 0.0f;
	m_fCombatSignificance = 0.0f;

	m_fExploration = 0.0f;
	m_fSighting = 0.0f;

	m_fStrategicBombing = 0.0f;
	//m_fTacticalBombing = 0.0f;
	m_fAirEscort = 0.0f;
	m_fAntiAir = 0.0f;

	m_fSubmarineCapability = 0.0f;
	m_fAntiSubmarine = 0.0f;

	m_fLandCitResHunter = 0.0f;
	m_fBombardment = 0.0f;
	m_fGroundAttackUnits = 0.0f;
	m_fGroundDefenseUnits = 0.0f;
	m_fInvasionDefense = 0.0f;

	m_fSeaResourceHunter = 0.0f;
	m_fSeaSupremacy = 0.0f;
	m_fShoreBombardment = 0.0f;

	m_fGroundTransportCapability = 0.0f;
	m_fTrade = 0.0f;

	// Combat
	m_UnitHPDefense_UnitTypeSet.Clear();
	//m_fUnitStrengthSoft = 0.0f;
	//m_fUnitStrengthArmor = 0.0f;
	//m_fUnitStrengthAircraft = 0.0f;
	//m_fUnitStrengthMissile = 0.0f;
	//m_fUnitStrengthShip = 0.0f;
	//m_fUnitStrengthSubmarine = 0.0f;

	m_CombatPowerVsUnitTypes.Clear();
	m_fCombatVsCity = 0.0f;
	//m_fCombatVsSoft = 0.0f;
	//m_fCombatVsArmor = 0.0f;
	//m_fCombatVsAircraft = 0.0f;
	//m_fCombatVsMissile = 0.0f;
	//m_fCombatVsShip = 0.0f;
	//m_fCombatVsSubmarine = 0.0f;
}

/*
void  CEOSAIUnit2Summary::Calculate( CAIPlayer* pAIPlayer )  // Calculate abilities of single unit
{
	ASSERT( pAIPlayer );
	Clear();

	CEOSAIUnit2Summary  UnitCapability;

	// Look through my military (and look through the units I'm making)
	CWorldDescPlayer* pWorldDescPlayer = pAIPlayer->GetWorldDesc();
	POSITION pos = pWorldDescPlayer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescPlayer->GetPoiList()->GetNext( pos )->GetPtr();
		ASSERT( pPoi );
		if( pPoi )
		{
			CUnit* pUnit = dynamic_cast< CUnit* >( pPoi );
			if( pUnit )
			{
				if( pUnit->IsMine() )
				{
					// INCOMPLETE: this does not take into account the current health of the unit
					//   That may not be a big deal.
					UnitCapability.Calculate( pUnit->GetUnitTemplate() );
					*this += UnitCapability;
				}
			}
		}
	}
}

void  CEOSAIUnit2Summary::CalculateInProduction( CAIPlayer* pAIPlayer )  // Calculate capabilities of units in production
{
	// Calculate-in the in-production information
	ASSERT( pAIPlayer );
	Clear();

	CEOSAIUnit2Summary  UnitCapability;

	// Look through my military (and look through the units I'm making)
	CWorldDescPlayer* pWorldDescPlayer = pAIPlayer->GetWorldDesc();
	POSITION pos = pWorldDescPlayer->GetPoiList()->GetHeadPosition();
	while( pos )
	{
		CPoi* pPoi = pWorldDescPlayer->GetPoiList()->GetNext( pos )->GetPtr();
		ASSERT( pPoi );
		if( pPoi )
		{
			CCity* pCity = dynamic_cast< CCity* >( pPoi );
			if( pCity )
			{
				if( pCity->IsMine() )
				{
					POSITION pos = pCity->GetProductionQueue()->GetHeadPosition();
					while( pos )
					{
						CBuildOption* pBuildOption = pCity->GetProductionQueue()->GetNext( pos )->GetBuildOption();
						if( pBuildOption->IsAUnit() )
						{
							CUnitTemplate* pTemplate = pBuildOption->GetUnitTemplate();
							UnitCapability.CalculateValue( pTemplate, pAIPlayer->CalculateActualCost( pTemplate ) );
							*this += UnitCapability;
						}
					}
				}
			}
		}
	}
}
*/

void  CEOSAIUnit2Summary::AddToSummary( CEOSAIPoiObject* pAIPoiObject )  // Calculate abilities of single unit
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
	if( pAIUnit )
	{
		m_AIPoiObjects.AddTail( pAIPoiObject );

		CEOSAIUnitTemplate* pAIUnitTemplate = pAIUnit->GetAIUnitTemplate();
		AddToSummaryValue( pAIUnitTemplate );

		m_fProductionValue += pAIUnitTemplate->GetProductionCost();
		
		CEOSAIUnit2CombatCapability* pUnit2CombatCapability = pAIUnitTemplate->GetAIUnitCombatCapability();
		m_fCombatSignificance += pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
		int g=0;
	}
}
/*
void  CEOSAIUnit2Summary::AddToSummary( CUnitTemplate* pUnitTemplate )  // Calculate abilities of single unit
{
	m_UnitTemplates.AddTail( pUnitTemplate );
	AddToSummaryValue( pUnitTemplate );

	m_fProductionValue += pUnitTemplate->GetProductionCost();
	m_fCombatSignificance += pUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	int g=0;
}
*/
void  CEOSAIUnit2Summary::AddToSummary( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	m_AIUnitTemplates.AddTail( pAIUnitTemplate );
	AddToSummaryValue( pAIUnitTemplate );

	m_fProductionValue += pAIUnitTemplate->GetProductionCost();
	m_fCombatSignificance += pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
	int g=0;
}


void  CEOSAIUnit2Summary::RemoveFromSummary( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
	ASSERT( pAIUnit );
	if( pAIUnit )
	{
		POSITION pos = m_AIPoiObjects.GetHeadPosition();
		while( pos )
		{
			POSITION prevPos = pos;
			CEOSAIPoiObject* pAIPoiObjectInList = m_AIPoiObjects.GetNext( pos );
			if( pAIPoiObjectInList == pAIPoiObject )
			{
				m_fProductionValue -= pAIUnit->GetProductionCost();
				m_fCombatSignificance -= pAIUnit->GetAIUnitTemplate()->GetAIUnitCombatCapability()->GetCombatSignificance();
				m_AIPoiObjects.RemoveAt( prevPos );
				return;
			}
		}
	}
}
/*
void  CEOSAIUnit2Summary::RemoveFromSummary( CUnitTemplate* pUnitTemplate )
{
	POSITION pos = m_UnitTemplates.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CUnitTemplate* pUnitTemplateInList = m_UnitTemplates.GetNext( pos );
		if( pUnitTemplateInList == pUnitTemplate )
		{
			m_fProductionValue -= pUnitTemplate->GetProductionCost();
			m_fCombatSignificance -= pUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
			m_UnitTemplates.RemoveAt( prevPos );
			return;
		}
	}
}
*/
void  CEOSAIUnit2Summary::RemoveFromSummary( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	POSITION pos = m_AIUnitTemplates.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIUnitTemplate* pAIUnitTemplateInList = m_AIUnitTemplates.GetNext( pos );
		if( pAIUnitTemplateInList == pAIUnitTemplate )
		{
			m_fProductionValue -= pAIUnitTemplate->GetProductionCost();
			m_fCombatSignificance -= pAIUnitTemplate->GetAIUnitCombatCapability()->GetCombatSignificance();
			m_AIUnitTemplates.RemoveAt( prevPos );
			return;
		}
	}
}

// iBattleResult 0 = Stalemate, 1 = I Win, 2 = Other Wins
/*
void  CEOSAIUnit2Summary::CalculateCombat( CEOSAIUnit2Summary* pOtherSummary,
		long* iBattleResult, float* pfProductionDamageToMe, float* pfProductionDamageToOther,
		float* pfTimeToEndOfBattle )
{
	//
}
*/
void  CEOSAIUnit2Summary::Set( CEOSAIUnitTemplate* pTemplate )  // Calculate abilities of single unit
{
	//CUnitTemplate* pUnitTemplate = pAIUnitTemplate->GetUnitTemplate();
	//
	m_fExploration = pTemplate->GetExplorationCapability();
	m_fSighting = pTemplate->GetSightingCapability();

	m_fStrategicBombing = pTemplate->GetStrategicBombingCapability();
	//m_fTacticalBombing = pTemplate->GetTacticalBombingCapability();
	m_fAirEscort = pTemplate->GetAirEscortCapability();
	m_fAntiAir = pTemplate->GetAntiAirCapability();

	m_fSubmarineCapability = pTemplate->GetSubmarineCapability();
	m_fAntiSubmarine = pTemplate->GetAntiSubmarineCapability();

	m_fLandCitResHunter = pTemplate->GetLandCitResHunterCapability();
	m_fBombardment = pTemplate->GetBombardmentCapability();
	m_fGroundAttackUnits = pTemplate->GetOffensiveGroundCapability();
	m_fGroundDefenseUnits = pTemplate->GetDefensiveGroundCapability();
	m_fInvasionDefense = pTemplate->GetInvasionDefense();

	m_fSeaResourceHunter = pTemplate->GetSeaResourceHunterCapability();
	m_fSeaSupremacy = pTemplate->GetSeaSupremacyCapability();
	m_fShoreBombardment = pTemplate->GetShoreBombardmentCapability();

	m_fGroundTransportCapability = pTemplate->GetGroundTransportCapability();
//	m_fTrade = pTemplate->GetCargoCapacity();

	// Combat
	m_UnitHPDefense_UnitTypeSet.Clear();
	m_UnitHPDefense_UnitTypeSet.Set( pTemplate->GetCombatUnitTypeString(), pTemplate->GetHPDefenseValue() );
	//m_fUnitStrengthSoft = pTemplate->GetUnitStrengthSoft();
	//m_fUnitStrengthArmor = pTemplate->GetUnitStrengthArmor();
	//m_fUnitStrengthAircraft = pTemplate->GetUnitStrengthAircraft();
	//m_fUnitStrengthMissile = pTemplate->GetUnitStrengthMissile();
	//m_fUnitStrengthShip = pTemplate->GetUnitStrengthShip();
	//m_fUnitStrengthSubmarine = pTemplate->GetUnitStrengthSubmarine();

	//m_CombatVs.Clear();
	//m_CombatVs.Set( pTemplate->GetCombatVs() );
	m_CombatPowerVsUnitTypes = pTemplate->GetCombatPowerVsUnitTypes();
	m_fCombatVsCity = pTemplate->GetAICombatVsCity();
	//m_fCombatVsSoft = pTemplate->GetAICombatVsSoft();
	//m_fCombatVsArmor = pTemplate->GetAICombatVsArmor();
	//m_fCombatVsAircraft = pTemplate->GetAICombatVsAircraft();
	//m_fCombatVsMissile = pTemplate->GetAICombatVsMissile();
	//m_fCombatVsShip = pTemplate->GetAICombatVsShip();
	//m_fCombatVsSubmarine = pTemplate->GetAICombatVsSubmarine();
}

void  CEOSAIUnit2Summary::SetAndDivideByCost( CEOSAIUnitTemplate* pTemplate, float fCost )
{
	this->Set( pTemplate );
	this->Multiply( 1.0f / fCost );
}


void  CEOSAIUnit2Summary::operator=( CEOSAIUnit2Summary& Other )
{
	m_fExploration        = Other.m_fExploration;
	m_fSighting           = Other.m_fSighting;

	m_fStrategicBombing   = Other.m_fStrategicBombing;
	//m_fTacticalBombing    = Other.m_fTacticalBombing;
	m_fAirEscort          = Other.m_fAirEscort;
	m_fAntiAir            = Other.m_fAntiAir;

	m_fSubmarineCapability= Other.m_fSubmarineCapability;
	m_fAntiSubmarine      = Other.m_fAntiSubmarine;

	m_fLandCitResHunter   = Other.m_fLandCitResHunter;
	m_fBombardment        = Other.m_fBombardment;
	m_fGroundAttackUnits  = Other.m_fGroundAttackUnits;
	m_fGroundDefenseUnits = Other.m_fGroundDefenseUnits;
	m_fInvasionDefense    = Other.m_fInvasionDefense;

	m_fSeaResourceHunter  = Other.m_fSeaResourceHunter;
	m_fSeaSupremacy       = Other.m_fSeaSupremacy;
	m_fShoreBombardment   = Other.m_fShoreBombardment;

	m_fGroundTransportCapability = Other.m_fGroundTransportCapability;
	m_fTrade                     = Other.m_fTrade;

	// UnitStrength is Defense * Current HP
	m_UnitHPDefense_UnitTypeSet = Other.m_UnitHPDefense_UnitTypeSet;
	//m_fUnitStrengthSoft      = Other.m_fUnitStrengthSoft;
	//m_fUnitStrengthArmor     = Other.m_fUnitStrengthArmor;
	//m_fUnitStrengthAircraft  = Other.m_fUnitStrengthAircraft;
	//m_fUnitStrengthMissile   = Other.m_fUnitStrengthMissile;
	//m_fUnitStrengthShip      = Other.m_fUnitStrengthShip;
	//m_fUnitStrengthSubmarine = Other.m_fUnitStrengthSubmarine;

	// CombatVs is the AttackVs value (to-hit*damage) * Current HP
	m_CombatPowerVsUnitTypes = Other.m_CombatPowerVsUnitTypes;
	m_fCombatVsCity      = Other.m_fCombatVsCity;
	//m_fCombatVsSoft      = Other.m_fCombatVsSoft;
	//m_fCombatVsArmor     = Other.m_fCombatVsArmor;
	//m_fCombatVsAircraft  = Other.m_fCombatVsAircraft;
	//m_fCombatVsMissile   = Other.m_fCombatVsMissile;
	//m_fCombatVsShip      = Other.m_fCombatVsShip;
	//m_fCombatVsSubmarine = Other.m_fCombatVsSubmarine;
}

void  CEOSAIUnit2Summary::operator+=( CEOSAIUnit2Summary& Other )
{
	m_fExploration        += Other.m_fExploration;
	m_fSighting           += Other.m_fSighting;

	m_fStrategicBombing   += Other.m_fStrategicBombing;
	//m_fTacticalBombing    += Other.m_fTacticalBombing;
	m_fAirEscort          += Other.m_fAirEscort;
	m_fAntiAir            += Other.m_fAntiAir;

	m_fSubmarineCapability+= Other.m_fSubmarineCapability;
	m_fAntiSubmarine      += Other.m_fAntiSubmarine;

	m_fLandCitResHunter   += Other.m_fLandCitResHunter;
	m_fBombardment        += Other.m_fBombardment;
	m_fGroundAttackUnits  += Other.m_fGroundAttackUnits;
	m_fGroundDefenseUnits += Other.m_fGroundDefenseUnits;
	m_fInvasionDefense    += Other.m_fInvasionDefense;

	m_fSeaResourceHunter  += Other.m_fSeaResourceHunter;
	m_fSeaSupremacy       += Other.m_fSeaSupremacy;
	m_fShoreBombardment   += Other.m_fShoreBombardment;

	m_fGroundTransportCapability += Other.m_fGroundTransportCapability;
	m_fTrade                     += Other.m_fTrade;

	// UnitStrength is Defense * Current HP
	m_UnitHPDefense_UnitTypeSet += Other.m_UnitHPDefense_UnitTypeSet;
	//m_fUnitStrengthSoft      += Other.m_fUnitStrengthSoft;
	//m_fUnitStrengthArmor     += Other.m_fUnitStrengthArmor;
	//m_fUnitStrengthAircraft  += Other.m_fUnitStrengthAircraft;
	//m_fUnitStrengthMissile   += Other.m_fUnitStrengthMissile;
	//m_fUnitStrengthShip      += Other.m_fUnitStrengthShip;
	//m_fUnitStrengthSubmarine += Other.m_fUnitStrengthSubmarine;

	// CombatVs is the AttackVs value (to-hit*damage) * Current HP
	m_CombatPowerVsUnitTypes += Other.m_CombatPowerVsUnitTypes;
	m_fCombatVsCity      += Other.m_fCombatVsCity;
	//m_fCombatVsSoft      += Other.m_fCombatVsSoft;
	//m_fCombatVsArmor     += Other.m_fCombatVsArmor;
	//m_fCombatVsAircraft  += Other.m_fCombatVsAircraft;
	//m_fCombatVsMissile   += Other.m_fCombatVsMissile;
	//m_fCombatVsShip      += Other.m_fCombatVsShip;
	//m_fCombatVsSubmarine += Other.m_fCombatVsSubmarine;
}

void  CEOSAIUnit2Summary::operator-=( CEOSAIUnit2Summary& Other )
{
	m_fExploration        -= Other.m_fExploration;
	m_fSighting           -= Other.m_fSighting;

	m_fStrategicBombing   -= Other.m_fStrategicBombing;
	//m_fTacticalBombing    -= Other.m_fTacticalBombing;
	m_fAirEscort          -= Other.m_fAirEscort;
	m_fAntiAir            -= Other.m_fAntiAir;

	m_fSubmarineCapability-= Other.m_fSubmarineCapability;
	m_fAntiSubmarine      -= Other.m_fAntiSubmarine;

	m_fLandCitResHunter   -= Other.m_fLandCitResHunter;
	m_fBombardment        -= Other.m_fBombardment;
	m_fGroundAttackUnits  -= Other.m_fGroundAttackUnits;
	m_fGroundDefenseUnits -= Other.m_fGroundDefenseUnits;
	m_fInvasionDefense    -= Other.m_fInvasionDefense;

	m_fSeaResourceHunter  -= Other.m_fSeaResourceHunter;
	m_fSeaSupremacy       -= Other.m_fSeaSupremacy;
	m_fShoreBombardment   -= Other.m_fShoreBombardment;

	m_fGroundTransportCapability -= Other.m_fGroundTransportCapability;
	m_fTrade                     -= Other.m_fTrade;

	// UnitStrength is Defense * Current HP
	m_UnitHPDefense_UnitTypeSet -= Other.m_UnitHPDefense_UnitTypeSet;
	//m_fUnitStrengthSoft      -= Other.m_fUnitStrengthSoft;
	//m_fUnitStrengthArmor     -= Other.m_fUnitStrengthArmor;
	//m_fUnitStrengthAircraft  -= Other.m_fUnitStrengthAircraft;
	//m_fUnitStrengthMissile   -= Other.m_fUnitStrengthMissile;
	//m_fUnitStrengthShip      -= Other.m_fUnitStrengthShip;
	//m_fUnitStrengthSubmarine -= Other.m_fUnitStrengthSubmarine;

	// CombatVs is the AttackVs value (to-hit*damage) * Current HP
	m_CombatPowerVsUnitTypes -= Other.m_CombatPowerVsUnitTypes;
	m_fCombatVsCity      -= Other.m_fCombatVsCity;
	//m_fCombatVsSoft      -= Other.m_fCombatVsSoft;
	//m_fCombatVsArmor     -= Other.m_fCombatVsArmor;
	//m_fCombatVsAircraft  -= Other.m_fCombatVsAircraft;
	//m_fCombatVsMissile   -= Other.m_fCombatVsMissile;
	//m_fCombatVsShip      -= Other.m_fCombatVsShip;
	//m_fCombatVsSubmarine -= Other.m_fCombatVsSubmarine;
}
/*
void  CEOSAIUnit2Summary::operator-=( CUnitTemplate* pUnitTemplate )
{
	CEOSAIUnit2Summary Temp;
	Temp.Calculate( pUnitTemplate );
	*this -= Temp;
}
*/
void  CEOSAIUnit2Summary::Normalize()  // scale all values to the Sum is 1.0
{
	float fSum = Sum();
	if( fSum == 0.0f )
	{
		ASSERT( false );
		return;
	}
	Multiply( 1.0f / fSum );
}

void  CEOSAIUnit2Summary::NothingBelowZero()
{
	if( m_fExploration < 0.0f ) m_fExploration = 0.0f;
	if( m_fSighting < 0.0f ) m_fSighting = 0.0f;

	if( m_fStrategicBombing < 0.0f ) m_fStrategicBombing = 0.0f;
	//if( m_fTacticalBombing < 0.0f ) m_fTacticalBombing = 0.0f;
	if( m_fAirEscort < 0.0f ) m_fAirEscort = 0.0f;
	if( m_fAntiAir < 0.0f ) m_fAntiAir = 0.0f;

	if( m_fSubmarineCapability < 0.0f ) m_fSubmarineCapability = 0.0f;
	if( m_fAntiSubmarine < 0.0f ) m_fAntiSubmarine = 0.0f;

	if( m_fLandCitResHunter < 0.0f ) m_fLandCitResHunter = 0.0f;
	if( m_fBombardment < 0.0f ) m_fBombardment = 0.0f;
	if( m_fGroundAttackUnits < 0.0f ) m_fGroundAttackUnits = 0.0f;
	if( m_fGroundDefenseUnits < 0.0f ) m_fGroundDefenseUnits = 0.0f;
	if( m_fInvasionDefense < 0.0f ) m_fInvasionDefense = 0.0f;

	if( m_fSeaResourceHunter < 0.0f ) m_fSeaResourceHunter = 0.0f;
	if( m_fSeaSupremacy < 0.0f ) m_fSeaSupremacy = 0.0f;
	if( m_fShoreBombardment < 0.0f ) m_fShoreBombardment = 0.0f;

	if( m_fGroundTransportCapability < 0.0f ) m_fGroundTransportCapability = 0.0f;
	if( m_fTrade < 0.0f ) m_fTrade = 0.0f;

	// UnitStrength is Defense * Current HP
	m_UnitHPDefense_UnitTypeSet.NothingBelowZero();
	//if( m_fUnitStrengthSoft ) m_fUnitStrengthSoft = 0.0f;
	//if( m_fUnitStrengthArmor ) m_fUnitStrengthArmor = 0.0f;
	//if( m_fUnitStrengthAircraft ) m_fUnitStrengthAircraft = 0.0f;
	//if( m_fUnitStrengthMissile ) m_fUnitStrengthMissile = 0.0f;
	//if( m_fUnitStrengthShip ) m_fUnitStrengthShip = 0.0f;
	//if( m_fUnitStrengthSubmarine ) m_fUnitStrengthSubmarine = 0.0f;

	// CombatVs is the AttackVs value (to-hit*damage) * Current HP
	m_CombatPowerVsUnitTypes.NothingBelowZero();
	if( m_fCombatVsCity ) m_fCombatVsCity = 0.0f;
	//if( m_fCombatVsSoft ) m_fCombatVsSoft = 0.0f;
	//if( m_fCombatVsArmor ) m_fCombatVsArmor = 0.0f;
	//if( m_fCombatVsAircraft ) m_fCombatVsAircraft = 0.0f;
	//if( m_fCombatVsMissile ) m_fCombatVsMissile = 0.0f;
	//if( m_fCombatVsShip ) m_fCombatVsShip = 0.0f;
	//if( m_fCombatVsSubmarine ) m_fCombatVsSubmarine = 0.0f;

}

void  CEOSAIUnit2Summary::Multiply( float fValue )
{
	m_fExploration *= fValue;
	m_fSighting *= fValue;

	m_fStrategicBombing *= fValue;
	//m_fTacticalBombing *= fValue;
	m_fAirEscort *= fValue;
	m_fAntiAir *= fValue;

	m_fSubmarineCapability *= fValue;
	m_fAntiSubmarine *= fValue;

	m_fLandCitResHunter *= fValue;
	m_fBombardment *= fValue;
	m_fGroundAttackUnits *= fValue;
	m_fGroundDefenseUnits *= fValue;
	m_fInvasionDefense *= fValue;

	m_fSeaResourceHunter *= fValue;
	m_fSeaSupremacy *= fValue;
	m_fShoreBombardment *= fValue;

	m_fGroundTransportCapability *= fValue;
	m_fTrade *= fValue;

	// UnitStrength is Defense * Current HP
	m_UnitHPDefense_UnitTypeSet.Multiply( fValue );
	//m_fUnitStrengthSoft      *= fValue;
	//m_fUnitStrengthArmor     *= fValue;
	//m_fUnitStrengthAircraft  *= fValue;
	//m_fUnitStrengthMissile   *= fValue;
	//m_fUnitStrengthShip      *= fValue;
	//m_fUnitStrengthSubmarine *= fValue;

	// CombatVs is the AttackVs value (to-hit*damage) * Current HP
	m_CombatPowerVsUnitTypes *= fValue;
	m_fCombatVsCity      *= fValue;
	//m_fCombatVsSoft      *= fValue;
	//m_fCombatVsArmor     *= fValue;
	//m_fCombatVsAircraft  *= fValue;
	//m_fCombatVsMissile   *= fValue;
	//m_fCombatVsShip      *= fValue;
	//m_fCombatVsSubmarine *= fValue;
}

float CEOSAIUnit2Summary::Sum()
{
	return
		m_fExploration +
		m_fSighting +

		m_fStrategicBombing +
		//m_fTacticalBombing +
		m_fAirEscort +
		m_fAntiAir +

		m_fSubmarineCapability +
		m_fAntiSubmarine +

		m_fLandCitResHunter +
		m_fBombardment +
		m_fGroundAttackUnits +
		m_fGroundDefenseUnits +
		m_fInvasionDefense +

		m_fSeaResourceHunter +
		m_fSeaSupremacy +
		m_fShoreBombardment +

		m_fGroundTransportCapability +
		m_fTrade +

		// UnitStrength is Defense * Current HP
		m_UnitHPDefense_UnitTypeSet.GetCummulativeValue() +
		//m_fUnitStrengthSoft +
		//m_fUnitStrengthArmor +
		//m_fUnitStrengthAircraft +
		//m_fUnitStrengthMissile +
		//m_fUnitStrengthShip +
		//m_fUnitStrengthSubmarine +

		// CombatVs is the AttackVs value (to-hit*damage) * Current HP
		m_CombatPowerVsUnitTypes.GetCummulativeValue() +
		m_fCombatVsCity +
		//m_fCombatVsSoft +
		//m_fCombatVsArmor +
		//m_fCombatVsAircraft +
		//m_fCombatVsMissile +
		//m_fCombatVsShip +
		//m_fCombatVsSubmarine +

		0.01f;  // so that we never return 0 (that would be bad for some later calculations
}

float CEOSAIUnit2Summary::SumOfCombatValues()
{
	return
		// UnitStrength is Defense * Current HP
		m_UnitHPDefense_UnitTypeSet.GetCummulativeValue() +
		//m_fUnitStrengthSoft +
		//m_fUnitStrengthArmor +
		//m_fUnitStrengthAircraft +
		//m_fUnitStrengthMissile +
		//m_fUnitStrengthShip +
		//m_fUnitStrengthSubmarine +

		// CombatVs is the AttackVs value (to-hit*damage) * Current HP
		m_CombatPowerVsUnitTypes.GetCummulativeValue() +
		m_fCombatVsCity +
		//m_fCombatVsSoft +
		//m_fCombatVsArmor +
		//m_fCombatVsAircraft +
		//m_fCombatVsMissile +
		//m_fCombatVsShip +
		//m_fCombatVsSubmarine +

		0.01f;  // so that we never return 0 (that would be bad for some later calculations
}


void  CEOSAIUnit2Summary::AddToSummaryValue( CEOSAIUnitTemplate* pUnitTemplate )  // Calculate abilities of single unit
{
	//CUnitTemplate* pUnitTemplate = pAIUnitTemplate->GetUnitTemplate();
	//
	//m_AIUnitTemplates.AddTail( pAIUnitTemplate );

	//
	m_fExploration += pUnitTemplate->GetExplorationCapability();
	m_fSighting += pUnitTemplate->GetSightingCapability();

	m_fStrategicBombing += pUnitTemplate->GetStrategicBombingCapability();
	//m_fTacticalBombing = pUnitTemplate->GetTacticalBombingCapability();
	m_fAirEscort += pUnitTemplate->GetAirEscortCapability();
	m_fAntiAir += pUnitTemplate->GetAntiAirCapability();

	m_fSubmarineCapability += pUnitTemplate->GetSubmarineCapability();
	m_fAntiSubmarine += pUnitTemplate->GetAntiSubmarineCapability();

	m_fLandCitResHunter += pUnitTemplate->GetLandCitResHunterCapability();
	m_fBombardment += pUnitTemplate->GetBombardmentCapability();
	m_fGroundAttackUnits += pUnitTemplate->GetOffensiveGroundCapability();
	m_fGroundDefenseUnits += pUnitTemplate->GetDefensiveGroundCapability();
	m_fInvasionDefense += pUnitTemplate->GetInvasionDefense();

	m_fSeaResourceHunter += pUnitTemplate->GetSeaResourceHunterCapability();
	m_fSeaSupremacy += pUnitTemplate->GetSeaSupremacyCapability();
	m_fShoreBombardment += pUnitTemplate->GetShoreBombardmentCapability();

	m_fGroundTransportCapability += pUnitTemplate->GetGroundTransportCapability();
//	m_fTrade += pUnitTemplate->GetCargoCapacity();

	// Combat
	m_UnitHPDefense_UnitTypeSet.Add( pUnitTemplate->GetCombatUnitTypeString(), pUnitTemplate->GetHPDefenseValue() );

	m_CombatPowerVsUnitTypes.Add( pUnitTemplate->GetCombatPowerVsUnitTypes() );
	m_fCombatVsCity += pUnitTemplate->GetAICombatVsCity();
}
