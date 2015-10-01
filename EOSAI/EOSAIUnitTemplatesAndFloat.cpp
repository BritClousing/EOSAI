
#include "stdafx.h"
#include "EOSAIUnitTemplatesAndFloat.h"
//#include "AIUnitCombatCapability.h"
#include "EOSAIUnitTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIUnit2TemplatesAndFloat::~CEOSAIUnit2TemplatesAndFloat()
{
	Clear();
}

void  CEOSAIUnit2TemplatesAndFloat::Clear()
{
	while( m_List.IsEmpty() == FALSE ){ delete m_List.RemoveHead(); }
}

void  CEOSAIUnit2TemplatesAndFloat::SetAllValuesToZero()
{
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		m_List.GetNext( pos )->m_fValue = 0.0f;
	}
}

void  CEOSAIUnit2TemplatesAndFloat::Add( CEOSAIUnitTemplate* pAIUnitTemplate, float f )
{
	ASSERT( pAIUnitTemplate );

	// If it exists in the list, add to it
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pAIUnitTemplateAndFloat = m_List.GetNext( pos );
		if( pAIUnitTemplateAndFloat->m_pAIUnitTemplate == pAIUnitTemplate )
		{
			pAIUnitTemplateAndFloat->m_fValue += f;
			return;
		}
	}
	CEOSAIUnit2TemplateAndFloat* pAIUnitTemplateAndFloat = new CEOSAIUnit2TemplateAndFloat();
	pAIUnitTemplateAndFloat->m_pAIUnitTemplate = pAIUnitTemplate;
	pAIUnitTemplateAndFloat->m_fValue = f;
	m_List.AddTail( pAIUnitTemplateAndFloat );
}

void  CEOSAIUnit2TemplatesAndFloat::Set( CEOSAIUnitTemplate* pAIUnitTemplate, float f )
{
	ASSERT( pAIUnitTemplate );

	// If it exists in the list, add to it
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pAIUnitTemplateAndFloat = m_List.GetNext( pos );
		if( pAIUnitTemplateAndFloat->m_pAIUnitTemplate == pAIUnitTemplate )
		{
			pAIUnitTemplateAndFloat->m_fValue = f;
			return;
		}
	}
	CEOSAIUnit2TemplateAndFloat* pAIUnitTemplateAndFloat = new CEOSAIUnit2TemplateAndFloat();
	pAIUnitTemplateAndFloat->m_pAIUnitTemplate = pAIUnitTemplate;
	pAIUnitTemplateAndFloat->m_fValue = f;
	m_List.AddTail( pAIUnitTemplateAndFloat );
}

float CEOSAIUnit2TemplatesAndFloat::GetValue( CEOSAIUnitTemplate* pAIUnitTemplate )
{
	// If it exists in the list, add to it
	POSITION pos = m_List.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnit2TemplateAndFloat* pAIUnitTemplateAndFloat = m_List.GetNext( pos );
		if( pAIUnitTemplateAndFloat->m_pAIUnitTemplate == pAIUnitTemplate )
		{
			return pAIUnitTemplateAndFloat->m_fValue;
		}
	}
	return 0.0f;
}
