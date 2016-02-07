#include "stdafx.h"
#include "Interproc.h"

#define BOOST_DATE_TIME_NO_LIB
//#include "boost/interprocess/interprocess_fwd.hpp"
//#include <boost/interprocess/managed_shared_memory.hpp>
//#include <boost/interprocess/shared_memory_object.hpp>
//#include <boost/interprocess/mapped_region.hpp>

//#include <boost/interprocess/managed_shared_memory.hpp>
//#include <boost/interprocess/containers/vector.hpp>
//#include <boost/interprocess/containers/string.hpp>
//#include <boost/interprocess/allocators/allocator.hpp>

#include "EOSAIMain.h"
#include "EOSAIInterface.h"
#include "EOSAITechnologyDesc.h"
//#include "EOSAI/City.h"
#include "City.h"
#include "SharedMemory.h"
#include <Mmsystem.h> // timeGetTime()
//#import Winmm.lib
DWORD m_dwLastUpdateTime = 0;

bool m_bIsProcessing = false;
DWORD m_dwLastIsProcessingMessage = 0;

bool    Interproc::m_bStatusUpdated = false;
CString Interproc::m_strFullStatus;

using namespace boost::interprocess;

#include < time.h >
#include < iostream >
#include < stdio.h >
#include < stdlib.h >
//#include < unistd.h >

shared_memory_object* g_pSharedMemoryObject = NULL;
mapped_region* g_pMappedRegion = NULL;
SharedMemoryBuffer* g_pSharedMemoryBuffer = NULL;
bool bHasBeenInitialized = false;


/*
http://stackoverflow.com/questions/10578522/conversion-of-cstring-to-char
// Convert to a char* string from a wide character
// CStringW string. To be safe, we allocate two bytes for each
// character in the original string, including the terminating
// null.
const size_t newsizew = (origw.GetLength() + 1)*2;
char *nstringw = new char[newsizew];
size_t convertedCharsw = 0;
wcstombs_s(&convertedCharsw, nstringw, newsizew, origw, _TRUNCATE );
cout << nstringw << " (char *)" << endl;
*/


void ConvertUnicodeToChar(CString strName, char* pOutput, int iSize)
{
	const size_t newsizew = (strName.GetLength() + 1) * 2;
	char *nstringw = new char[newsizew];
	size_t convertedCharsw = 0;
	wcstombs_s(&convertedCharsw, nstringw, newsizew, strName, _TRUNCATE);
	//std::cout << "nstringw=" << nstringw << " (char *)" << std::endl;
	CopyString(nstringw, pOutput, iSize);
	delete nstringw;
}
void ConvertUnicodeToChar(CString strSource, char10& czDest){ ConvertUnicodeToChar(strSource, czDest.m_szText, 10); }
void ConvertUnicodeToChar(CString strSource, char20& czDest){ ConvertUnicodeToChar(strSource, czDest.m_szText, 20); }
void ConvertUnicodeToChar(CString strSource, char2000& czDest){ ConvertUnicodeToChar(strSource, czDest.m_szText, 2000); }


// Create the shared memory file
void Interproc::Initialize()
{
	AddStatusLine(_T("EOSAI Initializing Interprocess Communication"));
	AddStatusLine(_T("EOSAI Initializing Interprocess Communication-2"));

	//ASSERT(g_pInterprocSharedMemoryObject == NULL);
	//ASSERT(g_pInterprocMappedRegion == NULL);
	//pShm = NULL;// (create_only, "EOSAISharedMemory", read_write);
	//pRegion = NULL; // (shm, read_write);

	// Remove any earlier shared-files
	boost::interprocess::shared_memory_object::remove("EOSAISharedMemory");
	boost::interprocess::shared_memory_object::remove("MySharedMemory");
	boost::interprocess::shared_memory_object::remove("shared_memory");

	using namespace boost::interprocess;

	//qDebug() << "ParentProcess";
	//engine.rootContext()->setContextProperty("MyController", "parentContext");
	//Remove shared memory on construction and destruction
	//struct shm_remove
	//{
	//shm_remove() { shared_memory_object::remove("EOSAISharedMemory"); }
	//~shm_remove(){ shared_memory_object::remove("EOSAISharedMemory"); }
	//} remover;
	
	if (true)
	{
		// http://blog.wolfgang-vogl.com/?p=528
		// http://www.boost.org/doc/libs/1_42_0/doc/html/boost/interprocess/interprocess_semaphore.html

		//srand((unsigned int)time(NULL));

		//use old shared memory if exists else create a new one
		ASSERT(g_pSharedMemoryObject == NULL);
		//g_pSharedMemoryObject = new shared_memory_object(open_or_create, "shared_memory", read_write);
		//shared_memory_object shm(open_or_create, "shared_memory", read_write);
		g_pSharedMemoryObject = new shared_memory_object(open_or_create, "EOSAISharedMemory", read_write);

		//set the size of the memory object
		//shm.truncate(sizeof(SharedMemoryBuffer));
		g_pSharedMemoryObject->truncate(sizeof(SharedMemoryBuffer));

		//map the whole shared memory in this process
		//mapped_region region(*g_pSharedMemoryObject, read_write);
		g_pMappedRegion = new mapped_region(*g_pSharedMemoryObject, read_write);
		//mapped_region region(shm, read_write);

		//get the region address
		void* pAddr = g_pMappedRegion->get_address();

		//create a shared memory buffer in memory
		//SharedMemoryBuffer *data = new (addr)SharedMemoryBuffer;
		//SharedMemoryBuffer *data = (SharedMemoryBuffer*)addr; // new(addr)SharedMemoryBuffer;
		g_pSharedMemoryBuffer = static_cast<SharedMemoryBuffer*>(pAddr);

		//
		// How should it work?
		//   EOSAI should have one semaphore. If it's set to 0, then there is no lock. Set the lock, and read/write. Set back to unlock after it is done.
		//     What if the EOSAI UI crashes? It might retain the lock. Maybe I could have a "who's reading/writing" value (EOSAI, EOSAIUI-timestamp).
		//     If the EOSAI UI detects another EOSAIUI-timestamp, it could use only the latest timestamp. Older timestamps shutdown?
		//
		//   EOSAI should have a values: W1 R0, which allows the writer to change values
		//   

		//while (true){
		//for (int j = 0; j < 10; j++ )
		{
			// wait until the written number gets executed
			// If "writer" is 0, wait. If "writer" is 1, then continue (and decrement the number).
			//g_pSharedMemoryBuffer->writer.wait();
			// If "lock" is 0, wait. If "lock" is 1, then continue (and decrement the number).
//			g_pSharedMemoryBuffer->lock.wait();
			//g_pSharedMemoryBuffer->writer.wait();

			//print the square root of the original value
			//std::cout << "Square Root: " << g_pSharedMemoryBuffer->m_iValue << "\n";

			g_pSharedMemoryBuffer->m_iFileVersion = 1;

			//string_copy(g_pSharedMemoryBuffer->m_strEOSAIVersion, "0.1.333", 20);
			//string_copy(g_pSharedMemoryBuffer->m_strEOSAIBuildDate, __DATE__, 20);
			CopyString("0.1.333", g_pSharedMemoryBuffer->m_strEOSAIVersion, 20);
			CopyString(__DATE__, g_pSharedMemoryBuffer->m_strEOSAIBuildDate, 20); // I should probably read the created date of the DLL. 

			g_pSharedMemoryBuffer->m_iPlayerNamesUpdate = 10;
			g_pSharedMemoryBuffer->m_iGameState = 0;
			g_pSharedMemoryBuffer->m_iCurrentTurn = 0;
			g_pSharedMemoryBuffer->m_qwTimestamp = 0;
			g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn = 0;

			//create the original value
			//g_pSharedMemoryBuffer->m_iValue = 22;// rand() % 100 + 1;

			//std::string myString = "Brit";
			//if (g_pSharedMemoryBuffer->m_iValue < 50){ myString = "Brit2"; }
			//else{ myString = "Brit3"; }
			//memcpy(g_pSharedMemoryBuffer->str, myString.c_str(), 100);

			//for (int i = 0; i < MAX_NUMBER_OF_PLAYERS-1; i++)
			//{
			//	g_pSharedMemoryBuffer->m_Players[i].m_strPlayerName[0] = 0;
				//g_pSharedMemoryBuffer->m_strPlayerNames[i][0] = 0;
			//}

			g_pSharedMemoryBuffer->m_iNumberOfPlayers = 0;

			for (int x = 0; x < 100; x++)
			{
				for (int y = 0; y < 100; y++)
				{
					g_pSharedMemoryBuffer->m_GameMap[x][y].r = 0;
					g_pSharedMemoryBuffer->m_GameMap[x][y].g = 0;
					g_pSharedMemoryBuffer->m_GameMap[x][y].b = 0;
				}
			}


			//std::wstring myString2 = _T("Unicode1");
			//if (g_pSharedMemoryBuffer->value < 50){ myString2 = _T("Unicode2"); }
			//else{ myString2 = _T("Unicode3"); }
			//memcpy(g_pSharedMemoryBuffer->str2, myString2.c_str(), 100);

			//Sleep(50);
			//reader can execute written number
			// Increment the "reader" value. The other process will handle the data if "reader" is set to 1.
			//data->reader.post();
			// Increment the "lock" value. The other process will handle the data if "lock" is set to 1.
			g_pSharedMemoryBuffer->lock.post();
			//Sleep(50);
		}
		int j = 0;
	}
	int h = 0;

	bHasBeenInitialized = true;
}

void Interproc::IsProcessing(bool b)
{
	m_bIsProcessing = b;
	if (m_bIsProcessing)
	{
		m_dwLastIsProcessingMessage = timeGetTime();
	}
}

void Interproc::AddStatusLine(CString str)
{
	m_bStatusUpdated = true;

	CTime Time = CTime::GetCurrentTime();
//	_ftprintf(fpCheckpoint, _T("%d/%d/%d %d:%02d:%02d\n"),
//		Time.GetYear(), Time.GetMonth(), Time.GetDay(),
//		Time.GetHour(), Time.GetMinute(), Time.GetSecond());

	CString strFullLine;
	strFullLine.Format(_T("%d/%d %d:%02d:%02d | %s\n"), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(), str );
	m_strFullStatus = strFullLine + m_strFullStatus;
}

void Interproc::UpdateSharedMemory()
{
	using namespace boost::interprocess;

	if (bHasBeenInitialized == false) return;

	// Only update this once every 200 milliseconds
	DWORD dwCurrentTimeInMilliseconds = timeGetTime();
	if (dwCurrentTimeInMilliseconds < m_dwLastUpdateTime + 200) return; // Don't do an update yet
	m_dwLastUpdateTime = dwCurrentTimeInMilliseconds;

	EnumAIOverallProcessingState eProcessingProcessing = EnumAIOverallProcessingState_Idle;
	if (m_bIsProcessing)
	{
		eProcessingProcessing = EnumAIOverallProcessingState_Processing;
	}
	else
	{
		DWORD dwTimeSinceLastIsProcessingMessage = dwCurrentTimeInMilliseconds - m_dwLastIsProcessingMessage;
		if (dwTimeSinceLastIsProcessingMessage < 3000) eProcessingProcessing = EnumAIOverallProcessingState_Processing;
	}

	ASSERT(g_pSharedMemoryBuffer);
	if (g_pSharedMemoryBuffer == NULL) return;

	g_pSharedMemoryBuffer->lock.wait();  // Wait for a lock
	{
		if (g_pEOSAIInterface)
		{
			// First, read data written by the EOSAIDebugger
			g_pEOSAIMain->SetGameShouldPauseAtTheEndOfTurn(g_pSharedMemoryBuffer->m_iGameShouldPauseAtTheEndOfTurn);

			// Update current turn
			g_pSharedMemoryBuffer->m_iCurrentTurn = g_pEOSAIInterface->GetCurrentTurn();

			
			CTime currentTime = CTime::GetCurrentTime();  //CTime::GetAsSystemTime();
			g_pSharedMemoryBuffer->m_qwTimestamp = (unsigned long long) currentTime.GetTime();

			//
			g_pSharedMemoryBuffer->m_eAIOverallProcessingState = eProcessingProcessing;

			CString strTemp;
			// Update CommonData
			if (m_bStatusUpdated)
			{
				/*
				CString strStatus;
				for (int i = 0; i < 4; i++)
				{
					strTemp.Format(_T("Turn x%d\n"), g_pEOSAIInterface->GetCurrentTurn()); strStatus += strTemp;
					strTemp.Format(_T("Loaded %d Poi\n"), g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetCount()); strStatus += strTemp;
					strTemp.Format(_T("  %d Cities\n"), g_pEOSAIMain->GetAICommonData()->GetNumberOfCities()); strStatus += strTemp;
					strTemp.Format(_T("  %d Resources\n"), g_pEOSAIMain->GetAICommonData()->GetNumberOfResources()); strStatus += strTemp;
					strTemp.Format(_T("  %d Airfields\n"), g_pEOSAIMain->GetAICommonData()->GetNumberOfAirfields()); strStatus += strTemp;
					strTemp.Format(_T("  %d Units\n"), g_pEOSAIMain->GetAICommonData()->GetNumberOfUnits()); strStatus += strTemp;
					//
					strTemp.Format(_T("%d Unit Templates\n"), g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates()->GetCount()); strStatus += strTemp;
					strTemp.Format(_T("Warning: %d Building Descriptions\n"), g_pEOSAIMain->GetAIGameRules()->GetBuildingDescriptionList()->GetCount()); strStatus += strTemp;
					strTemp.Format(_T("Error: %d Technology Descriptions\n"), g_pEOSAIMain->GetAIGameRules()->GetTechnologyDescsList()->GetCount()); strStatus += strTemp;
				}
				*/
				
				m_bStatusUpdated = false;
				ConvertUnicodeToChar(m_strFullStatus, g_pSharedMemoryBuffer->m_czCommonData_Status);
				//ConvertUnicodeToChar(strStatus, g_pSharedMemoryBuffer->m_czCommonData_Status);
				g_pSharedMemoryBuffer->m_bStatusListUpdated = true;
			}
			{
				CString strStatus;
				g_pSharedMemoryBuffer->m_iNumberOfPoi = g_pEOSAIMain->GetAICommonData()->GetNumberOfPoi();
				int iNumberOfPlayers = g_pEOSAIInterface->GetNumberOfGamePlayers();

				//SharedMemory_Poi  m_CommonData_Poi[MAX_NUMBER_OF_POI_IN_LIST];
				int iCurrentSharedMemoryPoi = 0;
				POSITION pos = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetHeadPosition();
				while (pos)
				{
					CEOSAIPoiObject* pPoiObject = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetNext(pos);
					if (iCurrentSharedMemoryPoi < MAX_NUMBER_OF_POI_IN_LIST)
					{
						g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_fPosX = pPoiObject->GetLocation().GetRealX();
						g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_fPosY = pPoiObject->GetLocation().GetRealY();
						g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_iOwner = pPoiObject->GetOwner();

						CString strDesc;
						if (pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City)
						{
							CopyString("City", g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szPoiType );

							CEOSAICity* pCity = dynamic_cast<CEOSAICity*>(pPoiObject);
							strDesc.Format( _T("%1.0f Prod"), pCity->GetProduction());
							ConvertUnicodeToChar(strDesc, g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szMapDesc);
							ConvertUnicodeToChar(strDesc, g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szLongDesc);
							ConvertUnicodeToChar(pPoiObject->GetName(), g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szName);
						}
						else if (pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource)
						{
							CopyString("Resource", g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szPoiType );

							CEOSAIResource* pRes = dynamic_cast<CEOSAIResource*>(pPoiObject);
							strDesc.Format(_T("%1.0f %s"), pRes->GetResourcePerTurn(), pRes->GetResourceType());
							ConvertUnicodeToChar(strDesc, g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szMapDesc);
							ConvertUnicodeToChar(strDesc, g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szLongDesc);
							ConvertUnicodeToChar(pPoiObject->GetName(), g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szName);
						}
						else if (pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit)
						{
							CopyString("Unit", g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szPoiType );

							CEOSAIUnit* pUnit = dynamic_cast<CEOSAIUnit*>(pPoiObject);
							strDesc.Format(_T("%d/%d HP"), pUnit->GetCurrentHP(), pUnit->GetAIUnitTemplate()->GetMaxHP());
							if (g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_iOwner > 0)
							{
								ConvertUnicodeToChar(pUnit->GetAIUnitTemplate()->GetInternalName(), g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szMapDesc);
							}
							ConvertUnicodeToChar(strDesc, g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szLongDesc);
							ConvertUnicodeToChar(pUnit->GetAIUnitTemplate()->GetInternalName(), g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szName);
						}
						else
						{
						}

						/*
						if (pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit)
							CopyString("Unit", g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szPoiType, 10);
						if (pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_City)
							CopyString("City", g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szPoiType, 10);
						if (pPoiObject->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Resource)
							CopyString("Resource", g_pSharedMemoryBuffer->m_CommonData_Poi[iCurrentSharedMemoryPoi].m_szPoiType, 10);
						*/
					}
					iCurrentSharedMemoryPoi++;
				}

				int iNumberOfCities = g_pEOSAIMain->GetAICommonData()->GetNumberOfCities();
				strTemp.Format(_T("Cities (%d)\n"), iNumberOfCities); strStatus += strTemp;
				int iNumberOfGamePlayers = g_pEOSAIInterface->GetNumberOfGamePlayers();
				for (int iPlayer = 0; iPlayer <= iNumberOfGamePlayers; iPlayer++)
				{
					strTemp.Format(_T("  Player %d\n"), iPlayer); strStatus += strTemp;
					POSITION pos = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetHeadPosition();
					while (pos)
					{
						CEOSAIPoiObject* pPoiObject = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetNext(pos);
						if (pPoiObject->GetOwner() == iPlayer)
						{
							CEOSAICity* pCity = dynamic_cast<CEOSAICity*>(pPoiObject);
							if (pCity)
							{
								strTemp.Format(_T("    %s: Pop:%0.2f, Prod:%0.1f\n"), pCity->GetName(), pCity->GetPopulation(), pCity->GetProduction()); strStatus += strTemp;
							}
						}
					}
				}
				strStatus += "\n";

				int iNumberOfResources = g_pEOSAIMain->GetAICommonData()->GetNumberOfResources();
				strTemp.Format(_T("Resources (%d)\n"), iNumberOfResources); strStatus += strTemp;
				for (int iPlayer = 0; iPlayer <= iNumberOfGamePlayers; iPlayer++)
				{
					strTemp.Format(_T("  Player %d\n"), iPlayer); strStatus += strTemp;
					POSITION pos = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetHeadPosition();
					while (pos)
					{
						CEOSAIPoiObject* pPoiObject = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetNext(pos);
						if (pPoiObject->GetOwner() == iPlayer)
						{
							CEOSAIResource* pRes = dynamic_cast<CEOSAIResource*>(pPoiObject);
							if (pRes)
							{
								strTemp.Format(_T("    %s %0.1f/turn \"%s\"\n"), pRes->GetResourceType(), pRes->GetResourcePerTurn(), pRes->GetName()); strStatus += strTemp;
							}
						}
					}
				}
				strStatus += "\n";

				strTemp.Format(_T("Airfields (%d)\n"), g_pEOSAIMain->GetAICommonData()->GetNumberOfAirfields()); strStatus += strTemp;
				strStatus += "\n";

				int iNumberOfUnits = g_pEOSAIMain->GetAICommonData()->GetNumberOfResources();
				strTemp.Format(_T("Units (%d)\n"), iNumberOfUnits); strStatus += strTemp;
				for (int iPlayer = 0; iPlayer <= iNumberOfGamePlayers; iPlayer++)
				{
					strTemp.Format(_T("  Player %d\n"), iPlayer); strStatus += strTemp;
					POSITION pos = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetHeadPosition();
					while (pos)
					{
						CEOSAIPoiObject* pPoiObject = g_pEOSAIMain->GetAICommonData()->GetAIPoiObjects()->GetNext(pos);
						if (pPoiObject->GetOwner() == iPlayer)
						{
							CEOSAIUnit* pUnit = dynamic_cast<CEOSAIUnit*>(pPoiObject);
							if (pUnit)
							{
								strTemp.Format(_T("    %s \"%s\"\n"), pUnit->GetAIUnitTemplate()->GetInternalName(), pUnit->GetName()); strStatus += strTemp;
							}
						}
					}
				}
				strStatus += "\n";

				//
				ConvertUnicodeToChar(strStatus, g_pSharedMemoryBuffer->m_szCommonData_Poi, 5000);
			}
			{
				CString strStatus;
				int iNumberOfUnitTemplates = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates()->GetCount();
				//strTemp.Format(_T("%d Unit Templates\n"), iNumberOfUnits); strStatus += strTemp;
				strTemp.Format(_T("Unit Templates (%d)\n"), iNumberOfUnitTemplates); strStatus += strTemp;
				POSITION pos = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates()->GetHeadPosition();
				while ( pos )
				{
					CEOSAIUnitTemplate* pUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplates()->GetNext(pos);
					if (pUnitTemplate)
					{
						strTemp.Format(_T("    %s - Cost:%1.0f\n"), pUnitTemplate->GetInternalName(), pUnitTemplate->GetProductionCost() ); strStatus += strTemp;
					}
				}
				strStatus += "\n";

				//
				g_pSharedMemoryBuffer->m_iCommonData_NumberOfUnitTemplates = iNumberOfUnitTemplates;
				ConvertUnicodeToChar(strStatus, g_pSharedMemoryBuffer->m_szCommonData_UnitTemplates, 5000);
			}
			{
				CString strStatus;
				int iNumberOfTechnologies = g_pEOSAIMain->GetAIGameRules()->GetTechnologyDescsList()->GetCount();
				//strTemp.Format(_T("%d Unit Templates\n"), iNumberOfUnits); strStatus += strTemp;
				strTemp.Format(_T("Technologies (%d)\n"), iNumberOfTechnologies); strStatus += strTemp;
				POSITION pos = g_pEOSAIMain->GetAIGameRules()->GetTechnologyDescsList()->GetHeadPosition();
				while (pos)
				{
					CEOSAITechnologyDesc* pTechnologyDesc = g_pEOSAIMain->GetAIGameRules()->GetTechnologyDescsList()->GetNext(pos);
					if (pTechnologyDesc)
					{
						strTemp.Format(_T("    %s - Cost:%d\n"), pTechnologyDesc->GetInternalName(), pTechnologyDesc->GetCost() ); strStatus += strTemp;
					}
				}
				strStatus += "\n";

				//
				g_pSharedMemoryBuffer->m_iCommonData_NumberOfTechnologies = iNumberOfTechnologies;
				ConvertUnicodeToChar(strStatus, g_pSharedMemoryBuffer->m_szCommonData_Technologies, 5000);
			}

			// Map Information
			g_pSharedMemoryBuffer->m_iCommonData_MapWidth = g_pWorldDistanceTool->GetPixelWidth();
			g_pSharedMemoryBuffer->m_iCommonData_MapHeight = g_pWorldDistanceTool->GetPixelHeight();

			//
			for (int ix = 0; ix < GAME_MAP_IMAGE_WIDTH; ix++)
			{
				for (int iy = 0; iy < GAME_MAP_IMAGE_HEIGHT; iy++)
				{
					float fX = ((float)ix) / ((float)GAME_MAP_IMAGE_WIDTH) * (float)g_pWorldDistanceTool->GetPixelWidth();
					float fY = ((float)iy) / ((float)GAME_MAP_IMAGE_HEIGHT) * (float)g_pWorldDistanceTool->GetPixelHeight();

					/*
					#define TERRAIN_UNKNOWN    0 // special case ( returned by CWorldDesc::GetActualTerrainType )
					#define TERRAIN_WATER      1 // special case ( returned by CWorldDesc::GetActualTerrainType )
					#define TERRAIN_COAST      2 // special case ( this is used to keep ships off the coast )
					#define TERRAIN_ARCTIC     3
					#define TERRAIN_DESERT     4
					#define TERRAIN_GRASS      5
					#define TERRAIN_FOREST     6
					#define TERRAIN_ROAD       7 // special case ( returned by CWorldDesc::GetActualTerrainType )
					#define TERRAIN_CITY       8 // special case ( returned by CWorldDesc::GetActualTerrainType )
					#define TERRAIN_MOUNTAIN   9 // special case ( returned by CWorldDesc::GetActualTerrainType )
					#define TERRAIN_AIR       10 // special case
					#define TERRAIN_SWAMP     11
					#define TERRAIN_COUNT     12
					#define TERRAIN_IMPASSIBLE 100
					*/

					RGBPixel rgb;
					/* I might want to switch to this, instead of passing the colors
					int iTerrainType = g_pEOSAIInterface->GetTerrainColorByPixelLocation(fX, fY);
					g_pSharedMemoryBuffer->m_TerrainColor[0] = rgb.Set(0, 0, 0);
					if (iTerrainType == TERRAIN_UNKNOWN){ rgb.Set(0, 0, 0); }
					else if (iTerrainType == TERRAIN_WATER){ rgb.Set(0, 100, 200); }
					else if (iTerrainType == TERRAIN_COAST){ rgb.Set(100, 100, 200); }
					else if (iTerrainType == TERRAIN_ARCTIC){ rgb.Set(200, 200, 200); }
					else if (iTerrainType == TERRAIN_DESERT){ rgb.Set(200, 200, 100); }
					else if (iTerrainType == TERRAIN_GRASS){ rgb.Set(100, 255, 100); }
					else if (iTerrainType == TERRAIN_FOREST){ rgb.Set(80, 155, 80); }
					else if (iTerrainType == TERRAIN_ROAD){ rgb.Set(100, 100, 100); }
					else if (iTerrainType == TERRAIN_CITY){ rgb.Set(150, 150, 150); }
					else if (iTerrainType == TERRAIN_MOUNTAIN){ rgb.Set(200, 200, 100); }
					else if (iTerrainType == TERRAIN_AIR){ rgb.Set(100, 255, 100); }
					else if (iTerrainType == TERRAIN_SWAMP){ rgb.Set(100, 255, 100); }
					else{ rgb.Set(100, 100, 100); }
					g_pSharedMemoryBuffer->m_GameMapTerrain[ix][iy] = iTerrainType;
					*/

					//int iTerrainType = 1;
					int iTerrainType = g_pEOSAIInterface->GetTerrainColorByPixelLocation(fX,fY);
					if (     iTerrainType == TERRAIN_UNKNOWN ){ rgb.Set(0, 0, 0); }
					else if (iTerrainType == TERRAIN_WATER   ){ rgb.Set(0, 100, 200); }
					else if (iTerrainType == TERRAIN_COAST   ){ rgb.Set(100, 100, 200); }
					else if (iTerrainType == TERRAIN_ARCTIC  ){ rgb.Set(200, 200, 200); }
					else if (iTerrainType == TERRAIN_DESERT  ){ rgb.Set(200, 200, 100); }
					else if (iTerrainType == TERRAIN_GRASS   ){ rgb.Set(100, 255, 100); }
					else if (iTerrainType == TERRAIN_FOREST  ){ rgb.Set( 80, 155,  80); }
					else if (iTerrainType == TERRAIN_ROAD    ){ rgb.Set(100, 100, 100); }
					else if (iTerrainType == TERRAIN_CITY    ){ rgb.Set(200, 200, 200); }
					else if (iTerrainType == TERRAIN_MOUNTAIN){ rgb.Set( 80,  80,  80); }
					else if (iTerrainType == TERRAIN_AIR     ){ rgb.Set(100, 255, 100); }
					else if (iTerrainType == TERRAIN_SWAMP   ){ rgb.Set(100, 255, 100); }
					else{ rgb.Set(100, 100, 100); }
					g_pSharedMemoryBuffer->m_GameMap[ix][iy].r = rgb.r;
					g_pSharedMemoryBuffer->m_GameMap[ix][iy].g = rgb.g;
					g_pSharedMemoryBuffer->m_GameMap[ix][iy].b = rgb.b;
				}
			}

			// Update players
			int iNumberOfGamePlayers = g_pEOSAIInterface->GetNumberOfGamePlayers();
			g_pSharedMemoryBuffer->m_iNumberOfPlayers = iNumberOfGamePlayers;
			for (int iPlayer = 1; iPlayer <= iNumberOfGamePlayers; iPlayer++)
			{
				if (g_pEOSAIInterface->GetGamePlayer(iPlayer) == NULL) continue;

				if (g_pEOSAIInterface->GetGamePlayer(iPlayer)->IsAI())
				{
					g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType = AIPlayer;
					if (g_pEOSAIInterface->GetAIPlayer(iPlayer))
					{
						EOSAI::AIPlayer* pAIPlayer = g_pEOSAIInterface->GetAIPlayer(iPlayer);
						g_pSharedMemoryBuffer->m_Players[iPlayer].m_iIntelligence3 = g_pEOSAIInterface->GetAIPlayer(iPlayer)->GetAIBrainIntelligence3();

						// Processing State
						if (g_pEOSAIInterface->GetAIPlayer(iPlayer)->m_eProcessingState == EOSAI::AIPlayer::enumWaitingForServerToPlayerUpdate)
						{
							g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState = Unprocessed;
						}
						if (g_pEOSAIInterface->GetAIPlayer(iPlayer)->m_eProcessingState == EOSAI::AIPlayer::enumBeginProcessingTurn)
						{
							//ASSERT(false); // I don't think we can currently get into this situation right now. I need to add more processing states.
							g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState = AnalyzingMap;
						}
						if (g_pEOSAIInterface->GetAIPlayer(iPlayer)->m_eProcessingState == EOSAI::AIPlayer::enumReadyToSendOrdersToServer)
						{
							g_pSharedMemoryBuffer->m_Players[iPlayer].m_eProcessingState = Complete;
						}
						/*
						enumWaitingForServerToPlayerUpdate = 1,
							//enumWaitingForTurnAndRulesUpdate = 1,
							//enumWaitingForAutosaveAndLocalPlayerTurnReplayEnd = 2,
							enumBeginProcessingTurn = 3,
							//enumProcessingTurn = 4,
							enumReadyToSendOrdersToServer = 5, // Waiting For Local Human Player To Submit Turn
						*/
					}
				}
				if (g_pEOSAIInterface->GetGamePlayer(iPlayer)->IsHuman())
				{
					g_pSharedMemoryBuffer->m_Players[iPlayer].m_ePlayerType = HumanPlayer;
				}

				CString strName = g_pEOSAIInterface->GetGamePlayer(iPlayer)->GetPlayerName();
				//char strOutputName[20];
				//ConvertUnicodeToChar(strName, strOutputName, 20);
				/*
				http://stackoverflow.com/questions/10578522/conversion-of-cstring-to-char
				// Convert to a char* string from a wide character
				// CStringW string. To be safe, we allocate two bytes for each
				// character in the original string, including the terminating
				// null.
				const size_t newsizew = (origw.GetLength() + 1)*2;
				char *nstringw = new char[newsizew];
				size_t convertedCharsw = 0;
				wcstombs_s(&convertedCharsw, nstringw, newsizew, origw, _TRUNCATE );
				cout << nstringw << " (char *)" << endl;
				*/
				/*
				const size_t newsizew = (strName.GetLength() + 1) * 2;
				char *nstringw = new char[newsizew];
				size_t convertedCharsw = 0;
				wcstombs_s(&convertedCharsw, nstringw, newsizew, strName, _TRUNCATE);
				std::cout << "nstringw=" << nstringw << " (char *)" << std::endl;
				*/
				ConvertUnicodeToChar(strName, g_pSharedMemoryBuffer->m_Players[iPlayer].m_czPlayerName);
				//g_pSharedMemoryBuffer->m_Players[iPlayer].SetPlayerName(strName);
				//g_pSharedMemoryBuffer->m_Players[iPlayer].SetPlayerName(strOutputName);



				//g_pSharedMemoryBuffer->m_Players[3].SetPlayerName("XYZ");

				//g_pSharedMemoryBuffer->m_iNumberOfPlayers = g_pEOSAIInterface->GetNumberOfPlayers();
				//g_pSharedMemoryBuffer->m_Players[iPlayer].SetPlayerName(g_pEOSAIInterface->GetGamePlayer(iPlayer)->GetPlayerName().GetCurrentTurn();
				//char* pPlayerName = g_pEOSAIInterface->GetGamePlayer(iPlayer)->GetPlayerName();
				//g_pSharedMemoryBuffer->m_Players[iPlayer].SetPlayerName(pPlayerName); // g_pEOSAIInterface->GetGamePlayer(iPlayer)->GetPlayerName().GetCurrentTurn();
			}
		}
	}
	g_pSharedMemoryBuffer->lock.post();  // Release the lock
}

void Interproc::Shutdown()
{
	boost::interprocess::shared_memory_object::remove("EOSAISharedMemory");
	bHasBeenInitialized = false;
}

