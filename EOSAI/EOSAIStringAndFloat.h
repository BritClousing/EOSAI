
#pragma once

namespace EOSAI
{
class StringAndFloat
{
	public:
		StringAndFloat(){ m_fValue = 0.0f; }
		StringAndFloat( CString str, float f ){ m_strValue = str; m_fValue = f; }
		CString  m_strValue;
		float    m_fValue;
};
}
