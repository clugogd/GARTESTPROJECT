#include "Audio.h"
#include <assert.h>
#include <string>
using std::string;

#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include <AK/SoundEngine/Common/AkModule.h>			// Default memory and stream managers
#include <AK/MusicEngine/Common/AkMusicEngine.h>	// Music Engine
#include <AK/Tools/Common/AkLock.h>
#include <AK/Tools/Common/AkMonitorError.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkDefaultIOHookBlocking.h>
#include "Wwise_IDs.h"

/*
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>		// Streaming Manager
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>	// AkStreamMgrModule
#include <AK/SoundEngine/Common/AkMemoryMgr.h>		// Memory Manager
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>	// Spatial Audio module

#include <AK/IBytes.h>
*/

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

#ifdef AK_WIN
	#include <AkSoundEngineDLL.h>
#else
#include <AK/Plugin/AllPluginsRegistrationHelpers.h>

namespace AK
{
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
}

CAkDefaultIOHookBlocking g_lowLevelIO;

#endif

using namespace AK;

static const AkGameObjectID LISTENER_ID = 10001;
static const AkGameObjectID EMITTER_OBJ = 100;

string szBankName = "music.bnk";

CAudio::CAudio()
{
	m_nMaxPools = 20;
}


CAudio::~CAudio()
{
}

bool CAudio::Init()
{
	//	Initialize the audio engine
	//	Memory
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = m_nMaxPools;

	//	Streaming
	AkStreamMgrSettings stmSettings;
	StreamMgr::GetDefaultSettings(stmSettings);

	AkDeviceSettings deviceSettings;
	StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	AkInitSettings l_InitSettings;
	AkPlatformInitSettings l_platInitSettings;
	SoundEngine::GetDefaultInitSettings(l_InitSettings);
	SoundEngine::GetDefaultPlatformInitSettings(l_platInitSettings);

	//	Setting pool sizes for this application.
	l_InitSettings.uDefaultPoolSize = 2 * 1024 * 1024;
	l_platInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	AkMusicSettings musicInit;
	MusicEngine::GetDefaultInitSettings(musicInit);

#ifdef AK_WIN
	AKRESULT eResult = SOUNDENGINE_DLL::Init(&memSettings, &stmSettings, &deviceSettings, &l_InitSettings, &l_platInitSettings, &musicInit);

	if (eResult != AK_Success)
	{
		//	Ten we will run the game without sound
		SOUNDENGINE_DLL::Term();
		return false;
	}

	//	Load initialization and main soundbanks
	SOUNDENGINE_DLL::SetBasePath(L"Assets/Soundbanks");
	AK::StreamMgr::SetCurrentLanguage(L"English(US)");
#else
	// Create and initialise an instance of our memory manager.
	if (MemoryMgr::Init(&memSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the memory manager.");
		return;
	}

	// Create and initialise an instance of the default stream manager.
	if (!StreamMgr::Create(stmSettings))
	{
		AKASSERT(!"Could not create the Stream Manager");
		return;
	}

	// Create an IO device.
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		AKASSERT(!"Cannot create streaming I/O device");
		return;
	}

	// Initialize sound engine.
	if (SoundEngine::Init(&l_InitSettings, &l_platInitSetings) != AK_Success)
	{
		AKASSERT(!"Cannot initialize sound engine");
		return;
	}

	// Initialize music engine.
	if (MusicEngine::Init(&musicInit) != AK_Success)
	{
		AKASSERT(!"Cannot initialize music engine");
		return;
	}

	// Register plugins
	/// Note: This a convenience method for rapid prototyping. 
	/// To reduce executable code size register/link only the plug-ins required by your game 
	if (AK::SoundEngine::RegisterAllPlugins() != AK_Success)
	{
		AKASSERT(!"Error while registering plug-ins");
		return;
	}

	// load initialization and main soundbanks

	g_lowLevelIO.SetBasePath(AKTEXT("soundbanks/Mac/"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
#endif

#ifndef AK_OPTIMIZED
	//	Initialize communication
	AkCommSettings settingsComm;
	AK::Comm::GetDefaultInitSettings(settingsComm);
	AKPLATFORM::SafeStrCpy(settingsComm.szAppNetworkName, "DX11TestProject", AK_COMM_SETTINGS_MAX_STRING_SIZE);
	if (AK::Comm::Init(settingsComm) != AK_Success)
	{
		AKASSERT(!"Cannot Initialize music communication");
		return false;
	}
#endif	//AK_OPTIMIZED

	AkBankID bankID;
	AKRESULT retValue;
	retValue = SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	retValue = SoundEngine::LoadBank("main.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	//	Initialize volume parameters to sensible default values
	MusicVolume(255);
	SoundVolume(255);
	VoiceVolume(255);

	LoadBank();

	return true;
}
bool CAudio::Update()
{
//	HandleInput();

#ifdef AK_WIN
	SOUNDENGINE_DLL::Tick();
#else
	SoundEngine::RenderAudio();
#endif

	return true;
}
bool CAudio::Shutdown()
{
	Term();
	return true;
}
bool CAudio::Term()
{
#ifdef AK_WIN
	SOUNDENGINE_DLL::Term();
#else
#ifndef AK_OPTIMIZED
	Comm::Term();
#endif	//AK_OPTIMIZED

	MusicEngine::Term();
	if (AK::SoundEngine::IsInitialized())
	{
		SoundEngine::Term();
	}

	g_lowLevelIO.Term();
	
	if (IAkStreamMgr::Get())
		IAkStreamMgr::Get()->Destroy();

	if (AK::MemoryMgr::IsInitialized())
	{
		MemoryMgr::Term();
	}
#endif
	return true;
}

void CAudio::RegisterEntity(CBaseObject* entity, char* in_name)
{

}
void CAudio::UnregisterEntity(CBaseObject* entity)
{

}

//	Set a wwise state
void CAudio::SendState(int stategroup, int state)
{
}

//	Set a wwise game parameter
void CAudio::SendGameParam(int param, float value)
{
}

//	Set the position of a non-dynamic entity
void CAudio::SendSetPosition(void* entity, float x, float y, float z)
{
	AkVector position = { x,y,z };
	AkVector front = { 0,0,1.0f };
	AkVector up = { 0,1.0f,0 };
	AkSoundPosition soundSource;
	soundSource.Set(position, front, up);
	SoundEngine::SetPosition((AkGameObjectID)entity, soundSource);
}

//	Stop all sounds playing on entity
void CAudio::StopAllSounds(void *entity)
{
	SoundEngine::StopAll((AkGameObjectID)entity);
}

//	Load bank
void CAudio::LoadBank()
{
	AkBankID bankID;
	if (szBankName[0] && SoundEngine::LoadBank(szBankName.c_str(), AK_DEFAULT_POOL_ID, bankID) != AK_Success)
	{
		szBankName[0] = 0;
	}

}

//	Unload bank
void CAudio::UnloadBank()
{
	if (!szBankName[0])
		return;

	SoundEngine::UnloadBank(szBankName.c_str(),NULL);
}

void CAudio::SoundVolume(int volume)
{
	SoundEngine::SetRTPCValue(GAME_PARAMETERS::SFXVOLUME, (AkRtpcValue)volume);
}

void CAudio::MusicVolume(int volume)
{
	SoundEngine::SetRTPCValue(GAME_PARAMETERS::MUSICVOLUME, (AkRtpcValue)volume);
}

void CAudio::VoiceVolume(int volume)
{ 
	SoundEngine::SetRTPCValue(GAME_PARAMETERS::VOICEVOLUME, (AkRtpcValue)volume);
}

void CAudio::PlayOneShot(unsigned long eventID)
{
	AkPlayingID soundID;
	soundID = SoundEngine::PostEvent(eventID, 0);
	printf("%d", soundID);
}