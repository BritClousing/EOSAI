/*
#include <stdafx.h>
#include "EOSAIUnitTemplate.h"
#include "UnitTemplate.h"
#include "EOSAICitResSummary.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnitTemplate::CEOSAIUnit2Template( CUnitTemplate* pUnitTemplate )
{
	ASSERT( pUnitTemplate );
	m_pUnitTemplate = pUnitTemplate;
}
*/

#include "stdafx.h"

//#ifdef DO_NOT_COMPILE
#include "EOSAIUnit2.h"
#include "EOSAIUnitGroup.h"
#include "EOSAIUnitTemplate.h"
//#include "SoundManager.h"
//#include "BCString.h"
//#include "BCXMLReader.h"
//#include "TWDx.h"
//#include "WorldDesc.h"
//#include "TechnologyDesc.h"
//#include "BCList.h"
#include "EOSAISettings.h"

#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIFortification.h"
#include "EOSAIAirfield.h"
//#include "EOSAICitResValueMultiplier.h"
#include "EOSAICitResSummary.h"
//#include "UnitTemplateImage.h"
#include "EOSAIJobCapability.h"
//#include "UnitCategory.h"
#include "EOSAICombatUnitType.h"
#include "EOSAIUnitTag.h"
//#include "UnitTemplateB.h"
//#include "StringAndLong.h"
//#include "Checkpoint.h"
#include <Math.h>

namespace EOSAI
{
	class CCommonData;
};
extern EOSAI::CCommonData* g_pEOSAICommonData;

#include <Mmsystem.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ABCABC - Double the HP, increase double the to-hit


/*
//void  CUnitCombatAnimation::LoadFiles( CUnitset* pUnitset )
void  CUnitTemplateCombatAnimationImage::LoadFiles( CString strUnitAnimation, long iGLCenterX, long iGLCenterY )
{
	/-*
	if( m_strUnitAnimation != "" )
	{
		m_pUnitAnimationTemplate = pUnitset->GetAnimationManager()->GetAnimationTemplateByBaseFilename( strUnitAnimation );

		if( m_pUnitAnimationTemplate == NULL )
		{
			m_pUnitAnimationTemplate = new CAnimationTemplate();
			m_pUnitAnimationTemplate->SetName( m_strUnitAnimation );
			m_pUnitAnimationTemplate->SetBaseFilename( strUnitAnimation );
			m_pUnitAnimationTemplate->SetImageCenterX( iGLCenterX );
			m_pUnitAnimationTemplate->SetImageCenterY( iGLCenterY );
			m_pUnitAnimationTemplate->LoadPNGSequence();
			pUnitset->GetAnimationManager()->AddAnimationTemplate( m_pUnitAnimationTemplate );
			//
			if( m_strUnitAnimationSound != "" )
			{
				m_pUnitAnimationTemplate->SetSound( "Data\\Sound\\" + m_strUnitAnimationSound );
				m_pUnitAnimationTemplate->LoadSound();
			}
		}
	}
	*-/

	m_UnitAnimationTemplate.SetBaseFilename( strUnitAnimation );
	m_UnitAnimationTemplate.SetImageCenterX( iGLCenterX );
	m_UnitAnimationTemplate.SetImageCenterY( iGLCenterY );

	// Load the UnitAnimation, UnitAnimationSound
	if( m_strUnitAnimation != "" )
	{
		m_UnitAnimationTemplate.LoadPNGSequence();
	}
	if( m_strUnitAnimationSound != "" )
	{
		m_UnitAnimationTemplate.SetSound( "Data\\Sound\\" + m_strUnitAnimationSound );
		m_UnitAnimationTemplate.LoadSound();
	}

	// Make sure the sound is loaded
	/-*
	if( m_strUnitAnimationSound != "" &&
		g_pSoundManager->SoundExists( m_strUnitAnimationSound ) == false )
	{
		g_pSoundManager->LoadSound( m_strUnitAnimationSound );
	}
	if( m_strAnimationSoundAtTargetLocation != "" &&
		g_pSoundManager->SoundExists( m_strAnimationSoundAtTargetLocation ) == false )
	{
		g_pSoundManager->LoadSound( m_strAnimationSoundAtTargetLocation );
	}
	*-/

	// m_strIntermediateAnimation - special pre-defined values (MachineGun, Rocket, etc)
	/-*
	if( m_strIntermediateAnimation != "" )
	{
		ASSERT( CAnimationManager::IsValidIntermediateAnimation( m_strIntermediateAnimation ) );
	}
	*-/
}

void CUnitTemplateCombatAnimationImage::FreeAnimationsAndSounds()
{
	//m_UnitAnimationTemplate.Clear();
}
*/

#ifdef DO_NOT_COMPILE
void CUnitTemplateCombatAnimationDesc::Serialize( CEOSAISerial* pSerial )
{
	char cVersion = 2;
	pSerial->Serialize( cVersion );

	if( cVersion <= 1 )
	{
		pSerial->SerializeANSI8( m_strName );
	}
	if( cVersion >= 2 )
	{
		#ifdef UNICODE
		pSerial->SerializeUnicode( m_strName );
		#else
		pSerial->SerializeANSI8( m_strName );
		#endif UNICODE
	}

	long iCount = m_PauseBetweenAnimations.GetCount();
	pSerial->Serialize( iCount );
	POSITION pos = m_PauseBetweenAnimations.GetHeadPosition();
	while( pos )
	{
		float fPause = m_PauseBetweenAnimations.GetNext( pos );
		pSerial->Serialize( fPause );
	}

	if( cVersion <= 1 )
	{
		pSerial->SerializeANSI8( m_strUnitAnimation );
		pSerial->SerializeANSI8( m_strIntermediateAnimation );
		pSerial->SerializeANSI8( m_strTargetAnimation );
	}
	if( cVersion <= 2 )
	{
		#ifdef UNICODE
		pSerial->SerializeUnicode( m_strUnitAnimation );
		pSerial->SerializeUnicode( m_strIntermediateAnimation );
		pSerial->SerializeUnicode( m_strTargetAnimation );
		#else
		pSerial->SerializeANSI8( m_strUnitAnimation );
		pSerial->SerializeANSI8( m_strIntermediateAnimation );
		pSerial->SerializeANSI8( m_strTargetAnimation );
		#endif
	}
}

void CUnitTemplateCombatAnimationDesc::Deserialize( CEOSAISerial* pSerial )
{
	char cVersion = 0;
	pSerial->Deserialize( cVersion );

	//pSerial->DeserializeANSI8( m_strName );
	if( cVersion <= 1 )
	{
		pSerial->DeserializeANSI8( m_strName );
	}
	if( cVersion >= 2 )
	{
		#ifdef UNICODE
		pSerial->DeserializeUnicode( m_strName );
		#else
		pSerial->DeserializeANSI8( m_strName );
		#endif UNICODE
	}

	long iCount;
	pSerial->Deserialize( iCount );
	for( long i=0; i<iCount; i++ )
	{
		float fPause;
		pSerial->Deserialize( fPause );
		m_PauseBetweenAnimations.AddTail( fPause );
	}

	if( cVersion <= 1 )
	{
		pSerial->DeserializeANSI8( m_strUnitAnimation );
		pSerial->DeserializeANSI8( m_strIntermediateAnimation );
		pSerial->DeserializeANSI8( m_strTargetAnimation );
	}
	if( cVersion >= 2 )
	{
		#ifdef UNICODE
		pSerial->DeserializeUnicode( m_strUnitAnimation );
		pSerial->DeserializeUnicode( m_strIntermediateAnimation );
		pSerial->DeserializeUnicode( m_strTargetAnimation );
		#else
		pSerial->DeserializeANSI8( m_strUnitAnimation );
		pSerial->DeserializeANSI8( m_strIntermediateAnimation );
		pSerial->DeserializeANSI8( m_strTargetAnimation );
		#endif
	}
}

void CUnitTemplateCombatAnimationDesc::AppendDataToXMLString( CStringANSI8& strData )
{
	strData += _T("<UTCombatAnim>");

	strData += _T("<Name>") + m_strName + _T("</Name>");

	strData += _T("<AnimationPauses>");
	bool bOneItemAdded = false;
	POSITION pos = m_PauseBetweenAnimations.GetHeadPosition();
	while( pos )
	{
		float fPause = m_PauseBetweenAnimations.GetNext( pos );
		CString strText;
		strText.Format( _T("%0.2f"), fPause );
		if( bOneItemAdded ){ strData += _T(","); }
		strData += strText;
		bOneItemAdded = true;
	}
	strData += _T("</AnimationPauses>");

	if( m_strUnitAnimation != _T("") )
	{
		strData += _T("<AnimationKeyword>") + m_strUnitAnimation + _T("</AnimationKeyword>");
	}
	if( m_strIntermediateAnimation != _T("") )
	{
		strData += _T("<IntermediateKeyword>") + m_strIntermediateAnimation + _T("</IntermediateKeyword>");
	}
	if( m_strTargetAnimation != _T("") )
	{
		strData += _T("<TargetAnimation>") + m_strTargetAnimation + _T("</TargetAnimation>");
	}

	strData += _T("</UTCombatAnim>\n");
}

void  CUnitTemplateCombatAnimationDesc::ReadXMLData( CBCXMLItem* pXMLItem )
{
	m_strName = pXMLItem->GetValue( ANSI8("Name") );
	/*
	if( m_strName == "Explode" )
	{
		int g=0;
	}
	*/

	CStringANSI8 strPauses = pXMLItem->GetValue( ANSI8("AnimationPauses") );
	#ifdef UNICODE
	USES_CONVERSION;
	SetPauseBetweenAnimations( A2T(strPauses) );
	#else UNICODE
	SetPauseBetweenAnimations( strPauses );
	#endif UNICODE

	m_strUnitAnimation = pXMLItem->GetValue( ANSI8("AnimationKeyword") );
	m_strIntermediateAnimation = pXMLItem->GetValue( ANSI8("IntermediateKeyword") );
	m_strTargetAnimation = pXMLItem->GetValue( ANSI8("TargetAnimation") );
	/*
	if( m_strTargetAnimation == "AtomicBlast" )
	{
		int h=0;
	}
	*/
}

void CUnitTemplateCombatAnimationDesc::SetPauseBetweenAnimations( CString strPauses )
{
	m_PauseBetweenAnimations.RemoveAll();
	while( strPauses != _T("") )
	{
		float fPause = _tstof( strPauses );
		if( fPause > 0.0f )
		{
			m_PauseBetweenAnimations.AddTail( fPause );
		}

		long iFind = strPauses.Find( _T(",") );
		if( iFind == -1 ){ strPauses = _T(""); }
		else{ strPauses = strPauses.Right( strPauses.GetLength()-iFind-1 ); }
	}
}
/*
float  CUnitTemplateCombatAnimationDesc::GetRandomPause()
{
	if( m_PauseBetweenAnimations.GetCount() == 0 )
	{
		// Lookup the default in the CUnitTemplateCombatAnimationImage
		CUnitTemplateCombatAnimationImage* pImage = ((CUnitTemplateB*)m_pUnitTemplate)->GetUnitTemplateImage()->GetUnitCombatAnimation( m_strUnitAnimation );
		float fPause = 0.30f;
		if( pImage )
		{
			fPause = _tstof( pImage->m_strDefaultPauseBetweenAnimations );
		}

		// Randomize the value a little bit
		long iRand2 = rand()%6;
		if( iRand2 == 0 ){ fPause -= 0.04f; }
		if( iRand2 == 1 ){ fPause -= 0.02f; }
		if( iRand2 == 2 ){ fPause += 0.00f; }
		if( iRand2 == 3 ){ fPause += 0.01f; }
		if( iRand2 == 4 ){ fPause += 0.03f; }
		if( iRand2 == 5 ){ fPause += 0.05f; }
		if( fPause < 0.0f ){ fPause = 0.01f; }

		return fPause;
	}
	else
	{
		long iRand = rand()%m_PauseBetweenAnimations.GetCount();
		float fPause = m_PauseBetweenAnimations.GetAt( m_PauseBetweenAnimations.FindIndex(iRand) );

		// Randomize the value a little bit
		long iRand2 = rand()%6;
		if( iRand2 == 0 ){ fPause -= 0.04f; }
		if( iRand2 == 1 ){ fPause -= 0.02f; }
		if( iRand2 == 2 ){ fPause += 0.00f; }
		if( iRand2 == 3 ){ fPause += 0.01f; }
		if( iRand2 == 4 ){ fPause += 0.03f; }
		if( iRand2 == 5 ){ fPause += 0.05f; }
		if( fPause < 0.0f ){ fPause = 0.01f; }

		return fPause;
	}
}
*/
CString CUnitTemplateCombatAnimationDesc::GetPauseBetweenAnimationsString()
{
	CString strPauses;
	POSITION pos = m_PauseBetweenAnimations.GetHeadPosition();
	while( pos )
	{
		float fPause = m_PauseBetweenAnimations.GetNext( pos );
		CString strText;
		strText.Format( _T("%0.2f"), fPause );
		if( strPauses != _T("") ){ strPauses += _T(","); }
		strPauses += strText;
	}
	return strPauses;
}

CAnimationTemplate*  CUnitTemplateCombatAnimationDesc::GetUnitAnimationTemplate()
{
	if( m_strUnitAnimation == _T("") ){ return NULL; }

	ASSERT( m_pUnitTemplate );
	ASSERT( ((CUnitTemplateB*)m_pUnitTemplate)->GetUnitTemplateImage() );
	CUnitTemplateCombatAnimationImage* pAnim = ((CUnitTemplateB*)m_pUnitTemplate)->GetUnitTemplateImage()->GetUnitCombatAnimation( m_strUnitAnimation );
	//ASSERT( pAnim );
	if( pAnim == NULL )
	{
		if( ((CUnitTemplateB*)m_pUnitTemplate)->GetUnitTemplateImage()->GetUnitCombatAnimations()->IsEmpty() == FALSE )
		{
			CUnitTemplateCombatAnimationImage* pAnim = ((CUnitTemplateB*)m_pUnitTemplate)->GetUnitTemplateImage()->GetUnitCombatAnimations()->GetHead();
			if( pAnim ){ return pAnim->GetUnitAnimationTemplate(); }
		}
		return NULL;
	}
	return pAnim->GetUnitAnimationTemplate();
	//return m_pUnitTemplate->GetUnitTemplateImage()->GetUnitCombatAnimation( m_strUnitAnimation )->GetUnitAnimationTemplate();
	/*
	if( m_strUnitAnimation != "" )
	{
		CUnitTemplateCombatAnimationImage* pAnim = m_pUnitTemplate->GetUnitCombatAnimation( m_strUnitAnimation );
		if( pAnim )
		{
			return &pAnim->m_UnitAnimationTemplate;
			//return pAnim->m_pUnitAnimationTemplate;
		}
	}
	return NULL;
	*/
}
#endif DO_NOT_COMPILE

//

CEOSAIUnitTemplate::CEOSAIUnitTemplate( EOSAI::CGameRules* pAIGameRules )
{
	m_pAIGameRules = pAIGameRules;
	//m_AllowUnitsubset.SetUnitset( pUnitset );
	//m_AllowUnitsubset.m_bListContainsPositiveItems = false;

	m_AIUnitCombatCapability.SetMyAIUnitTemplate( this );
	m_bAIJobCapabilityHasBeenSet = false;

	//m_pBuildOption = NULL;

	m_pCombatUnitType = NULL;

//	m_eSpecialUnitSymbol = EnumSpecialUnitSymbol_None;

	// Cost
	m_iProductionCost = 0;
	m_fIronCost = 0.0f;

	// Maintenance
//	m_fFoodConsumption = 0.0f;
//	m_fOilConsumption = 0.0f;

//	m_iPrereqTechCost = 0;

	// Container
	m_iMySizeInContainer = 1;
	m_iNumberOfUnitsThatICanContain = 0;
	m_bCanDropoffGroundUnitsAnywhere = false;
	m_bCanPickupGroundUnitsAnywhere = false;
	m_bCanLaunchAirUnits = false;
	m_bIsLandingPadForAirUnits = false;

	//
	// Combat
	//
		m_iMaxHP = 1;

		m_bIsACombatUnit = true;
		m_bCanAttack = true;
		//
		m_bAutomaticallyRearmEveryTurn = false;
		m_bCanCaptureCities = false;
		m_bCanCaptureResources = false;
		m_bCanCaptureAirfields = false;
		m_bCanDamageAirfields = false;
		m_bCanDestroyAirfields = false;

		// Attack Versus City Specifics
		//
			// Population
			m_fKillPopulationToHit01 = 0.0f;   // 0.0 - 1.0
			m_fKillPopulationDeaths = 0.0f;    // 0.0 - infinite
			m_fNumberOfRefugeesOnHit = 0.0f;
			// Buildings
			m_fDestroyBuildingsToHit01 = 0.0f; // 0.0 - 1.0
			m_iDestroyBuildingsDamage = 0;
			//m_bDestroyOneBuilding = false;
			//m_bDestroyAllBuildings = false;
			// Items in Production
			m_fDamageToItemsInProductionToHit01 = 0.0f; // 0.0 - 1.0
			m_iDamageToItemsInProductionDamage = 0;  // 0.0 - infinite

		/*
		m_fDamageToPopulation = 0.0f;
		m_bCanDestroyBuildings = false;
		m_iDamageToItemsInProduction = 0;
		*/
		//m_eSubTargeting = EnumSubTargeting_Undefined;

		m_bCanEntrench = false;
		m_fTimeToEntrench = -1.0f;
		//m_fDefenseEntrenchedBonus = 0.0f; // if this is 0, the unit cannot entrench
		//m_fLandedOrDockedDefenseValue = 0.0f;

		m_fDefenseBaseValue = 0.0f;
		m_fDefenseWhenLandedOrDocked = 0.0f;
		//m_fDefenseWater = 0.0f;
		//m_fDefenseAir = 0.0f;
		//m_fDefenseArctic = 0.0f;
		//m_fDefenseGrass = 0.0f;
		//m_fDefenseForest = 0.0f;
		//m_fDefenseMountain = 0.0f;
		//m_fDefenseDesert = 0.0f;
		//m_fDefenseCity = 0.0f;
		//m_fDefenseFortification = 0.0f;
	//
		m_bSelfDestructsOnAttack = false;
		m_bLaunchesIntoSpace = false;
		m_fSpaceLaunchDistance = 0.0f;
		m_fSpaceLifespan = 0.0f;

		m_bIsABallisticMissile = false;
		m_fSelfDestructBlastRadius = 0.0f;
	//
		m_bTracksTargetMovements = true;
	//
		m_bCanBuildAirfields = false;
		m_fTimeToBuildAirfield = 0.0f;

		//m_bCanRepairAirUnits = false;
		m_fAirUnitRepairRate = 0.0f;
		m_fSeaUnitRepairRate = 0.0f;
		m_fGroundUnitRepairRate = 0.0f;

	//	m_bFreighterRole = false;
	//	m_fCargoCapacity = 0.0f;

	//
	// AI
	//
		m_iIndex = 0;

	//
	// Animation
	//
		//m_fIdleAnimationFrequency = 0.0f;

	//m_bCanBuildAirfields = false;
	//m_bMissileAttribute_AllowWaypoints = false;

	// Visibility
	m_fViewRange = 60.0f; // the distance a unit can see.
	//m_fRadarRange = 0.0f;

	m_fAverageViewedRange = 60.0f;
	m_bIsSubmarine = false;

	// Range the distance a unit can move from a particular node.
	//   A node can be a city, an aircraft carrier, an airfield, etc.
	//   Once a unit moves it's range, it crashes.  
	//   Hence, it needs to return to it's node to avoid this.
	m_bHasARange = false;
	//m_fMovementRange = 0.0f;
	m_fMovementLimit = 0.0f;
	m_iMovementUnitType = 0;
	m_iCombatUnitType = 0;

	//m_eCombatUnitType = EnumPoiObjectType::enum_Unit;

	m_bCanLandAfterLaunch = true;
	m_bMovementCanInvolveWaypoints = true;
}

CEOSAIUnitTemplate::~CEOSAIUnitTemplate()
{
	while( m_AttackVsList.IsEmpty() == FALSE ){ delete m_AttackVsList.RemoveHead(); }
	//while( m_ViewRangeList.IsEmpty() == FALSE ){ delete m_ViewRangeList.RemoveHead(); }
	while( m_CanContainUnitTagList.IsEmpty() == FALSE ){ m_CanContainUnitTagList.RemoveHead(); }
	while( m_UnitTagList.IsEmpty() == FALSE ){ delete m_UnitTagList.RemoveHead(); }

	while( m_ProductionAndConsumptionPerTurnList.IsEmpty() == FALSE ){ delete m_ProductionAndConsumptionPerTurnList.RemoveHead(); }
	while( m_ProductionAndConsumptionPerMoveList.IsEmpty() == FALSE ){ delete m_ProductionAndConsumptionPerMoveList.RemoveHead(); }

	/*
	while( !m_UnitsThatICanCarry.IsEmpty() )
	{
		delete m_UnitsThatICanCarry.RemoveHead();
	}
	*/
}

void  CEOSAIUnitTemplate::Serialize( CEOSAISerial* pSerial )
{
	long iCount;

	//short sVersion = 1;
	//#ifdef UNICODE
	//sVersion = 2;
	//#endif UNICODE
	//short sVersion = 4; // 4 = changed MovementUnitType to long
	//short sVersion = 5; // 5 = changed m_fFoodConsumption, m_fOilConsumption
	//short sVersion = 6; // 5 = added ground/sea repair
	short sVersion = 7; // 7 = added to the tags so they can be ignored when landed

	pSerial->Serialize( sVersion );

	//
	// Name
	//
		if( sVersion <= 1 )
		{
			pSerial->SerializeANSI8( m_strInternalName );
			pSerial->SerializeANSI8( m_strExternalName );
		}
		if( sVersion >= 2 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( m_strInternalName );
			pSerial->SerializeUnicode( m_strExternalName );
			#else
			ASSERT( false );
			pSerial->SerializeANSI8( m_strInternalName );
			pSerial->SerializeANSI8( m_strExternalName );
			#endif
		}
	//
	// Image and Notes
	//
		if( sVersion <= 1 )
		{
			pSerial->SerializeANSI8( m_strUnitImageName );
		}
		if( sVersion >= 2 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( m_strUnitImageName );
			#else
			ASSERT( false );
			pSerial->SerializeANSI8( m_strUnitImageName );
			#endif
		}

//		char iSpecialUnitSymbol = (char) m_eSpecialUnitSymbol;
//		pSerial->Serialize( iSpecialUnitSymbol );

		if( sVersion <= 1 )
		{
			pSerial->SerializeANSI8( m_strNotesText );
		}
		if( sVersion >= 2 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( m_strNotesText );
			#else
			ASSERT( false );
			pSerial->SerializeANSI8( m_strNotesText );
			#endif
		}
		/*
		strData += ANSI8("<ImageBase>") + CStringANSI8( m_strUnitImageName ) + ANSI8("</ImageBase>\n");
		if( m_eSpecialUnitSymbol != EnumSpecialUnitSymbol_None ){ strData += ANSI8("<SpecialSymbol>") + CBCStringANSI8::LongAsString( m_eSpecialUnitSymbol ) + ANSI8("</SpecialSymbol>\n"); }
		if( m_strNotesText != ANSI8("") ){ strData += ANSI8("<NotesText>") + CStringANSI8( m_strNotesText ) + ANSI8("</NotesText>\n"); }
		*/
	//
		if( sVersion <= 1 )
		{
			pSerial->SerializeANSI8( m_strUnitCategory );
		}
		if( sVersion >= 2 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( m_strUnitCategory );
			#else
			ASSERT( false );
			pSerial->SerializeANSI8( m_strUnitCategory );
			#endif
		}
		/*
		if( m_strUnitCategory != ANSI8("") ){ strData += ANSI8("<UnitCat>") + CStringANSI8( m_strUnitCategory ) + ANSI8("</UnitCat>\n"); }
		*/
	//
	// Unit Type
	//
		CString strCombatUnitType = GetCombatUnitTypeString();
		if( sVersion <= 1 )
		{
			CStringANSI8 strCombatUnitTypeANSI8; strCombatUnitTypeANSI8 = strCombatUnitType;
			pSerial->SerializeANSI8( strCombatUnitTypeANSI8 );
		}
		if( sVersion >= 2 && sVersion <= 3 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( strCombatUnitType );
			#else
			ASSERT( false );
			pSerial->SerializeANSI8( strCombatUnitType );
			#endif
		}
		if( sVersion >= 4 )
		{
			long iCombatUnitType = GetCombatUnitTypeLong();
			pSerial->Serialize( iCombatUnitType );
		}
		/*
		strData += ANSI8("<CombatType>");
		strData += this->GetCombatUnitTypeString();
		strData += ANSI8("</CombatType>\n");
		*/
		//
		CString strMovementUnitType = GetMovementUnitTypeString();
		if( sVersion <= 1 )
		{
			CStringANSI8 strMovementUnitTypeANSI8; strMovementUnitTypeANSI8 = strMovementUnitType;
			pSerial->SerializeANSI8( strMovementUnitTypeANSI8 );
		}
		if( sVersion >= 2 && sVersion <= 3 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( strMovementUnitType );
			#else
			ASSERT( false );
			pSerial->SerializeANSI8( strMovementUnitType );
			#endif
		}
		if( sVersion >= 4 )
		{
			ASSERT( m_iMovementUnitType > 0 );
			pSerial->Serialize( m_iMovementUnitType );
		}
		/*
		strData += ANSI8("<MovementType>") + CStringANSI8( GetMovementUnitTypeString() ) + ANSI8("</MovementType>\n");
		*/
	//
	// AllowUnitsubset information
	//
		//m_AllowUnitsubset.Serialize( pSerial );
		/*
		strData += ANSI8("<AllowUnitsubset>");
		//m_AllowUnitsubset.AppendDataToXMLString( strData );
		strData += ANSI8("</AllowUnitsubset>\n");
		*/
	//
	// View Range
	//
		/*
		pSerial->Serialize( m_fViewRange );
		iCount = m_ViewRangeList.GetCount();
		pSerial->Serialize( iCount );
		POSITION pos = m_ViewRangeList.GetHeadPosition();
		while( pos )
		{
			CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
			if( sVersion <= 1 )
			{
				pSerial->SerializeANSI8( pViewRange->m_strUnitTag );
			}
			if( sVersion >= 2 )
			{
				#ifdef UNICODE
				pSerial->SerializeUnicode( pViewRange->m_strUnitTag );
				#else
				ASSERT( false );
				pSerial->SerializeANSI8( pViewRange->m_strUnitTag );
				#endif
			}
			pSerial->Serialize( pViewRange->m_fViewRange );
		}
		*/
		/*
		strData += ANSI8("<View>");
		strData += ANSI8("<Default>") + CBCStringANSI8::FloatAsString( m_fViewRange ) + ANSI8("</Default>");
		POSITION pos = m_ViewRangeList.GetHeadPosition();
		while( pos )
		{
			strData += ANSI8("<Tag>");
			CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
			strData += ANSI8("<Name>") + CStringANSI8( pViewRange->m_strUnitTag ) + ANSI8("</Name>");
			strData += ANSI8("<View>") + CBCStringANSI8::FloatAsString( pViewRange->m_fViewRange ) + ANSI8("</View>");
			strData += ANSI8("</Tag>");
		}
		strData += ANSI8("</View>");
		*/
//		pSerial->Serialize( m_fRadarRange );
		/*
		if( m_fRadarRange > 0.0f )
		{
			strData += ANSI8("<RadarRange>") + CBCStringANSI8::FloatAsString( m_fRadarRange ) + ANSI8("</RadarRange>");
		}
		*/
	//
	// My Tags
	//
		iCount = m_UnitTagList.GetCount();
		pSerial->Serialize( iCount );
		POSITION pos = m_UnitTagList.GetHeadPosition();
		while( pos )
		{
			//CString strUnitTag = m_UnitTagList.GetNext( pos );
			CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
			if( sVersion <= 1 )
			{
				pSerial->SerializeANSI8( pUnitTag->m_strTag );
			}
			if( sVersion >= 2 && sVersion <= 6 )
			{
				#ifdef UNICODE
				pSerial->SerializeUnicode( pUnitTag->m_strTag );
				#else
				ASSERT( false );
				pSerial->SerializeANSI8( pUnitTag->m_strUnitTag );
				#endif
			}
			if( sVersion >= 7 )
			{
				pSerial->SerializeUnicode( pUnitTag->m_strTag );
				pSerial->Serialize( pUnitTag->m_bTagIsActiveOnlyWhenNotInsideAContainer );
			}
		}
		/*
		strData += ANSI8("<MyTags>");
		pos = m_UnitTagList.GetHeadPosition();
		while( pos )
		{
			CString  strUnitTag = m_UnitTagList.GetNext( pos );
			strData += ANSI8("<Tag>") + CStringANSI8( strUnitTag ) + ANSI8("</Tag>");
		}
		strData += ANSI8("</MyTags>");
		*/
	//
	// PreReqs
	//
		iCount = m_CityMustHaveList.GetCount();
		pSerial->Serialize( iCount );
		pos = m_CityMustHaveList.GetHeadPosition();
		while( pos )
		{
			CString strCityMustHave = m_CityMustHaveList.GetNext( pos );
			if( sVersion <= 1 )
			{
				pSerial->SerializeANSI8( strCityMustHave );
			}
			if( sVersion >= 2 )
			{
				#ifdef UNICODE
				pSerial->SerializeUnicode( strCityMustHave );
				#else
				ASSERT( false );
				pSerial->SerializeANSI8( strCityMustHave );
				#endif
			}
		}
		/*
		strData += ANSI8("<PreReqs>");
		pos = m_CityMustHaveList.GetHeadPosition();
		while( pos )
		{
			strData += ANSI8("<PreReq>") + CStringANSI8( m_CityMustHaveList.GetNext( pos ) ) + ANSI8("</PreReq>");
		}
		strData += ANSI8("</PreReqs>\n");
		*/
	//
	// Cost, Maintenence, HP
	//
		pSerial->Serialize( m_iProductionCost );
		pSerial->Serialize( m_fIronCost );

		//short sVersion = 5; // 5 = changed m_fFoodConsumption, m_fOilConsumption
		//pSerial->Serialize( m_fFoodConsumption );
		//pSerial->Serialize( m_fOilConsumption );
		iCount = m_ProductionAndConsumptionPerTurnList.GetCount();
		pSerial->Serialize( iCount );
		pos = m_ProductionAndConsumptionPerTurnList.GetHeadPosition();
		while( pos )
		{
			EOSAI::StringAndFloat* pStringAndFloat = m_ProductionAndConsumptionPerTurnList.GetNext( pos );
			pSerial->SerializeUnicode( pStringAndFloat->m_strValue );
			pSerial->Serialize( pStringAndFloat->m_fValue );
		}
		iCount = m_ProductionAndConsumptionPerMoveList.GetCount();
		pSerial->Serialize( iCount );
		pos = m_ProductionAndConsumptionPerMoveList.GetHeadPosition();
		while( pos )
		{
			EOSAI::StringAndFloat* pStringAndFloat = m_ProductionAndConsumptionPerMoveList.GetNext( pos );
			pSerial->SerializeUnicode( pStringAndFloat->m_strValue );
			pSerial->Serialize( pStringAndFloat->m_fValue );
		}

		pSerial->Serialize( m_iMaxHP );
		/*
		strData += ANSI8("<ProductionCost>") + CBCStringANSI8::LongAsString( m_iProductionCost ) + ANSI8("</ProductionCost>\n");
		strData += ANSI8("<IronCost>") + CBCStringANSI8::FloatAsString( ANSI8("%0.1f"), m_fIronCost ) + ANSI8("</IronCost>\n");

		strData += ANSI8("<FoodConsumption>") + CBCStringANSI8::FloatAsString( m_fFoodConsumption ) + ANSI8("</FoodConsumption>\n");
		strData += ANSI8("<OilConsumption>") + CBCStringANSI8::FloatAsString( m_fOilConsumption ) + ANSI8("</OilConsumption>\n");

		strData += ANSI8("<MaxHP>") + CBCStringANSI8::LongAsString( m_iMaxHP ) + ANSI8("</MaxHP>\n");
		*/
	//
	// Movement
	//
		pSerial->Serialize( m_MovementDescription.m_fMovementRate );
		if( this->IsAirUnit() )
		{
			pSerial->Serialize( m_fMovementLimit );
			//
			pSerial->Serialize( m_bLaunchesIntoSpace );
			pSerial->Serialize( m_fSpaceLaunchDistance );
			pSerial->Serialize( m_fSpaceLifespan );
			//
			pSerial->Serialize( m_bIsABallisticMissile );
			if( m_bIsABallisticMissile )
			{
				//m_MissileMovementDefinition.Serialize( pSerial );
				//CMissileMovementDefinition  MissileMovementDefinition;
				//MissileMovementDefinition.Serialize( pSerial );
			}
			//
			pSerial->Serialize( m_bCanLandAfterLaunch );
			pSerial->Serialize( m_bMovementCanInvolveWaypoints );
		}

		/*
		strData += ANSI8("<MovementRate>") + CBCStringANSI8::FloatAsString( m_MovementDescription.m_fMovementRate ) + ANSI8("</MovementRate>\n");
		if( this->IsAirUnit() )
		{
			strData += ANSI8("<MovementLimit>") + CBCStringANSI8::FloatAsString( m_fMovementLimit ) + ANSI8("</MovementLimit>\n");
		}
		if( this->IsAirUnit() &&
			m_bLaunchesIntoSpace )
		{
			strData += ANSI8("<LaunchesIntoSpace>") + CBCStringANSI8::BoolAsString10( m_bLaunchesIntoSpace ) + ANSI8("</LaunchesIntoSpace>\n");
			strData += ANSI8("<SpaceLaunchDistance>") + CBCStringANSI8::FloatAsString( m_fSpaceLaunchDistance ) + ANSI8("</SpaceLaunchDistance>\n");
			strData += ANSI8("<SpaceLifespan>") + CBCStringANSI8::FloatAsString( m_fSpaceLifespan ) + ANSI8("</SpaceLifespan>\n");
		}
		if( this->IsAirUnit() &&
			m_bIsABallisticMissile &&
			m_bLaunchesIntoSpace == false )
		{
			strData += ANSI8("<IsABallisticMissile>") + CBCStringANSI8::BoolAsString10( m_bIsABallisticMissile ) + ANSI8("</IsABallisticMissile>\n");
			//m_MissileMovementDefinition.AppendDataToXMLString( strData );
		}
		if( m_bIsABallisticMissile == false && m_bLaunchesIntoSpace == false )
		{
			if( m_bCanLandAfterLaunch )
			{
				strData += ANSI8("<UnitCanLandAfterLaunch>") + CBCStringANSI8::BoolAsString10( m_bCanLandAfterLaunch ) + ANSI8("</UnitCanLandAfterLaunch>\n");
			}
			if( m_bMovementCanInvolveWaypoints )
			{
				strData += ANSI8("<UnitMovementCanInvolveWaypoints>") + CBCStringANSI8::BoolAsString10( m_bMovementCanInvolveWaypoints ) + ANSI8("</UnitMovementCanInvolveWaypoints>\n");
			}
		}
		*/
	//
	// CanContain
	//
		pSerial->Serialize( m_iMySizeInContainer );
		pSerial->Serialize( m_iNumberOfUnitsThatICanContain );
		if( m_iNumberOfUnitsThatICanContain > 0 )
		{
			pSerial->Serialize( m_bCanDropoffGroundUnitsAnywhere );
			pSerial->Serialize( m_bCanPickupGroundUnitsAnywhere);
			pSerial->Serialize( m_bCanLaunchAirUnits );
			pSerial->Serialize( m_bIsLandingPadForAirUnits );

			iCount = m_CanContainUnitTagList.GetCount();
			pSerial->Serialize( iCount );
			pos = m_CanContainUnitTagList.GetHeadPosition();
			while( pos )
			{
				CString strUnitTag = m_CanContainUnitTagList.GetNext( pos );
				if( sVersion <= 1 )
				{
					pSerial->SerializeANSI8( strUnitTag );
				}
				if( sVersion >= 2 )
				{
					#ifdef UNICODE
					pSerial->SerializeUnicode( strUnitTag );
					#else
					ASSERT( false );
					pSerial->SerializeANSI8( strUnitTag );
					#endif
				}
			}
		}
		/*
		strData += ANSI8("<MySizeInContainer>") + CBCStringANSI8::LongAsString(m_iMySizeInContainer) + ANSI8("</MySizeInContainer>");
		if( m_iNumberOfUnitsThatICanContain > 0 )
		{
			strData += ANSI8("<CanContain>");
			strData += ANSI8("<NumberOfUnits>") + CBCStringANSI8::LongAsString(m_iNumberOfUnitsThatICanContain) + ANSI8("</NumberOfUnits>");
			strData += ANSI8("<DropoffGroundUnitsAnywhere>") + CBCStringANSI8::BoolAsString10(m_bCanDropoffGroundUnitsAnywhere) + ANSI8("</DropoffGroundUnitsAnywhere>");
			strData += ANSI8("<PickupGroundUnitsAnywhere>") + CBCStringANSI8::BoolAsString10(m_bCanPickupGroundUnitsAnywhere) + ANSI8("</PickupGroundUnitsAnywhere>");
			strData += ANSI8("<CanLaunchAirUnits>") + CBCStringANSI8::BoolAsString10(m_bCanLaunchAirUnits) + ANSI8("</CanLaunchAirUnits>");
			strData += ANSI8("<IsLandingPadForAirUnits>") + CBCStringANSI8::BoolAsString10(m_bIsLandingPadForAirUnits) + ANSI8("</IsLandingPadForAirUnits>");
			strData += ANSI8("<Tags>");
			pos = m_CanContainUnitTagList.GetHeadPosition();
			while( pos )
			{
				CString strUnitTag = m_CanContainUnitTagList.GetNext( pos );
				strData += ANSI8("<Tag>") + CStringANSI8( strUnitTag ) + ANSI8("</Tag>");
			}
			strData += ANSI8("</Tags>");
			strData += ANSI8("</CanContain>");
		}
		*/
	//
	// Combat
	//
		pSerial->Serialize( m_bIsACombatUnit );
		pSerial->Serialize( m_bCanAttack );
		pSerial->Serialize( m_bAutomaticallyRearmEveryTurn );
		//
		pSerial->Serialize( m_bCanCaptureCities );
		pSerial->Serialize( m_bCanCaptureResources );
		//
		pSerial->Serialize( m_bCanCaptureAirfields );
		pSerial->Serialize( m_bCanDamageAirfields );
		pSerial->Serialize( m_bCanDestroyAirfields );
		//
		pSerial->Serialize( m_bSelfDestructsOnAttack );
		pSerial->Serialize( m_fSelfDestructBlastRadius );
		//
	//	char iSubTargeting = (char) m_eSubTargeting;
	//	pSerial->Serialize( iSubTargeting );
		//
		pSerial->Serialize( m_bTracksTargetMovements );

		/*
		strData += ANSI8("<CombatUnit>") + CBCStringANSI8::BoolAsString10( m_bIsACombatUnit ) + ANSI8("</CombatUnit>\n");
		if( m_bCanAttack ){ strData += ANSI8("<CanAttack>") + CBCStringANSI8::BoolAsString10( m_bCanAttack ) + ANSI8("</CanAttack>\n"); }
		//
		if( m_bAutomaticallyRearmEveryTurn ){ strData += ANSI8("<AutomaticallyRearmEveryTurn>") + CBCStringANSI8::BoolAsString10( m_bAutomaticallyRearmEveryTurn ) + ANSI8("</AutomaticallyRearmEveryTurn>\n"); }
		//
		if( m_bCanCaptureCities    ){ strData += ANSI8("<CanCaptureCities>") + CBCStringANSI8::BoolAsString10( m_bCanCaptureCities ) + ANSI8("</CanCaptureCities>\n"); }
		if( m_bCanCaptureResources ){ strData += ANSI8("<CanCaptureResources>") + CBCStringANSI8::BoolAsString10( m_bCanCaptureResources ) + ANSI8("</CanCaptureResources>\n"); }

		if( m_bCanCaptureAirfields ){ strData += ANSI8("<CanCaptureAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanCaptureAirfields ) + ANSI8("</CanCaptureAirfields>\n"); }
		if( m_bCanDamageAirfields  ){ strData += ANSI8("<CanDamageAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanDamageAirfields ) + ANSI8("</CanDamageAirfields>\n"); }
		if( m_bCanDestroyAirfields ){ strData += ANSI8("<CanDestroyAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanDestroyAirfields ) + ANSI8("</CanDestroyAirfields>\n"); }
		//
		if( m_bSelfDestructsOnAttack )
		{
			strData += ANSI8("<SelfDestructsOnAttack>") + CBCStringANSI8::BoolAsString10( m_bSelfDestructsOnAttack ) + ANSI8("</SelfDestructsOnAttack>\n");
			strData += ANSI8("<SelfDestructBlastRadius>") + CBCStringANSI8::FloatAsString( m_fSelfDestructBlastRadius ) + ANSI8("</SelfDestructBlastRadius>\n");
		}
		strData += ANSI8("<SubTargeting>") + CBCStringANSI8::LongAsString( m_eSubTargeting ) + ANSI8("</SubTargeting>\n");
		//
		if( m_bTracksTargetMovements == false )
		{
			strData += ANSI8("<TracksTarget>0</TracksTarget>\n");
		}
		*/
		//

		// Attack Versus City Specifics
		//
		// Population
		pSerial->Serialize( m_fKillPopulationToHit01 );
		pSerial->Serialize( m_fKillPopulationDeaths );
		//
		if( sVersion >= 3 )
		{
			pSerial->Serialize( m_fNumberOfRefugeesOnHit );
		}
		#ifdef _DEBUG
		if( sVersion <= 2 )
		{
			m_fNumberOfRefugeesOnHit = m_fKillPopulationDeaths*5.0f;
			m_fKillPopulationDeaths *= 0.3f;
		}
		#endif
		// Buildings
		pSerial->Serialize( m_fDestroyBuildingsToHit01 );
		pSerial->Serialize( m_iDestroyBuildingsDamage );
		// Items in Production
		pSerial->Serialize( m_fDamageToItemsInProductionToHit01 );
		pSerial->Serialize( m_iDamageToItemsInProductionDamage );
		/*
		// Population
		if( m_fKillPopulationToHit01 > 0.0f ){ strData += ANSI8("<PopulationToHit>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fKillPopulationToHit01 ) + ANSI8("</PopulationToHit>\n"); }
		if( m_fKillPopulationDeaths  > 0.0f ){ strData += ANSI8("<PopulationDeaths>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fKillPopulationDeaths ) + ANSI8("</PopulationDeaths>\n"); }
		// Buildings
		if( m_fDestroyBuildingsToHit01 > 0.0f ){ strData += ANSI8("<BuildingsToHit>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fDestroyBuildingsToHit01 ) + ANSI8("</BuildingsToHit>\n"); }
		if( m_iDestroyBuildingsDamage > 0 ){ strData += ANSI8("<DestroyBuildingsDamage>") + CBCStringANSI8::LongAsString( m_iDestroyBuildingsDamage ) + ANSI8("</DestroyBuildingsDamage>\n"); }
		// Items in Production
		if( m_fDamageToItemsInProductionToHit01 > 0.0f ){ strData += ANSI8("<DamageToItemsInProductionToHit01>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fDamageToItemsInProductionToHit01 ) + ANSI8("</DamageToItemsInProductionToHit01>\n"); }
		if( m_iDamageToItemsInProductionDamage > 0 ){ strData += ANSI8("<DamageToItemsInProduction>") + CBCStringANSI8::LongAsString( m_iDamageToItemsInProductionDamage ) + ANSI8("</DamageToItemsInProduction>\n"); }
		*/

		//
		/*
		iCount = m_UnitTemplateCombatAnimationDescs.GetCount();
		pSerial->Serialize( iCount );
		pos = m_UnitTemplateCombatAnimationDescs.GetHeadPosition();
		while( pos )
		{
			CUnitTemplateCombatAnimationDesc* pAnim = m_UnitTemplateCombatAnimationDescs.GetNext( pos );
			pAnim->Serialize( pSerial );
		}
		*/
		/*
		pos = m_UnitTemplateCombatAnimationDescs.GetHeadPosition();
		while( pos )
		{
			CUnitTemplateCombatAnimationDesc* pAnim = m_UnitTemplateCombatAnimationDescs.GetNext( pos );
			//pAnim->AppendDataToXMLString( strData );
		}
		*/
		//

		m_AttackVsCitResFort.Serialize( pSerial );
		/*
		strData += ANSI8("<CitResFort>");
		//m_AttackVsCitResFort.AppendDataToXMLString( strData );
		strData += ANSI8("</CitResFort>\n");
		*/
	//
	// Attack
	//
		iCount = m_AttackVsList.GetCount();
		pSerial->Serialize( iCount );
		pos = m_AttackVsList.GetHeadPosition();
		while( pos )
		{
			CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
			pAttackVs->Serialize( pSerial );
		}
		/*
		pos = m_AttackVsList.GetHeadPosition();
		while( pos )
		{
			CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
			//pAttackVs->AppendDataToXMLString( strData );
		}
		strData += ANSI8("\n");
		*/
	//
	// Entrenched, Docked, Landed Defense
	//
		pSerial->Serialize( m_bCanEntrench );
		if( m_bCanEntrench )
		{
			pSerial->Serialize( m_fTimeToEntrench );
		}
		/*
		if( m_bCanEntrench )
		{
			strData += ANSI8("<CanEntrench>") + CBCStringANSI8::BoolAsString10( m_bCanEntrench ) + ANSI8("</CanEntrench>\n");
			strData += ANSI8("<TimeToEntrench>") + CBCStringANSI8::FloatAsString( m_fTimeToEntrench ) + ANSI8("</TimeToEntrench>\n");
		}
		*/
	//
	// Defense
	//
		pSerial->Serialize( m_fDefenseBaseValue );
		pSerial->Serialize( m_fDefenseWhenLandedOrDocked );
		/*
		strData += ANSI8("<DefenseBase>") + CBCStringANSI8::FloatAsString( m_fDefenseBaseValue ) + ANSI8("</DefenseBase>\n");
		strData += ANSI8("<DefenseLOD>") + CBCStringANSI8::FloatAsString( m_fDefenseWhenLandedOrDocked ) + ANSI8("</DefenseLOD>\n");
		*/

		pSerial->Serialize( m_bCanBuildAirfields );
		if( m_bCanBuildAirfields )
		{
			pSerial->Serialize( m_fTimeToBuildAirfield );
		}
	/*
	if( m_bCanBuildAirfields )
	{
		strData += ANSI8("<CanBuildAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanBuildAirfields ) + ANSI8("</CanBuildAirfields>\n");
		strData += ANSI8("<TimeToBuildAirfield>") + CBCStringANSI8::FloatAsString( m_fTimeToBuildAirfield ) + ANSI8("</TimeToBuildAirfield>\n");
	}
	*/
		pSerial->Serialize( m_fAirUnitRepairRate );
		pSerial->Serialize( m_fSeaUnitRepairRate ); // Version 6
		pSerial->Serialize( m_fGroundUnitRepairRate ); // Version 6
	/*
	if( m_fAirUnitRepairRate > 0.0f )//m_bCanRepairAirUnits )
	{
		strData += ANSI8("<AirUnitRepairRate>") + CBCStringANSI8::FloatAsString( m_fAirUnitRepairRate ) + ANSI8("</AirUnitRepairRate>\n");
	}
	*/
//		pSerial->Serialize( m_fCargoCapacity );
	/*
	if( m_fCargoCapacity > 0.0f )
	{
		strData += ANSI8("<CargoCapacity>") + CBCStringANSI8::FloatAsString( m_fCargoCapacity ) + ANSI8("</CargoCapacity>\n");
	}
	*/
//		pSerial->Serialize( m_bFreighterRole );
	/*
	if( m_bFreighterRole )
	{
		strData += ANSI8("<Freighter>1</Freighter>\n");
	}
	*/
	//
		if( sVersion <= 1 )
		{
			pSerial->SerializeANSI8( m_strUpgradeTo );
		}
		if( sVersion >= 2 )
		{
			#ifdef UNICODE
			pSerial->SerializeUnicode( m_strUpgradeTo );
			#else
			pSerial->SerializeANSI8( m_strUpgrade );
			#endif
		}

		iCount = m_UpgradeToList.GetCount();
		pSerial->Serialize( iCount );
		pos = m_UpgradeToList.GetHeadPosition();
		while( pos )
		{
			CUnitsubsetUpgradeTo2* pObj = m_UpgradeToList.GetNext( pos );
			if( sVersion <= 1 )
			{
				pSerial->SerializeANSI8( pObj->m_strUpgradeTo );
				pSerial->SerializeANSI8( pObj->m_strUnitsubset );
			}
			if( sVersion >= 2 )
			{
				#ifdef UNICODE
				pSerial->SerializeUnicode( pObj->m_strUpgradeTo );
				pSerial->SerializeUnicode( pObj->m_strUnitsubset );
				#else
				pSerial->SerializeANSI8( pObj->m_strUpgrade );
				pSerial->SerializeANSI8( pObj->m_strUnitsubset );
				#endif
			}
		}

	/*
	if( m_strUpgrade != ANSI8("") )
	{
		strData += ANSI8("<Upgrade>\n");
		strData += ANSI8("<To>") + CStringANSI8( m_strUpgrade ) + ANSI8("</To>\n");
		//
		POSITION pos = m_UpgradeList.GetHeadPosition();
		while( pos )
		{
			CUnitsubsetUpgrade* pObj = m_UpgradeList.GetNext( pos );
			strData += ANSI8("<UnitsubsetUpgrade>\n");
			strData += ANSI8("<To>") + CStringANSI8( pObj->m_strUpgrade ) + ANSI8("</To>\n");
			strData += ANSI8("<Unitsubset>") + CStringANSI8( pObj->m_strUnitsubset ) + ANSI8("</Unitsubset>\n");
			strData += ANSI8("</UnitsubsetUpgrade>\n");
		}
		strData += ANSI8("</Upgrade>\n");
	}
	*/
	//SetImageDirectory( GetImageBase() );
}

#ifdef DO_NOT_COMPILE
void  CEOSAIUnit2Template::AppendDataToXMLString( CStringANSI8& strData )
{
	ASSERT( false ); // Depreciate?

	strData += ANSI8("<Unit>");
	//
	// Name
	//
		strData += ANSI8("<InternalName>") + CStringANSI8( m_strInternalName ) + ANSI8("</InternalName>\n");
		strData += ANSI8("<ExternalName>") + CStringANSI8( m_strExternalName ) + ANSI8("</ExternalName>\n");
	//
	// Image and Notes
	//
		strData += ANSI8("<ImageBase>") + CStringANSI8( m_strUnitImageName ) + ANSI8("</ImageBase>\n");
		if( m_eSpecialUnitSymbol != EnumSpecialUnitSymbol_None ){ strData += ANSI8("<SpecialSymbol>") + CBCStringANSI8::LongAsString( m_eSpecialUnitSymbol ) + ANSI8("</SpecialSymbol>\n"); }
		if( m_strNotesText != ANSI8("") ){ strData += ANSI8("<NotesText>") + CStringANSI8( m_strNotesText ) + ANSI8("</NotesText>\n"); }
	//
		if( m_strUnitCategory != ANSI8("") ){ strData += ANSI8("<UnitCat>") + CStringANSI8( m_strUnitCategory ) + ANSI8("</UnitCat>\n"); }
	//
	// Unit Type
	//
		strData += ANSI8("<CombatType>");
		strData += this->GetCombatUnitTypeString();
		strData += ANSI8("</CombatType>\n");
		//
		strData += ANSI8("<MovementType>") + CStringANSI8( GetMovementUnitTypeString() ) + ANSI8("</MovementType>\n");
	//
	// AllowUnitsubset information
	//
		strData += ANSI8("<AllowUnitsubset>");
		m_AllowUnitsubset.AppendDataToXMLString( strData );
		strData += ANSI8("</AllowUnitsubset>\n");
	//
	// View Range
	//
		strData += ANSI8("<View>");
		strData += ANSI8("<Default>") + CBCStringANSI8::FloatAsString( m_fViewRange ) + ANSI8("</Default>");
		POSITION pos = m_ViewRangeList.GetHeadPosition();
		while( pos )
		{
			strData += ANSI8("<Tag>");
			CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
			strData += ANSI8("<Name>") + CStringANSI8( pViewRange->m_strUnitTag ) + ANSI8("</Name>");
			strData += ANSI8("<View>") + CBCStringANSI8::FloatAsString( pViewRange->m_fViewRange ) + ANSI8("</View>");
			strData += ANSI8("</Tag>");
		}
		strData += ANSI8("</View>");

		if( m_fRadarRange > 0.0f )
		{
			strData += ANSI8("<RadarRange>") + CBCStringANSI8::FloatAsString( m_fRadarRange ) + ANSI8("</RadarRange>");
		}
	//
	// My Tags
	//
		strData += ANSI8("<MyTags>");
		pos = m_UnitTagList.GetHeadPosition();
		while( pos )
		{
			//CString  strUnitTag = m_UnitTagList.GetNext( pos );
			//strData += ANSI8("<Tag>") + CStringANSI8( strUnitTag ) + ANSI8("</Tag>");
			CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
			strData += ANSI8("<Tag>") + CStringANSI8( pUnitTag->m_strTag ) + ANSI8("</Tag>");
		}
		strData += ANSI8("</MyTags>");
	//
	// PreReqs
	//
		strData += ANSI8("<PreReqs>");
		pos = m_CityMustHaveList.GetHeadPosition();
		while( pos )
		{
			strData += ANSI8("<PreReq>") + CStringANSI8( m_CityMustHaveList.GetNext( pos ) ) + ANSI8("</PreReq>");
		}
		strData += ANSI8("</PreReqs>\n");
	//
	// Cost, Maintenence, HP
	//
		strData += ANSI8("<ProductionCost>") + CBCStringANSI8::LongAsString( m_iProductionCost ) + ANSI8("</ProductionCost>\n");
		strData += ANSI8("<IronCost>") + CBCStringANSI8::FloatAsString( ANSI8("%0.1f"), m_fIronCost ) + ANSI8("</IronCost>\n");

		float fFoodConsumption = 0.0f;
		strData += ANSI8("<FoodConsumption>") + CBCStringANSI8::FloatAsString( fFoodConsumption ) + ANSI8("</FoodConsumption>\n");
		InvokeProductionAndConsumptionPerTurn( _T("Food"), fFoodConsumption );
		float fOilConsumption = 0.0f;
		strData += ANSI8("<OilConsumption>") + CBCStringANSI8::FloatAsString( fOilConsumption ) + ANSI8("</OilConsumption>\n");
		InvokeProductionAndConsumptionPerMove( _T("Oil"), fOilConsumption );

		strData += ANSI8("<MaxHP>") + CBCStringANSI8::LongAsString( m_iMaxHP ) + ANSI8("</MaxHP>\n");
	//
	// Movement
	//
		strData += ANSI8("<MovementRate>") + CBCStringANSI8::FloatAsString( m_MovementDescription.m_fMovementRate ) + ANSI8("</MovementRate>\n");
		if( this->IsAirUnit() )
		{
			strData += ANSI8("<MovementLimit>") + CBCStringANSI8::FloatAsString( m_fMovementLimit ) + ANSI8("</MovementLimit>\n");
		}
		if( this->IsAirUnit() &&
			m_bLaunchesIntoSpace )
		{
			strData += ANSI8("<LaunchesIntoSpace>") + CBCStringANSI8::BoolAsString10( m_bLaunchesIntoSpace ) + ANSI8("</LaunchesIntoSpace>\n");
			strData += ANSI8("<SpaceLaunchDistance>") + CBCStringANSI8::FloatAsString( m_fSpaceLaunchDistance ) + ANSI8("</SpaceLaunchDistance>\n");
			strData += ANSI8("<SpaceLifespan>") + CBCStringANSI8::FloatAsString( m_fSpaceLifespan ) + ANSI8("</SpaceLifespan>\n");
		}
		if( this->IsAirUnit() &&
			m_bIsABallisticMissile &&
			m_bLaunchesIntoSpace == false )
		{
			strData += ANSI8("<IsABallisticMissile>") + CBCStringANSI8::BoolAsString10( m_bIsABallisticMissile ) + ANSI8("</IsABallisticMissile>\n");
			//m_MissileMovementDefinition.AppendDataToXMLString( strData );
			CMissileMovementDefinition  MissileMovementDefinition;
			MissileMovementDefinition.AppendDataToXMLString( strData );
		}
		if( m_bIsABallisticMissile == false && m_bLaunchesIntoSpace == false )
		{
			if( m_bCanLandAfterLaunch )
			{
				strData += ANSI8("<UnitCanLandAfterLaunch>") + CBCStringANSI8::BoolAsString10( m_bCanLandAfterLaunch ) + ANSI8("</UnitCanLandAfterLaunch>\n");
			}
			if( m_bMovementCanInvolveWaypoints )
			{
				strData += ANSI8("<UnitMovementCanInvolveWaypoints>") + CBCStringANSI8::BoolAsString10( m_bMovementCanInvolveWaypoints ) + ANSI8("</UnitMovementCanInvolveWaypoints>\n");
			}
		}
	//
	// CanContain
	//
		strData += ANSI8("<MySizeInContainer>") + CBCStringANSI8::LongAsString(m_iMySizeInContainer) + ANSI8("</MySizeInContainer>");
		if( m_iNumberOfUnitsThatICanContain > 0 )
		{
			strData += ANSI8("<CanContain>");
			strData += ANSI8("<NumberOfUnits>") + CBCStringANSI8::LongAsString(m_iNumberOfUnitsThatICanContain) + ANSI8("</NumberOfUnits>");
			strData += ANSI8("<DropoffGroundUnitsAnywhere>") + CBCStringANSI8::BoolAsString10(m_bCanDropoffGroundUnitsAnywhere) + ANSI8("</DropoffGroundUnitsAnywhere>");
			strData += ANSI8("<PickupGroundUnitsAnywhere>") + CBCStringANSI8::BoolAsString10(m_bCanPickupGroundUnitsAnywhere) + ANSI8("</PickupGroundUnitsAnywhere>");
			strData += ANSI8("<CanLaunchAirUnits>") + CBCStringANSI8::BoolAsString10(m_bCanLaunchAirUnits) + ANSI8("</CanLaunchAirUnits>");
			strData += ANSI8("<IsLandingPadForAirUnits>") + CBCStringANSI8::BoolAsString10(m_bIsLandingPadForAirUnits) + ANSI8("</IsLandingPadForAirUnits>");
			strData += ANSI8("<Tags>");
			pos = m_CanContainUnitTagList.GetHeadPosition();
			while( pos )
			{
				CString strUnitTag = m_CanContainUnitTagList.GetNext( pos );
				strData += ANSI8("<Tag>") + CStringANSI8( strUnitTag ) + ANSI8("</Tag>");
			}
			strData += ANSI8("</Tags>");
			strData += ANSI8("</CanContain>");
		}
	//
	// Combat
	//
		strData += ANSI8("<CombatUnit>") + CBCStringANSI8::BoolAsString10( m_bIsACombatUnit ) + ANSI8("</CombatUnit>\n");
		if( m_bCanAttack ){ strData += ANSI8("<CanAttack>") + CBCStringANSI8::BoolAsString10( m_bCanAttack ) + ANSI8("</CanAttack>\n"); }
		//
		if( m_bAutomaticallyRearmEveryTurn ){ strData += ANSI8("<AutomaticallyRearmEveryTurn>") + CBCStringANSI8::BoolAsString10( m_bAutomaticallyRearmEveryTurn ) + ANSI8("</AutomaticallyRearmEveryTurn>\n"); }
		//
		if( m_bCanCaptureCities    ){ strData += ANSI8("<CanCaptureCities>") + CBCStringANSI8::BoolAsString10( m_bCanCaptureCities ) + ANSI8("</CanCaptureCities>\n"); }
		if( m_bCanCaptureResources ){ strData += ANSI8("<CanCaptureResources>") + CBCStringANSI8::BoolAsString10( m_bCanCaptureResources ) + ANSI8("</CanCaptureResources>\n"); }

		if( m_bCanCaptureAirfields ){ strData += ANSI8("<CanCaptureAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanCaptureAirfields ) + ANSI8("</CanCaptureAirfields>\n"); }
		if( m_bCanDamageAirfields  ){ strData += ANSI8("<CanDamageAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanDamageAirfields ) + ANSI8("</CanDamageAirfields>\n"); }
		if( m_bCanDestroyAirfields ){ strData += ANSI8("<CanDestroyAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanDestroyAirfields ) + ANSI8("</CanDestroyAirfields>\n"); }
		//
		if( m_bSelfDestructsOnAttack )
		{
			strData += ANSI8("<SelfDestructsOnAttack>") + CBCStringANSI8::BoolAsString10( m_bSelfDestructsOnAttack ) + ANSI8("</SelfDestructsOnAttack>\n");
			strData += ANSI8("<SelfDestructBlastRadius>") + CBCStringANSI8::FloatAsString( m_fSelfDestructBlastRadius ) + ANSI8("</SelfDestructBlastRadius>\n");
		}
		strData += ANSI8("<SubTargeting>") + CBCStringANSI8::LongAsString( m_eSubTargeting ) + ANSI8("</SubTargeting>\n");
		//
		if( m_bTracksTargetMovements == false )
		{
			strData += ANSI8("<TracksTarget>0</TracksTarget>\n");
		}
		//

		// Attack Versus City Specifics
		//
		// Population
		if( m_fKillPopulationToHit01 > 0.0f ){ strData += ANSI8("<PopulationToHit>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fKillPopulationToHit01 ) + ANSI8("</PopulationToHit>\n"); }
		if( m_fKillPopulationDeaths  > 0.0f ){ strData += ANSI8("<PopulationDeaths>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fKillPopulationDeaths ) + ANSI8("</PopulationDeaths>\n"); }
		// Buildings
		if( m_fDestroyBuildingsToHit01 > 0.0f ){ strData += ANSI8("<BuildingsToHit>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fDestroyBuildingsToHit01 ) + ANSI8("</BuildingsToHit>\n"); }
		if( m_iDestroyBuildingsDamage > 0 ){ strData += ANSI8("<DestroyBuildingsDamage>") + CBCStringANSI8::LongAsString( m_iDestroyBuildingsDamage ) + ANSI8("</DestroyBuildingsDamage>\n"); }
		// Items in Production
		if( m_fDamageToItemsInProductionToHit01 > 0.0f ){ strData += ANSI8("<DamageToItemsInProductionToHit01>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fDamageToItemsInProductionToHit01 ) + ANSI8("</DamageToItemsInProductionToHit01>\n"); }
		if( m_iDamageToItemsInProductionDamage > 0 ){ strData += ANSI8("<DamageToItemsInProduction>") + CBCStringANSI8::LongAsString( m_iDamageToItemsInProductionDamage ) + ANSI8("</DamageToItemsInProduction>\n"); }

		//
		pos = m_UnitTemplateCombatAnimationDescs.GetHeadPosition();
		while( pos )
		{
			CUnitTemplateCombatAnimationDesc* pAnim = m_UnitTemplateCombatAnimationDescs.GetNext( pos );
			pAnim->AppendDataToXMLString( strData );
		}
		//

		strData += ANSI8("<CitResFort>");
		m_AttackVsCitResFort.AppendDataToXMLString( strData );
		strData += ANSI8("</CitResFort>\n");
	//
	// Attack
	//
		pos = m_AttackVsList.GetHeadPosition();
		while( pos )
		{
			CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
			pAttackVs->AppendDataToXMLString( strData );
		}
		strData += ANSI8("\n");
	//
	// Entrenched, Docked, Landed Defense
	//
		if( m_bCanEntrench )
		{
			strData += ANSI8("<CanEntrench>") + CBCStringANSI8::BoolAsString10( m_bCanEntrench ) + ANSI8("</CanEntrench>\n");
			strData += ANSI8("<TimeToEntrench>") + CBCStringANSI8::FloatAsString( m_fTimeToEntrench ) + ANSI8("</TimeToEntrench>\n");
		}
	//
	// Defense
	//
		strData += ANSI8("<DefenseBase>") + CBCStringANSI8::FloatAsString( m_fDefenseBaseValue ) + ANSI8("</DefenseBase>\n");
		strData += ANSI8("<DefenseLOD>") + CBCStringANSI8::FloatAsString( m_fDefenseWhenLandedOrDocked ) + ANSI8("</DefenseLOD>\n");

	if( m_bCanBuildAirfields )
	{
		strData += ANSI8("<CanBuildAirfields>") + CBCStringANSI8::BoolAsString10( m_bCanBuildAirfields ) + ANSI8("</CanBuildAirfields>\n");
		strData += ANSI8("<TimeToBuildAirfield>") + CBCStringANSI8::FloatAsString( m_fTimeToBuildAirfield ) + ANSI8("</TimeToBuildAirfield>\n");
	}
	if( m_fAirUnitRepairRate > 0.0f )//m_bCanRepairAirUnits )
	{
		strData += ANSI8("<AirUnitRepairRate>") + CBCStringANSI8::FloatAsString( m_fAirUnitRepairRate ) + ANSI8("</AirUnitRepairRate>\n");
	}
	if( m_fCargoCapacity > 0.0f )
	{
		strData += ANSI8("<CargoCapacity>") + CBCStringANSI8::FloatAsString( m_fCargoCapacity ) + ANSI8("</CargoCapacity>\n");
	}
	if( m_bFreighterRole )
	{
		strData += ANSI8("<Freighter>1</Freighter>\n");
	}
	//
	if( m_strUpgrade != ANSI8("") )
	{
		strData += ANSI8("<Upgrade>\n");
		strData += ANSI8("<To>") + CStringANSI8( m_strUpgrade ) + ANSI8("</To>\n");
		//
		POSITION pos = m_UpgradeList.GetHeadPosition();
		while( pos )
		{
			CUnitsubsetUpgrade2* pObj = m_UpgradeList.GetNext( pos );
			strData += ANSI8("<UnitsubsetUpgrade>\n");
			strData += ANSI8("<To>") + CStringANSI8( pObj->m_strUpgrade ) + ANSI8("</To>\n");
			strData += ANSI8("<Unitsubset>") + CStringANSI8( pObj->m_strUnitsubset ) + ANSI8("</Unitsubset>\n");
			strData += ANSI8("</UnitsubsetUpgrade>\n");
		}
		strData += ANSI8("</Upgrade>\n");
	}

	strData += ANSI8("</Unit>\n");
}
#endif DO_NOT_COMPILE


/*
void CEOSAIUnit2Template::LoadMovementAnimation()
{
	if( m_strMovementAnimationBaseFilename != "" )
	{
		m_MovementAnimation.LoadPNGSequence( m_strMovementAnimationBaseFilename, m_iGLCenterX, m_iGLCenterY );
	}
}
*/
#include <sys/types.h>
#include <sys/stat.h>
/*
CAnimationTemplate* CEOSAIUnit2Template::GetIdleAnimation()
{
	return m_pUnitTemplateImage->GetIdleAnimation();
}

CString  CEOSAIUnit2Template::GetMovementAnimationPath()
{
	return m_pUnitTemplateImage->GetMovementAnimationPath();
}

CAnimationTemplate* CEOSAIUnit2Template::GetMovementAnimation()
{
	return m_pUnitTemplateImage->GetMovementAnimation();
}

CAnimationTemplate* CEOSAIUnit2Template::GetDieAnimation()
{
	return m_pUnitTemplateImage->GetDieAnimation();
}

CString CEOSAIUnit2Template::GetStaticCombatImageString()
{
	return m_pUnitTemplateImage->GetStaticCombatImageString();
}


CGLTexture* CEOSAIUnit2Template::GetStaticCombatImage( long iRotation )
{
	return m_pUnitTemplateImage->GetStaticCombatImage( iRotation );
	/-*
	if( iRotation == -1 )
	{
		return m_StaticCombatImage[ m_iDefaultRotation ].GetImage();
	}
	return m_StaticCombatImage[iRotation].GetImage();
	*-/
}

long  CEOSAIUnit2Template::GetNumberOfImages()
{
	return m_pUnitTemplateImage->GetNumberOfImages();
}
*/
/*
void  CEOSAIUnit2Template::LoadGLTexture( CString strBaseFilename )
{
	CString strFullPath = g_pApp->GetBaseDirectory() + "\\" + strBaseFilename;

	// Are the images in png or bmp format?
	bool bBmpFormat  = false;
	bool bPngFormat  = false;
	bool bPngFormat2 = false;
	int FileResult;
	struct _stat FileInfoBuffer;
	FileResult = _stat( strFullPath + "-0000-Black.bmp", &FileInfoBuffer );
	if( FileResult == 0 )
	{
		ASSERT( false );
		bBmpFormat = true;
	}
	FileResult = _stat( strFullPath + "-0000.png", &FileInfoBuffer );
	if( FileResult == 0 )
	{
		bPngFormat = true;
	}
	FileResult = _stat( strFullPath + "-r00.png", &FileInfoBuffer );
	if( FileResult == 0 )
	{
		bPngFormat2 = true;
	}

	/*
	for( long iRotation=0; iRotation<m_iNumberOfImages; iRotation++ )
	{
		if( bPngFormat2 )
		{
			CString strCompName;
			strCompName.Format( "%s-r%02d", strBaseFilename, iRotation );
			m_GLTextureArray[ iRotation ].LoadPNG( strCompName, m_iGLCenterX, m_iGLCenterY );
		}
		if( bPngFormat )
		{
			CString strCompName;
			strCompName.Format( "%s-00%02d", strBaseFilename, iRotation );
			m_GLTextureArray[ iRotation ].LoadPNG( strCompName, m_iGLCenterX, m_iGLCenterY );
		}
		else if( bBmpFormat )
		{
			CString strCompName;
			strCompName.Format( "%s-00%02d", strBaseFilename, iRotation );
			m_GLTextureArray[ iRotation ].Load( strCompName, m_iGLCenterX, m_iGLCenterY );
		}

		// Setup default center positions
		//m_GLTextureArray[ iRotation ].m_iCenterX = m_GLTextureArray[ iRotation ].m_iWidth/2;
		//m_GLTextureArray[ iRotation ].m_iCenterY = m_GLTextureArray[ iRotation ].m_iHeight/2;
	}
	*-/

	// Load StaticCombatImages
	/*
	if( m_strStaticCombatImage != "" )
	{
		bBmpFormat  = false;
		bPngFormat  = false;
		bPngFormat2 = false;
		CString strFullPath = g_pApp->GetBaseDirectory() + "\\" + m_strStaticCombatImage;
		FileResult = _stat( strFullPath + "-r00.png", &FileInfoBuffer );
		if( FileResult == 0 )
		{
			bPngFormat2 = true;
		}
		for( long iRotation=0; iRotation<m_iNumberOfImages; iRotation++ )
		{
			if( bPngFormat2 )
			{
				CString strCompName;
				strCompName.Format( "%s-r%02d", m_strStaticCombatImage, iRotation );
				m_StaticCombatImage[ iRotation ].LoadPNG( strCompName, m_iGLCenterX, m_iGLCenterY );
			}
		}
	}
	*-/
}
*/
/*
CGLTexture* CEOSAIUnit2Template::GetGLTexture( long iRotation )
{
	ASSERT( m_pUnitTemplateImage );
	return m_pUnitTemplateImage->GetGLTexture( iRotation );
	/-*
	if( iRotation == -1 )
	{
		return m_GLTextureArray[ m_iDefaultRotation ].GetImage();
	}
	return m_GLTextureArray[iRotation].GetImage();
	*-/
}

long CEOSAIUnit2Template::GetDefaultRotation()
{
	return m_pUnitTemplateImage->GetDefaultRotation();
}
*/
/*
void  CEOSAIUnit2Template::SetGrayMBitmap( CString strFilename )
{
	//	m_iImageCenterX = -1;
	//m_GrayMBitmap.m_iImageCenterY = -1;

	// Load into Masked Bitmaps
	if( CBCString::EndsWith( strFilename, ".bmp" ) )
	{
		ASSERT( false );
		m_GrayMBitmap.LoadBitmap( strFilename );
	}
	if( CBCString::EndsWith( strFilename, ".png" ) )
	{
		m_GrayMBitmap.LoadPNG( strFilename, crGray200, true );
	}
	m_GrayMBitmap.FindImageCenter( RGB(255,0,255) );

	//
	//ASSERT( m_GrayMBitmap.GetWidth()  == 46 );
	//ASSERT( m_GrayMBitmap.GetHeight() == 32 );
}
*/
/*
CMaskedBitmap* CEOSAIUnit2Template::GetGrayMBitmap()
{
	if( m_pUnitTemplateImage == NULL ) return NULL;
	return m_pUnitTemplateImage->GetGrayMBitmap();
}
*/
/*
void  CEOSAIUnit2Template::SetWhiteBitmap( CString strFilename )
{
	if( CBCString::EndsWith( strFilename, ".bmp" ) )
	{
		ASSERT( false );
		m_WhiteBitmapPath = strFilename;
		//m_WhiteBitmap.LoadBitmap( strFilename );
	}
	if( CBCString::EndsWith( strFilename, ".png" ) )
	{
		// The white bitmap should have the "-c" appended to it
		strFilename.Replace( ".png", "-c.png" );
		m_WhiteBitmapPath = strFilename;
		//m_WhiteBitmap.LoadPNG( strFilename, crGray200, true );
	}
	//m_WhiteBitmap.LoadPNG( strBitmapFilename, crWhite );
	//ASSERT( m_46x32Bitmap.GetWidth()  == 46 );
	//ASSERT( m_46x32Bitmap.GetHeight() == 32 );
}
*/

/*
CUnitTemplateCombatAnimationDesc* CEOSAIUnit2Template::GetUnitTemplateCombatAnimation( CString strName )
{
	POSITION pos = m_UnitTemplateCombatAnimationDescs.GetHeadPosition();
	while( pos )
	{
		CUnitTemplateCombatAnimationDesc* pDesc = m_UnitTemplateCombatAnimationDescs.GetNext( pos );
		if( pDesc->m_strName == strName )
		{
			return pDesc;
		}
	}
	return NULL;
}

CUnitTemplateCombatAnimationDesc* CEOSAIUnit2Template::GetUnitTemplateCombatAnimation( CPoi* pTarget )
{
	CUnitTemplateCombatAnimationDesc* pAnimDesc = NULL;
	//pAnimDesc = GetUnitTemplate()->GetCombatAnimationDescription( pTarget );

	CString strAnimation;
	CUnit* pOtherUnit = dynamic_cast< CUnit* >( pTarget );
	if( pOtherUnit )
	{
		CEOSAIAttackVs2* pAttackVs = this->GetAttackVs( pTarget );
		ASSERT( pAttackVs );
		//strUnitAnimationKeyword = pAttackVs->m_strUnitAnimationKeyword;
		strAnimation = pAttackVs->m_strAnimation;
	}
	else
	{
		strAnimation = m_AttackVsCitResFort.m_strAnimation;// m_strAttackVsCityAnimation;
	}

	if( strAnimation == _T("") )
	{
		pAnimDesc = NULL;//GetFirstCombatAnimationDescription();
	}
	else
	{
		pAnimDesc = GetUnitTemplateCombatAnimation( strAnimation );
		if( pAnimDesc == NULL )
		{
			//pAnimDesc = GetFirstCombatAnimationDescription();
		}
	}

	return pAnimDesc;
}
*/
/*
CUnitTemplateCombatAnimationImage*  CEOSAIUnit2Template::GetUnitCombatAnimation( CString strName )
{
	return m_pUnitTemplateImage->GetUnitCombatAnimation( strName );
	/-*
	POSITION pos = m_UnitCombatAnimations.GetHeadPosition();
	while( pos )
	{
		CUnitTemplateCombatAnimationImage* pDesc = m_UnitCombatAnimations.GetNext( pos );
		if( pDesc->m_strName == strName )
		{
			return pDesc;
		}
	}
	ASSERT( false );
	return NULL;
	*-/
}
*/
/*
void  CEOSAIUnit2Template::LoadTextures()
{
	if( m_pUnitTemplateImage )
	{
		m_pUnitTemplateImage->LoadTextures();
	}

	if( GetUnitset()->GetFlagLoadAnimations() )
	{
		//Checkpoint::Write( "LoadingAnimations" );
		POSITION pos = m_UnitTemplateCombatAnimationDescs.GetHeadPosition();
		while( pos )
		{
			CUnitTemplateCombatAnimationDesc* pDesc = m_UnitTemplateCombatAnimationDescs.GetNext( pos );
			if( pDesc->m_strTargetAnimation != _T("") )
			{
				//CString str;
				//str.Format( "Loading CombatAnimationDesc: %s", pDesc->m_strTargetAnimation );
				//Checkpoint::Write( str );
				m_pUnitset->GetAnimationManager()->InvokeAnimation( pDesc->m_strTargetAnimation );
			}
		}
	}
}
*/
void  CEOSAIUnitTemplate::InvokeUnitsubsetUpgradeTo( CString strUnitsubset, CString strUpgradeTo )
{
	POSITION pos = m_UpgradeToList.GetHeadPosition();
	while( pos )
	{
		CUnitsubsetUpgradeTo2* pObj = m_UpgradeToList.GetNext( pos );
		if( pObj->m_strUnitsubset == strUnitsubset )
		{
			pObj->m_strUpgradeTo = strUpgradeTo;
			return;
		}
	}

	CUnitsubsetUpgradeTo2* pObj = new CUnitsubsetUpgradeTo2();
	pObj->m_strUnitsubset = strUnitsubset;
	pObj->m_strUpgradeTo = strUpgradeTo;
	m_UpgradeToList.AddTail( pObj );
}

CString CEOSAIUnitTemplate::GetUnitsubsetUpgradeTo( CString strUnitsubset )
{
	POSITION pos = m_UpgradeToList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CUnitsubsetUpgradeTo2* pObj = m_UpgradeToList.GetNext( pos );
		if( pObj->m_strUnitsubset == strUnitsubset )
		{
			return pObj->m_strUpgradeTo;
		}
	}
	return m_strUpgradeTo; // return the default upgrade
}

float CEOSAIUnitTemplate::GetUpgradeMoneyCost( CString strUnitsubset )
{
	CString strUpgrade = GetUnitsubsetUpgradeTo( strUnitsubset );
	CEOSAIUnitTemplate* pUpgrade = m_pAIGameRules->GetAIUnitTemplate( strUpgrade );
	if( pUpgrade )
	{
		float fProductionDiff = pUpgrade->GetProductionCost() - this->GetProductionCost();
		if( fProductionDiff < 0.0f ){ fProductionDiff = 0.0f; }
		return max( 5.0f, fProductionDiff );
		//return max( 5.0f, pUpgrade->GetProductionCost()/10.0f + fProductionDiff );
	}
	return 0.0;
}

float CEOSAIUnitTemplate::GetUpgradeIronCost( CString strUnitsubset )
{
	CString strUpgrade = GetUnitsubsetUpgradeTo( strUnitsubset );
	CEOSAIUnitTemplate* pUpgrade = m_pAIGameRules->GetAIUnitTemplate( strUpgrade );
	if( pUpgrade )
	{
		float fIronDiff = pUpgrade->GetIronCost() - this->GetIronCost();
		if( fIronDiff < 0.0f ){ fIronDiff = 0.0f; }
		return max( 0.0f, fIronDiff );
	}
	return 0.0;
}

float CEOSAIUnitTemplate::GetUpgradeTime( CString strUnitsubset )
{
	CString strUpgrade = GetUnitsubsetUpgradeTo( strUnitsubset );
	CEOSAIUnitTemplate* pUpgrade = m_pAIGameRules->GetAIUnitTemplate( strUpgrade );
	if( pUpgrade )
	{
		//float fDiff = pUpgrade->GetProductionCost() - this->GetProductionCost();
		//if( fDiff < 0.0f ){ fDiff = 0.0f; }
		//return max( 5.0f, pUpgrade->GetProductionCost()/10.0f + fProductionDiff );
		return max( 0.99f, (pUpgrade->GetProductionCost()/100.0f)-0.01f );
	}
	return 0.0;
}

void  CEOSAIUnitTemplate::RemoveUnitsubsetUpgradeTo( CString strUnitsubset )
{
	POSITION pos = m_UpgradeToList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CUnitsubsetUpgradeTo2* pObj = m_UpgradeToList.GetNext( pos );
		if( pObj->m_strUnitsubset == strUnitsubset )
		{
			m_UpgradeToList.RemoveAt( prevPos );
		}
	}
}

CEOSAIUnitTemplate* CEOSAIUnitTemplate::GetUpgradesFrom() // this doesn't always work right since multiple units can upgrade to this one
{
	CString strUnitTemplate = GetInternalName();
	POSITION pos = m_pAIGameRules->GetAIUnitTemplates()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplate* pOtherTemplate = m_pAIGameRules->GetAIUnitTemplates()->GetNext( pos );
		if( pOtherTemplate->GetGenericUpgradeTo() == strUnitTemplate )
		{
			return pOtherTemplate;
		}
	}
	return NULL;
}

//
// Unit Tags
//
void  CEOSAIUnitTemplate::AddUnitTag( CString strTag )
{
	CUnitTag* pUnitTag = new CUnitTag( strTag );
	m_UnitTagList.AddTail( pUnitTag );
	//m_UnitTagList.AddTail( strTag );
}

void  CEOSAIUnitTemplate::RemoveUnitTag( CString strTag )
{
	POSITION pos = m_UnitTagList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		//CString strUnitTagInList = m_UnitTagList.GetNext( pos );
		//if( strUnitTagInList == strTag ){ m_UnitTagList.RemoveAt( prevPos ); }
		CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
		if( pUnitTag->m_strTag == strTag )
		{
			m_UnitTagList.RemoveAt( prevPos );
			delete pUnitTag;
		}
	}
	/*
	POSITION pos = m_UnitTagList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
		if( pUnitTag->m_strTag == strTag ){ m_UnitTagList.RemoveAt( prevPos ); }
	}
	*/
}

bool  CEOSAIUnitTemplate::UnitTagExists( CString strTag )
{
	/*
	POSITION pos = m_UnitTagList.GetHeadPosition();
	while( pos )
	{
		CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
		if( pUnitTag->m_strTag == strTag ){ return true; }
	}
	return false;
	*/
	POSITION pos = m_UnitTagList.GetHeadPosition();
	while( pos )
	{
		//CString strUnitTagInList = m_UnitTagList.GetNext( pos );
		//if( strUnitTagInList == strTag ){ return true; }
		CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
		if( pUnitTag->m_strTag == strTag ){ return true; }
	}
	return false;
}

CUnitTag* CEOSAIUnitTemplate::GetUnitTag( CString strTag )
{
	POSITION pos = m_UnitTagList.GetHeadPosition();
	while( pos )
	{
		//CString strUnitTagInList = m_UnitTagList.GetNext( pos );
		//if( strUnitTagInList == strTag ){ return NULL; }
		CUnitTag* pUnitTag = m_UnitTagList.GetNext( pos );
		if( pUnitTag->m_strTag == strTag ){ return pUnitTag; }
	}
	return NULL;
}

/*
CList< CEOSAIStringAndLong* >*  CEOSAIUnitTemplate::GetTargetPriorityTags()
{
	return &m_TargetPriorityTagList;
}
*/
/*
long  CEOSAIUnitTemplate::GetTargetPriorityTag( CString str )
{
	POSITION pos = m_TargetPriorityTagSet.m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIStringAndLong* pStringAndLong = m_TargetPriorityTagSet.m_List.GetNext( pos );
		if( pStringAndLong->m_str == str ) return pStringAndLong->m_iValue;
	}
	return 4;
}
*/
//
//
//
/*
EnumMovementType CEOSAIUnitTemplate::GetMovementType()
{
	if( IsGroundUnit() ) return EOSAIEnumMovementType3_Land;
	if( IsAircraft()   ) return EOSAIEnumMovementType3_Air;
	if( IsMissile()    ) return EOSAIEnumMovementType3_Air;
	if( IsSeaUnit()    ) return EOSAIEnumMovementType3_Water;
	return EOSAIEnumMovementType3_Undefined;
}

void  CEOSAIUnit2Template::SetMovementType2( EnumMovementType eType )
{
	m_eMovementType = eType;
	if( m_eMovementType == EOSAIEnumMovementType3_Air )
	{
		m_MovementDescription.m_bMovesInAir = true;
	}
}
*/

// Four types: SoftGround, Armor, Air, Sea
/*
void   CEOSAIUnit2Template::SetMovementUnitType( CString strMovementUnitType )
{
	m_strMovementUnitType = strMovementUnitType;
	ASSERT( m_pUnitset->GetMovementUnitType( strMovementUnitType ) );

	// Update the MovementDescription object
	CMovementUnitType* pType = m_pUnitset->GetMovementUnitType( m_strMovementUnitType );
	m_MovementDescription.Set( pType );
}

CMovementUnitType*  CEOSAIUnit2Template::GetMovementUnitType()
{
	return m_pUnitset->GetMovementUnitType( m_strMovementUnitType );
}
*/

void CEOSAIUnitTemplate::SetMovementUnitTypeByExternalName( CString strExternalName )
{
	// Update the MovementDescription object
	CEOSAIMovementUnitType* pType = m_pAIGameRules->GetMovementUnitTypeByExternalName( strExternalName );
	ASSERT( pType );
	if( pType )
	{
		m_iMovementUnitType = pType->m_iMovementUnitType;
		m_MovementDescription.Set( pType );
	}
}

void CEOSAIUnitTemplate::SetMovementUnitType( long iMovementUnitType )
{
	m_iMovementUnitType = iMovementUnitType;
	ASSERT( m_pAIGameRules->GetMovementUnitType( iMovementUnitType ) );

	// Update the MovementDescription object
	CEOSAIMovementUnitType* pType = m_pAIGameRules->GetMovementUnitType( m_iMovementUnitType );
	ASSERT( pType );
	if( pType )
	{
		m_MovementDescription.Set( pType );
	}
}

CEOSAIMovementUnitType* CEOSAIUnitTemplate::GetMovementUnitType()
{
	return m_pAIGameRules->GetMovementUnitType( m_iMovementUnitType );
}

long CEOSAIUnitTemplate::GetMovementUnitTypeLong()
{
	return m_iMovementUnitType;
}

CString CEOSAIUnitTemplate::GetMovementUnitTypeString() //{ return m_strMovementUnitType; }
{
	CEOSAIMovementUnitType* pType = m_pAIGameRules->GetMovementUnitType( m_iMovementUnitType );
	if( pType )
	{
		return pType->m_strExternalName;
	}
	return _T("");
}

long CEOSAIUnitTemplate::GetNormalTerrain()
{
	if( GetMovementType() == EOSAIEnumMovementType3_Air )
	{
		return TERRAIN_AIR;
	}
	if( GetMovementType() == EOSAIEnumMovementType3_Water )
	{
		return TERRAIN_WATER;
	}
	if( GetMovementType() == EOSAIEnumMovementType3_Land )
	{
		return TERRAIN_GRASS;
	}
	ASSERT( false );
	return 0;
}

void CEOSAIUnitTemplate::SetCombatUnitType( long iCombatUnitType )
{
	CEOSAICombatUnitType* pCombatUnitType = m_pAIGameRules->GetCombatUnitType( iCombatUnitType );
	ASSERT( pCombatUnitType );
	m_iCombatUnitType = iCombatUnitType;
	m_pCombatUnitType = pCombatUnitType;
}

//void CEOSAIUnit2Template::SetCombatUnitType( CString strUnitType, CCombatUnitType* pCombatUnitType )
void CEOSAIUnitTemplate::SetCombatUnitTypeByExternalName( CString strUnitType )
{
	CEOSAICombatUnitType* pCombatUnitType = m_pAIGameRules->GetCombatUnitTypeByExternalName( strUnitType );
	ASSERT( pCombatUnitType );
	if( pCombatUnitType )
	{
		//m_strCombatUnitType = strUnitType;
		ASSERT( pCombatUnitType );
		m_iCombatUnitType = pCombatUnitType->m_iCombatUnitType;
		ASSERT( m_iCombatUnitType > 0 );

		m_pCombatUnitType = pCombatUnitType;
	}
}

CString CEOSAIUnitTemplate::GetCombatUnitTypeString()
{
	CEOSAICombatUnitType* pCombatUnitType = m_pAIGameRules->GetCombatUnitType( m_iCombatUnitType );
	if( pCombatUnitType )
	{
		return pCombatUnitType->m_strExternalName;//>m_iCombatUnitType;
	}
	ASSERT( false );
	return _T("");
}


long CEOSAIUnitTemplate::GetCombatUnitTypeWhenLandedOrDocked()
{
	//CCombatUnitType* pCombatUnitType = m_pUnitset->GetCombatUnitType( m_strCombatUnitType );	
	CEOSAICombatUnitType* pCombatUnitType = m_pAIGameRules->GetCombatUnitType( m_iCombatUnitType );	
	ASSERT( pCombatUnitType );
	//if( pCombatUnitType->m_strLandedOrDockedCombatType == _T("") )
	if( pCombatUnitType->m_iLandedOrDockedCombatType == 0 )
	{
		return m_iCombatUnitType;
	}
	return pCombatUnitType->m_iLandedOrDockedCombatType;
}

bool  CEOSAIUnitTemplate::CanBlockAndBeBlocked()
{
	//CCombatUnitType* pCombatUnitType = m_pUnitset->GetCombatUnitType( m_strCombatUnitType );	
	CEOSAICombatUnitType* pCombatUnitType = m_pAIGameRules->GetCombatUnitType( m_iCombatUnitType );
	ASSERT( pCombatUnitType );
	return pCombatUnitType->m_bCanBlockAndBeBlocked;
}

long  CEOSAIUnitTemplate::GetDrawOrder()
{
	CEOSAICombatUnitType* pCombatUnitType = m_pAIGameRules->GetCombatUnitType( m_iCombatUnitType );	
	//ASSERT( pCombatUnitType );
	if( pCombatUnitType ){ return pCombatUnitType->m_iDrawOrder; }
	return 0;
}

bool  CEOSAIUnitTemplate::IsGroundUnit()
{
	CEOSAIMovementUnitType* pType = m_pAIGameRules->GetMovementUnitType( GetMovementUnitTypeLong() );
	if( pType )
	{
		return pType->m_eMovementType == EOSAIEnumMovementType3_Land;
	}
	//ASSERT( false );
	return false;
	//return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Land;
}

bool  CEOSAIUnitTemplate::IsAirUnit() //{ return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Air; }
{
	CEOSAIMovementUnitType* pType = m_pAIGameRules->GetMovementUnitType( GetMovementUnitTypeLong() );
	if( pType )
	{
		return pType->m_eMovementType == EOSAIEnumMovementType3_Air;
	}
	//ASSERT( false );
	return false;
	//return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Air;
}

bool  CEOSAIUnitTemplate::IsSeaUnit() //{ return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Air; }
{
	CEOSAIMovementUnitType* pType = m_pAIGameRules->GetMovementUnitType( GetMovementUnitTypeLong() );
	if( pType )
	{
		return pType->m_eMovementType == EOSAIEnumMovementType3_Water;
	}
	//ASSERT( false );
	return false;
	//return m_MovementDescription.GetMovementType() == EOSAIEnumMovementType3_Air;
}

// Six types: SoftGround, Armor, Aircraft, Missile, Ship, Submarine
/*
void   CEOSAIUnit2Template::SetCombatUnitType( CString strCombatUnitType )
{
	m_strCombatUnitType = strCombatUnitType;
	ASSERT( m_pUnitset->GetCombatUnitType( strCombatUnitType ) );
}
*/

bool  CEOSAIUnitTemplate::CanContain( CEOSAIPoiObject* pAIPoiObject )
{
	CEOSAIUnit2* pAIUnit = dynamic_cast< CEOSAIUnit2* >( pAIPoiObject );
	if( pAIUnit )
	{
		/*
		DWORD dwUnitType = pUnit->GetUnitType();
		if( m_dwUnitTypesThatICanContain & dwUnitType )
		{
			return true;
		}
		// Use UnitTags
		*/
		return CanContain( pAIUnit->GetAIUnitTemplate() );
	}
	/*
	CEOSAIUnit2Group* pAIUnitGroup = dynamic_cast< CEOSAIUnit2Group* >( pAIPoiObject );
	if( pAIUnitGroup )
	{
		POSITION pos = pUnitGroup->GetUnitsInsideMe()->GetHeadPosition();
		while( pos )
		{
			CPoiObjectULink* pLink = pUnitGroup->GetUnitsInsideMe()->GetNext( pos );
			CEOSAIPoiObject* pPoiObject = pLink->GetPtr();
			// Recursive
			if( !this->CanContain( pPoiObject ) ) return false;
		}
		return true;
	}
	*/

	return false;
}

bool  CEOSAIUnitTemplate::CanContain( CEOSAIUnitTemplate* pAITemplate )
{
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		CString strUnitTagInList = m_CanContainUnitTagList.GetNext( pos );
		if( pAITemplate->UnitTagExists( strUnitTagInList ) )
		{
			return true;
		}
	}

	return false;
}

bool  CEOSAIUnitTemplate::CanContainGroundUnits()
{
	ASSERT( m_pAIGameRules );
	CList< CEOSAIUnitTemplate* >  UnitTemplateList;
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		//CUnitTag* pTag = m_CanContainUnitTagList.GetNext( pos );
		//m_pUnitset->GetUnitTemplates( pTag->m_strTag, &UnitTemplateList );
		CString  strUnitTagInList = m_CanContainUnitTagList.GetNext( pos );
		m_pAIGameRules->GetAIUnitTemplates( strUnitTagInList, &UnitTemplateList );

		POSITION pos2 = UnitTemplateList.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnitTemplate* pUnitTemplate = UnitTemplateList.GetNext( pos2 );
			if( pUnitTemplate->IsGroundUnit() ){ return true; }
		}
	}
	return false;
}

long  CEOSAIUnitTemplate::GetNumberOfGroundUnitsThatICanContain()
{
	if( CanContainGroundUnits() )
	{
		return m_iNumberOfUnitsThatICanContain;
	}
	// this is more complicated now
	/*
	if( m_dwUnitTypesThatICanContain & ( Infantry | Cavalry | LightVehicle | HeavyVehicle ) )
	{
		return m_iNumberOfUnitsThatICanContain;
	}
	*/
	return 0;
}

bool  CEOSAIUnitTemplate::CanContainAirUnits()
{
	if( CanContainUnits() == false ) return false;

	ASSERT( m_pAIGameRules );
	CList< CEOSAIUnitTemplate* >  UnitTemplateList;
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		//CUnitTag* pTag = m_CanContainUnitTagList.GetNext( pos );
		//m_pUnitset->GetUnitTemplates( pTag->m_strTag, &UnitTemplateList );
		CString  strUnitTagInList = m_CanContainUnitTagList.GetNext( pos );
		m_pAIGameRules->GetAIUnitTemplates( strUnitTagInList, &UnitTemplateList );

		POSITION pos2 = UnitTemplateList.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnitTemplate* pAIUnitTemplate = UnitTemplateList.GetNext( pos2 );
			//if( pUnitTemplate->IsAircraft() ){ return true; }
			if( pAIUnitTemplate->IsAirUnit() ){ return true; }
		}
	}
	return false;
}

bool  CEOSAIUnitTemplate::CanLaunchAirUnits()
{
	if( CanContainUnits() == false ) return false;
	if( IsLaunchingPadForAirUnits() == false ) return false;

	ASSERT( m_pAIGameRules );
	CList< CEOSAIUnitTemplate* >  UnitTemplateList;
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		//CUnitTag* pTag = m_CanContainUnitTagList.GetNext( pos );
		//m_pUnitset->GetUnitTemplates( pTag->m_strTag, &UnitTemplateList );
		CString  strUnitTagInList = m_CanContainUnitTagList.GetNext( pos );
		m_pAIGameRules->GetAIUnitTemplates( strUnitTagInList, &UnitTemplateList );

		POSITION pos2 = UnitTemplateList.GetHeadPosition();
		while( pos2 )
		{
			CEOSAIUnitTemplate* pUnitTemplate = UnitTemplateList.GetNext( pos2 );
			//if( pUnitTemplate->IsAircraft() ){ return true; }
			if( pUnitTemplate->IsAirUnit() ){ return true; }
		}
	}
	return false;
}

long  CEOSAIUnitTemplate::GetNumberOfAirUnitsThatICanContain()
{
	if( CanContainAirUnits() )
	{
		return m_iNumberOfUnitsThatICanContain;
	}
	// this is more complicated now
	/*
	if( m_dwUnitTypesThatICanContain & ( LightAircraft | HeavyAircraft ) )
	{
		return m_iNumberOfUnitsThatICanContain;
	}
	*/
	return 0;
}

//
// ViewRange
//
/*
void  CEOSAIUnit2Template::InvokeViewRange( CString strUnitTag, float fViewRange )
{
	POSITION pos = m_ViewRangeList.GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
		if( pViewRange->m_strUnitTag == strUnitTag )
		{
			pViewRange->m_fViewRange = fViewRange;
			return;
		}
	}
	CViewRange* pViewRange = new CViewRange();
	pViewRange->m_strUnitTag = strUnitTag;
	pViewRange->m_fViewRange = fViewRange;
	m_ViewRangeList.AddTail( pViewRange );
}

void CEOSAIUnit2Template::RemoveViewRange( CString strUnitTag )
{
	POSITION pos = m_ViewRangeList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
		if( pViewRange->m_strUnitTag == strUnitTag )
		{
			m_ViewRangeList.RemoveAt( prevPos );
			return;
		}
	}
}

CViewRange* CEOSAIUnit2Template::GetViewRangeObj( CString strUnitTag )//, bool bUseUnitDefaults )
{
	POSITION pos = m_ViewRangeList.GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
		if( pViewRange->m_strUnitTag == strUnitTag )
		{
			return pViewRange;
		}
	}
	return NULL;
}

float CEOSAIUnit2Template::GetViewRange( CString strUnitTag, bool bUseUnitDefaults )
{
	ASSERT( false );
	/*
	POSITION pos = m_ViewRangeList.GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
		if( pViewRange->m_strUnitTag == strUnitTag )
		{
			return pViewRange->m_fViewRange;
		}
	}
	if( bUseUnitDefaults )
	{
		pos = m_pAIGameRules->GetUnitDefaultViewRangeList()->GetHeadPosition();
		while( pos )
		{
			CViewRange* pViewRange = m_pAIGameRules->GetUnitDefaultViewRangeList()->GetNext( pos );
			if( pViewRange->m_strUnitTag == strUnitTag )
			{
				return pViewRange->m_fViewRange;
			}
		}
	}
	return m_fViewRange;
	*-/
	return 0.0f;
}
*/
float CEOSAIUnitTemplate::GetViewRange( CEOSAIPoiObject* pPoi )
{
	/*
	EOSAI::PoiMobile* pOtherPoiMobile = dynamic_cast< CEOSAIUnit2* >( pPoi );
	if( pOtherPoiMobile )
	{
		CEOSAIUnit2* pOtherUnit = dynamic_cast< CEOSAIUnit2* >( pPoi );
		if( pOtherUnit )
		{
			CEOSAIUnitTemplate* pOtherUnitTemplate = pOtherUnit->GetAIUnitTemplate();
			return GetViewRange( pOtherUnitTemplate );
		}
	}
	else
	{
		return m_fViewRange; // default view range
	}
	return 0.0f;
	*/
	return m_fViewRange;
}

float CEOSAIUnitTemplate::GetViewRange( CEOSAIUnitTemplate* pUnitTemplate )
{
	/*
	POSITION pos = m_ViewRangeList.GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange = m_ViewRangeList.GetNext( pos );
		if( pUnitTemplate->UnitTagExists( pViewRange->m_strUnitTag ) )
		{
			return pViewRange->m_fViewRange;
		}
	}
	pos = m_pAIGameRules->GetUnitDefaultViewRangeList()->GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange = m_pAIGameRules->GetUnitDefaultViewRangeList()->GetNext( pos );
		if( pUnitTemplate->UnitTagExists( pViewRange->m_strUnitTag ) )
		{
			return pViewRange->m_fViewRange;
		}
	}
	*/
	return m_fViewRange; // default view range
}

//

bool CEOSAIUnitTemplate::CanAttack( CEOSAIPoiObject* pTarget )
{
	if( CanCapture( pTarget ) || CanDegrade( pTarget ) || CanDestroy( pTarget ) ) return true;
	return false;
}

bool CEOSAIUnitTemplate::CanDestroy( CEOSAIPoiObject* pTarget )
{
	CEOSAIAirfield* pAIAirfield = dynamic_cast< CEOSAIAirfield* >( pTarget );
	if( pAIAirfield )
	{
		return CanDestroyAirfields();
	}
	// The ability to attack PoiMobile depends on the attacker's specific combat capabilities
	CEOSAIPoiObject* pTargetPoiObject = dynamic_cast< CEOSAIPoiObject* >( pTarget );
	EOSAI::PoiMobile* pTargetPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pTarget );
	if( pTargetPoiMobile )
	{
		CEOSAIUnit2* pTargetUnit = dynamic_cast< CEOSAIUnit2* >( pTargetPoiMobile );
		if( pTargetUnit )
		{
			//CEOSAIAttackVs2* pAttackVs = GetAttackVs( pTargetPoiObject, enum_UseDefendersCurrentProneState );
			CEOSAIAttackVs2* pAttackVs = GetAttackVs( pTargetPoiObject, pTargetPoiObject->IsLandedOrDocked() );
			if( pAttackVs == NULL ) return false;
			ASSERT( pAttackVs );
			return ( pAttackVs->m_bAlwaysHits || pAttackVs->m_fToHit > 0.0f );
		}
		/*
		CUnitGroup* pTargetUnitGroup = dynamic_cast< CUnitGroup* >( pTargetPoiMobile );
		if( pTargetUnitGroup )
		{
			bool bCanAttack = false;
			POSITION pos = pTargetUnitGroup->m_UnitsInsideMe.GetHeadPosition();
			while( pos )
			{
				CPoiObject* pInsidePoiObject = pTargetUnitGroup->m_UnitsInsideMe.GetNext( pos )->GetPtr();
				if( CanAttack( pInsidePoiObject ) ){ bCanAttack = true; }
				//if( CanAttackPoi( pInsidePoiObject ) ){ bCanAttack = true; }
			}
			return bCanAttack;
		}
		*/
	}
	return false;
}


bool  CEOSAIUnitTemplate::OneAttackPerLandingVs( CEOSAIPoiObject* pTarget )
{
	/*
	if( dynamic_cast< CUnitGroup* >( pTarget ) )
	{
		CUnitGroup* pUnitGroup = dynamic_cast< CUnitGroup* >( pTarget );
		POSITION pos = pUnitGroup->GetUnitsInsideMe()->GetHeadPosition();
		while( pos )
		{
			CPoiObject* pPoiObject = pUnitGroup->GetUnitsInsideMe()->GetNext( pos )->GetPtr();
			if( OneAttackPerLandingVs( pPoiObject ) ) return true;
		}
		return false;
	}
	*/
	CEOSAIAttackVs2* pAttackVs = this->GetAttackVs( pTarget );
	ASSERT( pAttackVs );
	if( pAttackVs )
	{
		return pAttackVs->GetOABL();
	}
	return false;
}

bool  CEOSAIUnitTemplate::CanCapture( CEOSAIPoiObject* pPoiObject )
{
	if( dynamic_cast< CEOSAICity* >( pPoiObject ) )
	{
		return CanCaptureCities();
	}
	if( dynamic_cast< CEOSAIResource* >( pPoiObject ) )
	{
		CEOSAIResource* pResource = dynamic_cast< CEOSAIResource* >( pPoiObject );
		if( IsGroundUnit() && pResource->ResourceSource_IsOnLand()          ){ return CanCaptureResources(); }
		if( IsSeaUnit()    && pResource->ResourceSource_IsOnLand() == false ){ return CanCaptureResources(); }
		return false;
	}
	if( dynamic_cast< CEOSAIFortification* >( pPoiObject ) )
	{
		return CanCaptureFortifications();
	}
	if( dynamic_cast< CEOSAIAirfield* >( pPoiObject ) )
	{
		return CanCaptureAirfields();
	}
	return false;
}


bool  CEOSAIUnitTemplate::CanCapture( EnumAIPoiObjectType enumPoiObjectType )
{
	if( enumPoiObjectType == EnumAIPoiObjectType::enum_City )
	{
		return CanCaptureCities();
	}
	if( enumPoiObjectType == EnumAIPoiObjectType::enum_Resource )
	{
		//CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoiObject );
		//if( IsGroundUnit() && pResource->ResourceSource_IsOnLand()          ){ return CanCaptureResources(); }
		//if( IsSeaUnit()    && pResource->ResourceSource_IsOnLand() == false ){ return CanCaptureResources(); }
		return CanCaptureResources();
	}
	if( enumPoiObjectType == EnumAIPoiObjectType::enum_Fortification )
	{
		return CanCaptureFortifications();
	}
	if( enumPoiObjectType == EnumAIPoiObjectType::enum_Airfield )
	{
		return CanCaptureAirfields();
	}
	return false;
}

bool  CEOSAIUnitTemplate::CanDegrade( CEOSAIPoiObject* pPoiObject )
{
	if( dynamic_cast< CEOSAICity* >( pPoiObject ) )
	{
		return CanDegradeCities();
	}
	if( dynamic_cast< CEOSAIAirfield* >( pPoiObject ) )
	{
		return CanDamageAirfields() || CanDestroyAirfields();
	}
	return false;
}

/*
bool  CEOSAIUnitTemplate::AttackMethodIsBombingVs( CPoiObject* pTarget )
{
	if( AttackMethodIsBombing() )
	{
		return
			( pTarget->IsAirUnit() == false ) ||
			( pTarget->IsAirUnit() && pTarget->IsLandedOrDocked() );
			//pTarget->IsCurrentlyASurfaceTarget() ||
			//pTarget->IsCurrentlyAnUnderwaterTarget();
	}
	return false;
}
*/

//
// UnitTag Containment
//
void  CEOSAIUnitTemplate::AddCanContainUnitTag( CString strTag )
{
	//CUnitTag* pUnitTag = new CUnitTag( strTag );
	//m_CanContainUnitTagList.AddTail( pUnitTag );
	m_CanContainUnitTagList.AddTail( strTag );
}

void  CEOSAIUnitTemplate::RemoveCanContainUnitTag( CString strTag )
{
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CString strUnitTagInList = m_CanContainUnitTagList.GetNext( pos );
		if( strUnitTagInList == strTag )
		{
			m_CanContainUnitTagList.RemoveAt( prevPos );
		}
	}
}

bool  CEOSAIUnitTemplate::CanContainUnitTagExists( CString strTag )
{
	/*
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		CUnitTag* pUnitTag = m_CanContainUnitTagList.GetNext( pos );
		if( pUnitTag->m_strTag == strTag ){ return true; }
	}
	return false;
	*/
	POSITION pos = m_CanContainUnitTagList.GetHeadPosition();
	while( pos )
	{
		CString strUnitTagInList = m_CanContainUnitTagList.GetNext( pos );
		if( strUnitTagInList == strTag ){ return true; }
	}
	return false;
}


/*
void  CEOSAIUnitTemplate::SetCanContain( CString strClassName )
{
	m_UnitsThatICanCarry.AddHead( strClassName );
}
*/
/*
void CEOSAIUnitTemplate::GetPrereqTechList_Top( CStringList* pPrereqTechList )
{
	// Find the prerequisite tech
	POSITION pos = m_pUnitset->GetTechnologyList()->GetHeadPosition();
	while( pos )
	{
		CTechnologyDesc* pTechnologyDesc = m_pUnitset->GetTechnologyList()->GetNext( pos );
		if( pTechnologyDesc->EnablesUnit( m_strInternalName ) )
		{
			pPrereqTechList->AddTail( pTechnologyDesc->GetInternalName() );
		}
	}
}

void CEOSAIUnitTemplate::GetPrereqTechList_All( CStringList* pPrereqTechList )
{
	CStringList TopPrereqTech;
	GetPrereqTechList_Top( &TopPrereqTech );

	POSITION pos = TopPrereqTech.GetHeadPosition();
	while( pos )
	{
		CString strTech = TopPrereqTech.GetNext( pos );
		CTechnologyDesc* pTechnologyDesc = m_pUnitset->GetTechnologyDesc( strTech );
		if( pTechnologyDesc )
		{
			CString strPrereqTech = pTechnologyDesc->GetInternalName();
			if( CBCList::ExistsInList( strPrereqTech, pPrereqTechList ) == false )
			{
				pPrereqTechList->AddTail( strPrereqTech );
				pTechnologyDesc->GetPrereqTechList( pPrereqTechList );
			}
		}
	}
}

void CEOSAIUnitTemplate::CalculatePrereqTechCost2()
{
	CStringList  PrereqTechList;
	GetPrereqTechList_All( &PrereqTechList );

	//
	long iTechCost = 0;
	POSITION pos = PrereqTechList.GetHeadPosition();
	while( pos )
	{
		CString strPrereqTech = PrereqTechList.GetNext( pos );
		CTechnologyDesc* pTechnologyDesc = m_pUnitset->GetTechnologyDesc( strPrereqTech );
		if( pTechnologyDesc )
		{
			iTechCost += pTechnologyDesc->GetCost();
		}
	}
	m_iPrereqTechCost = iTechCost;
}

long CEOSAIUnitTemplate::GetPrereqTechCost()
{
	CStringList  PrereqTechList;
	GetPrereqTechList_All( &PrereqTechList );

	//
	long iTechCost = 0;
	POSITION pos = PrereqTechList.GetHeadPosition();
	while( pos )
	{
		CString strPrereqTech = PrereqTechList.GetNext( pos );
		CTechnologyDesc* pTechnologyDesc = m_pUnitset->GetTechnologyDesc( strPrereqTech );
		if( pTechnologyDesc )
		{
			iTechCost += pTechnologyDesc->GetCost();
		}
	}
	return iTechCost;
}
*/
bool CEOSAIUnitTemplate::CityMustHave( CString strBuilding )
{
	POSITION pos = m_CityMustHaveList.GetHeadPosition();
	while( pos )
	{
		CString strBuildingInList = m_CityMustHaveList.GetNext( pos );
		if( strBuilding == strBuildingInList )
		{
			return true;
		}
	}
	return false;
}

void CEOSAIUnitTemplate::RemoveCityMustHave( CString strBuilding )
{
	POSITION pos = m_CityMustHaveList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CString strBuildingInList = m_CityMustHaveList.GetNext( pos );
		if( strBuilding == strBuildingInList )
		{
			m_CityMustHaveList.RemoveAt( prevPos );
			return;
		}
	}
}
/*
void  CEOSAIUnitTemplate::OnSelectSoundFile( CString str )
{
	m_pUnitTemplateImage->OnSelectSoundFile( str );
	/-*
	if( str == "" )
	{
		while( m_OnSelectSoundList.IsEmpty() == FALSE )
		{
			delete m_OnSelectSoundList.RemoveHead();
		}
	}
	else
	{
		CBCSound* pSound = new CBCSound();
		if( pSound )
		{
			CString strFullPath = g_pApp->GetBaseDirectory() + "\\" + str;
			pSound->LoadSound( strFullPath );
			if( pSound->ContainsData() )
			{
				m_OnSelectSoundList.AddTail( pSound );
			}
			else
			{
				delete pSound;
			}
		}
	}
	*-/
}
*/

/////////////////////////////////

float CEOSAIUnitTemplate::GetProductionAndConsumptionPerTurnAndMove( CString strResource )
{
	return
		GetProductionAndConsumptionPerTurn( strResource ) +
		GetProductionAndConsumptionPerMove( strResource );
}

void CEOSAIUnitTemplate::InvokeProductionAndConsumptionPerTurn( CString strResource, float fValue )
{
	//if( fValue < 0.0f ){ fValue = 0.0f; }
	POSITION pos = m_ProductionAndConsumptionPerTurnList.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerTurnList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			p->m_fValue = fValue;
			return;
		}
	}
	EOSAI::StringAndFloat* p = new EOSAI::StringAndFloat( strResource, fValue );
	m_ProductionAndConsumptionPerTurnList.AddTail( p );
}

void CEOSAIUnitTemplate::RemoveProductionAndConsumptionPerTurn( CString strResource )
{
	POSITION pos = m_ProductionAndConsumptionPerTurnList.GetHeadPosition();
	while( pos )
	{
		POSITION prev = pos;
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerTurnList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			m_ProductionAndConsumptionPerTurnList.RemoveAt( prev );
			delete p;
			return;
		}
	}
}

float CEOSAIUnitTemplate::GetProductionAndConsumptionPerTurn( CString strResource )
{
	POSITION pos = m_ProductionAndConsumptionPerTurnList.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerTurnList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			return p->m_fValue;
		}
	}
	return 0.0f;
}

EOSAI::StringAndFloat*  CEOSAIUnitTemplate::GetProductionAndConsumptionPerTurnObj( CString strResource )
{
	POSITION pos = m_ProductionAndConsumptionPerTurnList.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerTurnList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			return p;
		}
	}
	return NULL;
}

/////////////////////////////////

void CEOSAIUnitTemplate::InvokeProductionAndConsumptionPerMove( CString strResource, float fValue )
{
	//if( fValue < 0.0f ){ fValue = 0.0f; }
	POSITION pos = m_ProductionAndConsumptionPerMoveList.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerMoveList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			p->m_fValue = fValue;
			return;
		}
	}
	EOSAI::StringAndFloat* p = new EOSAI::StringAndFloat( strResource, fValue );
	m_ProductionAndConsumptionPerMoveList.AddTail( p );
}

void CEOSAIUnitTemplate::RemoveProductionAndConsumptionPerMove( CString strResource )
{
	POSITION pos = m_ProductionAndConsumptionPerMoveList.GetHeadPosition();
	while( pos )
	{
		POSITION prev = pos;
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerMoveList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			m_ProductionAndConsumptionPerMoveList.RemoveAt( prev );
			delete p;
			return;
		}
	}
}

float CEOSAIUnitTemplate::GetProductionAndConsumptionPerMove( CString strResource )
{
	POSITION pos = m_ProductionAndConsumptionPerMoveList.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerMoveList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			return p->m_fValue;
		}
	}
	return 0.0f;
}

EOSAI::StringAndFloat*  CEOSAIUnitTemplate::GetProductionAndConsumptionPerMoveObj( CString strResource )
{
	POSITION pos = m_ProductionAndConsumptionPerMoveList.GetHeadPosition();
	while( pos )
	{
		EOSAI::StringAndFloat* p = m_ProductionAndConsumptionPerMoveList.GetNext( pos );
		if( p->m_strValue == strResource )
		{
			return p;
		}
	}
	return NULL;
}
/*
float CEOSAIUnitTemplate::GetOilConsumption()
{
	return 0.0f;
}
*/
/*
float CEOSAIUnitTemplate::AmountOfFoodNeededThisTurn()
{
	return 0.0f;
}
*/
/*
void CEOSAIUnitTemplate::PlayOnSelectSound()
{
	m_pUnitTemplateImage->PlayOnSelectSound();
}
*/
bool  CEOSAIUnitTemplate::CanDegradeCities()
{
	if( CanKillPopulation() ) return true;
	if( CanDestroyBuildings() ) return true;
	if( CanDamageItemsInProduction() ) return true;
	return false;
}

bool  CEOSAIUnitTemplate::CanTargetCities()
{
	if( CanKillPopulation() ) return true;
	if( CanDestroyBuildings() ) return true;
	if( CanCaptureCities() ) return true;
	if( CanDamageItemsInProduction() ) return true;

	// I might be able to bomb/attack units inside
	//   Note: I thought about changing this to check the units inside the actual city, but 
	//   there's a problem with that -
	//   - Sometimes I want to attack cities and don't know what's inside
	POSITION pos = GetAttackVsList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = GetAttackVsList()->GetNext( pos );
		if( pAttackVs->GetAlwaysHits() || pAttackVs->GetToHit() > 0.0f )
		{
			return true;
		}
	}
	return false;
	//return GetDamageToPopulation() > 0.0f || CanDestroyBuildings() || CanCaptureCities() || GetDamageToItemsInProduction(); } // return m_fDamageToPopulation > 0.0f || m_bCanDestroyBuildings; } should be able to destroy units inside, too
}

// Refugees
float CEOSAIUnitTemplate::GetNumberOfRefugeesOnHit()
{
	//return 2.0f*m_fKillPopulationDeaths;//m_fNumberOfRefugeesOnHit;
	return m_fNumberOfRefugeesOnHit;
}

void  CEOSAIUnitTemplate::SetNumberOfRefugeesOnHit( float f )
{
	m_fNumberOfRefugeesOnHit = f;
}

/*
void  CEOSAIUnitTemplate::DeleteUnitTemplateCombatAnimation( CUnitTemplateCombatAnimationDesc* pAnim )
{
	POSITION pos = m_UnitTemplateCombatAnimationDescs.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CUnitTemplateCombatAnimationDesc* pAnimInList = m_UnitTemplateCombatAnimationDescs.GetNext( pos );
		if( pAnimInList == pAnim )
		{
			delete pAnimInList;
			m_UnitTemplateCombatAnimationDescs.RemoveAt( prevPos );
		}
	}
}
*/
/*
void  CEOSAIUnitTemplate::OnCombatVsGroundSound( CString str )
{
	CBCSound* pSound = new CBCSound();
	if( pSound )
	{
		CString strFullPath = g_pApp->GetBaseDirectory() + "\\" + str;
		pSound->LoadSound( strFullPath );
		if( pSound->ContainsData() )
		{
			m_OnCombatVsGroundSoundList.AddTail( pSound );
		}
		else
		{
			delete pSound;
		}
	}
}

void CEOSAIUnitTemplate::PlayOnCombatVsGroundSound()
{
	if( m_OnCombatVsGroundSoundList.IsEmpty() ) return;

	int iRand = ( rand() % m_OnCombatVsGroundSoundList.GetCount() );

	POSITION pos = m_OnCombatVsGroundSoundList.GetHeadPosition();
	while( pos )
	{
		CBCSound* pSound = m_OnCombatVsGroundSoundList.GetNext( pos );
		if( iRand == 0 )
		{
			//PlaySound( (LPCTSTR) pSound, NULL, SND_MEMORY | SND_ASYNC );
			pSound->Play();
		}
		iRand--;
	}
}

void CEOSAIUnitTemplate::OnCombatVsGroundAnimation( CString str )
{
	// Do something here
}
*/

//float CEOSAIUnitTemplate::GetMovementRangeAssumingIAttack()
// Aircraft want to return, Missiles/Kamikazis do not
float CEOSAIUnitTemplate::GetMovementRangeForAttack()
{
	if( SelfDestructsOnAttack() ){ return m_fMovementLimit; }
	return m_fMovementLimit/2.0f; // The unit wants to return
	//float fMovementRange = GetMovementRange();
	//if( this->SelfDestructsOnAttack() == false ){ fMovementRange /= 2.0f; }
	//return fMovementRange;
}
/*
float CEOSAIUnitTemplate::GetMovementRangeAssumingIWantToReturn()
{
	float fMovementRange = GetMovementRange();
	if( this->IsMissile() == false ){ fMovementRange /= 2.0f; }
	return fMovementRange;
}
*/

CEOSAIAttackVs2*  CEOSAIUnitTemplate::GetAttackVs( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked )
{
	if( dynamic_cast< CEOSAICity* >( pTarget ) ||
		dynamic_cast< CEOSAIResource* >( pTarget ) ||
		dynamic_cast< CEOSAIFortification* >( pTarget ) ||
		dynamic_cast< CEOSAIAirfield* >( pTarget ) )
	{
		return GetAttackVsCitResFort();
	}
	CEOSAIUnit2* pOtherUnit = dynamic_cast< CEOSAIUnit2* >( pTarget );
	if( pOtherUnit )
	{
		if( pOtherUnit->IsLandedOrDocked() )
		{
			return GetAttackVsCombatUnitType( pOtherUnit->GetAIUnitTemplate()->GetCombatUnitTypeWhenLandedOrDocked() );
		}
		return GetAttackVsCombatUnitType( pOtherUnit->GetCombatUnitTypeLong() );
		//return GetAttackVs( pOtherUnit->GetCombatUnitTypeString() );
		//return GetAttackVs( pOtherUnit->GetUnitTemplate()->GetCombatUnitType() );
	}

	ASSERT( false );
	return NULL;
}

float CEOSAIUnitTemplate::GetAttackRangeMaximum()
{
	float fMaxAttackRange = m_AttackVsCitResFort.GetAttackRange();
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
		if( pAttackVs )
		{
			fMaxAttackRange = max( fMaxAttackRange, pAttackVs->GetAttackRange() );
		}
	}
	return fMaxAttackRange;
}
/*
CEOSAIAttackVs2* CEOSAIUnitTemplate::GetAttackVs( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked )
{
	if( dynamic_cast< CEOSAICity* >( pTarget ) ||
		dynamic_cast< CEOSAIResource* >( pTarget ) ||
		dynamic_cast< CEOSAIFortification* >( pTarget ) ||
		dynamic_cast< CEOSAIAirfield* >( pTarget ) )
	{
		return GetAttackVsCitResFort();
	}
	CEOSAIUnit2* pOtherUnit = dynamic_cast< CEOSAIUnit2* >( pTarget );
	if( pOtherUnit )
	{
		if( bIsLandedOrDocked )
		{
			return GetAttackVsCombatUnitType( pOtherUnit->GetAIUnitTemplate()->GetCombatUnitTypeWhenLandedOrDocked() );
		}
		return GetAttackVsCombatUnitType( pOtherUnit->GetCombatUnitTypeLong() );
	}

	ASSERT( false );
	return NULL;
}
*/
CEOSAIAttackVs2* CEOSAIUnitTemplate::GetAttackVsCombatUnitType( long iCombatUnitType )
{
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
		if( pAttackVs->m_iAttackVsCombatUnitType == iCombatUnitType )
		{
			return pAttackVs;
		}
	}
	return NULL;
}
/*
CEOSAIAttackVs2* CEOSAIUnitTemplate::GetAttackVs( CString strCombatUnitType )
{
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
		if( pAttackVs->m_strCombatUnitType == strCombatUnitType )
		{
			return pAttackVs;
		}
	}
	//ASSERT( false );
	return NULL;
}
*/
CEOSAIAttackVs2* CEOSAIUnitTemplate::GetAttackVs( EnumAIPoiObjectType ePoiObjectType, CEOSAIUnitTemplate* pTargetUnitTemplate, bool bIsLandedOrDocked )
{
	if( ePoiObjectType == EnumAIPoiObjectType::enum_City ||
		ePoiObjectType == EnumAIPoiObjectType::enum_Resource ||
		ePoiObjectType == EnumAIPoiObjectType::enum_Fortification ||
		ePoiObjectType == EnumAIPoiObjectType::enum_Airfield )
	{
		return this->GetAttackVsCitResFort();
	}
	if( ePoiObjectType == EnumAIPoiObjectType::enum_Unit )
	{
		ASSERT( pTargetUnitTemplate );
		//CString strCombatUnitType = pTargetUnitTemplate->GetCombatUnitTypeString();
		long iCombatUnitType = pTargetUnitTemplate->GetCombatUnitTypeLong();
		if( bIsLandedOrDocked )
		{
			iCombatUnitType = pTargetUnitTemplate->GetCombatUnitTypeWhenLandedOrDocked();
		}
		CEOSAIAttackVs2* pAttackVs = GetAttackVsCombatUnitType( iCombatUnitType );
		return pAttackVs;
	}
	ASSERT( false );
	return NULL;
}


/*
CEOSAIAttackVs2* CEOSAIUnitTemplate::GetAttackVs( EnumPoiObjectType eUnitType )
{
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
		if( pAttackVs->m_eCombatUnitType == eUnitType )
		{
			return pAttackVs;
		}
	}
	ASSERT( false );
	return NULL;
}
*/
void CEOSAIUnitTemplate::DeleteAttackVs( CEOSAIAttackVs2* pAttackVs )
{
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAIAttackVs2* pAttackVsInList = m_AttackVsList.GetNext( pos );
		if( pAttackVsInList == pAttackVs )
		{
			m_AttackVsList.RemoveAt( prevPos );
			delete pAttackVsInList;
			return;
		}
	}
}


float CEOSAIUnitTemplate::GetAttackToHitVs( EnumAIPoiObjectType ePoiObjectType, CEOSAIUnitTemplate* pTargetUnitTemplate, bool bIsLandedOrDocked )
{
	ASSERT( ePoiObjectType == EnumAIPoiObjectType::enum_Airfield ||
			ePoiObjectType == EnumAIPoiObjectType::enum_City ||
			ePoiObjectType == EnumAIPoiObjectType::enum_Fortification ||
			ePoiObjectType == EnumAIPoiObjectType::enum_Resource ||
			ePoiObjectType == EnumAIPoiObjectType::enum_Unit );

	if( ePoiObjectType == EnumAIPoiObjectType::enum_Airfield ||
		ePoiObjectType == EnumAIPoiObjectType::enum_City ||
		ePoiObjectType == EnumAIPoiObjectType::enum_Fortification ||
		ePoiObjectType == EnumAIPoiObjectType::enum_Resource )
	{
		if( GetAttackVsCitResFort() == NULL ) return 0.0f; // happens if [this] is a city or something
		if( GetAttackVsCitResFort()->GetToHit() > 0.0f ){ return GetAttackVsCitResFort()->GetToHit(); }
		if( GetAttackVsCitResFort()->GetAlwaysHits() ){ return 1.0f; }
		//
		if( ePoiObjectType == EnumAIPoiObjectType::enum_City )
		{
			if( this->CanCaptureCities() ){ return 1.0f; }
			return 0.0f;
		}
		if( ePoiObjectType == EnumAIPoiObjectType::enum_Resource )
		{
			if( this->CanCaptureResources() ){ return 1.0f; }
			return 0.0f;
		}
		if( ePoiObjectType == EnumAIPoiObjectType::enum_Fortification )
		{
			if( this->CanCaptureCities() ){ return 1.0f; }
			return 0.0f;
		}
		if( ePoiObjectType == EnumAIPoiObjectType::enum_Airfield )
		{
			if( this->CanCaptureAirfields() ){ return 1.0f; }
			return 0.0f;
		}
	}
	else
	{
		//CString strTargetCombatUnitType = pTargetUnitTemplate->GetCombatUnitTypeString();
		long iTargetCombatUnitType = pTargetUnitTemplate->GetCombatUnitTypeLong();
		if( bIsLandedOrDocked )
		{
			iTargetCombatUnitType = pTargetUnitTemplate->GetCombatUnitTypeWhenLandedOrDocked();
		}
		return GetAttackVsCombatUnitType( iTargetCombatUnitType )->GetToHit();
	}
	ASSERT( false );
	return 0.0f;
}

//float CEOSAIUnitTemplate::GetAttackToHitVs( CPoiObject* pTarget, Enum_Prone eDefendersProneState )
float CEOSAIUnitTemplate::GetAttackToHitVs( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked )
{
	CEOSAIUnit2* pUnit = dynamic_cast< CEOSAIUnit2* >( pTarget );
	if( pUnit )
	{
		return GetAttackToHitVs( pTarget->GetAIPoiObjectType(), pUnit->GetAIUnitTemplate(), bIsLandedOrDocked );
	}
	return GetAttackToHitVs( pTarget->GetAIPoiObjectType(), NULL, false );
	return 0.0f;
}

float CEOSAIUnitTemplate::GetAttackToHitVs( CEOSAIUnitTemplate* pTarget )
{
	//CString strTargetCombatUnitType = pTarget->GetCombatUnitTypeString();
	//return GetAttackVs( strTargetCombatUnitType )->GetToHit();
	long iTargetCombatUnitType = pTarget->GetCombatUnitTypeLong();
	return GetAttackVsCombatUnitType( iTargetCombatUnitType )->GetToHit();
}

float CEOSAIUnitTemplate::GetAttackToHitVs( long iCombatUnitType )
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVsCombatUnitType( iCombatUnitType );
	if( pAttackVs )
	{
		return pAttackVs->GetToHit();
	}
	ASSERT( false );
	return 0.0f;
}
/*
float CEOSAIUnitTemplate::GetAttackToHitVs( CString strCombatUnitType )
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( strCombatUnitType );
	if( pAttackVs )
	{
		return pAttackVs->GetToHit();
	}
	ASSERT( false );
	return 0.0f;
}
*/
/*
float CEOSAIUnitTemplate::GetAttackToHitVsPoiObjectType( EnumPoiObjectType ePoiObjectType )
{
	if( ePoiObjectType.IsUnit() )
	{
		if( ePoiObjectType == EnumPoiObjectType::enum_Soft )
		{
			return GetAttackVsSoft()->GetToHit();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Armor )
		{
			return GetAttackVsArmor()->GetToHit();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Aircraft )
		{
			return GetAttackVsAircraft()->GetToHit();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Missile )
		{
			return GetAttackVsMissile()->GetToHit();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Ship )
		{
			return GetAttackVsShip()->GetToHit();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Submarine )
		{
			return GetAttackVsSubmarine()->GetToHit();
		}
		ASSERT( false );
	}
	//CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_City )
	{
		if( this->CanCaptureCities() ){ return 1.0f; }
		return 0.0f;
	}
	//CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_Resource )
	{
		if( this->CanCaptureResources() )
		{
			return 1.0f;
		}
		return 0.0f;
	}
	//CFortification* pFort = dynamic_cast< CFortification* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_Fortification )
	{
		if( this->CanCaptureCities() ) // CanCaptureFortifications() )
		{
			return 1.0f;
		}
		return 0.0f;
	}
	//CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_Airfield )
	{
		if( this->CanCaptureAirfields() )
		{
			return 1.0f;
		}
		return 0.0f;
	}
	ASSERT( false );
	return 0.0f;
}
*/

// Attack Damage
//long CEOSAIUnitTemplate::GetAttackDamageVs( CPoiObject* pTarget, Enum_Prone eDefendersProneState )
long CEOSAIUnitTemplate::GetAttackDamageVs( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked )
{
	if( dynamic_cast< CEOSAICity* >( pTarget ) )
	{
		if( this->CanCaptureCities() ){ return 1; }
		return 0;
	}
	if( dynamic_cast< CEOSAIResource* >( pTarget ) )
	{
		if( this->CanCaptureResources() ){ return 1; }
		return 0;
	}
	if( dynamic_cast< CEOSAIFortification* >( pTarget ) )
	{
		if( this->CanCaptureCities() ){ return 1; }
		return 0;
	}
	if( dynamic_cast< CEOSAIAirfield* >( pTarget ) )
	{
		if( this->CanCaptureAirfields() ){ return 1; }
		return 0;
	}
	if( dynamic_cast< CEOSAIUnit2* >( pTarget ) )
	{
		CEOSAIUnit2* pUnit = (CEOSAIUnit2*) pTarget;
		//CString strTargetCombatUnitType = pUnit->GetCombatUnitTypeString();
		long iTargetCombatUnitType = pUnit->GetCombatUnitTypeLong();
		//if( eDefendersProneState == enum_UseDefendersCurrentProneState &&
		//	pTarget->IsLandedOrDocked() )
		if( bIsLandedOrDocked )
		{
			iTargetCombatUnitType = pUnit->GetCombatUnitTypeWhenLandedOrDocked();
		}

		//return GetAttackVs( strTargetCombatUnitType )->GetDamage();
		return GetAttackDamageVs( iTargetCombatUnitType );//strTargetCombatUnitType );
	}
/*
	long iValue = GetAttackDamageVsPoiObjectType( pTarget->GetPoiObjectType() );

	if( eDefendersProneState == enum_UseDefendersCurrentProneState )
	{
		iValue = GetAttackVs( pTarget )->GetDamage();
		/-*
		EnumPoiObjectType ePoiObjectType = pTarget->GetPoiObjectType();
		if( ePoiObjectType == EnumPoiObjectType::enum_Aircraft ||
			ePoiObjectType == EnumPoiObjectType::enum_Missile ||
			ePoiObjectType == EnumPoiObjectType::enum_Ship ||
			ePoiObjectType == EnumPoiObjectType::enum_Submarine )
		{
			if( pTarget->IsLandedOrDocked() )
			{
				iValue = GetAttackVsSoft()->GetDamage();
			}
		}
		*-/
	}
	return iValue;
	*/
	ASSERT( false );
	return 0;
}

long  CEOSAIUnitTemplate::GetAttackDamageVs( CEOSAIUnitTemplate* pTarget )
{
	//CString strTargetCombatUnitType = pTarget->GetCombatUnitTypeString();
	//return GetAttackDamageVs( strTargetCombatUnitType );
	long iTargetCombatUnitType = pTarget->GetCombatUnitTypeLong();
	return GetAttackDamageVs( iTargetCombatUnitType );
}

long  CEOSAIUnitTemplate::GetAttackDamageVs( long iTargetCombatUnitType )
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVsCombatUnitType( iTargetCombatUnitType );
	ASSERT( pAttackVs );
	if( pAttackVs ){ return pAttackVs->GetDamage(); }
	return 0;
}
/*
long  CEOSAIUnitTemplate::GetAttackDamageVs( CString strTargetCombatUnitType )
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( strTargetCombatUnitType );
	ASSERT( pAttackVs );
	if( pAttackVs ){ return pAttackVs->GetDamage(); }
	return 0;
	//return GetAttackVs( strTargetCombatUnitType )->GetDamage();
}
*/
float CEOSAIUnitTemplate::GetAttackRangeVs( CEOSAIUnitTemplate* pTarget )
{
	//CString strTargetCombatUnitType = pTarget->GetCombatUnitTypeString();
	//CEOSAIAttackVs2* pAttackVs = GetAttackVs( strTargetCombatUnitType );
	long iTargetCombatUnitType = pTarget->GetCombatUnitTypeLong();
	CEOSAIAttackVs2* pAttackVs = GetAttackVsCombatUnitType( iTargetCombatUnitType );
	//ASSERT( pAttackVs );
	if( pAttackVs ){ return pAttackVs->GetAttackRange(); }
	return 0.0f;
	//return GetAttackVs( strTargetCombatUnitType )->GetAttackRange();
}

/*
long CEOSAIUnitTemplate::GetAttackDamageVsPoiObjectType( EnumPoiObjectType ePoiObjectType )
{
	if( ePoiObjectType.IsUnit() )
	{
		if( ePoiObjectType == EnumPoiObjectType::enum_Soft )
		{
			return GetAttackVsSoft()->GetDamage();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Armor )
		{
			return GetAttackVsArmor()->GetDamage();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Aircraft )
		{
			return GetAttackVsAircraft()->GetDamage();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Missile )
		{
			return GetAttackVsMissile()->GetDamage();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Ship )
		{
			return GetAttackVsShip()->GetDamage();
		}
		else if( ePoiObjectType == EnumPoiObjectType::enum_Submarine )
		{
			return GetAttackVsSubmarine()->GetDamage();
		}
		ASSERT( false );
	}
	//CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_City )
	{
		if( this->CanCaptureCities() )
		{
			return 1;
		}
		return 0;
	}
	//CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_Resource )
	{
		if( this->CanCaptureResources() )
		{
			return 1;
		}
		return 0;
	}
	//CFortification* pFort = dynamic_cast< CFortification* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_Fortification )
	{
		if( this->CanCaptureCities() ) // CanCaptureFortifications() )
		{
			return 1;
		}
		return 0;
	}
	//CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
	if( ePoiObjectType == EnumPoiObjectType::enum_Airfield )
	{
		if( this->CanCaptureAirfields() )
		{
			return 1;
		}
		return 0;
	}
	ASSERT( false );
	return 0;
}
*/

/*
float CEOSAIUnitTemplate::GetAttackToHitVsPoiObject( CPoiObject* pPoiObject, Enum_Prone eDefendersProneState )
{
	EnumPoiObjectType eType = pPoiObject->GetPoiObjectType();
	CUnit* pUnit = dynamic_cast< CUnit* >( pPoiObject );
	if( pUnit )
	{
		if( pUnit->IsSoftGroundUnit() )
		{
			return GetAttackVsSoft()->GetToHit();
		}
		else if( pUnit->IsArmor() )
		{
			return GetAttackVsArmor()->GetToHit();
		}
		else if( pUnit->IsAircraft() )
		{
			if( eDefendersProneState == Enum_CanBeProne && pUnit->IsLandedOrDocked() )
			{
				return GetAttackVsSoft()->GetToHit();
			}
			return GetAttackVsAircraft()->GetToHit();
		}
		else if( pUnit->IsMissile() )
		{
			if( eDefendersProneState == Enum_CanBeProne && pUnit->IsLandedOrDocked() )
			{
				return GetAttackVsSoft()->GetToHit();
			}
			return GetAttackVsMissile()->GetToHit();
		}
		else if( pUnit->IsShip() )
		{
			if( eDefendersProneState == Enum_CanBeProne && pUnit->IsLandedOrDocked() )
			{
				return GetAttackVsSoft()->GetToHit();
			}
			return GetAttackVsShip()->GetToHit();
		}
		else if( pUnit->IsSubmarine() )
		{
			if( eDefendersProneState == Enum_CanBeProne && pUnit->IsLandedOrDocked() )
			{
				//return GetAttackVsShipToHit();
				return GetAttackVsSoft()->GetToHit();
			}
			return GetAttackVsSubmarine()->GetToHit();
		}
		ASSERT( false );
	}
	//CCity* pCity = dynamic_cast< CCity* >( pPoiObject );
	if( eType == EnumPoiObjectType::enum_City )
	{
		if( this->CanCaptureCities() )
		{
			return 1.0f;
		}
		/*
		if( this->GetStrategicBombingValue() > 0.0f )
		{
			return this->GetStrategicBombingValue();
		}
		*-/
		return 0.0f;
	}
	//CResourceSource* pResource = dynamic_cast< CResourceSource* >( pPoiObject );
	if( eType == EnumPoiObjectType::enum_Resource )
	{
		if( this->CanCaptureResources() )
		{
			return 1.0f;
		}
		return 0.0f;
	}
	//CFortification* pFort = dynamic_cast< CFortification* >( pPoiObject );
	if( eType == EnumPoiObjectType::enum_Fortification )
	{
		if( this->CanCaptureCities() ) // CanCaptureFortifications() )
		{
			return 1.0f;
		}
		return 0.0f;
	}
	//CAirfield* pAirfield = dynamic_cast< CAirfield* >( pPoiObject );
	if( eType == EnumPoiObjectType::enum_Airfield )
	{
		if( this->CanCaptureAirfields() )
		{
			return 1.0f;
		}
		return 0.0f;
	}
	ASSERT( false );
	return 0.0f;
}
*/
/*
CUnitTemplateCombatAnimationImage* CEOSAIUnitTemplate::GetCombatAnimationDescription( CPoi* pTarget )
{
	CUnitTemplateCombatAnimationImage* pAnimDesc = NULL;
	//pAnimDesc = GetUnitTemplate()->GetCombatAnimationDescription( pTarget );

	CString strAnimation;
	CUnit* pOtherUnit = dynamic_cast< CUnit* >( pTarget );
	if( pOtherUnit )
	{
		CEOSAIAttackVs2* pAttackVs = this->GetAttackVs( pTarget );
		ASSERT( pAttackVs );
		//strUnitAnimationKeyword = pAttackVs->m_strUnitAnimationKeyword;
		strAnimation = pAttackVs->m_strAnimation;
	}
	else
	{
		strAnimation = m_AttackVsCitResFort.m_strAnimation;// m_strAttackVsCityAnimation;
	}

	if( strAnimation == "" )
	{
		pAnimDesc = GetFirstCombatAnimationDescription();
	}
	else
	{
		pAnimDesc = GetCombatAnimationDescription( strAnimation );
		if( pAnimDesc == NULL )
		{
			pAnimDesc = GetFirstCombatAnimationDescription();
		}
	}

	return pAnimDesc;
}
*/
float CEOSAIUnitTemplate::GetDefenseValueForNormalTerrain()
{
	EOSAIEnumMovementType3 eMovementType = GetMovementType();
	if( eMovementType == EOSAIEnumMovementType3_Air )
	{
		//return GetDefenseValue( TERRAIN_AIR );
		if( m_pCombatUnitType )
		{
			return m_pCombatUnitType->GetDefenseMult( TERRAIN_AIR ) * m_fDefenseBaseValue;
		}
	}
	if( eMovementType == EOSAIEnumMovementType3_Water )
	{
		//return GetDefenseValue( TERRAIN_AIR );
		//return m_fDefenseWater;
		if( m_pCombatUnitType )
		{
			return m_pCombatUnitType->GetDefenseMult( TERRAIN_WATER ) * m_fDefenseBaseValue;
		}
	}
	if( eMovementType == EOSAIEnumMovementType3_Land )
	{
		//return GetDefenseValue( TERRAIN_GRASS );
		//return m_fDefenseGrass;
		if( m_pCombatUnitType )
		{
			return m_pCombatUnitType->GetDefenseMult( TERRAIN_GRASS ) * m_fDefenseBaseValue;
		}
	}
	ASSERT( false );
	return 0.0f;
}


float CEOSAIUnitTemplate::GetDefenseValue( long iTerrainType )
{
	if( iTerrainType == TERRAIN_WATER )
	{
		if( IsGroundUnit() && m_pCombatUnitType->GetDefenseMult( TERRAIN_WATER ) == 0.0f )
		{
			return m_pCombatUnitType->GetDefenseMult( TERRAIN_GRASS )*m_fDefenseBaseValue;
		}
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_WATER ) * m_fDefenseBaseValue;
		//return GetDefenseWater();
	}
	if( iTerrainType == TERRAIN_COAST )
	{
		if( IsGroundUnit() && m_pCombatUnitType->GetDefenseMult( TERRAIN_WATER ) == 0.0f )
		{
			return m_pCombatUnitType->GetDefenseMult( TERRAIN_GRASS )*m_fDefenseBaseValue;
		}
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_WATER ) * m_fDefenseBaseValue;
		//return GetDefenseWater();
	}
	if( iTerrainType == TERRAIN_ARCTIC )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_ARCTIC ) * m_fDefenseBaseValue;
		//return GetDefenseArctic();
	}
	if( iTerrainType == TERRAIN_DESERT )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_DESERT ) * m_fDefenseBaseValue;
		//return GetDefenseDesert();
	}
	if( iTerrainType == TERRAIN_GRASS )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_GRASS ) * m_fDefenseBaseValue;
		//return GetDefenseGrass();
	}
	if( iTerrainType == TERRAIN_FOREST )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_FOREST ) * m_fDefenseBaseValue;
		//return GetDefenseForest();
	}
	if( iTerrainType == TERRAIN_CITY )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_CITY ) * m_fDefenseBaseValue;
		//return GetDefenseCity();
	}
	if( iTerrainType == TERRAIN_MOUNTAIN )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_MOUNTAIN ) * m_fDefenseBaseValue;
		//return GetDefenseMountain();
	}
	if( iTerrainType == TERRAIN_SWAMP )
	{
		return m_pCombatUnitType->GetDefenseMult( TERRAIN_SWAMP ) * m_fDefenseBaseValue;
	}
	//ASSERT( false );
	//return 0.0f;// GetDefenseValue();

	// this can happen if a unit is sitting on "unknown" (but I can still see the unit because
	//   it's right on the edge of the unknown)
	return GetDefenseValueForNormalTerrain(); 
}

float CEOSAIUnitTemplate::GetCurrentDefense( long iTerrain, bool bEntrenched, bool bInFort, bool bIsLandedOrDocked )
{
	// Add-up the defense modifiers
	float fDefenseValue = 0.0f;
	if( IsGroundUnit() )
	{
		fDefenseValue = this->GetDefenseValue( iTerrain );
		if( bEntrenched )
		{
			fDefenseValue += this->GetDefenseEntrenchedBonus();
		}
		if( bInFort )
		{
			fDefenseValue += 1.0f;
		}
	}
	else
	{
		if( bIsLandedOrDocked )
		{
			fDefenseValue = this->GetDefenseWhenLandedOrDocked();
		}
		else
		{
			fDefenseValue = this->GetDefenseValueForNormalTerrain();
		}
	}
	return fDefenseValue;
}

float CEOSAIUnitTemplate::GetDefenseEntrenchedBonusPercent()
{
	if( m_pCombatUnitType == NULL )
	{
		m_pCombatUnitType = m_pAIGameRules->GetCombatUnitType( m_iCombatUnitType );
	}
	if( m_pCombatUnitType )
	{
		return m_pCombatUnitType->m_fEntrenchedDefenseMultiplierBonus;
	}
	ASSERT( false );
	return 0.0f;
}

float CEOSAIUnitTemplate::GetDefenseEntrenchedBonus()
{
	if( m_pCombatUnitType == NULL )
	{
		m_pCombatUnitType = m_pAIGameRules->GetCombatUnitType( m_iCombatUnitType );
	}
	if( m_pCombatUnitType )
	{
		return m_fDefenseBaseValue * m_pCombatUnitType->m_fEntrenchedDefenseMultiplierBonus;
	}
	ASSERT( false );
	return 0.0f;
	//CCombatUnitType* pType = m_pUnitset->GetCombatUnitType( m_strCombatUnitType );
	//if( pType ){ return m_fDefenseBaseValue * pType->m_fEntrenchedDefenseMultiplierBonus; }
	//return m_fDefenseBaseValue;
}

float CEOSAIUnitTemplate::GetDefenseMult( long iTerrain )
{
	if( iTerrain == TERRAIN_COAST ) iTerrain = TERRAIN_WATER;
	return m_pCombatUnitType->GetDefenseMult( iTerrain );
}

/*
float CEOSAIUnitTemplate::GetLandedOrDockedDefenseValuePercent()
{
	CCombatUnitType* pType = m_pUnitset->GetCombatUnitType( m_strCombatUnitType );
	if( pType ){ return pType->m_fLandedOrDockedMultiplier; }
	return 0.0f;
}
*/
/*
float CEOSAIUnitTemplate::GetLandedOrDockedDefenseValue()
{
	/*
	CCombatUnitType* pType = m_pUnitset->GetCombatUnitType( m_strCombatUnitType );
	if( pType ){ return m_fDefenceBaseValue * pType->m_fLandedOrDockedMultiplier; }
	return m_fDefenceBaseValue;
	*-/
	return m_fDefenseWhenLandedOrDocked;
}
*/

bool CEOSAIUnitTemplate::IsOfType( EOSAIEnumTypeOfUnit eTypeOfUnit )
{
	if( eTypeOfUnit == EOSAIEnumTypeOfUnit_CityHunter )
	{
		return CanCaptureCities();
	}
	if( eTypeOfUnit == EOSAIEnumTypeOfUnit_CityHunterTransport )
	{
		if( CanContainGroundUnits() )
		{
			// Technically, I need to check whether the transport
			//   can carry units that are CityHunters
			return true;
		}
	}
	return false;
}
/*
float CEOSAIUnitTemplate::GetAntiUnitCombat( CString strCombatUnitType )
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( strCombatUnitType );
	ASSERT( pAttackVs );
	if( pAttackVs )
	{
		return GetMaxHP() * GetDefenseValueForNormalTerrain() * 
			pAttackVs->GetDamage() * pAttackVs->GetToHit();
	}
	return 0.0f;
}
*/
//
// Values 0-10
//                                ---- Movement -----   ------ Combat Value --------------------
// Basic Unit Types:              MoveRate  MoveRange   Normal  Entrenched  Ranged  AttackTarget
// - Ground Units:
//   - Defense Unit (Infantry)        3         -          3        10         5    Ground+Air
//   - Attack Unit (Tank)             7         -         10         1         6    Ground+Air
//   - Ranged Unit (Artillery)        5         -          3         3        10    Ground+Air+Sea
//   - AntiAir Unit                   3         -          3        10        10           Air
// - Air Units:
//   - AntiAir (Fighter)              8         6         10         -         -           Air
//   - AntiShip (Tactical)            8         6         10         -         -    Ground
//   - AntiGround (Tactical)          8         6         10         -         -               Ship
//   - AntiCity (Bomber/Missile)      6        10         10         -         -                   City
// - Sea Units:
//   - AntiShip                       7         -         10         -         3               Ships
//   - AntiSub                        7         -         10         -         -               Subs
//   - AntiAir                        7         -          7         -         7           Air
//   - AntiGround Unit                7         -          7         -        10    Ground
//
// Units fit into these roles:
//   Example:
//     Infantry: Defense Unit (1.0), Attack Unit (0.4), Ranged Unit (0.0), AntiAir (0.0)
//     Destroyer: AntiShip (0.8), AntiSub (1.0), AntiAir (0.6), AntiGround (0.0)
//
// Instead of these categories, I can calculate attacks on each enemy/neutral unit.
//   However, the attack value needs to take into account a bunch of factors, including:
//     Is my unit the same MovementType3 as the target?  (Air->Ground/Air/Sea = good; Ground->Sea = bad)
//     Attack Range (Ground->Sea becomes possible with attack range; if the target doesn't move, it effects where I put my attacker)
//     Comparative MovementRate
//     Sighting (if my unit can't see subs...)
//     Can I expect the enemy unit to stay still (entrenched, city), can I hit it with artillery?
// I need to include a city-attack value, as well.
//

CEOSAIJobCapability*  CEOSAIUnitTemplate::GetAIJobCapability()
{
	if( m_bAIJobCapabilityHasBeenSet == false )
	{
		CalculateAIJobCapability( &m_AIJobCapability );
	}
	return &m_AIJobCapability;
}

void  CEOSAIUnitTemplate::CalculateAIJobCapability( CEOSAIJobCapability* pAIJobCapability )
{
	m_bAIJobCapabilityHasBeenSet = true;
	pAIJobCapability->Clear();

	float fUnitMobility = sqrt( GetMovementRate()/100.0f );
	if( this->HasARange() )
	{
		fUnitMobility = sqrt( GetMovementRate()/100.0f ) + sqrt( GetMovementLimit()/600.0f );
	}
	float fViewRange = sqrt( GetDefaultViewRange()/60.0f );
	//float fAttackRange = sqrt( GetDefaultAttackRange()/60.0f );

	//
	// Exploration, Sighting
	if( ( this->IsGroundUnit() || this->IsSeaUnit() ) ||
		( this->IsAirUnit() && this->CanLandAfterLaunch() ) )
	{
		pAIJobCapability->m_fExploration = fUnitMobility * fViewRange; //1.0;//GetDefaultViewRange();
		pAIJobCapability->m_fSighting = fUnitMobility * fViewRange;//1.0f;//GetDefaultViewRange();
	}

	// Degrade Cities
	pAIJobCapability->m_fKillPopulation = GetKillPopulationToHit01() * min( 8.0f, GetKillPopulationDeaths() );
	pAIJobCapability->m_fDestroyBuildings = GetDestroyBuildingsToHit01() * min( 3.0f, GetDestroyBuildingsDamage() );
	pAIJobCapability->m_fInProductionDamage = GetDamageToItemsInProductionToHit01() * (GetDamageToItemsInProductionDamage()/100.0f);

	// Aircraft
	pAIJobCapability->m_fAirEscort = 0.0f;
	pAIJobCapability->m_fAntiAir = 0.0f;  // anti-aircaft guns, fighters, SAM sites

	// Sea Units
	if( IsSeaUnit() )
	{
		pAIJobCapability->m_fSubmarineCapability = 0.0f;  // strength, speed, stealth of submarines
		pAIJobCapability->m_fAntiSubmarine = 0.0f;

		pAIJobCapability->m_fSeaResHunter = IsSeaResHunter() ? 1.0f : 0.0f;
		pAIJobCapability->m_fSeaSupremacy = 0.0f;  // ship-to-ship combat
		pAIJobCapability->m_fShoreBombardment = 0.0f;
	}
	// Ground (note: if GroundCitResHunter > 1, the minimum need is still 1)
	if( IsGroundUnit() )
	{
		pAIJobCapability->m_fGroundCitResHunter = IsGroundCitResHunter() ? 1.0f : 0.0f;

		pAIJobCapability->m_fGroundAttackUnits = 1.0f;  // units that can attack ground units (ground units, aircraft, ..)
		pAIJobCapability->m_fGroundDefenseUnits = 1.0f;
	}
	pAIJobCapability->m_fInvasionDefense = 1.0f;    // includes aircraft, coastal ships to attack transports, ...
	pAIJobCapability->m_iAirfieldBuilder = CanBuildAirfields() ? 1 : 0;

	// Transports/Carriers
	if( GetNumberOfGroundUnitsThatICanContain() > 0 &&
		CanPickupGroundUnitsAnywhere() && 
		CanDropoffGroundUnitsAnywhere() )
	{
		pAIJobCapability->m_fTransportCapability = (float) GetNumberOfGroundUnitsThatICanContain();
	}
	if( this->IsSubmarine() )
	{
		pAIJobCapability->m_fSubmarineCapability = GetSubmarineCapability();
	}
	if( IsAirUnitCarrier() && 
		IsLaunchingPadForAirUnits() && 
		IsLandingPadForAirUnits() )
	{
		pAIJobCapability->m_fCarrierCapability = (float) GetNumberOfAirUnitsThatICanContain();
	}

	if( GetAIUnitCombatCapability()->IsACombatUnit() )
	{
		pAIJobCapability->m_UnitTemplatesForCombat.AddTail( this );
	}
}

CEOSAIUnit2CombatCapability* CEOSAIUnitTemplate::GetAIUnitCombatCapability()
{
	ASSERT( g_pEOSAICommonData );
	return &m_AIUnitCombatCapability;
}

float CEOSAIUnitTemplate::GetExplorationCapability()
{
	// Airplanes are better than ground units, sea units because they can fly over
	//   ground and water.  However, they have a limited range.  
	//   I'm going to call this a wash and ignore those effects.
	return GetMovementRate()*GetDefaultViewRange()/10000.0f;
}

float CEOSAIUnitTemplate::GetSightingCapability()
{
	// Airplanes are better than ground units, sea units because they can fly over
	//   ground and water.  However, they have a limited range.  
	//   I'm going to call this a wash and ignore those effects.
	float f1 = GetMovementRate()*GetDefaultViewRange();
	float f2 = GetDefaultViewRange()*GetDefaultViewRange()*3.14f;
	return (f1 + f2)/10000.0f;
}

float CEOSAIUnitTemplate::GetGroundCitResHunterCapacity()
{
	if( IsGroundUnit() && CanCaptureCities() )
	{ 
		return sqrt( GetMovementRate() );
	}
	return 0.0f;
}

float CEOSAIUnitTemplate::GetStrategicBombingCapability()
{
	//return GetStrategicBombingValue() * GetDefenseValueForNormalTerrain() * GetMovementRange() / 1000.0f;
	return 1.0f * GetDefenseValueForNormalTerrain() * GetMovementLimit() / 1000.0f;
}
/*
float CEOSAIUnitTemplate::GetTacticalBombingCapability()
{
	//return GetTacticalBombingValue() * GetDefenseValue() * GetPixelRange();
	return GetAttackVsSoft()->GetToHit() * 
		   GetAttackVsShip()->GetToHit() * 
		   GetDefenseValueForNormalTerrain() * GetMovementLimit();
}
*/
float  CEOSAIUnitTemplate::GetAirEscortCapability()
{
	return 0.0f;
	ASSERT( false );
	/*
	if( this->IsAircraft() )
	{
		if( GetAttackVsAircraft() )
		{
			return GetAttackVsAircraft()->GetToHit() * GetDefenseValueForNormalTerrain() / 3.0f;
		}
	}
	*/
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetAntiAirCapability()
{
	/*
	POSITION pos = GetAttackVsList()->GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = GetAttackVsList()->GetNext( pos );
		this->GetUnitset()->GetCombatUnitType( asda
		if( pAttackVs->GetCombatUnitType()
	}
	*/
	return 0.0f;
	ASSERT( false );
	/*
	if( GetAttackVsAircraft() )
	{
		if( GetAttackVsAircraft() )
		{
			return GetAttackVsAircraft()->GetToHit() * GetDefenseValueForNormalTerrain() / 3.0f;
		}
	}
	*/
	return 0.0f;
}

float CEOSAIUnitTemplate::GetSubmarineCapability()
{
	if( this->IsSubmarine() )
	{
		return 1.0f;
	}
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetAntiSubmarineCapability()
{
	return 0.0f;
	ASSERT( false );
	//return GetAttackVs( EnumPoiObjectType::enum_Submarine )->GetToHit() * GetSubmarineDetect() * GetSubmarineDetectRange() * GetDefenseValueForNormalTerrain() / 120.0f;
	/*
	if( GetAttackVsSubmarine() )
	{
		return GetAttackVsSubmarine()->GetToHit() * GetDefenseValueForNormalTerrain() / 120.0f;
	}
	*/
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetLandCitResHunterCapability()
{
	if( IsGroundUnit() && this->CanCaptureCities() )
	{
		return 1.0f;
	}
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetBombardmentCapability()
{
	//return GetBombardRange() * GetBombardValue() / 15.0f;
	return 0.0f;
}
/*
float  CEOSAIUnitTemplate::GetOffensiveGroundCapability()
{
	if( IsGroundUnit() )
	{
		return GetAttackVsSoft()->GetToHit() * 
			   GetAttackVsArmor()->GetToHit() * 
			   GetDefenseValueForNormalTerrain() / 5.0f;
	}
	return 0.0f;
}
*/
/*
float  CEOSAIUnitTemplate::GetDefensiveGroundCapability()
{
	if( IsGroundUnit() )
	{
		/-*
		if( GetDefenseEntrenched() != GetDefenseValueForNormalTerrain() )
		{
			//return GetAttackVsGround() * GetDefenseEntrenched() / 5.0f;
			return GetAttackVs( EnumPoiObjectType::enum_Soft )->GetToHit() * 
				   GetAttackVs( EnumPoiObjectType::enum_Armor )->GetToHit() * 
				   GetDefenseEntrenched() / 5.0f;
		}
		//return GetAttackVsGround() * GetDefenseValue() / 5.0f;
		return GetAttackVs( EnumPoiObjectType::enum_Soft )->GetToHit() * 
			   GetAttackVs( EnumPoiObjectType::enum_Armor )->GetToHit() * 
			   GetDefenseValueForNormalTerrain() / 5.0f;
		*-/
		return GetAttackVsSoft()->GetToHit() * 
			   GetAttackVsArmor()->GetToHit() * 
			   ( GetDefenseValueForNormalTerrain() + GetDefenseEntrenchedBonus() ) / 5.0f;
	}
	return 0.0f;
}
*/
float  CEOSAIUnitTemplate::GetInvasionDefense()
{
	return 0.0f;
	ASSERT( false );
	// INCOMPLETE: I need to add more variables
	//    like adding coastal defense ships
	/*
	if( IsAirUnit() )
	{
		if( GetAttackVsShip() )
		{
			return GetAttackVsShip()->GetToHit() * GetDefenseValueForNormalTerrain();
		}
	}
	*/
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetSeaResourceHunterCapability()
{
	if( IsSeaUnit() && this->CanCaptureResources() )
	{
		return 1.0f;
	}
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetSeaSupremacyCapability()
{
	return 0.0f;
	ASSERT( false );
	/*
	if( this->IsSeaUnit() )
	{
		if( GetAttackVsShip() )
		{
			return GetAttackVsShip()->GetToHit() * GetDefenseValueForNormalTerrain();
		}
	}
	*/
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetShoreBombardmentCapability()
{
	if( this->IsSeaUnit() )
	{
		//return GetBombardRange() * GetBombardValue() / 15.0f;
	}
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetAirfieldBuilderCapacity()
{
	if( IsGroundUnit() && CanBuildAirfields() )
	{
		return 1.0f;
	}
	return 0.0f;
}

float  CEOSAIUnitTemplate::GetGroundTransportCapability()
{
	if( this->IsSeaUnit() && this->GetNumberOfGroundUnitsThatICanContain() > 0 )
	{
		return GetNumberOfGroundUnitsThatICanContain() * sqrt(GetMovementRate()) / 3.0f;
	}
	return 0.0f;
}


// Combat
//

void CEOSAIUnitTemplate::GetUnitStrength( CString* pstrUnitType, float* pfValue )
{
	*pstrUnitType = GetCombatUnitTypeString();
	*pfValue = GetMaxHP() * GetDefenseValueForNormalTerrain();
}

float CEOSAIUnitTemplate::GetExpectedHPDamage( CEOSAIUnit2* pTargetUnit, bool bUseNormalTerrainIfBetterThanCurrentDefense )
{
	CEOSAIAttackVs2* pAttackVs = this->GetAttackVsCombatUnitType( pTargetUnit->GetCombatUnitTypeLong() );
	if( pAttackVs )
	{
		float fTargetDefense = pTargetUnit->GetCurrentDefense();
		if( bUseNormalTerrainIfBetterThanCurrentDefense )
		{
			fTargetDefense = max( pTargetUnit->GetDefenseValueForNormalTerrain(), fTargetDefense );
		}
		float fDamage = (float) pAttackVs->GetDamage();
		if( pAttackVs->m_iExtraDamagePercentage > 0 )
		{
			float fExtraDamagePercentage = 0.01f * pAttackVs->m_iExtraDamagePercentage;
			fDamage += fExtraDamagePercentage * pAttackVs->m_iExtraDamage;
		}
		return pAttackVs->GetToHit() * fDamage / fTargetDefense;
	}
	return 0.0f;
}

float CEOSAIUnitTemplate::GetExpectedHPDamage( CEOSAIUnitTemplate* pTargetUnitTemplate )
{
	CEOSAIAttackVs2* pAttackVs = this->GetAttackVsCombatUnitType( pTargetUnitTemplate->GetCombatUnitTypeLong() );
	if( pAttackVs )
	{
		float fDamage = (float) pAttackVs->GetDamage();
		if( pAttackVs->m_iExtraDamagePercentage > 0 )
		{
			float fExtraDamagePercentage = 0.01f * pAttackVs->m_iExtraDamagePercentage;
			fDamage += fExtraDamagePercentage * pAttackVs->m_iExtraDamage;
		}
		//return pAttackVs->GetToHit() * pAttackVs->GetDamage() / pTargetUnitTemplate->GetDefenseValueForNormalTerrain();
		return pAttackVs->GetToHit() * fDamage / pTargetUnitTemplate->GetDefenseValueForNormalTerrain();
	}
	return 0.0f;
}

/*
float CEOSAIUnitTemplate::GetUnitStrengthSoft() // return 0.0 unless this is a soft unit (I need to know who my targets will be)
{
	if( IsSoftGroundUnit() == false ) return 0.0f;
	return GetMaxHP() * GetDefenseValueForNormalTerrain();
}

float CEOSAIUnitTemplate::GetUnitStrengthArmor()
{
	if( IsArmor() == false ) return 0.0f;
	return GetMaxHP() * GetDefenseValueForNormalTerrain();
}

float CEOSAIUnitTemplate::GetUnitStrengthAircraft()
{
	if( IsAircraft() == false ) return 0.0f;
	return GetMaxHP() * GetDefenseValueForNormalTerrain();
}

float CEOSAIUnitTemplate::GetUnitStrengthMissile()
{
	if( IsMissile() == false ) return 0.0f;
	return GetMaxHP() * GetDefenseValueForNormalTerrain();
}

float CEOSAIUnitTemplate::GetUnitStrengthShip()
{
	if( IsShip() == false ) return 0.0f;
	return GetMaxHP() * GetDefenseValueForNormalTerrain();
}

float CEOSAIUnitTemplate::GetUnitStrengthSubmarine()
{
	if( IsSubmarine() == false ) return 0.0f;
	return GetMaxHP() * GetDefenseValueForNormalTerrain();
}
*/

float CEOSAIUnitTemplate::GetAICombatSignificance()
{
	return m_AIUnitCombatCapability.GetCombatSignificance();
}
/*
float CEOSAIUnitTemplate::GetAIPowerValue()
{
	// Count the largest value extra (specialized units can sometimes be extra-dangerous)
	//   Plus, an infantry might have no combat values against subs or satellites, but that shouldn't count
	//   against its ground combat abilities
	float fLargestValue = 0.0f; 
	float fTotal = 0.0f;
	POSITION pos = GetCommonState()->GetActiveUnitset()->GetCombatUnitTypes()->GetHeadPosition();
	while( pos )
	{
		CCombatUnitType* pCombatUnitType = GetCommonState()->GetActiveUnitset()->GetCombatUnitTypes()->GetNext( pos );
		CEOSAIAttackVs2* pAttackVs = GetAttackVs( pCombatUnitType->m_strName );

		if( pAttackVs )
		{
			float fValue = GetDefenseValueForNormalTerrain() * GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
			fLargestValue = max( fLargestValue, fValue );
			fTotal += fValue;
		}
	}
	return 
		( fTotal + 2.0f*fLargestValue ) / 
		(GetCommonState()->GetActiveUnitset()->GetUnitCategories()->GetCount() + 2);
}
*/
EOSAI::StringAndFloatSet  CEOSAIUnitTemplate::GetCombatPowerVsUnitTypes()
{
	EOSAI::StringAndFloatSet  CombatPowerVsUnitTypes;
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
		CombatPowerVsUnitTypes.Set( pAttackVs->GetCombatUnitTypeString(),
			GetDefenseValueForNormalTerrain() * GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit() );
	}
	return CombatPowerVsUnitTypes;
}

float CEOSAIUnitTemplate::GetProductionAndIronCost1()
{
	return GetProductionCost() + GetIronCost();
}

// Multiply the Iron, Oil costs
float CEOSAIUnitTemplate::GetProductionAndIronCost( CEOSAICitResValueMultiplier* pCitResValueMultiplier )
{
	return
		pCitResValueMultiplier->m_fCityProductionValueMultiplier * this->GetProductionCost() +
		pCitResValueMultiplier->m_fIronValueMultiplier * this->GetIronCost();
}

/*
void  CEOSAIUnitTemplate::GetCombatPowerVsUnitTypes( EOSAI::StringAndFloatSet* pSet )
{
	pSet->Clear();
	//
	POSITION pos = m_AttackVsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIAttackVs2* pAttackVs = m_AttackVsList.GetNext( pos );
		pSet->Set( pAttackVs->GetCombatUnitType(),
			GetDefenseValueForNormalTerrain() * GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit() );
	}
}

float CEOSAIUnitTemplate::GetAICombatVsSoft() // How well can this unit attack my Soft units?
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( EnumPoiObjectType::enum_Soft );
	ASSERT( pAttackVs );
	return GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
}

float CEOSAIUnitTemplate::GetAICombatVsArmor()
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( EnumPoiObjectType::enum_Armor );
	ASSERT( pAttackVs );
	return GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
}

float CEOSAIUnitTemplate::GetAICombatVsAircraft()
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( EnumPoiObjectType::enum_Aircraft );
	ASSERT( pAttackVs );
	return GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
}

float CEOSAIUnitTemplate::GetAICombatVsMissile()
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( EnumPoiObjectType::enum_Missile );
	ASSERT( pAttackVs );
	return GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
}

float CEOSAIUnitTemplate::GetAICombatVsShip()
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( EnumPoiObjectType::enum_Ship );
	ASSERT( pAttackVs );
	return GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
}

float CEOSAIUnitTemplate::GetAICombatVsSubmarine()
{
	CEOSAIAttackVs2* pAttackVs = GetAttackVs( EnumPoiObjectType::enum_Submarine );
	ASSERT( pAttackVs );
	return GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
}
*/

/*
float  CEOSAIUnitTemplate::GetTradeCapability()
{
	return 1.0f;
}
*/

/*
CString CEOSAIUnitTemplate::GetHtmlDesc()
{
	ASSERT( false ); // want to eliminate UI-UnitTemplate.html

	CString strHtml;

	CString strPathToHtml;
	strPathToHtml  = g_pApp->GetBaseDirectory();
	strPathToHtml += "\\Data\\Manual\\html\\UI-UnitTemplate.html";
    // C:\Programming\EOS\Build\Data\Manual\html

	FILE* fp = fopen( strPathToHtml, "r" );
	if( fp == NULL ) return "";
	char buffer[100001];
	size_t iBytesRead = fread( (void*) buffer, sizeof( char ), 100000, fp );
	buffer[iBytesRead] = '\0';
	strHtml = buffer;
	fclose( fp );

	// Custom stuff
	{
		// Drop in the unit information
		//ASSERT( m_pUnit );
		CUnitTemplate* pUnitTemplate = this; // m_pUnit->GetUnitTemplate();
		ASSERT( pUnitTemplate );

		CString strTemp;
		CString strText;

		// Find the [ ID="UnitImage" ] tag.
		//long iUnitImageTag = strHtml.Find( _T("ID=\"UnitImage\"") );
		//ASSERT( iUnitImageTag );
		// Find the subsequent [ SRC= ] tag.
		//long iUnitImageSrcTag = strHtml.Find( _T("SRC=") );
		//ASSERT( iUnitImageSrcTag );
		//
		// Replace the Image
		CString strImagePath = g_pApp->GetBaseDirectory() + "\\" + pUnitTemplate->GetWhiteBitmapPath();
		strHtml.Replace( "../assets/images/Bomber-0010-a.gif", strImagePath ); // g_pApp->GetBaseDirectory() + "\\Data\\Images\\Units\\Armor-0010-White.bmp" );
		// Replace the UnitName tag
		strHtml.Replace( "UnitName", pUnitTemplate->GetExternalName() );
		// Find the UnitDetails table
		long iUnitDetailsTable = strHtml.Find( "ID=\"UnitDetails\"" );
		long iUnitDetailsRow = strHtml.Find( "<TR>", iUnitDetailsTable );
		long iUnitDetailsTableEnd = strHtml.Find( "</TABLE>", iUnitDetailsRow );
		ASSERT( iUnitDetailsTable != -1 );
		ASSERT( iUnitDetailsRow != -1 );
		ASSERT( iUnitDetailsTableEnd != -1 );
		// Cut out the existing UnitDetails
		strHtml.Delete( iUnitDetailsRow, iUnitDetailsTableEnd - iUnitDetailsRow );

		CString strNewInformation;

		// Combat information
		int iRow = 0;
		if( this->AttackMethodIsBombing() )
		{
			bool bAttackValueFound = false;

			strNewInformation += RowHtml( "Attack", "", "", -2 );
			if( GetAttackVs( EnumPoiObjectType::enum_Aircraft )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Aircraft )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Air Units", "", strText, 0 );
				bAttackValueFound = true;
			}
			strNewInformation += RowHtml( "", "", "", -1 );
			strNewInformation += RowHtml( "Bombing", "", "", -2 );
			iRow = 0;
			/*
			if( GetAttackVsGround() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVsGround() );
				strNewInformation += RowHtml( "&nbsp; vs. Ground Units", "", strText, iRow ); iRow++;
				bAttackValueFound = true;
			}
			*-/
			if( GetAttackVs( EnumPoiObjectType::enum_Soft )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Soft )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Soft Ground Units", "", strText, iRow ); iRow++;
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Armor )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Armor )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Armor", "", strText, iRow ); iRow++;
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Ship )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Ship )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Ships", "", strText, iRow ); iRow++;
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Submarine )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Submarine )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Submarines", "", strText, iRow ); iRow++;
				bAttackValueFound = true;
			}
			/*
			if( GetStrategicBombingValue() > 0.0f )
			{
				strText.Format( "%0.1f", GetStrategicBombingValue() );
				strNewInformation += RowHtml( "&nbsp; Strategic Bombing", "", strText, iRow ); iRow++;
				bAttackValueFound = true;
			}
			*-/
			//strNewInformation += RowHtml( "&nbsp; vs. Ground Units", "", "2", 0 );
			//strNewInformation += RowHtml( "&nbsp; vs. Ships", "", "2", 1 );
			//strNewInformation += RowHtml( "&nbsp; vs. Submarines", "", "2", 2 );
			//strNewInformation += RowHtml( "&nbsp; Strategic Bombing", "", "2", 3 );
			if( bAttackValueFound == false )
			{
				strNewInformation += RowHtml( "( none )", "", "", iRow ); iRow++;				
			}
		}
		else
		{
			bool bAttackValueFound = false;

			strNewInformation += RowHtml( "Attack", "", "", -2 );

			iRow = 0;
			/*
			if( GetAttackVsGround() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVsGround() );
				strNewInformation += RowHtml( "&nbsp; vs. Ground Units", "", strText, iRow++ );
				bAttackValueFound = true;
			}
			*-/
			if( GetAttackVs( EnumPoiObjectType::enum_Soft )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Soft )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Soft Ground Units", "", strText, iRow++ );
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Armor )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Armor )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Armor", "", strText, iRow++ );
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Aircraft )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Aircraft )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Air Units", "", strText, iRow++ );
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Ship )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Ship )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Ships", "", strText, iRow++ );
				bAttackValueFound = true;
			}
			if( GetAttackVs( EnumPoiObjectType::enum_Submarine )->GetToHit() > 0.0f )
			{
				strText.Format( "%0.1f", GetAttackVs( EnumPoiObjectType::enum_Submarine )->GetToHit() );
				strNewInformation += RowHtml( "&nbsp; vs. Submarines", "", strText, iRow++ );
				bAttackValueFound = true;
			}
			/*
			if( this->GetBombardRange() > 0.0f )
			{
				CString strText2;
				strText.Format( "&nbsp; Bombard (Range %0.0f)", this->GetBombardRange() );
				strText2.Format( "%0.1f", GetBombardValue() );
				strNewInformation += RowHtml( strText, "", strText2, iRow++ );
				bAttackValueFound = true;
			}
			*-/

			if( bAttackValueFound == false )
			{
				strNewInformation += RowHtml( "( none )", "", "", iRow ); iRow++;				
			}
		}
		strNewInformation += RowHtml( "", "", "", -1 );

		strNewInformation += RowHtml( "Defense", "", "", -2 );
		strText.Format( "%0.1f", GetDefenseValueForNormalTerrain() );
		strNewInformation += RowHtml( "&nbsp; Normal Conditions", "", strText, 0 ); iRow = 1;

		if( this->IsAircraft() )
		{
			strText.Format( "%0.1f", GetLandedOrDockedDefenseValue()  );
			strNewInformation += RowHtml( "&nbsp; When Landed", "", strText, 3 ); iRow++;
		}
		if( this->IsSeaUnit() )
		{
			strText.Format( "%0.1f", GetLandedOrDockedDefenseValue()  );
			strNewInformation += RowHtml( "&nbsp; When Docked", "", strText, 2 ); iRow++;
		}
		if( this->IsGroundUnit() )
		{
			if( this->GetDefenseEntrenchedBonus() != 0.0f ||
			    this->GetDefenseArctic()   != this->GetDefenseGrass() ||
			    this->GetDefenseForest()   != this->GetDefenseGrass() ||
			    this->GetDefenseDesert()   != this->GetDefenseGrass() ||
			    this->GetDefenseMountain() != this->GetDefenseGrass() ||
			    this->GetDefenseCity()     != this->GetDefenseGrass() )
			{
				strNewInformation += RowHtml( "&nbsp; Modifiers", "", "", 5 ); iRow++;

				if( this->GetDefenseEntrenchedBonus() != 0.0f )
				{
					strText.Format( "+ %0.1f", this->GetDefenseEntrenchedBonus()  );
					strNewInformation += RowHtml( "&nbsp; &nbsp; When Entrenched", "", strText, iRow++ );
				}
				if( this->GetDefenseArctic() != this->GetDefenseGrass() )
				{
					strText.Format( "%0.1f", this->GetDefenseArctic() );
					strNewInformation += RowHtml( "&nbsp; &nbsp; In Arctic", "", strText, iRow++ );
				}
				if( this->GetDefenseForest() != this->GetDefenseGrass() )
				{
					strText.Format( "%0.1f", this->GetDefenseForest() );
					strNewInformation += RowHtml( "&nbsp; &nbsp; In Forest", "", strText, iRow++ );
				}
				if( this->GetDefenseDesert() != this->GetDefenseGrass() )
				{
					strText.Format( "%0.1f", this->GetDefenseDesert() );
					strNewInformation += RowHtml( "&nbsp; &nbsp; In Desert", "", strText, iRow++ );
				}
				if( this->GetDefenseMountain() != this->GetDefenseGrass() )
				{
					strText.Format( "%0.1f", this->GetDefenseMountain() );
					strNewInformation += RowHtml( "&nbsp; &nbsp; In Mountains", "", strText, iRow++ );
				}
				if( this->GetDefenseCity() != this->GetDefenseGrass() )
				{
					strText.Format( "%0.1f", this->GetDefenseCity() );
					strNewInformation += RowHtml( "&nbsp; &nbsp; In City", "", strText, iRow++ );
				}
			}
		}
		strNewInformation += RowHtml( "", "", "", -1 );

		iRow = 0;
		/*
		if( this->GetSubmarineDetect() > 0.0f )
		{
			CString strText;
			strText.Format( "%0.1f", this->GetSubmarineDetect() );
			strNewInformation += RowHtml( "Submarine Detection", "", strText, iRow ); iRow++;
			strText.Format( "%0.1f", this->GetSubmarineDetectRange() );
			strNewInformation += RowHtml( "Submarine Detection Range", "", strText, iRow ); iRow++;
		}
		*-/
		/*
		if( this->GetSubmarineStealth() > 0.0f )
		{
			CString strText;
			strText.Format( "%0.1f", this->GetSubmarineStealth() );
			strNewInformation += RowHtml( "Stealth", "", strText, iRow ); iRow++;
		}
		*-/
		//strNewInformation += RowHtml( "", "", "", -1 );

		if( this->GetNumberOfUnitsThatICanContain() > 0 )
		{
			CString strText;
			strText.Format( "%d", this->GetNumberOfUnitsThatICanContain() );
			strNewInformation += RowHtml( "Number of Units I can carry", "", strText, iRow ); iRow++;
			//strNewInformation += RowHtml( "", "", "", -1 );
		}

		// Movement information
		strTemp.Format( "%2.0f", pUnitTemplate->GetMovementRate() );
		strNewInformation += RowHtml( "Movement Rate", "", strTemp, iRow ); iRow++;

		/*
		if( pUnitTemplate->MustLandBetweenTurns() )
		{
			strTemp.Format( "%2.0f", pUnitTemplate->GetMovementRange() );
			strNewInformation += RowHtml( "Movement Range", "", strTemp, iRow ); iRow++;
		}
		*-/
		strNewInformation += RowHtml( "", "", "", -1 );

		// Hit Points
		strTemp.Format( "%d", pUnitTemplate->GetMaxHP() );
		strNewInformation += RowHtml( "Hit Points", "", strTemp, 0 );
		strNewInformation += RowHtml( "", "", "", -1 );

		// Cost information
		strNewInformation += RowHtml( "Cost:", "", "", -2 );

		strTemp.Format( "%d", pUnitTemplate->GetProductionCost() );
		strNewInformation += RowHtml( "&nbsp; Production", "", strTemp, 0 );

		if( pUnitTemplate->GetIronCost() > 0.0f )
		{
			strTemp.Format( "%0.1f", pUnitTemplate->GetIronCost() );
			strNewInformation += RowHtml( "&nbsp; Iron", "", strTemp, 1 );
		}
		strNewInformation += RowHtml( "", "", "", -1 );

		// Maintenance information
		if( pUnitTemplate->GetOilConsumption() > 0 || pUnitTemplate->AmountOfFoodNeededThisTurn() > 0 )
		{
			strNewInformation += RowHtml( "Maintenance:", "", "", -2 );
			iRow = 0;

			if( pUnitTemplate->AmountOfFoodNeededThisTurn() > 0 )
			{
				strTemp.Format( "%0.2f", pUnitTemplate->AmountOfFoodNeededThisTurn() );
				strNewInformation += RowHtml( "&nbsp; Food", "", strTemp, iRow ); iRow++;
			}
			if( pUnitTemplate->GetOilConsumption() > 0 )
			{
				strTemp.Format( "%0.2f", pUnitTemplate->GetOilConsumption() );
				strNewInformation += RowHtml( "&nbsp; Oil (when moving)", "", strTemp, iRow ); iRow++;
			}
			strNewInformation += RowHtml( "", "", "", -1 );
		}

		strHtml.Replace( "UnitNotes", pUnitTemplate->GetNotesText() );

		//strNewInformation += RowHtml( "Submarine Detection!", "", "2", 0 );
		//strNewInformation += RowHtml( "", "", "", -1 );

		//CString strNewInformation = 
		//	"<TR><TD WIDTH=194 STYLE=\"background-image: url('../assets/images/Table-Background-1-Top.gif');\"><P>&nbsp;Submarine Detection</P></TD><TD WIDTH=68 STYLE=\"background-image: url('../assets/images/Table-Background-1-Top.gif');\"><P STYLE=\"text-align: center;\">1</P></TD></TR>";
		strHtml.Insert( iUnitDetailsRow, strNewInformation );

		// Change the path information
		strHtml.Replace( "../assets/", "%PathToAssets%" );

		//long iImageStart = strHtml

		// Find the <Information> tag, 
		// Rewind to the closest <td> tag, 
		// Forward to the </td>,
		// Replace this section with <

	}

	// Change the %PathToAssets% string to a real value
	CString strPathToAssets;
	strPathToAssets  = g_pApp->GetBaseDirectory();
	strPathToAssets += "\\Data\\Manual\\assets/";
	strHtml.Replace( "%PathToAssets%", strPathToAssets );

	CString strPathToHtmlPages;
	strPathToHtmlPages  = g_pApp->GetBaseDirectory();
	strPathToHtmlPages += "\\Data\\Manual\\html/";
	strHtml.Replace( "../html", strPathToHtmlPages );

	return strHtml;
}
*/
/*
CString CEOSAIUnitTemplate::RowHtml( CString strColumn1, 
							    CString strLink,
							    CString strColumn2,
							    long iRowNumber )
{
	CString strBackgroundImage;
	if( iRowNumber == -2 )
	{
		strBackgroundImage = "Table-Background-0.gif";
	}
	else if( iRowNumber == -1 )
	{
		strBackgroundImage = "Table-Background-0-Top.gif";
	}
	else if( iRowNumber == 0 )
	{
		strBackgroundImage = "Table-Background-1-Top.gif";
	}
	else if( iRowNumber%2 == 0 )
	{ 
		strBackgroundImage = "Table-Background-1.gif";
	}
	else
	{
		strBackgroundImage = "Table-Background-2.gif";
	}

	CString strHtml;
	strHtml = "<TR><TD WIDTH=194 STYLE=\"background-image: url('%PathToAssets%/images/";
	strHtml += strBackgroundImage;
	strHtml += "');\">"\
		"<P>&nbsp;";
	if( strLink.IsEmpty() == false )
	{
		strHtml += "<A HREF=\"";
		strHtml += strLink;
		strHtml += "\">";
	}
	strHtml += strColumn1;
	strHtml +=  "</A></P>"\
                "</TD>"\
                "<TD WIDTH=76 STYLE=\"background-image: url('%PathToAssets%/images/";
	strHtml += strBackgroundImage;
	strHtml += "');\">"\
                    "<P STYLE=\"text-align: center;\">";
	strHtml += strColumn2;
	strHtml += "</P>"\
               "</TD>"\
               "</TR>";

	return strHtml;
}
*/

