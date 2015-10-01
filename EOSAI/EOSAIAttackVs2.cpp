
#include "stdafx.h"
#include "EOSAIAttackVs2.h"
#include "EOSAICombatUnitType.h"
#include "EOSAIUnitTemplate.h"

//

CEOSAIAttackVs2::CEOSAIAttackVs2()
{
	m_pParentAIUnitTemplate = NULL;
	//m_eCombatUnitType = EnumPoiObjectType::enum_Undefined;
	//
	//m_bStandardRangePlus = false;
	//m_fAdditionalRange = 0.0f;
	//
	//m_bOneAttackBetweenLandings = false;
	m_fToHit = 0.0f;
	m_bAlwaysHits = false;
	m_iDamage = 0;
	m_iExtraDamage = 0;
	m_iExtraDamagePercentage = 0; // 0-100

	m_iAttackVsCombatUnitType = -2;

	//m_bAttackRangeIsDirectlyOverTarget = false;
	m_bStandardAttackRange = false;
	m_fAttackRange = 0.0f;
	m_fRateOfFire = 100.0f; // 100 attacks per turn
	//
	m_iAircraftTargetPriority = 0;
	//m_PauseBetweenAnimations = 0.0f;
}


void  CEOSAIAttackVs2::Serialize( CEOSAISerial* pSerial )
{
	char cVersion = 3;
	pSerial->Serialize( cVersion );

	/*
	if( cVersion <= 1 )
	{
		pSerial->SerializeANSI8( m_strCombatUnitType );
		pSerial->SerializeANSI8( m_strAnimation );
	}
	if( cVersion == 2 )
	{
		#ifdef UNICODE
		pSerial->SerializeUnicode( m_strCombatUnitType );
		pSerial->SerializeUnicode( m_strAnimation );
		#else
		pSerial->SerializeANSI8( m_strCombatUnitType );
		pSerial->SerializeANSI8( m_strAnimation );
		#endif UNICODE
	}
	*/
	if( cVersion >= 3 )
	{	
		pSerial->Serialize( m_iAttackVsCombatUnitType );
		pSerial->SerializeUnicode( m_strAnimation );
	}

	pSerial->Serialize( m_bAlwaysHits );
	pSerial->Serialize( m_fToHit );
	pSerial->Serialize( m_iDamage );

	pSerial->Serialize( m_iExtraDamage );
	pSerial->Serialize( m_iExtraDamagePercentage );

	pSerial->Serialize( m_fRateOfFire );
	pSerial->Serialize( m_bStandardAttackRange );
	pSerial->Serialize( m_fAttackRange );
	pSerial->Serialize( m_iAircraftTargetPriority );
}

void  CEOSAIAttackVs2::Deserialize( CEOSAISerial* pSerial )
{
	char cVersion = 1;
	pSerial->Deserialize( cVersion );

	if( cVersion <= 1 )
	{
		CString strCombatUnitType;
		pSerial->DeserializeANSI8( strCombatUnitType );
		SetCombatUnitTypeByExternalName( strCombatUnitType );

		pSerial->DeserializeANSI8( m_strAnimation );
	}
	if( cVersion == 2 )
	{
		#ifdef UNICODE
		CString strCombatUnitType;
		pSerial->DeserializeUnicode( strCombatUnitType );
		SetCombatUnitTypeByExternalName( strCombatUnitType );

		pSerial->DeserializeUnicode( m_strAnimation );
		#else
		pSerial->DeserializeANSI8( m_strCombatUnitType );
		pSerial->DeserializeANSI8( m_strAnimation );
		#endif UNICODE
	}
	if( cVersion >= 3 )
	{
		pSerial->Deserialize( m_iAttackVsCombatUnitType );
		pSerial->DeserializeUnicode( m_strAnimation );
	}

	pSerial->Deserialize( m_bAlwaysHits );
	pSerial->Deserialize( m_fToHit );
	pSerial->Deserialize( m_iDamage );

	pSerial->Deserialize( m_iExtraDamage );
	pSerial->Deserialize( m_iExtraDamagePercentage );

	pSerial->Deserialize( m_fRateOfFire );
	pSerial->Deserialize( m_bStandardAttackRange );
	pSerial->Deserialize( m_fAttackRange );
	pSerial->Deserialize( m_iAircraftTargetPriority );
	if( m_iAircraftTargetPriority < 1 || m_iAircraftTargetPriority > 5 ){ m_iAircraftTargetPriority = 5; }
}

#ifdef DO_NOT_COMPILE
void  CEOSAIAttackVs2::AppendDataToXMLString( CStringANSI8& strData )
{
	ASSERT( false );
	strData += ANSI8("<AttackVs>");

	//strData += "<CombatUnitType>" + CBCString::LongAsString( m_eCombatUnitType.m_v ) + "</CombatUnitType>";
	/*
	strData += "<CombatUnitType>";
	if(      m_eCombatUnitType == EnumPoiObjectType::enum_Soft ){ strData += "Soft"; }
	else if( m_eCombatUnitType == EnumPoiObjectType::enum_Armor ){ strData += "Armor"; }
	else if( m_eCombatUnitType == EnumPoiObjectType::enum_Aircraft ){ strData += "Aircraft"; }
	else if( m_eCombatUnitType == EnumPoiObjectType::enum_Missile ){ strData += "Missile"; }
	else if( m_eCombatUnitType == EnumPoiObjectType::enum_Ship ){ strData += "Ship"; }
	else if( m_eCombatUnitType == EnumPoiObjectType::enum_Submarine ){ strData += "Submarine"; }
	strData += "</CombatUnitType>\n";
	*/
	strData += ANSI8("<CombatUnitType>");
	CString strCombatUnitType = GetCombatUnitTypeString();
	strData += strCombatUnitType;
	strData += ANSI8("</CombatUnitType>");

	strData += ANSI8("<Animation>") + CStringANSI8( m_strAnimation ) + ANSI8("</Animation>");

	/*
	strData += "<AnimationPauses>";
	bool bOneItemAdded = false;
	POSITION pos = m_PauseBetweenAnimations.GetHeadPosition();
	while( pos )
	{
		float fPause = m_PauseBetweenAnimations.GetNext( pos );
		CString strText;
		strText.Format( "%0.2f", fPause );
		if( bOneItemAdded ){ strData += ","; }
		strData += strText;
		bOneItemAdded = true;
	}
	strData += "</AnimationPauses>";
	*/

	//strData += "<AnimationKeyword>" + m_strUnitAnimationKeyword + "</AnimationKeyword>";
	//strData += "<IntermediateKeyword>" + m_strIntermediateAnimation + "</IntermediateKeyword>";
	//strData += "<TargetAnimation>" + m_strTargetAnimation + "</TargetAnimation>";

//	if( m_bOneAttackBetweenLandings )
//	{
//		strData += "<OABL>" + CBCString::BoolAsString10( m_bOneAttackBetweenLandings ) + "</OABL>";
//	}

	if( m_bAlwaysHits )
	{
		//strData += ANSI8("<AlwaysHits>") + CBCStringANSI8::BoolAsString10( m_bAlwaysHits ) + ANSI8("</AlwaysHits>");
		strData += ANSI8("<AlwaysHits>");
		strData += CBCStringANSI8::BoolAsString10( m_bAlwaysHits );
		strData += ANSI8("</AlwaysHits>");
	}
	else
	{
		strData += ANSI8("<ToHit>");
		strData += CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fToHit );
		strData += ANSI8("</ToHit>");
	}
	//strData += ANSI8("<Damage>") + CBCStringANSI8::LongAsString( m_iDamage ) + ANSI8("</Damage>");
	strData += ANSI8("<Damage>");
	strData += CBCStringANSI8::LongAsString( m_iDamage );
	strData += ANSI8("</Damage>");

	if( m_iExtraDamagePercentage > 0 && m_iExtraDamage > 0 )
	{
		strData += ANSI8("<EDamage>") + CBCStringANSI8::LongAsString( m_iExtraDamage ) + ANSI8("</Damage>");
		strData += ANSI8("<EDamageP>") + CBCStringANSI8::LongAsString( m_iExtraDamagePercentage ) + ANSI8("</EDamageP>");
	}

	strData += ANSI8("<RateOfFire>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fRateOfFire ) + ANSI8("</RateOfFire>");

	//strData += "<RangeOT>" + CBCString::BoolAsString10( m_bAttackRangeIsDirectlyOverTarget ) + "</RangeOT>";
	if( m_bStandardAttackRange ){ strData += ANSI8("<SRange>") + CBCStringANSI8::BoolAsString10( m_bStandardAttackRange ) + ANSI8("</SRange>"); }
	if( m_fAttackRange > 0.0f ){ strData += ANSI8("<Range>") + CBCStringANSI8::FloatAsString( ANSI8("%0.3f"), m_fAttackRange ) + ANSI8("</Range>"); }

	if( m_iAircraftTargetPriority > 0 )
	{
		strData += ANSI8("<P>") + CBCStringANSI8::LongAsString( m_iAircraftTargetPriority ) + ANSI8("</P>");
	}

	strData += ANSI8("</AttackVs>\n");
}

void  CEOSAIAttackVs2::ReadXMLData( CBCXMLItem* pXMLItem )
{
	//m_eCombatUnitType.m_v = (EnumPoiObjectType) _ttol( pXMLItem->GetValue( "CombatUnitType" ) );
	//m_eCombatUnitType.m_v = (EnumPoiObjectType::Value) _ttol( pXMLItem->GetValue( "CombatUnitType" ) );
/*
	m_eCombatUnitType = (EnumPoiObjectType::Value) _ttol( pXMLItem->GetValue( "CombatUnitType" ) );
*/
	//m_strCombatUnitType = pXMLItem->GetValue( ANSI8("CombatUnitType") );
	CStringANSI8 strCombatUnitTypeANSI8 = pXMLItem->GetValue( ANSI8("CombatUnitType") );
	ASSERT( strCombatUnitTypeANSI8 != ANSI8("SoftGroundUnit") );

	CString strCombatUnitType; strCombatUnitType = strCombatUnitTypeANSI8;
	SetCombatUnitTypeByExternalName( strCombatUnitType );//m_strCombatUnitType );

	//m_eCombatUnitType = EnumPoiObjectType::enum_Unit;
	/*
	if(      m_strCombatUnitType == "Soft"      ){}// m_eCombatUnitType = EnumPoiObjectType::enum_Soft; }
	else if( m_strCombatUnitType == "Armor"     ){}// m_eCombatUnitType = EnumPoiObjectType::enum_Armor; }
	else if( m_strCombatUnitType == "Aircraft"  ){}// m_eCombatUnitType = EnumPoiObjectType::enum_Aircraft; }
	//else if( m_strCombatUnitType == "Missile"   ){ m_eCombatUnitType = EnumPoiObjectType::enum_Missile; }
	else if( m_strCombatUnitType == "Ship"      ){}// m_eCombatUnitType = EnumPoiObjectType::enum_Ship; }
	else if( m_strCombatUnitType == "Submarine" ){}// m_eCombatUnitType = EnumPoiObjectType::enum_Submarine; }
	//else{ m_eCombatUnitType = EnumPoiObjectType::enum_Unit; }
	//else{ ASSERT( false ); }
	*/
	//CString strPauses = pXMLItem->GetValue( "AnimationPauses" );
	//SetPauseBetweenAnimations( strPauses );
	/*
	while( strPauses != "" )
	{
		float fPause = _tstof( strPauses );
		m_PauseBetweenAnimations.AddTail( fPause );
		long iFind = strPauses.Find( "," );
		if( iFind == -1 ){ strPauses = ""; }
		else{ strPauses = strPauses.Right( strPauses.GetLength()-iFind-1 ); }
	}
	*/

	m_strAnimation = pXMLItem->GetValue( ANSI8("Animation") );

	//m_strUnitAnimationKeyword = pXMLItem->GetValue( "AnimationKeyword" );
	//m_strIntermediateAnimation = pXMLItem->GetValue( "IntermediateKeyword" );
	//m_strTargetAnimation = pXMLItem->GetValue( "TargetAnimation" );

//	m_bOneAttackBetweenLandings = pXMLItem->GetValue( "OABL" ) == "1" ? true : false;

	m_fToHit = atof( pXMLItem->GetValue( ANSI8("ToHit") ) );
	//m_fToHit *= 2.0f; // ABCABC
	m_bAlwaysHits = pXMLItem->GetValue( ANSI8("AlwaysHits") ) == ANSI8("1") ? true : false;
	m_iDamage = atol( pXMLItem->GetValue( ANSI8("Damage") ) );

	m_iExtraDamage = atol( pXMLItem->GetValue( ANSI8("EDamage") ) );
	m_iExtraDamagePercentage = atol( pXMLItem->GetValue( ANSI8("EDamageP") ) );

	m_fRateOfFire = atof( pXMLItem->GetValue( ANSI8("RateOfFire") ) );

	m_bStandardAttackRange = pXMLItem->GetValue( ANSI8("SRange") ) == ANSI8("1") ? true : false;
	//m_bStandardAttackRange = pXMLItem->GetValue( "RangeOT" ) == "1" ? false : true;
	//m_bAttackRangeIsDirectlyOverTarget = pXMLItem->GetValue( "RangeOT" ) == "1" ? true : false;
	m_fAttackRange = atof( pXMLItem->GetValue( ANSI8("Range") ) );

	m_iAircraftTargetPriority = atol( pXMLItem->GetValue( ANSI8("P") ) );
	if( m_iAircraftTargetPriority < 1 || m_iAircraftTargetPriority > 5 ){ m_iAircraftTargetPriority = 5; }

/*
	if( m_strCombatUnitType == "Aircraft" )
	{
		m_iExtraDamagePercentage = 10;
		m_iExtraDamage = 1;
	}
*/
	// TEMP
	/*
	if( m_bStandardAttackRange && m_fAttackRange > 0.0f )
	{
		m_fAttackRange -= 20.0f;
		if( m_fAttackRange < 0.0f ) m_fAttackRange = 0.0f;
	}
	*/

	// TEMPORARY
	/*
	if( m_strCombatUnitType == "Aircraft" ||
		m_strCombatUnitType == "Missile" ||
		m_strCombatUnitType == "SpaceObject" )
	{
		m_bStandardAttackRange = false;
	}
	*/
}
#endif DO_NOT_COMPILE

bool CEOSAIAttackVs2::operator==( CEOSAIAttackVs2& AttackVs2 )
{
	//ASSERT( m_strCombatUnitType == AttackVs2.m_strCombatUnitType );
	ASSERT( m_iAttackVsCombatUnitType == AttackVs2.m_iAttackVsCombatUnitType );
	return ( m_fToHit == AttackVs2.m_fToHit &&
			 m_bAlwaysHits == AttackVs2.m_bAlwaysHits &&
			 m_iDamage == AttackVs2.m_iDamage &&
			 m_iExtraDamage == AttackVs2.m_iExtraDamage &&
			 m_iExtraDamagePercentage == AttackVs2.m_iExtraDamage &&
			 m_bStandardAttackRange == AttackVs2.m_bStandardAttackRange &&
			 m_fAttackRange == AttackVs2.m_fAttackRange &&
			 m_fRateOfFire == AttackVs2.m_fRateOfFire );
}

CString CEOSAIAttackVs2::GetCombatUnitTypeString()
{
	ASSERT( m_pParentAIUnitTemplate );
	if( m_pParentAIUnitTemplate )
	{
		CEOSAICombatUnitType* pCombatUnitType = m_pParentAIUnitTemplate->GetAIGameRules()->GetCombatUnitType( m_iAttackVsCombatUnitType );
		ASSERT( pCombatUnitType );
		if( pCombatUnitType )
		{
			return pCombatUnitType->m_strExternalName;
		}
	}
	return _T("");
}

void CEOSAIAttackVs2::SetCombatUnitTypeByExternalName( CString strExternalName )
{
	if( strExternalName == _T("") ) // This is an attack vs City/Fort/Airfield
	{
		m_iAttackVsCombatUnitType = -1;
		return;
	}

	ASSERT( m_pParentAIUnitTemplate );
	if( m_pParentAIUnitTemplate )
	{
		CEOSAICombatUnitType* pCombatUnitType = m_pParentAIUnitTemplate->GetAIGameRules()->GetCombatUnitTypeByExternalName( strExternalName );
		ASSERT( pCombatUnitType );
		if( pCombatUnitType )
		{
			m_iAttackVsCombatUnitType = pCombatUnitType->m_iCombatUnitType;
		}
	}
}

bool CEOSAIAttackVs2::GetOABL()
{
	return m_pParentAIUnitTemplate->IsAirUnit();
	//return m_bOneAttackBetweenLandings;
}

long CEOSAIAttackVs2::CalculateDamage() // m_iDamage + (maybe) m_iExtraDamage
{
	if( m_iExtraDamagePercentage > 0 && m_iExtraDamage > 0 )
	{
		long iRand100 = rand()%100;
		if( iRand100 < m_iExtraDamagePercentage )
		{
			return m_iDamage + m_iExtraDamage;
		}
	}
	return m_iDamage;
}

void CEOSAIAttackVs2::SetAttackRangeIsDirectlyOverTarget( bool bOverTarget )
{
	if( bOverTarget )
	{
		m_bStandardAttackRange = false; m_fAttackRange = 0.0f;
	}
	else
	{
		m_bStandardAttackRange = true;
	}
}

float CEOSAIAttackVs2::GetFullAttackRange()
{
	if( GetAttackRangeIsDirectlyOverTarget() )
	{
		return 5.0f;
	}
	if( m_bStandardAttackRange )
	{
		//return STANDARD_ATTACK_RANGE + m_fAttackRange;
		return m_fAttackRange;
	}
	return m_fAttackRange;
}

#ifdef NOT_SURE_IF_THIS_IS_NECESSARY_FOR_AI
bool CEOSAIAttackVs2::IsWithinMeleeRange( CEOSAILocation Location1, CEOSAILocation Location2 )//, EnumAttackGoal eGoal )
{
	// Convert both Locations into Grid locations
	long iLocation1GridX = Location1.GetGridX();
	long iLocation1GridY = Location1.GetGridY();
	long iLocation2GridX = Location2.GetGridX();
	long iLocation2GridY = Location2.GetGridY();

	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	float fGridDistanceBetweenLoc1Loc2 = 0.0f;
	if( pWorldBuildDesc )
	{
		fGridDistanceBetweenLoc1Loc2 = pWorldBuildDesc->GetGridDistance( iLocation1GridX,iLocation1GridY, iLocation2GridX,iLocation2GridY );
	}
	else
	{
		float fGridDistanceX = iLocation1GridX - iLocation2GridX;
		float fGridDistanceY = iLocation1GridY - iLocation2GridY;
		fGridDistanceBetweenLoc1Loc2 = sqrt( fGridDistanceX*fGridDistanceX + fGridDistanceY*fGridDistanceY );
	}
	float fPixelDistanceBetweenLoc1Loc2 = fGridDistanceBetweenLoc1Loc2*4.0f;

	if( fPixelDistanceBetweenLoc1Loc2 < STANDARD_ATTACK_RANGE+4.5f )
	{
		return true;
	}
	return false;
}
#endif NOT_SURE_IF_THIS_IS_NECESSARY_FOR_AI

#ifdef NOT_SURE_IF_THIS_IS_NECESSARY_FOR_AI
// Take into account the unit template's combat range for this type of enemy
bool CEOSAIAttackVs2::IsWithinCombatRange( CEOSAILocation Location1, CEOSAILocation Location2, EOSAIEnumAttackGoal eGoal )
{
	float fAttackRange = 0.0f;
	if( eGoal == EOSAIEnumAttackGoal_Capture )
	{
		fAttackRange = 5.0f;
	}
	else
	{
		fAttackRange = GetFullAttackRange();
	}

	// Convert both Locations into Grid locations
	long iLocation1GridX = Location1.GetGridX();
	long iLocation1GridY = Location1.GetGridY();
	long iLocation2GridX = Location2.GetGridX();
	long iLocation2GridY = Location2.GetGridY();

	float fGridDistanceBetweenLoc1Loc2 = 0.0f;
	CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	if( pWorldBuildDesc )
	{
		fGridDistanceBetweenLoc1Loc2 = pWorldBuildDesc->GetGridDistance( iLocation1GridX,iLocation1GridY, iLocation2GridX,iLocation2GridY );
	}
	else
	{
		float fGridDistanceX = iLocation1GridX - iLocation2GridX;
		float fGridDistanceY = iLocation1GridY - iLocation2GridY;
		fGridDistanceBetweenLoc1Loc2 = sqrt( fGridDistanceX*fGridDistanceX + fGridDistanceY*fGridDistanceY );
	}
	float fPixelDistanceBetweenLoc1Loc2 = fGridDistanceBetweenLoc1Loc2*4.0f;

	if( fPixelDistanceBetweenLoc1Loc2 < fAttackRange+4.5f )
	{
		return true;
	}
	return false;
	/*
	bool  bUseStandardRange = false;
	float fAdditionalRange  = 0.0f;

	// Step1. Convert both Locations into Grid locations
	long iLocation1GridX = Location1.GetGridX();
	long iLocation1GridY = Location1.GetGridY();
	long iLocation2GridX = Location2.GetGridX();
	long iLocation2GridY = Location2.GetGridY();

	if( GetAttackRangeIsDirectlyOverTarget() )
	{
		bUseStandardRange = false;
		fAdditionalRange  = 5.0f;
	}
	else
	{
		bUseStandardRange = true;
		fAdditionalRange  = m_fAttackRange;
	}

	CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	if( bUseStandardRange )
	{
		float fDistance = pWorldBuildDesc->GetPixelDistance( Location1, Location2 );
		if( fDistance > fAdditionalRange+ZONEOFCONTROL_MAX )//46.0f )
		{
			return false;
		}

		fDistance = pWorldBuildDesc->GetDistanceBeyondStandardRange( Location1, Location2 );
		return fDistance <= fAdditionalRange;
	}
	else
	{
		float fDistance = pWorldBuildDesc->GetPixelDistance( Location1, Location2 );
		return fDistance < fAdditionalRange;
	}

	ASSERT( false );
	return false;
	*/
}
#endif NOT_SURE_IF_THIS_IS_NECESSARY_FOR_AI

