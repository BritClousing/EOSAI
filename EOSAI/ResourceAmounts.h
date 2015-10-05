#pragma once

#include <map>

namespace EOSAI
{
	class ResourceAmounts
	{
	public:
		void   Clear() { m_Resource.clear(); }

		float           Get(CString strType) { return m_Resource[strType]; }
		void            Set(CString strType, float f) { m_Resource[strType] = f; }
		float&          operator[](CString strType) { return m_Resource[strType]; }
		ResourceAmounts operator+(ResourceAmounts& otherRes)
		{
			ResourceAmounts newRes = *this;
			std::map<CString, float>::iterator iter = otherRes.m_Resource.begin();
			for (iter = otherRes.m_Resource.begin(); iter != otherRes.m_Resource.end(); iter++) { newRes[iter->first] += otherRes[iter->first]; }
			return newRes;
		}
		ResourceAmounts operator-(ResourceAmounts& otherRes)
		{
			ResourceAmounts newRes = *this;
			std::map<CString, float>::iterator iter = otherRes.m_Resource.begin();
			for (iter = otherRes.m_Resource.begin(); iter != otherRes.m_Resource.end(); iter++) { newRes[iter->first] -= otherRes[iter->first]; }
			return newRes;
		}
		ResourceAmounts& operator+=(ResourceAmounts& otherRes)
		{
			std::map<CString, float>::iterator iter = otherRes.m_Resource.begin();
			for (iter = otherRes.m_Resource.begin(); iter != otherRes.m_Resource.end(); iter++) { (*this)[iter->first] += otherRes[iter->first]; }
			return *this;
		}
		ResourceAmounts& operator-=(ResourceAmounts& otherRes)
		{
			ResourceAmounts newRes = *this;
			std::map<CString, float>::iterator iter = otherRes.m_Resource.begin();
			for (iter = otherRes.m_Resource.begin(); iter != otherRes.m_Resource.end(); iter++) { (*this)[iter->first] -= otherRes[iter->first]; }
			return *this;
		}
		float CombinedResourceValues()
		{
			float fValue = 0.0f;
			std::map<CString, float>::iterator iter = m_Resource.begin();
			for (iter = m_Resource.begin(); iter != m_Resource.end(); iter++) { fValue += m_Resource[iter->first]; }
			return fValue;
		}

		std::map<CString, float> m_Resource; // type and amount
	};
}
