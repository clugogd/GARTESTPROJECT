#include "Audio.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>									// Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>										// Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>									// Streaming Manager
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>									// Thread defines
#include <AK\IBytes.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
//#include "..\DX11TestProject\Dependencies\Wwise\Sample\Wwise_IDs.h"
#include "Dependencies/Wwise/Sample/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h"

#include <DirectXMath.h>

#define INIT_BANK "Assets/Soundbanks/Init2.bnk"
#define DEFAULT_BANK "Assets/Soundbanks/New_SoundBank.bnk"

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

CAudio::CAudio()
{
}


CAudio::~CAudio()
{
}

bool CAudio::Init() 
{
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	AK::MemoryMgr::Init(&memSettings);


	//
	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the default streaming manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}

	//
	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.      
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.

	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}


	//
	// Create the Sound Engine
	// Using default initialization parameters
	//

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	//
	// Initialize the music engine
	// Using default initialization parameters
	//

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}
	return true;
}

bool CAudio::Update()
{
	return true;
}

bool CAudio::Shutdown()
{
	return true;
}

void CAudio::setListenerPosition(const DirectX::XMFLOAT4X4* cameraMatrix)
{
}
bool CAudio::LoadBanks()
{
	return true;
}

