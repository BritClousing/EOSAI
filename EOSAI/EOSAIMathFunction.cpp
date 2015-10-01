
#include "stdafx.h"
#include "EOSAIMathFunction.h"
#include <math.h>


void  CEOSAIMathFunction::Clear()
{
	while( m_FunctionPairsList.IsEmpty() == FALSE ){ delete m_FunctionPairsList.RemoveHead(); }
}

void  CEOSAIMathFunction::SetInputOutput( float fInput, float fOutput )
{
	POSITION pos = m_FunctionPairsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIFunctionPair* pListPair = m_FunctionPairsList.GetNext( pos );
		if( fInput == pListPair->m_fInput )
		{
			pListPair->m_fOutput = fOutput;
			return;
		}
	}
	// This [at] value is not in the list, create it
	CEOSAIFunctionPair* pFunctionPair = new CEOSAIFunctionPair( fInput, fOutput );
	m_FunctionPairsList.AddTail( pFunctionPair );
}

float CEOSAIMathFunction::GetOutput( float fInput )
{
	if( m_FunctionPairsList.IsEmpty() )
	{
		ASSERT( false );
		return 0.0f;
	}

	CEOSAIFunctionPair* pLowerValue = NULL;
	CEOSAIFunctionPair* pUpperValue = NULL;

	// Find the closest upper and lower values
	//   Then find the intermediate value
	POSITION pos = m_FunctionPairsList.GetHeadPosition();
	while( pos )
	{
		CEOSAIFunctionPair* pListPair = m_FunctionPairsList.GetNext( pos );
		if( fInput == pListPair->m_fInput )
		{
			return pListPair->m_fOutput;
		}
		else if( fInput < pListPair->m_fInput )
		{
			if( pUpperValue == NULL ){ pUpperValue = pListPair; }
			else if( pListPair->m_fInput < pUpperValue->m_fInput ){ pUpperValue = pListPair; }
		}
		else if( fInput > pListPair->m_fInput )
		{
			if( pLowerValue == NULL ){ pLowerValue = pListPair; }
			else if( pListPair->m_fInput > pLowerValue->m_fInput ){ pLowerValue = pListPair; }
		}
	}

	if( pLowerValue == NULL && pUpperValue == NULL ){ ASSERT( false ); return 0.0f; }
	if( pLowerValue && pUpperValue == NULL ) return pLowerValue->m_fOutput;
	if( pLowerValue == NULL && pUpperValue ) return pUpperValue->m_fOutput;

	float fDistanceToLower = (float) fabs( pLowerValue->m_fInput - fInput );
	float fDistanceToUpper = (float) fabs( pUpperValue->m_fInput - fInput );

	float fLowerComponent = pLowerValue->m_fOutput * fDistanceToUpper / ( fDistanceToLower + fDistanceToUpper );
	float fUpperComponent = pUpperValue->m_fOutput * fDistanceToLower / ( fDistanceToLower + fDistanceToUpper );

	return fLowerComponent + fUpperComponent;
}

