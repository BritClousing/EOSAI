
#include "stdafx.h"
#include "ViewRange.h"
#include "UnitTemplate.h"

CViewRangeComparison::CViewRangeComparison( CUnitTemplate* pUnitTemplate1, CUnitTemplate* pUnitTemplate2 )
{
	m_pUnitTemplate1 = pUnitTemplate1;
	m_pUnitTemplate2 = pUnitTemplate2;

	m_fDefaultViewRange1 = m_pUnitTemplate1->GetDefaultViewRange();
	m_fDefaultViewRange2 = m_pUnitTemplate2->GetDefaultViewRange();

	POSITION pos = m_pUnitTemplate1->GetViewRangeList()->GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange1 = m_pUnitTemplate1->GetViewRangeList()->GetNext( pos );

		CViewRangeX2* pViewRangeX2 = new CViewRangeX2();
		pViewRangeX2->m_pViewRange1 = pViewRange1;
		m_TaggedViewRanges.AddTail( pViewRangeX2 );
	}
	pos = m_pUnitTemplate2->GetViewRangeList()->GetHeadPosition();
	while( pos )
	{
		CViewRange* pViewRange2 = m_pUnitTemplate2->GetViewRangeList()->GetNext( pos );

		// Does this already exist in the list?
		bool bAlreadyInTheList = false;
		POSITION pos2 = m_TaggedViewRanges.GetHeadPosition();
		while( pos2 )
		{
			CViewRangeX2* pViewRangeX2 = m_TaggedViewRanges.GetNext( pos2 );
			if( pViewRangeX2->m_pViewRange1 &&
				pViewRangeX2->m_pViewRange1->m_strUnitTag == pViewRange2->m_strUnitTag )
			{
				pViewRangeX2->m_pViewRange2 = pViewRange2;
				bAlreadyInTheList = true;
			}
		}
		if( bAlreadyInTheList == false )
		{
			CViewRangeX2* pViewRangeX2 = new CViewRangeX2();
			pViewRangeX2->m_pViewRange2 = pViewRange2;
			m_TaggedViewRanges.AddTail( pViewRangeX2 );
		}
	}

	//CList< CViewRangeX2* >  m_TaggedViewRanges;
}

bool CViewRangeComparison::ViewsRangesAreIdentical()
{
	if( m_fDefaultViewRange1 != m_fDefaultViewRange2 ) return false;
	//
	POSITION pos = m_TaggedViewRanges.GetHeadPosition();
	while( pos )
	{
		CViewRangeX2* pViewRangeX2 = m_TaggedViewRanges.GetNext( pos );
		if( pViewRangeX2->m_pViewRange1 == NULL || pViewRangeX2->m_pViewRange2 == NULL ) return false;
		if( pViewRangeX2->m_pViewRange1->m_fViewRange != pViewRangeX2->m_pViewRange2->m_fViewRange ) return false;
	}
	return true;
}

CString CViewRangeX2::GetUnitTag()
{
	if( m_pViewRange1 ) return m_pViewRange1->m_strUnitTag;
	if( m_pViewRange2 ) return m_pViewRange2->m_strUnitTag;
	return _T("");
}

