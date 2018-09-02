#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <vector>
using std::vector;
#include "BaseObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class CAudio
{
public:
	CAudio();
	~CAudio();

	//	Initialize the sound engine
	bool Init();
	bool Update();
	bool Shutdown();

	//	Terminate the sound engine
	bool Term();

private:
	//	Entity registration as game object in audio engine
	void RegisterEntity(CBaseObject* entity, char* in_name = nullptr);
	void UnregisterEntity(CBaseObject* entity);

	//	Set a wwise state
	void SendState(int stategroup, int state);

	//	Set a wwise game parameter
	void SendGameParam(int param, float value);

	//	Set the position of a non-dynamic entity
	void SendSetPosition(void* entity, float x, float y, float z);

	//	Stop all sounds playing on entity
	void StopAllSounds(void *entity);

	//	Load bank
	void LoadBank();

	//	Unload bank
	void UnloadBank();

	void SoundVolume(int volume);
	void MusicVolume(int volume);
	void VoiceVolume(int volume);
	
public:
	vector<CBaseObject*> registeredEntities;
	int m_nMaxPools;
	XMFLOAT4X4 listenerTransform;

public:
	void PlayOneShot(unsigned long eventID);
};

#endif
