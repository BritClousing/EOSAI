
#include "stdafx.h"
#include "EOSAIResourceEffectsSummary.h"
//#include "PoiObject.h"
//#include "City.h"
//#include "Airfield.h"
//#include "Unit.h"
#include "EOSAIMathFunction.h"
#include "EOSAICommonData2.h"
#include "AIPlayer.h"
//#include "WorldDesc.h"
/*
long CResourceEffectsSummary::GetAvailabilityNumber( float fAvailability )
{
	// Availability
	//   Critical (  0- 60% Availability)   Scarce
	//   Shortage ( 60-100% Availability)
	//   Adequate (    100% Availability)
	//   Surplus  (100-120% Availability)
	//   Ample    (120%+    Availability)
	if(  fAvailability <  0.6f ) return 1;
	eif( fAvailability <  1.0f ) return 2;
	eif( fAvailability == 1.0f ) return 3;
	eif( fAvailability >  1.0f && fAvailability < 1.20f ) return 4;
	eif( fAvailability >= 1.20f ) return 5;
	//
	ASSERT( false );
	return 3;
}
*/
/*
CString CResourceEffectsSummary::GetAvailabilityText( float fAvailability )
{
	// Availability
	//   Critical (  0- 60% Availability)   Scarce
	//   Shortage ( 60-100% Availability)
	//   Adequate (    100% Availability)
	//   Surplus  (100-120% Availability)
	//   Ample    (120%+    Availability)
	if( fAvailability <  0.6f ) return Lang( 2376, _T("Critical"));
	eif( fAvailability <  1.0f ) return Lang( 2377, _T("Shortage"));
	eif( fAvailability == 1.0f ) return Lang( 2378, _T("Adequate"));
	eif( fAvailability >  1.0f && fAvailability < 1.2f ) return Lang( 2379, _T("Surplus"));
	eif( fAvailability >= 1.20f ) return Lang( 2380, _T("Ample"));

	ASSERT( false );
	return _T("");
}
*/
CEOSAIResourceEffectsSummary::CEOSAIResourceEffectsSummary()
{
	Clear();
}

void CEOSAIResourceEffectsSummary::Clear()
{
	m_ResourceProduction.Clear();
	m_ResourceConsumption.Clear();
	m_ResourceTrade.Clear();
	m_ResourceDelta.Clear();

	m_ResourceConsumptionRate.Clear();
	m_ResourceExcessAmount.Clear();
	m_ResourceAvailabilityDisplay.Clear();
	/*
	m_fCityPopulationGrowth = 1.0f;
	//
	m_fMoneyBasedProductionMultiplier = 1.0f;
	m_fOilBasedProductionMultiplier = 1.0f;
	m_fFoodBasedProductionMultiplier = 1.0f;
	m_fIronBasedIronProductionMultiplier = 1.0f;
	//
	m_fOilBasedRangedUnitLaunchPercentage = 1.0f;
	m_fOilBasedGroundSeaUnitMovementRateMultiplier = 1.0f;
	*/
}


// Excess Oil -> Increased Production x Cities -> Increased Oil Consumption
// Reduced Oil -> Reduced Production x Cities -> Reduced Oil Consumption
//void ApplyAdjustedResourceAvailability( 
void CEOSAIResourceEffectsSummary::Calculate( EOSAI::AIPlayer* pAIPlayer, long iPlayer, EOSAI::StringAndFloatSet* pResourceTotals )
{
	// This should not be done in the AI code.  It's very specific to the game.  Maybe it should be done in the game itself and passed.

	//
	//
	//
	// Calculating the Resource Consumption, Production, trade adjustments, etc is going to be done inside EOSAINationalSummary, not here.
	//
	//
	//
	ASSERT(false);
	ASSERT(false);
	ASSERT(false);
	ASSERT(false);
	ASSERT(false);

	//g_pEOSAICommonData->GetAmountOfMoneyProduced
	//pAIPlayer->GetAIBrain()->A

	// Calcualte the Resource Deltas
	//m_ResourceProduction.Set(_T("Money"), pAIPlayer->AmountOfMoneyProducedOverTimeperiod(iPlayer, 1.0f));

#ifdef DO_NOT_COMPILE_MOVE_TO_MAIN_GAME

	// Calculate the resource consumption and production, figure out the delta and the availability
	// Based on the availability, adjust various production, movement rates, consumption the PoiObjects
	// Based on the PoiObjects' new consumption, adjust the total consumption
	//   (If excess resources leads to faster production, but the player has no cities, then it can't reduce consumption)
	// The system has to be setup to allow for any generic resources

	CStringList  Resources;
	Resources.AddHead( _T("Money") );
	Resources.AddHead( _T("Iron") );
	Resources.AddHead( _T("Food") );
	Resources.AddHead( _T("Oil") );

	// Add-up the resource consumption and production
	// Removing this section dropped processing time from 5.170023 to 0.151200
/*
	m_ResourceProduction.Set( _T("Money"), 1.0f );
	m_ResourceConsumption.Set( _T("Money"), 1.0f );
	m_ResourceProduction.Set( _T("Iron"), 1.0f );
	m_ResourceConsumption.Set( _T("Iron"), 1.0f );
	m_ResourceProduction.Set( _T("Food"), 1.0f );
	m_ResourceConsumption.Set( _T("Food"), 1.0f );
	m_ResourceProduction.Set( _T("Oil"), 1.0f );
	m_ResourceConsumption.Set( _T("Oil"), 1.0f );
*/
	m_ResourceProduction.Set( _T("Money"), pWorldDesc->AmountOfMoneyProducedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceConsumption.Set( _T("Money"), pWorldDesc->AmountOfMoneyConsumedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceProduction.Set( _T("Iron"), pWorldDesc->AmountOfIronProducedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceConsumption.Set( _T("Iron"), pWorldDesc->AmountOfIronConsumedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceProduction.Set( _T("Food"), pWorldDesc->AmountOfFoodProducedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceConsumption.Set( _T("Food"), pWorldDesc->AmountOfFoodConsumedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceProduction.Set( _T("Oil"), pWorldDesc->AmountOfOilProducedOverTimeperiod( iPlayer, 1.0f ) );
	m_ResourceConsumption.Set( _T("Oil"), pWorldDesc->AmountOfOilConsumedOverTimeperiod( iPlayer, 1.0f ) );

	// This clears the resource-effects as well
	//   (The resource effects can change the resource production values.  Example: cities producing money)
	m_ResourceTrade.Clear();
	m_ResourceDelta.Clear();
	m_ResourceConsumptionRate.Clear();
	m_ResourceExcessAmount.Clear();
	m_ResourceAvailabilityDisplay.Clear();
	//
	m_fCityPopulationGrowth = 1.0f;
	//
	m_fMoneyBasedProductionMultiplier = 1.0f;
	m_fOilBasedProductionMultiplier = 1.0f;
	m_fFoodBasedProductionMultiplier = 1.0f;
	m_fIronBasedIronProductionMultiplier = 1.0f;
	//
	m_fOilBasedRangedUnitLaunchPercentage = 1.0f;
	m_fOilBasedGroundSeaUnitMovementRateMultiplier = 1.0f;


	// Trade
	//EOSAI::StringAndFloatSet ResourceTrade;
	POSITION pos = pWorldDesc->m_TradeAgreements.GetHeadPosition();
	while( pos )
	{
		CTradeAgreement* pTradeAgreement = pWorldDesc->m_TradeAgreements.GetNext( pos );
		pTradeAgreement->AddOngoingMoneyAndResourceDeltas( iPlayer, &m_ResourceTrade );
	}

	//  0 Resource = 0.0 Availability
	// 10 Resource = 0.7 Availability
	// 20 Resource = 1.0 Availability
/*
	CEOSAIMathFunction  LowResourceToAvailability;
	LowResourceToAvailability.SetInputOutput(  0.0f, 0.0f );
	LowResourceToAvailability.SetInputOutput( 10.0f, 0.7f );
	LowResourceToAvailability.SetInputOutput( 20.0f, 1.0f );
	// 
	CEOSAIMathFunction  ResourceRatioToAvailability;
//	ResourceRatioToAvailability.SetInputOutput( 0.0f, 0.0f );
//	ResourceRatioToAvailability.SetInputOutput( 0.1f, 0.2f );
//	ResourceRatioToAvailability.SetInputOutput( 0.2f, 0.8f ); // Target 50, 80% Availability at 10
//	ResourceRatioToAvailability.SetInputOutput( 0.5f, 1.0f ); // Target 50, Availability declines at 25
	ResourceRatioToAvailability.SetInputOutput( 2.0f, 1.0f ); // Target 50, Availability increases at 100
	ResourceRatioToAvailability.SetInputOutput( 4.0f, 1.2f );
	ResourceRatioToAvailability.SetInputOutput( 8.0f, 1.4f );
	ResourceRatioToAvailability.SetInputOutput(16.0f, 1.5f );
	ResourceRatioToAvailability.SetInputOutput(32.0f, 1.6f );
	ResourceRatioToAvailability.SetInputOutput(64.0f, 1.7f );

	CEOSAIMathFunction  AvailabilityUpto1;
	AvailabilityUpto1.SetInputOutput( 0.0f, 0.0f );
	AvailabilityUpto1.SetInputOutput( 1.0f, 1.0f );

	//AvailabilityReducedPast1;
	CEOSAIMathFunction  NoEffectBelowZeroSlightIncreasesBeyond1;
	NoEffectBelowZeroSlightIncreasesBeyond1.SetInputOutput( 0.0f, 1.0f );
	NoEffectBelowZeroSlightIncreasesBeyond1.SetInputOutput( 1.0f, 1.0f );
	NoEffectBelowZeroSlightIncreasesBeyond1.SetInputOutput( 2.0f, 1.5f );

	CEOSAIMathFunction  AvailabilityNoneStep;
	AvailabilityNoneStep.SetInputOutput( 0.0f, 0.0f );
	AvailabilityNoneStep.SetInputOutput( 0.1f, 0.0f );
	AvailabilityNoneStep.SetInputOutput( 0.11f,1.0f );

	CEOSAIMathFunction  AvailabilityFoodToPopulationGrowth;
	AvailabilityFoodToPopulationGrowth.SetInputOutput( 0.0f, 0.95f );
	AvailabilityFoodToPopulationGrowth.SetInputOutput( 0.5f, 1.000f ); // Starvation starts
	AvailabilityFoodToPopulationGrowth.SetInputOutput( 0.8f, 1.002f ); // Normal population growth
	AvailabilityFoodToPopulationGrowth.SetInputOutput( 1.0f, 1.002f ); // Normal population growth (15 Pop->18 Pop in 100 turns)
	AvailabilityFoodToPopulationGrowth.SetInputOutput( 1.4f, 1.004f ); // Faster population growth
*/
	// Now that I've added up the Consumption and Production, figure out the resource availability
	//   The resource availability can vary from 0.0 to 2.0
	POSITION pos2 = Resources.GetHeadPosition();
	while( pos2 )
	{
		CString strResource = Resources.GetNext( pos2 );

		float fResourceConsumption = m_ResourceConsumption.Get( strResource );
		float fResourceProduction = m_ResourceProduction.Get( strResource );
		float fResourceTrade = m_ResourceTrade.Get( strResource );
		float fResourceDelta = fResourceProduction + fResourceTrade - fResourceConsumption;
		m_ResourceDelta.Set( strResource, fResourceDelta );

		float fResourceTotal = pResourceTotals->Get( strResource );

		float fResourceNow = fResourceTotal;
//		float fResourceNextOrNow = min( fResourceTotal, (fResourceTotal + fResourceDelta) );
//		if( fResourceNextOrNow < 0.0f ){ fResourceNextOrNow = 0.0f; }

		float fResourceTarget = 10.0f + (3.0f*max(5.0f,fResourceConsumption)); // Minimum Target of 25
//		float fResourceTarget = 10.0f + (3.0f*max(5.0f,fResourceConsumption)); // Minimum Target of 25
//		if( fResourceDelta < 0.0f ){ fResourceTarget += 2.0f*(-fResourceDelta); }
		ASSERT( fResourceTarget >= 25.0f );

		//float fResourceTargetRatio = fResourceNextOrNow/fResourceTarget;
		//float fAvailability = 1.0f;

		//  0 Resource = 0.0 Availability
		// 10 Resource = 0.7 Availability
		// 20 Resource = 1.0 Availability
		CEOSAIMathFunction  ResourceToAvailability;
		if( strResource == _T("Money") )
		{
			CEOSAIMathFunction  MoneyBasedProductionMultiplier;
			MoneyBasedProductionMultiplier.SetInputOutput(  0.0f, 1.0f ); // No effect
			MoneyBasedProductionMultiplier.SetInputOutput(  4.0f*fResourceTarget, 1.0f  ); // +0%  -  100 Money
			MoneyBasedProductionMultiplier.SetInputOutput( 16.0f*fResourceTarget, 1.1f );  // +10% -  400 Money
			MoneyBasedProductionMultiplier.SetInputOutput( 40.0f*fResourceTarget, 1.2f  ); // +20% - 1000 Money
			MoneyBasedProductionMultiplier.SetInputOutput(100.0f*fResourceTarget, 1.3f  ); // +30% - 2500 Money
			m_fMoneyBasedProductionMultiplier = MoneyBasedProductionMultiplier.GetOutput( fResourceTotal );//fResourceNextOrNow );

			// Consumption rate never drops below 1.0
			CEOSAIMathFunction  ResourceConsumptionRate;
			ResourceConsumptionRate.SetInputOutput(   0.0f, 1.0f );
			ResourceConsumptionRate.SetInputOutput(   4.0f*fResourceTarget, 1.0f );
			ResourceConsumptionRate.SetInputOutput(  16.0f*fResourceTarget, 1.1f );
			ResourceConsumptionRate.SetInputOutput(  40.0f*fResourceTarget, 1.2f );
			ResourceConsumptionRate.SetInputOutput( 100.0f*fResourceTarget, 1.3f );
			float fConsumptionRate = ResourceConsumptionRate.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceConsumptionRate.Set( strResource, fConsumptionRate );
			ASSERT( fConsumptionRate >= 1.0 && fConsumptionRate <= 1.3f );

			CEOSAIMathFunction  AvailabilityDisplay;
			//AvailabilityDisplay.SetInputOutput(  0.0f, 1.0f );
			//AvailabilityDisplay.SetInputOutput( 10.0f, 2.0f );
			AvailabilityDisplay.SetInputOutput( 25.0f, 3.0f );
			AvailabilityDisplay.SetInputOutput(  4.0f*fResourceTarget, 4.0f );
			AvailabilityDisplay.SetInputOutput( 16.0f*fResourceTarget, 5.0f );
			long iDisplayAvailability = (long) AvailabilityDisplay.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceAvailabilityDisplay.Set( strResource, iDisplayAvailability );

			m_ResourceExcessAmount.Set( _T("Money"), 4.0f*fResourceTarget );
		}
		if( strResource == _T("Iron") )
		{
			CEOSAIMathFunction  IronBasedIronProductionMultiplier;
			IronBasedIronProductionMultiplier.SetInputOutput(  0.0f, 0.0f );
			IronBasedIronProductionMultiplier.SetInputOutput( 10.0f, 0.7f );
			IronBasedIronProductionMultiplier.SetInputOutput( 25.0f, 1.0f );
			m_fIronBasedIronProductionMultiplier = IronBasedIronProductionMultiplier.GetOutput( fResourceTotal );//fResourceNextOrNow );

			CEOSAIMathFunction  ResourceConsumptionRate;
			ResourceConsumptionRate.SetInputOutput(  0.0f, 0.0f );
			ResourceConsumptionRate.SetInputOutput( 10.0f, 0.7f );
			ResourceConsumptionRate.SetInputOutput( 25.0f, 1.0f );
			float fConsumptionRate = ResourceConsumptionRate.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceConsumptionRate.Set( strResource, fConsumptionRate );
			ASSERT( fConsumptionRate >= 0.0 && fConsumptionRate <= 1.0f );

			CEOSAIMathFunction  AvailabilityDisplay;
			AvailabilityDisplay.SetInputOutput(  0.0f, 1.0f );
			AvailabilityDisplay.SetInputOutput( 10.0f, 2.0f );
			AvailabilityDisplay.SetInputOutput( 25.0f, 3.0f );
			long iDisplayAvailability = (long) AvailabilityDisplay.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceAvailabilityDisplay.Set( strResource, iDisplayAvailability );
		}
		if( strResource == _T("Oil") )
		{
			CEOSAIMathFunction  OilBasedProductionMultiplier;
			OilBasedProductionMultiplier.SetInputOutput(  0.0f, 0.1f ); // A little production
			OilBasedProductionMultiplier.SetInputOutput( 10.0f, 0.2f ); // A little production
			OilBasedProductionMultiplier.SetInputOutput( 25.0f, 1.0f ); // Normal Production
			OilBasedProductionMultiplier.SetInputOutput(  4.0f*fResourceTarget, 1.0f ); // Normal Production
			OilBasedProductionMultiplier.SetInputOutput(  8.0f*fResourceTarget, 1.1f ); // +10% - 200 Oil
			OilBasedProductionMultiplier.SetInputOutput( 16.0f*fResourceTarget, 1.2f ); // +20% - 400 Oil
			OilBasedProductionMultiplier.SetInputOutput( 32.0f*fResourceTarget, 1.3f ); // +30% - 800 Oil
			OilBasedProductionMultiplier.SetInputOutput( 64.0f*fResourceTarget, 1.4f ); // +40% - 1600 Oil
			m_fOilBasedProductionMultiplier = OilBasedProductionMultiplier.GetOutput( fResourceTotal );//fResourceNextOrNow );

			CEOSAIMathFunction  GroundSeaUnitMovementRateMultiplier;
			GroundSeaUnitMovementRateMultiplier.SetInputOutput(  0.0f, 0.1f ); // A little movement
			GroundSeaUnitMovementRateMultiplier.SetInputOutput( 10.0f, 0.7f ); // A little movement
			GroundSeaUnitMovementRateMultiplier.SetInputOutput( 25.0f, 1.0f ); // Normal Movement
			m_fOilBasedGroundSeaUnitMovementRateMultiplier = GroundSeaUnitMovementRateMultiplier.GetOutput( fResourceTotal );//fResourceNextOrNow );

			// Air units are never slowed down by reduced oil - 
			//   however, they should fail to launch if there's insufficient oil
			CEOSAIMathFunction  RangedUnitLaunchPercentage;
			RangedUnitLaunchPercentage.SetInputOutput(  0.0f, 0.0f ); // A little movement
			RangedUnitLaunchPercentage.SetInputOutput(  4.0f, 0.0f ); // A little movement
			RangedUnitLaunchPercentage.SetInputOutput( 10.0f, 1.0f ); // Normal Movement
			m_fOilBasedRangedUnitLaunchPercentage = RangedUnitLaunchPercentage.GetOutput( fResourceTotal );//fResourceNextOrNow );
			//m_fOilBasedAirUnitMovementRateMultiplier = RangedUnitLaunchPercentage.GetOutput( fResourceTotal );//fResourceNextOrNow );

			CEOSAIMathFunction  ResourceConsumptionRate;
			ResourceConsumptionRate.SetInputOutput(  0.0f, 0.0f );
			ResourceConsumptionRate.SetInputOutput( 10.0f, 0.7f );
			ResourceConsumptionRate.SetInputOutput( 25.0f, 1.0f );
			ResourceConsumptionRate.SetInputOutput(  4.0f*fResourceTarget, 1.0f ); // Normal Production
			ResourceConsumptionRate.SetInputOutput(  8.0f*fResourceTarget, 1.1f );
			ResourceConsumptionRate.SetInputOutput( 16.0f*fResourceTarget, 1.2f );
			ResourceConsumptionRate.SetInputOutput( 32.0f*fResourceTarget, 1.3f );
			ResourceConsumptionRate.SetInputOutput( 64.0f*fResourceTarget, 1.4f ); // +40% Production
			float fConsumptionRate = ResourceConsumptionRate.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceConsumptionRate.Set( strResource, fConsumptionRate );

			CEOSAIMathFunction  AvailabilityDisplay;
			AvailabilityDisplay.SetInputOutput(  0.0f, 1.0f );
			AvailabilityDisplay.SetInputOutput( 10.0f, 2.0f );
			AvailabilityDisplay.SetInputOutput( 25.0f, 3.0f );
			AvailabilityDisplay.SetInputOutput( 4.0f*fResourceTarget, 4.0f );
			AvailabilityDisplay.SetInputOutput( 8.0f*fResourceTarget, 5.0f );
			long iDisplayAvailability = (long) AvailabilityDisplay.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceAvailabilityDisplay.Set( strResource, iDisplayAvailability );

			m_ResourceExcessAmount.Set( _T("Oil"), 4.0f*fResourceTarget );
		}
		if( strResource == _T("Food") )
		{
			CEOSAIMathFunction  FoodBasedProductionMultiplier;
			FoodBasedProductionMultiplier.SetInputOutput(  0.0f, 0.5f);  // A little production
			//FoodBasedProductionMultiplier.SetInputOutput( 10.0f, 0.6f ); // A little production
			FoodBasedProductionMultiplier.SetInputOutput( 25.0f, 1.0f ); // Normal production
			m_fFoodBasedProductionMultiplier = FoodBasedProductionMultiplier.GetOutput( fResourceTotal );//fResourceNow );

			CEOSAIMathFunction  FoodToPopulationGrowth;
			FoodToPopulationGrowth.SetInputOutput(  0.0f, 0.96f );
			FoodToPopulationGrowth.SetInputOutput( 10.0f, 1.000f ); // Starvation starts
			FoodToPopulationGrowth.SetInputOutput( 25.0f, 1.002f ); // Normal population growth
			FoodToPopulationGrowth.SetInputOutput( 4.0f*fResourceTarget, 1.002f ); // Faster population growth
			FoodToPopulationGrowth.SetInputOutput( 20.0f*fResourceTarget, 1.004f ); // Double Growth, 800 Food
			FoodToPopulationGrowth.SetInputOutput( 50.0f*fResourceTarget, 1.005f ); // 2.5x Growth, 2000 Food
			m_fCityPopulationGrowth = FoodToPopulationGrowth.GetOutput( fResourceTotal );//fResourceNow );

			CEOSAIMathFunction  ResourceConsumptionRate;
			ResourceConsumptionRate.SetInputOutput(  0.0f, 0.0f );
			ResourceConsumptionRate.SetInputOutput( 10.0f, 0.7f );
			ResourceConsumptionRate.SetInputOutput( 40.0f, 1.0f ); // Start to get some declines in consumption at 40 - this is rationing, and nations have no negative effects until it gets down to 25
			//ResourceConsumptionRate.SetInputOutput( 25.0f, 1.0f );
			ResourceConsumptionRate.SetInputOutput( 4.0f*fResourceTarget, 1.0f ); // Faster population growth
			ResourceConsumptionRate.SetInputOutput( 20.0f*fResourceTarget, 1.3f ); // Faster population growth
			float fConsumptionRate = ResourceConsumptionRate.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceConsumptionRate.Set( strResource, fConsumptionRate );

			CEOSAIMathFunction  AvailabilityDisplay;
			AvailabilityDisplay.SetInputOutput(  0.0f, 1.0f );
			AvailabilityDisplay.SetInputOutput( 10.0f, 2.0f );
			AvailabilityDisplay.SetInputOutput( 25.0f, 3.0f );
			AvailabilityDisplay.SetInputOutput( 4.0f*fResourceTarget, 4.0f );
			AvailabilityDisplay.SetInputOutput( 8.0f*fResourceTarget, 5.0f );
			long iDisplayAvailability = (long) AvailabilityDisplay.GetOutput( fResourceTotal );//fResourceNow );
			m_ResourceAvailabilityDisplay.Set( strResource, iDisplayAvailability );

			m_ResourceExcessAmount.Set( _T("Food"), 4.0f*fResourceTarget );
		}

		// Availability numbers end up reducing the production rate of items that consume that resource
		/*
		if( strResource == _T("Money") )
		{
			m_ResourceAvailability.Set( _T("Money"), 1.0f ); // Money availability is always 1.0
		}
		else
		{
			CEOSAIMathFunction  ResourceAvailability;
			ResourceAvailability.SetInputOutput(  0.0f, 0.0f );
			ResourceAvailability.SetInputOutput( 10.0f, 0.7f );
			ResourceAvailability.SetInputOutput( 25.0f, 1.0f );

			float fAvailability = ResourceAvailability.GetOutput( fResourceNow );
			m_ResourceAvailability.Set( strResource, fAvailability );
		}
		*/
/*
		// Display Availability
		//   I want to display something - not a float value
		long iDisplayAvailability = 3;
		if(  fResourceNow < 10.0f ){ iDisplayAvailability = 1; }
		eif( fResourceNow < 25.0f ){ iDisplayAvailability = 2; }
		eif( fResourceNow <  4.0f*fResourceTarget ){ iDisplayAvailability = 3; }
		eif( fResourceNow < 10.0f*fResourceTarget ){ iDisplayAvailability = 4; }
		else{ iDisplayAvailability = 5; }
		m_ResourceAvailabilityDisplay.Set( strResource, iDisplayAvailability );
*/
		// Availability
		//   Critical (  0- 60% Availability)   Scarce
		//   Shortage ( 60-100% Availability)
		//   Adequate (    100% Availability)
		//   Surplus  (100-120% Availability)
		//   Ample    (120%+    Availability)

		/*
		if( strResource == _T("Money") )
		{
			//m_fMoneyBasedProductionMultiplier *= NoEffectBelowZeroSlightIncreasesBeyond1.GetOutput( fAvailability );
		}
		if( strResource == _T("Oil") )
		{
			// Apply Effects to cities first, then land/sea units, then air units
			//pSummary->m_fCityOilConsumptionMult = fAvailability;
			//m_fOilBasedProductionMultiplier *= fAvailability;
			//m_fGroundSeaUnitMovementRateMultiplier = AvailabilityUpto1.GetOutput( fAvailability );
			//m_fAirUnitMovementRateMultiplier = AvailabilityNoneStep.GetOutput( fAvailability ); // 1.0 = Normal
		}
		if( strResource == _T("Food") )
		{
			//m_fFoodBasedProductionMultiplier *= AvailabilityUpto1.GetOutput( fAvailability );
			//m_fCityPopulationGrowth = AvailabilityFoodToPopulationGrowth.GetOutput( fAvailability );
		}
		*/
	}

	// Apply the effects to the PoiObjects
	/*
	pos = PlayerActors.GetHeadPosition();
	while( pos )
	{
		CPoiObject* pPoiObject = PlayerActors.GetNext( pos );
		if( pPoiObject->GetAIPoiObjectType() == EnumPoiObjectType::enum_City )
		{
			CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
			ASSERT( pCity );
			if( pCity )
			{
				pCity->SetOilBasedProductionMultiplier( pSummary->m_fCityOilBasedProductionMultiplier );
				pCity->SetFoodBasedProductionMultiplier( pSummary->m_fCityFoodBasedProductionMultiplier );
				pCity->SetCityPopulationGrowth( pSummary->m_fCityPopulationGrowth );
			}
		}
		if( pPoiObject->GetAIPoiObjectType() == EnumPoiObjectType::enum_Unit )
		{
			CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
			if( pUnit )
			{
				if( pUnit->IsGroundUnit() || pUnit->IsSeaUnit() )
				{
					pUnit->SetOilBasedMovementRateMultiplier( pSummary->m_fGroundSeaUnitMovementRateMultiplier );
				}
				eif( pUnit->IsAirUnit() )
				{
					pUnit->SetOilBasedMovementRateMultiplier( pSummary->m_fAirUnitMovementRateMultiplier );
				}
				else
				{
					ASSERT( false );
				}
			}
		}
	}
	*/

/*
	// ResourceAvailability
	POSITION pos = ResourceAvailability.m_List.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* pResourceAvailability = ResourceAvailability.m_List.GetNext( pos );
		if( pResourceAvailability->m_strValue == _T("Money") )
		{
		}
		eif( pResourceAvailability->m_strValue == _T("Oil") )
		{
		}
		eif( pResourceAvailability->m_strValue == _T("Food") )
		{
		}
		eif( pResourceAvailability->m_strValue == _T("Iron") )
		{
		}
	}
*/
#endif DO_NOT_COMPILE_MOVE_TO_MAIN_GAME
}

long CEOSAIResourceEffectsSummary::GetAvailabilityNumber( CString strResource )
{
	long iDisplay = m_ResourceAvailabilityDisplay.GetValue( strResource );
	if( iDisplay < 1 ){ iDisplay = 1; }
	if( iDisplay > 5 ){ iDisplay = 5; }
	return iDisplay;
}
/*
CString CEOSAIResourceEffectsSummary::GetAvailabilityText( CString strResource )
{
	long iDisplay = m_ResourceAvailabilityDisplay.GetValue( strResource );
	if( iDisplay < 1 ){ iDisplay = 1; }
	if( iDisplay > 5 ){ iDisplay = 5; }

	// Availability
	//   Critical (  0- 60% Availability)   Scarce
	//   Shortage ( 60-100% Availability)
	//   Adequate (    100% Availability)
	//   Surplus  (100-120% Availability)
	//   Ample    (120%+    Availability)
	if(  iDisplay == 1 ) return Lang( 2376, _T("Critical"));
	eif( iDisplay == 2 ) return Lang( 2377, _T("Shortage"));
	eif( iDisplay == 3 ) return Lang( 2378, _T("Adequate"));
	eif( iDisplay == 4 ) return Lang( 2379, _T("Surplus"));
	eif( iDisplay == 5 ) return Lang( 2380, _T("Ample"));
	/*
	if( fAvailability <  0.6f ) return Lang( 2376, _T("Critical"));
	eif( fAvailability <  1.0f ) return Lang( 2377, _T("Shortage"));
	eif( fAvailability == 1.0f ) return Lang( 2378, _T("Adequate"));
	eif( fAvailability >  1.0f && fAvailability < 1.2f ) return Lang( 2379, _T("Surplus"));
	eif( fAvailability >= 1.20f ) return Lang( 2380, _T("Ample"));
	*-/

	ASSERT( false );
	return _T("");
}
*/

