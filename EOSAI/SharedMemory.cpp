
#include "stdafx.h"
#include "SharedMemory.h"

void CopyString(const char* src, char* dest, int iSize)
{
	int iStringLength = strlen(src);
	int iCharactersToCopy = min(iStringLength, iSize - 1);
	memcpy(dest, src, iCharactersToCopy);
	dest[iCharactersToCopy] = 0;
}

void CopyString(const char* src, char10& dest){ CopyString(src, dest.m_szText, 10); }
void CopyString(const char* src, char20& dest){ CopyString(src, dest.m_szText, 20); }

void CopyString(char10& src, char10& dest){ CopyString(src.m_szText, dest.m_szText, 10); }
void CopyString(char20& src, char20& dest){ CopyString(src.m_szText, dest.m_szText, 20); }
void CopyString(char2000& src, char2000& dest){ CopyString(src.m_szText, dest.m_szText, 2000); }
