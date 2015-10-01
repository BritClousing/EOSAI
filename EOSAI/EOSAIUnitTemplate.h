
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif
/*
#include "EOSAIEnumTypeOfUnit.h"
class CUnitTemplate;
class CAICitResValueMultiplier;
*/
// The AIUnitTemplate describes the unit's abilities.
//   If an AIUnit represents a single unit in the game, then AIUnitTemplate describes that type of unit (e.g. Sherman Tanks in general)
/*
class CEOSAIUnitTemplate
{
	//public:
		//CEOSAIUnitTemplate( CUnitTemplate* pUnitTemplate );
		//CUnitTemplate* GetUnitTemplate(){ return m_pUnitTemplate; }

	//private:
		//CUnitTemplate* m_pUnitTemplate;

		// Roles: "GroundCitResHunter", "AntiArmor firepower", "AirfieldBuilder", etc
		//   The roles are given a value.  The value depends on a number of factors, including
		//   the unit's relative usefulness against that type of unit.  (e.g. if unit X
		//   isn't very good against tanks, but it's the best unit I have against tanks, then
		//   it scores a little higher.  Afterall, it's better to use the best antitank unit I 
		//   have than use a different unit.)
		// Also, might be able to use the "roles" information to figure out what units to 
		//   use against other players.  If his unit X is bad against tactical bombers, I should
		//   consider using tactical bombers against him.
		// Factors that influence these values:
		//   Unit's production cost (including resources, resource multipliers)
		//   The attack/defense values against those types of units (on typical terrain)
		//   ?Movement Rate?
		//   ?Movement Range?
		//   These values in comparison to my other existing+buildable units
		// INCOMPLETE:
		//   The attack/defense values should be considered in light of invasion/defense situation
		//   (high attack values are better when invading)
		// All of these values range from 0 to 1.
		//
};
*/

#pragma once

#include "EOSAIEnumMovementType.h"
//#include "SoundManager.h"
//#include "ListOwned.h"
//#include "AnimationTemplate.h"
#include "EOSAIMovementDescription.h"
//#include "ViewRange.h"
//#include "EnumSubTargeting.h"
#include "EOSAIEnumPoiObjectType.h"
#include "PoiObject.h"
//#include "MissileMovementDefinition.h"
//#include "UnitsubsetPermission.h"
#include "EOSAIStringAndFloatSet.h"
//#include "EnumSpecialUnitSymbol.h"
#include "EOSAIUnitsubsetPermission.h"
#include "EOSAIUnit2CombatCapability.h"
#include "EOSAIJobCapability.h"
#include "EOSAIEnumTypeOfUnit.h"
#include "EOSAIGameRules.h"
#include "EOSAICombatUnitType.h"
#include "EOSAIAttackVs2.h"
//#include "EOSAIStringAndLongSet.h"
//#include "StringANSI8AndFloatSet.h"
class CUnitset;
class CEOSAIBuildOption;
//class CPoi;
class CUnitTag;
class CEOSAIPoiObject;
class CCombatUnitType;
class CBCXMLItem;
class CEOSAIUnitTemplate;
class CEOSAICitResValueMultiplier;
//class CUnitTemplateImage;
//class CUnitTemplateCombatAnimationImage;
class CEOSAIJobCapability;
#include <afxtempl.h>
#include "EOSAIEnumAttackGoal.h"
//#include "External\EnumAttackGoal.h"
typedef CList< CStringANSI8, CStringANSI8 >  CStringANSI8List;

class CUnitsubsetUpgradeTo2
{
	public:
		CString  m_strUnitsubset;
		CString  m_strUpgradeTo;
};


class DLLIMPEXP CEOSAIUnitTemplate
{
	public:
		//
		// Constructor / Destructor
		//
			//CEOSAIUnit2Template( CUnitset* pUnitset );
			CEOSAIUnitTemplate( EOSAI::CGameRules* pAIGameRules );
			~CEOSAIUnitTemplate();

			void  Serialize( CEOSAISerial* pSerial );
			//void  Deserialize( CEOSAISerial* pSerial );

			//void  AppendDataToXMLString( CStringANSI8& strData );
			//void  ReadXMLData( CUnitset* pUnitset, CBCXMLItem* pXMLItem );

			EOSAI::CGameRules*  GetAIGameRules(){ return m_pAIGameRules; }
			//CUnitset*     GetUnitset(){ return m_pUnitset; }

			CEOSAIUnitsubsetPermission* GetAllowUnitsubsetObj(){ return &m_AllowUnitsubset; }
			//CUnitsubsetPermission* GetAllowUnitsubsetObj(){ return &m_AllowUnitsubset; }

			//void          SetBuildOption( CBuildOption* pBuildOption ){ ASSERT( m_pBuildOption == NULL ); m_pBuildOption = pBuildOption; }
			//CBuildOption* GetBuildOption(){ return m_pBuildOption; }


		//
		// Images
		//
	//		CUnitTemplateImage* GetUnitTemplateImage(){ return m_pUnitTemplateImage; }
	//		CAnimationTemplate* GetIdleAnimation();
	//		CString             GetMovementAnimationPath();
	//		CAnimationTemplate* GetMovementAnimation();
	//		CAnimationTemplate* GetDieAnimation();

	//		CString        GetStaticCombatImageString();
	//		CGLTexture*    GetStaticCombatImage( long iRotation );
	//		long         GetNumberOfImages();
	//		CGLTexture*  GetGLTexture( long iRotation );

	//		long           GetDefaultRotation();

		//	CMaskedBitmap* GetGrayMBitmap();
			//CString        GetWhiteBitmapPath();

		//	void      SetUnitImageName( CString str ){ m_strUnitImageName = str; } // e.g. "Tank (Class 1)"
		//	void      LoadUnitImage( CString strUnitImageName ); // e.g. "Tank (Class 1)"
		//	CString   GetUnitImageName(){ return m_strUnitImageName; }

			//void                   SetSpecialUnitSymbol( EnumSpecialUnitSymbol e ){ m_eSpecialUnitSymbol = e; }
			//EnumSpecialUnitSymbol  GetSpecialUnitSymbol(){ return m_eSpecialUnitSymbol; }
			//CMaskedBitmap*         GetSpecialUnitSymbolBitmap();
			//CGLTexture*            GetSpecialUnitSymbolTexture();

			//void      LoadTextures();
		//
		// Upgrades
		//
			void    SetGenericUpgradeTo( CString strUpgradeTo ){ m_strUpgradeTo = strUpgradeTo; }
			//CString GetUpgrade(){ return m_strUpgrade; }
			CString     GetGenericUpgradeTo(){ return m_strUpgradeTo; }
			//
			CList< CUnitsubsetUpgradeTo2* >* GetUnitsubsetUpgradeToList(){ return &m_UpgradeToList; }
			void        InvokeUnitsubsetUpgradeTo( CString strUnitsubset, CString strUpgradeTo );
			CString     GetUnitsubsetUpgradeTo( CString strUnitsubset );
			void        RemoveUnitsubsetUpgradeTo( CString strUnitsubset );
			//
			CEOSAIUnitTemplate* GetUpgradesFrom(); // this doesn't always work right since multiple units can upgrade to this one
			//void    SetUpgradeProductionCost( long i ){ m_iUpgradeProductionCost = i; }
			//long    GetUpgradeProductionCost(){ return m_iUpgradeProductionCost; }

			float   GetUpgradeMoneyCost( CString strUnitsubset );
			float   GetUpgradeIronCost( CString strUnitsubset );
			float   GetUpgradeTime( CString strUnitsubset );

		//
		// Name
		//
			void        SetExternalName( CString strName ){ m_strExternalName = strName; }
			CString     GetExternalName(){ return m_strExternalName; }
			void        SetInternalName( CString strName ){ m_strInternalName = strName; }
			CString     GetInternalName(){ return m_strInternalName; }

			void        SetNotesText( CString strText ){ m_strNotesText = strText; }
			CString     GetNotesText(){ return m_strNotesText; }
		//
			void        SetUnitCategory( CString str ){ m_strUnitCategory = str; }
			CString     GetUnitCategory(){ return m_strUnitCategory; }
		//
		// Freighter
		//
		//	void  SetFreighterRole( bool b ){ m_bFreighterRole = b; }
		//	bool  GetFreighterRole(){ return m_bFreighterRole; }
		//
		// Unit Tags
		//
			void  AddUnitTag( CString strTag );
			void  RemoveUnitTag( CString strTag );
			bool  UnitTagExists( CString strTag );
			CUnitTag*  GetUnitTag( CString strTag );
			CList< CUnitTag* >*  GetUnitTagList(){ return &m_UnitTagList; }
			//CStringList*  GetUnitTagList(){ return &m_UnitTagList; }

			//EOSAI::StringAndLongSet*  GetTargetPriorityTags(){ return &m_TargetPriorityTagSet; }
			//long                GetTargetPriorityTag( CString str );
		//
		// Unit Type
		//
			//void         SetUnitType( EnumUnitType eUnitType ){ m_eCombatUnitType = eUnitType; }
			//EnumUnitType GetUnitType(){ return m_eCombatUnitType; }
			//void         SetUnitType2( CString strUnitType ){ m_strUnitType = strUnitType; }
			//CString      GetUnitType2(){ return m_strUnitType; }

			// Three types: Air, Land, Water
			CEOSAIMovementDescription* GetMovementDescription(){ return &m_MovementDescription; }
			EOSAIEnumMovementType3     GetMovementType(){ return m_MovementDescription.GetMovementType(); }
			// Four types: SoftGround, Armor, Air, Sea
			void                 SetMovementUnitTypeByExternalName( CString strMovementUnitType ); // *OLD* This is only used for backward compatibility
			void                 SetMovementUnitType( long iMovementUnitType );
			CEOSAIMovementUnitType* GetMovementUnitType();
			long                 GetMovementUnitTypeLong();
			CString              GetMovementUnitTypeString();//{ return m_strMovementUnitType; }
			/*
			void               SetMovementUnitType( CString strMovementUnitType );
			CMovementUnitType* GetMovementUnitType();
			CString            GetMovementUnitTypeString();//{ return m_strMovementUnitType; }
			*/

			long    GetNormalTerrain();

			// Six types: SoftGround, Armor, Aircraft, Missile, Ship, Submarine
			//void              SetCombatUnitType( CString strUnitType, EnumPoiObjectType eUnitType, CCombatUnitType* pCombatUnitType ){ m_strCombatUnitType = strUnitType; m_eCombatUnitType = eUnitType; m_pCombatUnitType = pCombatUnitType; }
			//void              SetCombatUnitType( CString strUnitType, CCombatUnitType* pCombatUnitType );
			void                SetCombatUnitType( long iCombatUnitType );
			void                SetCombatUnitTypeByExternalName( CString strUnitType );
			//EnumPoiObjectType GetCombatUnitType(){ return m_eCombatUnitType; }
			long                GetCombatUnitTypeLong(){ return m_iCombatUnitType; }
			CString             GetCombatUnitTypeString();
			CEOSAICombatUnitType*  GetCombatUnitTypePtr(){ return m_pCombatUnitType; }
			long                GetCombatUnitTypeWhenLandedOrDocked();

			bool  CanBlockAndBeBlocked();
			long  GetDrawOrder();

			bool  IsAmphibious(){ return false; }
			bool  IsGroundUnit();//{ return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Land; }
			bool  IsAirUnit();//{ return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Air; }
			bool  IsSeaUnit();//{ return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Water; }
		//
			void  IsSubmarine( bool b ){ m_bIsSubmarine = b; } // this is calculated
			bool  IsSubmarine(){ return m_bIsSubmarine; } // this is calculated
			bool  IsTransport(){ return GetNumberOfGroundUnitsThatICanContain() > 0; }
			bool  IsAirUnitCarrier(){ return GetNumberOfAirUnitsThatICanContain() > 0 && CanLaunchAirUnits(); }
			bool  IsGroundCitResHunter(){ return IsGroundUnit() && CanCaptureCities(); }
			bool  IsCityHunter(){ return CanCaptureCities(); }
			bool  IsSeaResHunter(){ return IsSeaUnit() && CanCaptureResources(); }
		//
			void  SetAverageViewedRange( float f ){ m_fAverageViewedRange = f; }
			float GetAverageViewedRange(){ return m_fAverageViewedRange; }
		//
		//
		//	void  GetPrereqTechList_Top( CStringList* pPrereqTechList );
		//	void  GetPrereqTechList_All( CStringList* pPrereqTechList );
		//	long  GetPrereqTechCost();
		//	long  m_iPrereqTechCost;
			// Precalculated values are used when sorting lists (because it continually re-asks for the value)
		//	void  CalculatePrereqTechCost2();
		//	long  GetPrereqTechCost2(){ return m_iPrereqTechCost; }
		//
			bool         CityMustHave( CString strReq );
			void         AddCityMustHave( CString strReq ){ m_CityMustHaveList.AddTail( strReq ); }
			void         RemoveCityMustHave( CString strReq );
			CStringList* GetCityMustHaveList(){ return &m_CityMustHaveList; }
		//
			bool         CanOnlyBeBuiltInCitiesAdjacentToWater(){ return IsSeaUnit(); }
		//
		//
			void  SetMaxHP( int iHP ){ m_iMaxHP = iHP; }
			int   GetMaxHP(){ return m_iMaxHP; }
		//
		// Production Cost
		//
			void  SetProductionCost( int val ){ m_iProductionCost = val; }
			float GetProductionCost(){ return (float)m_iProductionCost; }
			void  SetIronCost( float val ){ m_fIronCost = val; }
			float GetIronCost(){ return m_fIronCost; }
		//
		// Maintenance Cost
		//
			float GetProductionAndConsumptionPerTurnAndMove( CString strResource );

			void  InvokeProductionAndConsumptionPerTurn( CString strResource, float fAmount );
			float GetProductionAndConsumptionPerTurn( CString strResource );
			EOSAI::StringAndFloat* GetProductionAndConsumptionPerTurnObj( CString strResource );
			void  RemoveProductionAndConsumptionPerTurn( CString strResource );
			CList< EOSAI::StringAndFloat* >*  GetProductionAndConsumptionPerTurnList(){ return &m_ProductionAndConsumptionPerTurnList; }

			void  InvokeProductionAndConsumptionPerMove( CString strResource, float fAmount );
			float GetProductionAndConsumptionPerMove( CString strResource );
			EOSAI::StringAndFloat* GetProductionAndConsumptionPerMoveObj( CString strResource );
			void  RemoveProductionAndConsumptionPerMove( CString strResource );
			CList< EOSAI::StringAndFloat* >*  GetProductionAndConsumptionPerMoveList(){ return &m_ProductionAndConsumptionPerMoveList; }

		//	float AmountOfFoodNeededThisTurn();
		//	void  SetFoodConsumption( float val ){ m_fFoodConsumption = val; }
		//	float GetFoodConsumption(){ return 0.0f; }//m_fFoodConsumption; }
		//	float AmountOfFoodNeededThisTurn(){ return m_fFoodConsumption; }
		//	void  SetOilConsumption( float val ){ m_fOilConsumption = val; }
		//	float GetOilConsumption(){ return 0.0f; }//m_fOilConsumption; }
		//
		//
			//void  ClearOnSelectSound();
			//void  OnSelectSoundFile( CString str );
			//void  PlayOnSelectSound();

			//void  OnCombatVsGroundSound( CString str );
			//void  PlayOnCombatVsGroundSound();
			//void  OnCombatVsGroundAnimation( CString str );
		//
		// Movement
		//
			float GetTerrainCost( char iTerrain ); // TerrainCost = 1 / MovementRate( iTerrain )

			float GetMovementRate( long iTerrain ){ return m_MovementDescription.GetMovementRate( iTerrain ); }
			//void  SetMovementRate( long iTerrain, float val ){ m_MovementDescription.m_fTerrainMovementRateMultiplier[ iTerrain ] = val; }

			//float GetMovementRate(){ return m_MovementDescription.GetHighestMovementRate(); }
			float GetMovementRate(){ return m_MovementDescription.GetMovementRate(); }
			void  SetMovementRate( float f ){ m_MovementDescription.m_fMovementRate = f; }

			CEOSAITerrainBasedValue*  GetTerrainBasedMovementDesc(){ ASSERT( false ); return NULL; }

			//void  SetMovementRange( float val ){ m_fMovementLimit = val; }
			//float GetMovementRange(){ return m_fMovementLimit; }
			float GetMovementRangeForAttack(); // depends on whether the unit self-destructs (Missiles/Kamikazis)
			//float GetMovementRangeAssumingIWantToReturn();
			void  SetMovementLimit( float val ){ m_fMovementLimit = val; }
			float GetMovementLimit(){ return m_fMovementLimit; }
			//float GetMaximumMovementRange(){ return m_fMaximumMovementRange; }
			//void  SetMaximumMovementRange( float val ){ m_fMaximumMovementRange = val; }

			void  LaunchesIntoSpace( bool b ){ m_bLaunchesIntoSpace = b; }
			bool  LaunchesIntoSpace(){ return m_bLaunchesIntoSpace; }
			void  SetSpaceLaunchDistance( float f ){ m_fSpaceLaunchDistance = f; }
			float GetSpaceLaunchDistance(){ return m_fSpaceLaunchDistance; }
			void  SetSpaceLifespan( float f ){ m_fSpaceLifespan = f; }
			float GetSpaceLifespan(){ return m_fSpaceLifespan; }

			//bool  HasARange(){ return m_fMovementRange > 0.0f; }
			bool  HasARange(){ return m_bHasARange; }
			void  HasARange( bool b ){ m_bHasARange = b; }
			float GetGridRange(){ return m_fMovementLimit/4.0f; }

			void  CanLandAfterLaunch( bool b ){ m_bCanLandAfterLaunch = b; }
			bool  CanLandAfterLaunch(){ return m_bCanLandAfterLaunch; }
			void  MovementCanInvolveWaypoints( bool b ){ m_bMovementCanInvolveWaypoints = b; }
			bool  MovementCanInvolveWaypoints(){ return m_bMovementCanInvolveWaypoints; }

			void  IsABallisticMissile( bool b ){ m_bIsABallisticMissile = b; }
			bool  IsABallisticMissile(){ return m_bIsABallisticMissile; }
			//CMissileMovementDefinition*  GetMissileMovementDefinition(){ return &m_MissileMovementDefinition; }
		//
		// Container
		//
			void  SetMySizeInContainer( int val ){ m_iMySizeInContainer = val; }
			long  GetMySizeInContainer(){ return m_iMySizeInContainer; }
			bool  CanContain( CEOSAIPoiObject* pPoi );
			bool  CanContain( CEOSAIUnitTemplate* pTemplate );
			bool  CanContainGroundUnits();
			long  GetNumberOfGroundUnitsThatICanContain();
			bool  CanContainAirUnits();
			bool  CanLaunchAirUnits();
			long  GetNumberOfAirUnitsThatICanContain();
			//bool  CanContainAircraft();
			//long  GetNumberOfAircraftThatICanContain();
			// Container
			bool  CanContainUnits(){ return m_iNumberOfUnitsThatICanContain > 0; }
			void  SetNumberOfUnitsThatICanContain( long val ){ m_iNumberOfUnitsThatICanContain = val; }
			long  GetNumberOfUnitsThatICanContain(){ return m_iNumberOfUnitsThatICanContain; }
			void  CanDropoffGroundUnitsAnywhere( bool b ){ m_bCanDropoffGroundUnitsAnywhere = b; }
			bool  CanDropoffGroundUnitsAnywhere(){ return m_bCanDropoffGroundUnitsAnywhere; }
			void  CanPickupGroundUnitsAnywhere( bool b ){ m_bCanPickupGroundUnitsAnywhere = b; }
			bool  CanPickupGroundUnitsAnywhere(){ return m_bCanPickupGroundUnitsAnywhere; }
			void  IsLaunchingPadForAirUnits( bool b ){ m_bCanLaunchAirUnits = b; }
			bool  IsLaunchingPadForAirUnits(){ return m_bCanLaunchAirUnits; }
			void  IsLandingPadForAirUnits( bool b ){ m_bIsLandingPadForAirUnits = b; }
			bool  IsLandingPadForAirUnits(){ return m_bIsLandingPadForAirUnits; }

		//
		// UnitTag Containment
		//
			void  AddCanContainUnitTag( CString strTag );
			void  RemoveCanContainUnitTag( CString strTag );
			bool  CanContainUnitTagExists( CString strTag );
			//CList< CUnitTag* >*  GetCanContainUnitTagList(){ return &m_CanContainUnitTagList; }
			CStringList*  GetCanContainUnitTagList(){ return &m_CanContainUnitTagList; }
		//
		//	void  SetCargoCapacity( float val ){ m_fCargoCapacity = val; }
		//	float GetCargoCapacity(){ return m_fCargoCapacity; }			
		//
		// Visibility
		//
			void  SetDefaultViewRange( float fVal ){ m_fViewRange = fVal; }
			float GetDefaultViewRange(){ return m_fViewRange; }

		//	void  InvokeViewRange( CString strUnitTag, float fViewRange );
		//	void  RemoveViewRange( CString strUnitTag );
		//	CViewRange* GetViewRangeObj( CString strUnitTag );
		//	float GetViewRange( CString strUnitTag, bool bUseUnitDefaults );
			float GetViewRange( CEOSAIPoiObject* pPoi );
			float GetViewRange( CEOSAIUnitTemplate* pUnitTemplate );
		//	CList< CViewRange* >*  GetViewRangeList(){ return &m_ViewRangeList; }

			// Radar
		//	float GetRadarRange(){ return m_fRadarRange; }
		//	void  SetRadarRange( float f ){ m_fRadarRange = f; }
			//bool  IsRadarVisible(){ return true; }
		//
		//
		// Air units on the ground cannot attack, have no view range
			bool  UnitIsDisabledWhenLandedOrDocked(){ return IsAirUnit(); }
		//
		// Combat Capabilities
		//
			bool  GetOABL(){ return this->IsAirUnit(); }

			void  CanAttack( bool bVal ){ m_bCanAttack = bVal; }
			bool  CanAttack(){ return m_bCanAttack; }
			bool  CanAttack( CEOSAIPoiObject* pTarget );

			//void  AttackMethodIsBombing( bool bVal ){ m_bAttackMethodIsBombing = bVal; }
			//bool  AttackMethodIsBombing(){ return m_bAttackMethodIsBombing; }
			//bool  AttackMethodIsBombingVs( CPoiObject* pPoiObject );
			bool  OneAttackPerLandingVs( CEOSAIPoiObject* pTarget );
			//
			bool  AutomaticallyRearmEveryTurn(){ return m_bAutomaticallyRearmEveryTurn; }
			void  AutomaticallyRearmEveryTurn( bool b ){ m_bAutomaticallyRearmEveryTurn = b; }

			bool  CanCapture( EnumAIPoiObjectType  enumPoiObjectType );
			bool  CanCapture( CEOSAIPoiObject* pTarget );
			bool  CanDegrade( CEOSAIPoiObject* pTarget );
			bool  CanDestroy( CEOSAIPoiObject* pTarget );

			void  CanCaptureCities( bool bVal ){ m_bCanCaptureCities = bVal; }
			bool  CanCaptureCities(){ return m_bCanCaptureCities; }
			bool  CanCaptureFortifications(){ return CanCaptureCities(); }

			void  CanCaptureResources( bool bVal ){ m_bCanCaptureResources = bVal; }
			bool  CanCaptureResources(){ return m_bCanCaptureResources; }

			void  CanCaptureAirfields( bool bVal ){ m_bCanCaptureAirfields = bVal; }
			bool  CanCaptureAirfields(){ return m_bCanCaptureAirfields; }
			void  CanDamageAirfields( bool bVal ){ m_bCanDamageAirfields = bVal; }
			bool  CanDamageAirfields(){ return m_bCanDamageAirfields; }
			void  CanDestroyAirfields( bool bVal ){ m_bCanDestroyAirfields = bVal; }
			bool  CanDestroyAirfields(){ return m_bCanDestroyAirfields; }

			void  SelfDestructsOnAttack( bool b ){ m_bSelfDestructsOnAttack = b; }
			bool  SelfDestructsOnAttack(){ return m_bSelfDestructsOnAttack; }

			//
			// CitResFort
			//
			virtual float GetAttackRangeOnCitResFort(){ return m_AttackVsCitResFort.m_fAttackRange; }
			CEOSAIAttackVs2*   GetAttackVsCitResFort(){ return &m_AttackVsCitResFort; }

			//
			// City Attack Values
			//
			bool    CanDegradeCities();
			bool    CanTargetCities();//{ return GetDamageToPopulation() > 0.0f || CanDestroyBuildings() || CanCaptureCities() || GetDamageToItemsInProduction(); } // return m_fDamageToPopulation > 0.0f || m_bCanDestroyBuildings; } should be able to destroy units inside, too
			//void    SetAttackVsCityAnimation( CString strAttackVsCityAnimation ){ m_strAttackVsCityAnimation = strAttackVsCityAnimation; }
			//CString GetAttackVsCityAnimation(){ return m_strAttackVsCityAnimation; }

			// Population
			bool   CanKillPopulation(){ return m_fKillPopulationToHit01 > 0.0f && m_fKillPopulationDeaths > 0.0f; }
			float  GetKillPopulationToHit01(){ return m_fKillPopulationToHit01; } // 0.0 - 1.0
			void   SetKillPopulationToHit01( float f ){ m_fKillPopulationToHit01 = f; } // 0.0 - 1.0
			float  GetKillPopulationDeaths(){ return m_fKillPopulationDeaths; } // 0.0 - infinite
			void   SetKillPopulationDeaths( float f ){ m_fKillPopulationDeaths = f; } // 0.0 - infinite
			// Refugees
			float  GetNumberOfRefugeesOnHit(); // 0.0 - infinite
			void   SetNumberOfRefugeesOnHit( float f ); // 0.0 - infinite
			// Buildings
			bool   CanDestroyBuildings(){ return m_fDestroyBuildingsToHit01 > 0.0f && m_iDestroyBuildingsDamage > 0; }
			float  GetDestroyBuildingsToHit01(){ return m_fDestroyBuildingsToHit01; } // 0.0 - 1.0
			void   SetDestroyBuildingsToHit01( float f ){ m_fDestroyBuildingsToHit01 = f; } // 0.0 - 1.0
			long   GetDestroyBuildingsDamage(){ return m_iDestroyBuildingsDamage; }
			void   SetDestroyBuildingsDamage( long i ){ m_iDestroyBuildingsDamage = i; }
			//bool   GetDestroyOneBuilding(){ return m_bDestroyOneBuilding; }
			//void   SetDestroyOneBuilding( bool b ){ m_bDestroyOneBuilding = b; }
			//bool   GetDestroyAllBuildings(){ return m_bDestroyAllBuildings; }
			//void   SetDestroyAllBuildings( bool b ){ m_bDestroyAllBuildings = b; }
			// Items in Production
			bool   CanDamageItemsInProduction(){ return m_fDamageToItemsInProductionToHit01 > 0.0f && m_iDamageToItemsInProductionDamage > 0; }
			float  GetDamageToItemsInProductionToHit01(){ return m_fDamageToItemsInProductionToHit01; } // 0.0 - 1.0
			void   SetDamageToItemsInProductionToHit01( float f ){ m_fDamageToItemsInProductionToHit01 = f; } // 0.0 - 1.0
			long   GetDamageToItemsInProductionDamage(){ return m_iDamageToItemsInProductionDamage; } // 0.0 - infinite
			void   SetDamageToItemsInProductionDamage( long i ){ m_iDamageToItemsInProductionDamage = i; } // 0.0 - infinite
			/*
			void    SetDamageToPopulation( float f ){ m_fDamageToPopulation = f; }
			float   GetDamageToPopulation(){ return m_fDamageToPopulation; }
			void  CanDestroyBuildings( bool b ){ m_bCanDestroyBuildings = b; }
			bool  CanDestroyBuildings(){ return m_bCanDestroyBuildings; }
			void  SetDamageToItemsInProduction( long iDamage ){ m_iDamageToItemsInProduction = iDamage; }
			long  GetDamageToItemsInProduction(){ return m_iDamageToItemsInProduction; }
			*/

		//	void  AddUnitTemplateCombatAnimation( CUnitTemplateCombatAnimationDesc* pAnim ){ m_UnitTemplateCombatAnimationDescs.AddTail( pAnim ); }
		//	void  DeleteUnitTemplateCombatAnimation( CUnitTemplateCombatAnimationDesc* pAnim );
		//	CList< CUnitTemplateCombatAnimationDesc* >*  GetUnitTemplateCombatAnimations(){ return &m_UnitTemplateCombatAnimationDescs; }
		//	CUnitTemplateCombatAnimationDesc* GetUnitTemplateCombatAnimation( CString strName );
		//	CUnitTemplateCombatAnimationDesc* GetUnitTemplateCombatAnimation( CPoi* pTarget );

			//CUnitTemplateCombatAnimationImage*  GetUnitCombatAnimation( CString strName );
			//void                   AddUnitCombatAnimation( CUnitTemplateCombatAnimationImage* pAnim ){ m_UnitCombatAnimations.AddTail( pAnim ); }
			//CList< CUnitTemplateCombatAnimationImage* >*  GetUnitCombatAnimations(){ return &m_UnitCombatAnimations; }

			void       AddAttackVsObject( CEOSAIAttackVs2* pAttackVs ){ m_AttackVsList.AddTail( pAttackVs ); }
			void       DeleteAttackVs( CEOSAIAttackVs2* pAttackVs );
			float      GetAttackRangeMaximum();
			//CEOSAIAttackVs2* GetAttackVs( CPoi* pPoi );
			CEOSAIAttackVs2* GetAttackVs( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked = false );
			CEOSAIAttackVs2* GetAttackVsCombatUnitType( long iCombatUnitType );
			//CEOSAIAttackVs2* GetAttackVs( CString strCombatUnitType );
			CEOSAIAttackVs2* GetAttackVs( EnumAIPoiObjectType ePoiObjectType, CEOSAIUnitTemplate* pTargetUnitTemplate, bool bIsLandedOrDocked );
			//CEOSAIAttackVs2* GetAttackVs( EnumPoiObjectType eUnitType );
			CList< CEOSAIAttackVs2* >*  GetAttackVsList(){ return &m_AttackVsList; }
			// Attack To-Hit
			float  GetAttackToHitVs( EnumAIPoiObjectType ePoiObjectType, CEOSAIUnitTemplate* pTargetUnitTemplate, bool bIsLandedOrDocked );
			//float  GetAttackToHitVs( CPoiObject* pPoiObject, Enum_Prone eDefendersProneState );
			float  GetAttackToHitVs( CEOSAIPoiObject* pPoiObject, bool bIsLandedOrDocked );
			float  GetAttackToHitVs( CEOSAIUnitTemplate* pTarget );
			//float  GetAttackToHitVsPoiObjectType( EnumPoiObjectType ePoiObjectType );
			float  GetAttackToHitVs( long iCombatUnitType );
			//float  GetAttackToHitVs( CString strCombatUnitType );
			// Attack Damage
			//long   GetAttackDamageVs( CPoiObject* pTarget, Enum_Prone eDefendersProneState );
			long   GetAttackDamageVs( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked );
			long   GetAttackDamageVs( CEOSAIUnitTemplate* pTarget );
			long   GetAttackDamageVs( long iTargetCombatUnitType );
			//long   GetAttackDamageVs( CString strTargetCombatUnitType );
			// Range
			float  GetAttackRangeVs( CEOSAIUnitTemplate* pTarget );
			//CEOSAIAttackVs2* GetAttackVsSoft(){ return GetAttackVs( "Soft" ); }
		//	CEOSAIAttackVs2* GetAttackVsArmor(){ return GetAttackVs( "Armor" ); }
	//		CEOSAIAttackVs2* GetAttackVsAircraft(){ return GetAttackVs( "Aircraft" ); }
		//	CEOSAIAttackVs2* GetAttackVsMissile(){ return GetAttackVs( "Missile" ); }
	//		CEOSAIAttackVs2* GetAttackVsShip(){ return GetAttackVs( "Ship" ); }
	//		CEOSAIAttackVs2* GetAttackVsSubmarine(){ return GetAttackVs( "Submarine" ); }

			// Defense Values
			void  SetDefenseBase( float f ){ m_fDefenseBaseValue = f; }
			float GetDefenseBase(){ return m_fDefenseBaseValue; }
			void  SetDefenseWhenLandedOrDocked( float f ){ m_fDefenseWhenLandedOrDocked = f; }
			float GetDefenseWhenLandedOrDocked(){ return m_fDefenseWhenLandedOrDocked; }
			float GetDefenseValueForNormalTerrain();//{ return m_fDefenseValue; }
			float GetDefenseValue( long iTerrainType );
			float GetCurrentDefense( long iTerrain, bool bEntrenched, bool bInFort, bool bLandedOrDocked );

			void  SetCanEntrench( bool b ){ m_bCanEntrench = b; }
			bool  GetCanEntrench(){ return m_bCanEntrench; }
			//bool  GetCanEntrench(){ return m_fDefenseEntrenchedBonus > 0.0f && m_fTimeToEntrench >= 0.0f; }
			void  SetTimeToEntrench( float fValue ){ m_fTimeToEntrench = fValue; }
			float GetTimeToEntrench(){ return m_fTimeToEntrench; }
			//void  SetDefenseEntrenchedBonus( float fValue ){ m_fDefenseEntrenchedBonus = fValue; }
			//float GetDefenseEntrenchedBonus(){ return m_fDefenseEntrenchedBonus; }
			float GetDefenseEntrenchedBonusPercent();
			float GetDefenseEntrenchedBonus();

			float GetDefenseMult( long iTerrain );
			//void  SetLandedOrDockedDefenseValue( float fValue ){ m_fLandedOrDockedDefenseValue = fValue; }
			//float GetLandedOrDockedDefenseValue(){ return m_fLandedOrDockedDefenseValue; }
			//float   GetLandedOrDockedDefenseValuePercent();
			//float   GetLandedOrDockedDefenseValue();
			CString GetLandedOrDockedCombatUnitType();

			float GetDefenseAir(){ return GetDefenseValue( TERRAIN_AIR ); }
			float GetDefenseWater(){ return GetDefenseValue( TERRAIN_WATER ); }
			float GetDefenseArctic(){ return GetDefenseValue( TERRAIN_ARCTIC ); }
			float GetDefenseGrass(){ return GetDefenseValue( TERRAIN_GRASS ); }
			float GetDefenseForest(){ return GetDefenseValue( TERRAIN_FOREST ); }
			float GetDefenseMountain(){ return GetDefenseValue( TERRAIN_MOUNTAIN ); }
			float GetDefenseDesert(){ return GetDefenseValue( TERRAIN_DESERT ); }
			float GetDefenseCity(){ return GetDefenseValue( TERRAIN_CITY ); }
			float GetDefenseFortification(){ return m_fDefenseBaseValue * m_pCombatUnitType->m_fFortificationDefenseMultiplier; }
			float GetDefenseFortificationMult(){ return m_pCombatUnitType->m_fFortificationDefenseMultiplier; }

			void  SetTracksTargetMovements( bool b ){ m_bTracksTargetMovements = b; }
			bool  GetTracksTargetMovements(){ return m_bTracksTargetMovements; }

			//bool  Missile_CanTrackTarget(){ return m_bMissile_CanTrackTarget; }
			//void  Missile_CanTrackTarget( bool b ){ m_bMissile_CanTrackTarget = b; }
			void  SetSelfDestructBlastRadius( float f ){ m_fSelfDestructBlastRadius = f; }
			float GetSelfDestructBlastRadius(){ return m_fSelfDestructBlastRadius; }

		//	void              SetSubTargeting( EnumSubTargeting eSubTargeting ){ m_eSubTargeting = eSubTargeting; }
		//	EnumSubTargeting  GetSubTargeting(){ return m_eSubTargeting; }

		//
			//void  CanBuildAirfields( bool b ){ m_bCanBuildAirfields = b; }
			//bool  CanBuildAirfields(){ return m_bCanBuildAirfields; }
			void  CanBuildAirfields( bool b ){ m_bCanBuildAirfields = b; }
			bool  CanBuildAirfields(){ return m_bCanBuildAirfields; }//m_fTimeToBuildAirfield > 0.0f; }
			void  SetTimeToBuildAirfield( float fVal ){ m_fTimeToBuildAirfield = fVal; }
			float GetTimeToBuildAirfield(){ return m_fTimeToBuildAirfield; }
		//
			//void  CanRepairAirUnits( bool b ){ m_bCanRepairAirUnits = b; }
			//bool  CanRepairAirUnits(){ return m_bCanRepairAirUnits; }
			void  SetAirUnitRepairRate( float fVal ){ m_fAirUnitRepairRate = fVal; }
			float GetAirUnitRepairRate(){ return m_fAirUnitRepairRate; }
			void  SetSeaUnitRepairRate( float fVal ){ m_fSeaUnitRepairRate = fVal; }
			float GetSeaUnitRepairRate(){ return m_fSeaUnitRepairRate; }
			void  SetGroundUnitRepairRate( float fVal ){ m_fGroundUnitRepairRate = fVal; }
			float GetGroundUnitRepairRate(){ return m_fGroundUnitRepairRate; }
		//
		// AI support functions
		//
			bool   IsOfType( EOSAIEnumTypeOfUnit eTypeOfUnit );
			//float  GetAntiUnitCombat( CString strUnitType );

			void   SetIndex( long i ){ m_iIndex = i; } // this is used to quickly lookup information owned by the AI
			long   GetIndex(){ return m_iIndex; } // this is used to quickly lookup information owned by the AI

//#ifdef THINGS_TO_COMPILE_EVENTUALLY
//			I don't think m_AIJobCapability is being calculated
//#endif
			CEOSAIJobCapability*  GetAIJobCapability();
			void   CalculateAIJobCapability( CEOSAIJobCapability* pAIJobCapability );

			//CEOSAIUnit2CombatCapability* GetAIUnitCombatCapability(){ return &m_AIUnitCombatCapability; } // Cached information
			CEOSAIUnit2CombatCapability* GetAIUnitCombatCapability();

			//float  GetStealthAntiShipWarfareCapability();
			float  GetExplorationCapability();
			float  GetSightingCapability();

			float  GetGroundCitResHunterCapacity();
			
			float  GetStrategicBombingCapability();
			float  GetTacticalBombingCapability(){ return 0.0f; }
			float  GetAirEscortCapability();
			float  GetAntiAirCapability();

			float  GetSubmarineCapability();
			float  GetAntiSubmarineCapability();
			//
			float  GetLandCitResHunterCapability();
			float  GetBombardmentCapability();
			float  GetOffensiveGroundCapability(){ return 0.0f; }
			float  GetDefensiveGroundCapability(){ return 0.0f; }
			float  GetInvasionDefense();

			float  GetSeaResourceHunterCapability();
			float  GetSeaSupremacyCapability();
			float  GetShoreBombardmentCapability();

			float  GetAirfieldBuilderCapacity();

			float  GetGroundTransportCapability();

			// AI support functions (Combat)
			//
			float GetHPDefenseValue(){ return GetMaxHP() * GetDefenseValueForNormalTerrain(); }
			void  GetUnitStrength( CString* pstrUnitType, float* pfValue );
			float GetExpectedHPDamage( CEOSAIUnit2* pTarget, bool bUseNormalTerrainIfBetterThanCurrentDefense );
			float GetExpectedHPDamage( CEOSAIUnitTemplate* pTarget );

			bool  IsACombatUnit(){ return m_bIsACombatUnit; }
			void  IsACombatUnit( bool b ){ m_bIsACombatUnit = b; }
			float GetAICombatSignificance();
			//float GetAIPowerValue();
			EOSAI::StringAndFloatSet  GetCombatPowerVsUnitTypes();
			float GetAICombatVsCity(){ return 1.0f; }  // INCOMPLETE

			// result = production + IronMult*1
			float GetProductionAndIronCost1();
			// result = production + IronMult*Iron
			float GetProductionAndIronCost( CEOSAICitResValueMultiplier* pCitResValueMultiplier );

		//
		// D# value (not recorded, but it reflect says the order of the UnitTemplates in the list)
		//
			void SetNumValue( long i ){ m_iNumValue = i; }
			long GetNumValue(){ return m_iNumValue; }

		//
		// Html 
		//
		//CString GetHtmlDesc();
		//CString RowHtml( CString strColumn1, 
		//				 CString strLink,
		//				 CString strColumn2,
		//				 long iRowNumber );

	protected:

		//
			long  m_iIndex; // this is used by the AI for quick lookups
		//
		// Name
		//
			CString    m_strExternalName;
			CString    m_strInternalName;
		//
			EOSAI::CGameRules*  m_pAIGameRules;
			//CBuildOption*  m_pBuildOption;
		//
			CEOSAIUnitsubsetPermission  m_AllowUnitsubset;
		//
		// Unit Type
		//
			// Type
			//EnumPoiObjectType  m_eCombatUnitType;
			//CString           m_strCombatUnitType;
			long                m_iCombatUnitType;
			//CCombatUnitType*  m_pCombatUnitType;
			CEOSAICombatUnitType*  m_pCombatUnitType;
			//CString           m_strMovementUnitType;
			long                m_iMovementUnitType;
		//
		// View Range
		//
			float m_fViewRange; // the distance a unit can see.
			//CList< CViewRange* >  m_ViewRangeList;

			// the viewed range is calculated
			float m_fAverageViewedRange;
			bool  m_bIsSubmarine;
		//
		// Cost, Maintenence, HP
		//
			long  m_iProductionCost;
			float m_fIronCost;

			// Maintenance
		//	float m_fFoodConsumption;
		//	float m_fOilConsumption;
			CList< EOSAI::StringAndFloat* >  m_ProductionAndConsumptionPerTurnList;
			CList< EOSAI::StringAndFloat* >  m_ProductionAndConsumptionPerMoveList;

			// Combat Number
			long  m_iMaxHP;
		//
		// Movement
		//
			// Missile Trajectory information is used only if this is a missile or fires missiles (like SAM Vehicle)
			//   Note: Right now, I don't read/record MissileMovementDefinition for units that fire
			//   missiles - they use the default values.
			// The MovementDescription duplicates most of the information inside MovementUnitType
			//   I use MovementDescription for pathfinding.  I can't use MovementUnitType because
			//   the UnitGroups can have unique movement abilities (e.g. mixed infantry/vehicle values)
			CEOSAIMovementDescription m_MovementDescription;
			//float                m_fMovementRate;
			bool                   m_bLaunchesIntoSpace;
			float                  m_fSpaceLaunchDistance;
			float                  m_fSpaceLifespan;

			bool                        m_bIsABallisticMissile;
			//CMissileMovementDefinition  m_MissileMovementDefinition;

			bool  m_bHasARange;
			float m_fMovementLimit;
			//float m_fMaximumMovementRange;

			bool  m_bCanLandAfterLaunch;
			bool  m_bMovementCanInvolveWaypoints;
		//
		// Combat
		//
			bool  m_bIsACombatUnit;
			bool  m_bCanAttack;  // some units (freighters) cannot attack

			bool  m_bAutomaticallyRearmEveryTurn;

			//bool  m_bAttackMethodIsBombing;
			bool  m_bCanCaptureCities;     // most ground units
			bool  m_bCanCaptureResources;  // most ground units, sea units, not aircraft
			bool  m_bCanCaptureAirfields;
			bool  m_bCanDamageAirfields;
			bool  m_bCanDestroyAirfields;

			bool  m_bSelfDestructsOnAttack;
			float m_fSelfDestructBlastRadius;
		//	EnumSubTargeting m_eSubTargeting;

			bool  m_bTracksTargetMovements;

			// Attack Versus City Specifics
			//
				// Population
				float  m_fKillPopulationToHit01;   // 0.0 - 1.0
				float  m_fKillPopulationDeaths;    // 0.0 - infinite
				float  m_fNumberOfRefugeesOnHit;
				// Buildings
				float  m_fDestroyBuildingsToHit01; // 0.0 - 1.0
				long   m_iDestroyBuildingsDamage;
				//bool   m_bDestroyOneBuilding;
				//bool   m_bDestroyAllBuildings;
				// Items in Production
				float  m_fDamageToItemsInProductionToHit01; // 0.0 - 1.0
				long   m_iDamageToItemsInProductionDamage;  // 0.0 - infinite
			/*
			float   m_fDamageToPopulation;
			bool    m_bCanDestroyBuildings;
			long    m_iDamageToItemsInProduction;
			*/

			CEOSAIAttackVs2           m_AttackVsCitResFort; // ignores to-hit, damage
			CList< CEOSAIAttackVs2* > m_AttackVsList;

			bool  m_bCanEntrench;
			float m_fTimeToEntrench;

			float m_fDefenseBaseValue;
			float m_fDefenseWhenLandedOrDocked;

			//float m_fRadarRange; // This is only used for drawing the "radar" circle, it doesn't affect things
			//
			bool  m_bCanBuildAirfields;
			float m_fTimeToBuildAirfield;
			//bool  m_bCanRepairAirUnits;
			float m_fAirUnitRepairRate;
			float m_fSeaUnitRepairRate;
			float m_fGroundUnitRepairRate;

		//	bool  m_bFreighterRole;
		//	float m_fCargoCapacity;

			CEOSAIUnit2CombatCapability  m_AIUnitCombatCapability;

			bool                         m_bAIJobCapabilityHasBeenSet;
			CEOSAIJobCapability          m_AIJobCapability;
		//
		//
		CString      m_strUnitImageName; // e.g. "Tank (Class 1)"
		//CString  m_strBasePath;  // e.g. "Data\\Images\\Units\\Artillery (Class 1)\\Artillery1" (the "Artillery1" part comes from the _ImageData file)
	//	CUnitTemplateImage* m_pUnitTemplateImage;

		CString      m_strUnitCategory;

		CString      m_strNotesText;

		CString      m_strUpgradeTo;
		CList< CUnitsubsetUpgradeTo2* >  m_UpgradeToList;

		//long     m_iDefaultRotation;

	//	EnumSpecialUnitSymbol  m_eSpecialUnitSymbol;

		//
		// Animation
		//
			//CList< CUnitTemplateCombatAnimationDesc* >   m_UnitTemplateCombatAnimationDescs;

		//////////////////////////////////////////////////////


		// City Prerequisite list
		CStringList    m_CityMustHaveList;

		//
		// Container
		//
			long     m_iMySizeInContainer;
			long     m_iNumberOfUnitsThatICanContain;
			//
			bool     m_bCanDropoffGroundUnitsAnywhere;
			bool     m_bCanPickupGroundUnitsAnywhere;
			bool     m_bCanLaunchAirUnits;
			bool     m_bIsLandingPadForAirUnits;
			//
			CStringList  m_CanContainUnitTagList;

		//
		// My Tags
		//
			//CStringList  m_UnitTagList;
			CList< CUnitTag* >  m_UnitTagList;
		//
			//EOSAI::StringAndLongSet  m_TargetPriorityTagSet;
		//
			long  m_iNumValue;
};




