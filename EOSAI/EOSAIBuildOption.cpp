
#include "stdafx.h"

#include "EOSAISettings.h"
#include "EOSAIBuildOption.h"
#include "EOSAIBuildOrder.h"
#include "EOSAIBuildingDescription.h"
#include "EOSAIUnitTemplate.h"
//#include "CommonState.h"
//#include "EOSAISerial.h"
//#include "City.h"
//#include "Player.h"
//#include "LanguagePack.h"
//extern CString Lang( long iID, CString strEnglish );

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
void CBuildOrder::AddToWorld( CWorldDesc* pWorldDesc, long iObjectId )
{
	//m_City.Set( pWorldDesc->GetResourceTable(), iObjectId );
	m_City.Set( pWorldDesc->GetResourceTable(), m_City.GetObjectId() );
	CBaseObject::AddToWorld( pWorldDesc, iObjectId );
}
*/
/*
void CEOSAIBuildOption::Serialize( CEOSAISerial* pSerial )
{
	//pSerial->Serialize( CBuildOrder_string );
	//pSerial->Serialize( m_City.GetObjectId() );
	pSerial->SerializeANSI8( m_strBuildTarget );
	//pSerial->Serialize( m_fProductionInvested );
}

void CEOSAIBuildOption::Deserialize( CEOSAISerial* pSerial )
{
	//CString strClassName;
	//pSerial->Deserialize( strClassName );
	//ASSERT( strClassName == CBuildOrder_string );

	//long iObjectId;
	//pSerial->Deserialize( iObjectId ); m_City.SetObjectId( iObjectId );
	pSerial->DeserializeANSI8( m_strBuildTarget );
	//pSerial->Deserialize( m_fProductionInvested );
}
*/
/*
CBuildOption* CEOSAIBuildOption::GetBuildOption()
{
	//CBuildOption* pBuildOption = m_pWorldDesc->GetCommonState()->GetBuildOptions()->GetBuildOption( m_strBuildTarget );
	CBuildOption* pBuildOption = GetCommonState()->GetActiveUnitset()->GetBuildOption( m_strBuildTarget );
	ASSERT( pBuildOption );
	return pBuildOption;
}

CString CEOSAIBuildOption::GetBuildTargetText()
{
	if( m_strBuildTarget == _T("<Money>") ) return Lang( 98, _T("Money") );
	return m_strBuildTarget;
}
*/

/*
void  CBuildOrder::ReadFlat( CBuildOrder_ServerToPlayerFlat* pFlat )
{
	this->SetCity( pFlat->m_iCity );
	this->m_strBuildTarget      = pFlat->m_strBuildTarget;
	//this->m_fProductionInvested = pFlat->m_fProductionInvested;
	CBaseObject::ReadFlat( pFlat );
}

void  CBuildOrder::WriteFlat( CBuildOrder_ServerToPlayerFlat* pFlat )
{
	pFlat->m_iCity               = this->m_City.GetObjectId();
	pFlat->m_strBuildTarget      = this->m_strBuildTarget;
	//pFlat->m_fProductionInvested = this->m_fProductionInvested;
	CBaseObject::WriteFlat( pFlat );
}

void  CBuildOrder::ReadFlat( CBuildOrder_PlayerToServerFlat* pFlat )
{
	this->SetCity( pFlat->m_iCity );
	this->m_strBuildTarget = pFlat->m_strBuildTarget;
	CBaseObject::ReadFlat( pFlat );
}

void  CBuildOrder::WriteFlat( CBuildOrder_PlayerToServerFlat* pFlat )
{
	pFlat->m_iCity          = this->m_City.GetObjectId();
	pFlat->m_strBuildTarget = this->m_strBuildTarget;
	CBaseObject::WriteFlat( pFlat );
}
*/
/*
CCity*  CBuildOrder::GetCity()
{ 
	return m_City.GetPtr();
}

void  CBuildOrder::SetCity( long iCityId )
{
	m_City.SetObjectId( iCityId );
}
*/

/*
CBuildCompletion* CBuildOrder::GetBuildCompletion( CCity* pCity )
{
	// 

	POSITION pos = pCity->GetBuildCompletionList()->GetHeadPosition();
	while( pos )
	{
		CBuildCompletion* pBuildCompletion = pCity->GetBuildCompletionList()->GetNext( pos )->GetPtr();
		if( 
	}
}
*/

CEOSAIBuildOption::~CEOSAIBuildOption()
{
	// for debug purposes, only
	m_pAIUnitTemplate = (CEOSAIUnitTemplate*)77;
	//m_pAIBuildingDescription = 78;
	int g = 0;
}

/*
float CEOSAIBuildOption::GetProductionInvested( CCity* pCity )
{
	if( this->IsMoney() ) return 0.0f;
	//if( this->IsIncProd() ) return 0.0f;
	ASSERT( this->GetBuildOption()->IsAUnit() || this->GetBuildOption()->IsABuilding() );

	// If there is a matching build order before this one, the calculation result will be different
	bool bSameItemFoundEarlier = false;
	POSITION pos = pCity->GetBuildOrders()->GetHeadPosition();
	while( pos )
	{
		CEOSAIBuildOption* pBuildOrder = pCity->GetBuildOrders()->GetNext( pos );
		if( pBuildOrder == this )
		{
			break;
		}
		if( pBuildOrder->GetBuildTarget() == this->GetBuildTarget() )
		{
			bSameItemFoundEarlier = true;
		}
	}

	if( !bSameItemFoundEarlier )
	{
		CBuildCompletion* pBuildCompletion = pCity->GetPartiallyCompletedItem( GetBuildTarget() );
		if( pBuildCompletion )
		{
			return pBuildCompletion->GetProductionInvested();
		}
	}
	return 0.0f;
}
*/
/*
float CEOSAIBuildOption::ProductionUntilCompletion( CCity* pCity )
{
	if( this->IsMoney() ) return 1000000.0f;
/=*
	if( this->IsMoney() ) return 0.0f;
	if( this->IsIncProd() ) return 0.0f;
	ASSERT( this->IsAUnit() );

	// If there is a matching build order before this one, the calculation result will be different
	bool bSameItemFoundEarlier = false;
	POSITION pos = pCity->GetBuildOrders()->GetHeadPosition();
	while( pos )
	{
		CBuildOrder* pBuildOrder = pCity->GetBuildOrders()->GetNext( pos );
		if( pBuildOrder == this )
		{
			break;
		}
		if( pBuildOrder->GetBuildTarget() == this->GetBuildTarget() )
		{
			bSameItemFoundEarlier = true;
		}
	}

	if( bSameItemFoundEarlier )
	{
		return GetBuildOption()->GetProductionCost();
	}

	CBuildCompletion* pCompletion = pCity->GetBuildCompletionFromPartiallyCompletedList( this->GetBuildTarget() );
	if( pCompletion )
	{
		return GetBuildOption()->GetProductionCost() - pCompletion->GetProductionInvested();
	}

	return GetBuildOption()->GetProductionCost();
*-/
	return ( GetBuildOption()->GetProductionCost() - GetProductionInvested( pCity ) ) + 0.00001f;

	//CBuildOption* pBuildOption = m_pWorldDesc->GetCommonState()->GetBuildOptions()->GetBuildOption( m_strBuildTarget );

	//float fProductionRate = m_City.GetPtr()->GetPlayerProductionMultiplier() * m_City.GetPtr()->GetProduction();

	//return ( pBuildOption->ProductionNeeded() - m_fProductionInvested ) / fProductionRate;
}
*/
/*
float CEOSAIBuildOption::TimeUntilCompletion( CCity* pCity )
{
	float fProductionLeft = ProductionUntilCompletion( pCity );
	if( fProductionLeft == 0.0f ) return 0.0f;

	//float fProductionRate = pCity->GetPlayerProductionMultiplier() * pCity->GetTotalProduction();
	float fProductionRate = pCity->GetTotalProduction( true, this->GetBuildOption() );
	if( fProductionRate > 0.0f )
	{
		return fProductionLeft / fProductionRate;
	}
	else
	{
		return 1000.0f;
	}
}

float CEOSAIBuildOption::TimeNeededForBuildTarget( CCity* pCity )
{
	float fProductionLeft = GetBuildOption()->GetProductionCost();
	if( fProductionLeft == 0.0f ) return 0.0f;
	//float fProductionRate = pCity->GetPlayerProductionMultiplier() * pCity->GetTotalProduction();
	float fProductionRate = pCity->GetTotalProduction( true, this->GetBuildOption() );

	if( fProductionRate > 0.0f )
	{
		return fProductionLeft / fProductionRate;
	}
	else
	{
		return 1000.0f;
	}
}
*/

bool CEOSAIBuildOption::CanOnlyBeBuiltInCitiesAdjacentToWater()
{
	if( this->IsABuilding() )
	{
		return m_pAIBuildingDescription->CanOnlyBeBuiltInCitiesAdjacentToWater();
	}
	if( this->IsAUnit() )
	{
		return m_pAIUnitTemplate->CanOnlyBeBuiltInCitiesAdjacentToWater();
	}
	return false;
}

float CEOSAIBuildOption::GetProductionCost()
{
	if( IsMoney() ) return 1.0f;
	if( m_pAIUnitTemplate )
	{
		return m_pAIUnitTemplate->GetProductionCost();
	}
	if( m_pAIBuildingDescription )
	{
		return m_pAIBuildingDescription->GetProductionCost();
	}
	ASSERT( false );
	return 0;
}

float CEOSAIBuildOption::ProductionNeeded()
{
	if( IsMoney() ) return 0.0f;

	//CUnitTemplate* pUnitTemplate = GetUnitTemplate();
	if( m_pAIUnitTemplate )
	{
		return m_pAIUnitTemplate->GetProductionCost();
	}
	if( m_pAIBuildingDescription )
	{
		return m_pAIBuildingDescription->GetProductionCost();
	}
	ASSERT( false );
	return 0;
}

CString CEOSAIBuildOption::GetInternalName()
{
	if( IsMoney() ) return _T("<Money>");

	if( m_pAIUnitTemplate )
	{
		return m_pAIUnitTemplate->GetInternalName();
	}
	if( m_pAIBuildingDescription )
	{
		return m_pAIBuildingDescription->GetInternalName();
	}
	ASSERT( false );
	return _T("");
}

EOSAI::ResourceAmounts CEOSAIBuildOption::GetResourceProductionPerTurn(float fCityProduction)
{
	// TODO: This is too game-specific. I need to genericize this.
	EOSAI::ResourceAmounts res;
	if (this->IsMoney()) res[_T("Money")] = 0.5f * fCityProduction;
	/*
	if (m_pUnitTemplate && strResource == _T("Iron"))
	{
		return m_pUnitTemplate->GetIronCost();
	}
	if (m_pBuildingDescription)
	{
		return m_pBuildingDescription->GetCost(strResource);//>GetIronCost();
	}
	*/
	return res;
}

EOSAI::ResourceAmounts CEOSAIBuildOption::GetResourceConsumptionPerTurn(float fCityProduction)
{
	// TODO: This is too game-specific. I need to genericize this.
	EOSAI::ResourceAmounts res;
	if (m_pAIUnitTemplate && m_pAIUnitTemplate->GetIronCost() != 0.0f )
	{
		if (GetProductionCost() > 0.0f)
		{
			float fPercentageTime01 = fCityProduction / GetProductionCost();
			if (fPercentageTime01 < 0.0f) fPercentageTime01 = 0.0f;
			if (fPercentageTime01 > 1.0f) fPercentageTime01 = 1.0f;
			res[_T("Iron")] = fPercentageTime01 * m_pAIUnitTemplate->GetIronCost();
		}
	}
	if (m_pAIBuildingDescription)
	{
		if (GetProductionCost() > 0.0f)
		{
			float fPercentageTime01 = fCityProduction / GetProductionCost();
			if (fPercentageTime01 < 0.0f) fPercentageTime01 = 0.0f;
			if (fPercentageTime01 > 1.0f) fPercentageTime01 = 1.0f;

			/*
			POSITION pos = m_pAIBuildingDescription->m_ResourceCostList.m_List.GetHeadPosition();
			while (pos)
			{
				EOSAI::StringAndFloat* p = m_pAIBuildingDescription->m_ResourceCostList.m_List.GetNext(pos);
				res[p->m_strValue] += p->m_fValue;
			}
			*/
			std::map<CString, float>::iterator iter = m_pAIBuildingDescription->m_ResourceCostList.m_Resource.begin();
			for (iter = m_pAIBuildingDescription->m_ResourceCostList.m_Resource.begin(); 
				iter != m_pAIBuildingDescription->m_ResourceCostList.m_Resource.end(); iter++)
			{
				res[iter->first] += m_pAIBuildingDescription->m_ResourceCostList[iter->first];
			}

			//if(m_pAIBuildingDescription->m_ResourceCostList)
			/*

			if (m_pAIBuildingDescription->GetIronCost() != 0.0f)
			{
				res[_T("Iron")] = fPercentageTime01 * m_pAIBuildingDescription->GetIronCost();
			}
			*/
			//return m_pBuildingDescription->GetCost(strResource);//>GetIronCost();
		}
	}
	return res;
}


//////////////////////////////////////////////////////
/*
CBuildOrder_ServerToPlayerFlat::CBuildOrder_ServerToPlayerFlat()
{
	m_iCity = 0;
	//m_fProductionInvested = 0.0f;
	//m_fIronPointsSpent = 0.0f;
}

void CBuildOrder_ServerToPlayerFlat::Serialize( CEOSAISerial* pSerial )
{
	// First, the class name
	pSerial->Serialize( CBuildOrder_ServerToPlayerFlat_string );

	// A version number
	pSerial->Serialize( (long) 1 );

	pSerial->Serialize( m_iCity );
	pSerial->Serialize( m_strBuildTarget );
	//pSerial->Serialize( m_fProductionInvested );
	//pSerial->Serialize( m_fIronPointsSpent );
	CBaseObject_Flat::Serialize( pSerial );
}

void CBuildOrder_ServerToPlayerFlat::Deserialize( CEOSAISerial* pSerial )
{
	// First, the class name
	CString strClassName = pSerial->ExtractString();
	VERIFY( strClassName == CBuildOrder_ServerToPlayerFlat_string );

	// A version number
	int iVersion = pSerial->ExtractLong();

	pSerial->Deserialize( m_iCity );
	pSerial->Deserialize( m_strBuildTarget );
	//pSerial->Deserialize( m_fProductionInvested );
	//pSerial->Deserialize( m_fIronPointsSpent );
	CBaseObject_Flat::Deserialize( pSerial );
}


void CBuildOrder_PlayerToServerFlat::Serialize( CEOSAISerial* pSerial )
{
	// First, the class name
	pSerial->Serialize( CBuildOrder_PlayerToServerFlat_string );

	// A version number
	pSerial->Serialize( (long) 1 );

	pSerial->Serialize( m_iCity );
	pSerial->Serialize( m_strBuildTarget );
	CBaseObject_Flat::Serialize( pSerial );
}

void CBuildOrder_PlayerToServerFlat::Deserialize( CEOSAISerial* pSerial )
{
	// First, the class name
	CString strClassName = pSerial->ExtractString();
	VERIFY( strClassName == CBuildOrder_PlayerToServerFlat_string );

	// A version number
	int iVersion = pSerial->ExtractLong();

	// INCOMPLETE: Make sure the player is only issuing
	//   build orders for cities that he owns.

	pSerial->Deserialize( m_iCity );
	pSerial->Deserialize( m_strBuildTarget );
	CBaseObject_Flat::Deserialize( pSerial );
}
*/
