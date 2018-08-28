#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "Camera.h"
#include "BaseObject.h"
#include <vector>
#include "Defines.h"

#if !_DEBUG
#define AK_OPTIMIZED
#endif

namespace DirectX
{
	struct XMFLOAT4X4;
}

class CAudio
{
public:
	CAudio();
	~CAudio();

	bool Init();
	bool Update();
	bool Shutdown();

	void setListenerPosition(const DirectX::XMFLOAT4X4* cameraMatrix);
	bool LoadBanks();
	//void registerObject(EventMessageBase* e);

private:
	const DirectX::XMFLOAT4X4* audioListener;
	std::vector<BaseObject*> objectList;
};


/////////////////////////////////////////////////////////////////////////////////
// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
/////////////////////////////////////////////////////////////////////////////////
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

#endif
