
//-------- DllExports.h --------//
#ifndef DLLEXPORTS_H
#define DLLEXPORTS_H

//#ifdef __dll__
#ifdef ThisIsADLL
#define IMPEXP __declspec(dllexport)
#else
#define IMPEXP __declspec(dllimport)
#endif 	// __ThisIsADLL__

#include "AIPlayer.h"

class CInterface;
class CWorldDistanceTool;
//class CEOSAILocation;

//extern "C"
//void* IMPEXP CreateFooClassInstance();
//extern "C" BOOL PASCAL EXPORT ExportedFunction();
//extern "C" void* EXPORT CreateAIPlayerManager2();
//extern "C" void* EXPORT GetGenieAIPtr();

//extern "C" void  EXPORT SetEOSAI( CInterface* pAIInterface, CEOSAIWorldDistanceTool* pAIWorldDistanceTool );

#endif	// DLLEXPORTS_H

