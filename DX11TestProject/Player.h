#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "BaseObject.h"

class Player :
	public BaseObject
{
public:
	Player();
	~Player();

	void Init() {}
	void Update() {}
	void Shutdown() {}
};

#endif