
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include < boost/interprocess/sync/interprocess_semaphore.hpp >
#define min(a, b) (((a) < (b)) ? (a) : (b))

using namespace boost::interprocess;
#define MAX_NUMBER_OF_PLAYERS 10+1

class RGBPixel{ public: void Set(int r1, int g1, int b1){ r = r1; g = g1; b = b1; } unsigned char r, g, b; };

class char10{ public: char10(){ m_szText[0] = 0; } char m_szText[10]; int size(){ return 10; } };
class char20{ public: char20(){ m_szText[0] = 0; } char m_szText[20]; int size(){ return 20; } };
class char100{ public: char100(){ m_szText[0] = 0; } char m_szText[100]; int size(){ return 100; } };
class char2000{ public: char2000(){ m_szText[0] = 0; } char m_szText[2000]; int size(){ return 2000; } };

void CopyString(const char* src, char* dest, int iSize);
void CopyString(const char* src, char10& dest);
void CopyString(const char* src, char20& dest);
void CopyString(char10& src, char10& dest);
void CopyString(char20& src, char20& dest);
void CopyString(char2000& src, char2000& dest);


// Overall processing State
#ifndef EnumAIOverallProcessingState_DEF
#define EnumAIOverallProcessingState_DEF
enum EnumAIOverallProcessingState
{
	EnumAIOverallProcessingState_Undefined,
	EnumAIOverallProcessingState_Processing,
	EnumAIOverallProcessingState_Idle,
};
#endif

enum EnumPlayerType
{
	Nonexistent,
	HumanPlayer,
	AIPlayer
};

enum EnumProcessingState
{
	Unprocessed,
	AnalyzingMap,
	Desires,
	TacticalProjects,
	ProcessingMessages,
	Complete,
};

enum EnumStanceTowardsOtherPlayers
{
	Self,
	Teammate,
	Ally,
	Neutral,
	Cautious,
	ColdWar,
	War
};

class SharedMemory_Desire
{
	public:
		SharedMemory_Desire(){ m_strDesc[0] = 0; }
		
		void  SetDesc(std::string str){ CopyString(str.c_str(), m_strDesc, 50); }

		bool  m_bValid = false;
		char  m_strDesc[50]; // e.g. "Oil Field #2 (+5 Oil) - ResourceTarget", "Russian T-32 Tank Squad - EnemyUnit"
		float m_fPosX = 0.0f;
		float m_fPosY = 0.0f;
		float m_fValue = 0.0f;
		float m_fDistance = 0.0f;
};

class SharedMemory_TacticalProject
{
	public:
		SharedMemory_TacticalProject(){ m_strDesc[0] = 0; for (int i = 0; i < 10; i++){ m_iDesires[i] = 0; } }
		void  SetDesc(std::string str){ CopyString(str.c_str(), m_strDesc, 50); }

		bool  m_bValid = false;
		char  m_strDesc[50];
		float m_fPosX = 0.0f;
		float m_fPosY = 0.0f;
		float m_fValue = 0.0f;
		float m_fDistance = 0.0f;
		int   m_iDesires[10];
};

class SharedMemory_Resource
{
	public:
		SharedMemory_Resource(){ m_strName[0] = 0; }
		char  m_strName[20];
		float m_fAmount;
};

class SharedMemory_EOSAIAIPlayer
{
	public:
		SharedMemory_EOSAIAIPlayer(){}
		//void SetPlayerName(std::string str){ CopyString(str.c_str(), m_czPlayerName ); }
		//void SetPlayerName(CString str){ ConvertUnicodeToChar(str, m_czPlayerName); }
		char* GetProcessingStateText()
		{
			if (m_eProcessingState == Unprocessed) return "Unprocessed";
			return "-";
		}

		//
		EnumPlayerType m_ePlayerType = Nonexistent; // AI, Human, Nonexistent
		//char  m_strPlayerName[20];  // e.g. "Rommel"
		char20  m_czPlayerName;  // e.g. "Rommel"
		int     m_iIntelligence3 = 1; // 1 = dumb, 3 = average, 5 = smart
		EnumProcessingState  m_eProcessingState = Unprocessed; // processing desires, tactial projects, etc.

		// Personality Traits
		

		// Foreign Relations
		float                          m_FeelingsTowardsOtherPlayers[MAX_NUMBER_OF_PLAYERS];
		EnumStanceTowardsOtherPlayers  m_StanceTowardsOtherPlayers[MAX_NUMBER_OF_PLAYERS];

		// Power
		float  m_fManufacturingPower;
		float  m_fUnitPower;

		// Resources
		SharedMemory_Resource         m_Resources[20];

		// Desires, Tactical Projects
		int                           m_iNumberOfDesires = 0;
		SharedMemory_Desire           m_Desires[200];
		int                           m_iNumberOfTacticalProjects = 0;
		SharedMemory_TacticalProject  m_TacticalProjects[50];
};

class SharedMemory_Region
{
	public:
		int   m_iOwner;
		float m_fOwnershipLevel01;
		float m_fPosX;
		float m_fPosY;
};

class SharedMemory_Poi
{
	public:
		char10 m_szName; // "London", "Infantry", etc
		char10 m_szMapDesc; // Short Desc: "10 Prod", "8 Oil", "Infantry"
		char20 m_szLongDesc; // Long Desc: "10 Prod", "8 Oil", "2/2 HP"
		int    m_iOwner;
        float  m_fPosX;
        float  m_fPosY;
        char10 m_szPoiType;  // Unit, City, Resource, etc.
};

#define GAME_MAP_IMAGE_WIDTH 200
#define GAME_MAP_IMAGE_HEIGHT 200
//#define COMMONDATA_STATUS_SIZE 2000
#define MAX_NUMBER_OF_POI_IN_LIST 100

class SharedMemoryBuffer
{
	public:
		SharedMemoryBuffer(){}

		interprocess_semaphore lock = 1;

		long    m_iFileVersion = 1;
		char    m_strEOSAIVersion[20];
		char    m_strEOSAIBuildDate[20];
		unsigned long long m_qwTimestamp;

		// Update flags
		int     m_iPlayerNamesUpdate = 0;

		int     m_iGameState = 1; // 0 = no game, 1 = game happening AI processing, 2 = game happening AI not processing
		int     m_iTurn = 1;
		EnumAIOverallProcessingState  m_eAIOverallProcessingState = EnumAIOverallProcessingState_Undefined;

		int     m_iNumberOfPlayers = 0;
		SharedMemory_EOSAIAIPlayer  m_Players[MAX_NUMBER_OF_PLAYERS];

		// Common Data
		//
			char2000 m_czCommonData_Status;
			//char    m_szCommonData_Status[COMMONDATA_STATUS_SIZE];
			int      m_iNumberOfPoi = 0;
			char     m_szCommonData_Poi[5000];
			SharedMemory_Poi  m_CommonData_Poi[MAX_NUMBER_OF_POI_IN_LIST];
			int     m_iCommonData_NumberOfUnitTemplates = 0;
			char    m_szCommonData_UnitTemplates[5000];
			int     m_iCommonData_NumberOfTechnologies = 0;
			char    m_szCommonData_Technologies[5000];

			int      m_iCommonData_MapWidth = 0;
			int      m_iCommonData_MapHeight = 0;
			RGBPixel m_GameMap[GAME_MAP_IMAGE_WIDTH][GAME_MAP_IMAGE_HEIGHT];

		// Processing Text
		//char    m_str
};

#endif SHARED_MEMORY_H
