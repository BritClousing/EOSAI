
#pragma once

class CUnitTag
{
	public:
		CUnitTag()
		{
			m_bTagIsActiveOnlyWhenNotInsideAContainer = false;
		}
		CUnitTag( CString str )
		{
			m_strTag = str;
			m_bTagIsActiveOnlyWhenNotInsideAContainer = false;
		}

		CString  m_strTag;
		bool     m_bTagIsActiveOnlyWhenNotInsideAContainer;
		//bool     m_bTagIsActiveOnlyWhenNotLandedOrDocked;
};

