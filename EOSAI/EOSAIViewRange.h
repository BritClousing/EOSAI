
#pragma once

class CViewRange
{
	public:
		CViewRange(){ m_fViewRange = 0.0f; }

		CString  m_strUnitTag;  // "Soft", "Armor", "Aircraft", etc
		float    m_fViewRange;
};

// Used for comparing two units
class CAIUnitTemplate2;
class CViewRangeX2;
class CViewRangeComparison
{
	public:
		CViewRangeComparison( CUnitTemplate* pUnitTemplate1, CUnitTemplate* pUnitTemplate2 );
		bool   ViewsRangesAreIdentical();

		CUnitTemplate* m_pUnitTemplate1;
		CUnitTemplate* m_pUnitTemplate2;

		float  m_fDefaultViewRange1;
		float  m_fDefaultViewRange2;

		CList< CViewRangeX2* >  m_TaggedViewRanges;
};

class CViewRangeX2
{
	public:
		CViewRangeX2()
		{
			m_pViewRange1 = NULL;
			m_pViewRange2 = NULL;
		}
		CString GetUnitTag();

		CViewRange*  m_pViewRange1;
		CViewRange*  m_pViewRange2;
};

