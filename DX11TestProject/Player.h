#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "BaseObject.h"

class CPlayer :
	public CBaseObject
{
public:
	CPlayer();
	~CPlayer();

	void Init() {}
	void Update() {}
	void Shutdown() {}
};

#endif