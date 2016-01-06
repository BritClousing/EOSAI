
#include "stdafx.h"
#include "EOSAIUnitTemplatesAndFloat.h"
//#include "AIUnitCombatCapability.h"
#include "EOSAIUnitTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnitTemplatesAndFloat::~CEOSAIUnitTemplatesAndFloat()
{
	Clear();
}

void  CEOSAIUnitTemplatesAndFloat::Clear()
{
	while( m_List.IsEmpty() == FALSE ){ delete m_List.RemoveHead(); }
}

void  CEOSAIUnitTemplatesAndFloat::SetAllValuesToZero()
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		m_List.GetNext( pos )->m_fValue = 0.0f;
	}
}

void  CEOSAIUnitTemplatesAndFloat::Add( CEOSAIUnitTemplate* pAIUnitTemplate, float f )
{
	ASSERT( pAIUnitTemplate );

	// If it exists in the list, add to it
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplateAndFloat* pAIUnitTemplateAndFloat = m_List.GetNext( pos );
		if( pAIUnitTemplateAndFloat->m_pAIUnitTemplate == pAIUnitTemplate )
		{
			pAIUnitTemplateAndFloat->m_fValue += f;
			return;
		}
	}
	CEOSAIUnitTemplateAndFloat* pAIUnitTemplateAndFloat = new CEOSAIUnitTemplateAndFloat();
	pAIUnitTemplateAndFloat->m_pAIUnitTemplate = pAIUnitTemplate;
	pAIUnitTemplateAndFloat->m_fValue = f;
	m_List.AddTail( pAIUnitTemplateAndFloat );
}

void  CEOSAIUnitTemplatesAndFloat::Set( CEOSAIUnitTemplate* pAIUnitTemplate, float f )
{
	ASSERT( pAIUnitTemplate );

	// If it exists in the list, add to it
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplateAndFloat* pAIUnitTemplateAndFloat = m_List.GetNext( pos );
		if( pAIUnitTemplateAndFloat->m_pAIUnitTemplate == pAIUnitTemplate )
		{
			pAIUnitTemplateAndFloat->m_fValue = f;
			return;
		}
	}
	CEOSAIUnitTemplateAndFloat* pAIUnitTemplateAndFloat = new CEOSAIUnitTemplateAndFloat();
	pAIUnitTemplateAndFloat->m_pAIUnitTemplate = pAIUnitTemplate;
	pAIUnitTemplateAndFloat->m_fValue = f;
	m_List.AddTail( pAIUnitTemplateAndFloat );
}

float CEOSAIUnitTemplatesAndFloat::GetValue( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	// If it exists in the list, add to it
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitTemplateAndFloat* pAIUnitTemplateAndFloat = m_List.GetNext( pos );
		if( pAIUnitTemplateAndFloat->m_pAIUnitTemplate == pAIUnitTemplate )
		{
			return pAIUnitTemplateAndFloat->m_fValue;
		}
	}
	return 0.0f;
}
