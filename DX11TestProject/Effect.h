#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "BaseObject.h"
class Effect :
	public BaseObject
{
public:
	Effect();
	~Effect();

	void Init() {}
	void Update() {}
	void Shutdown() {}
};

#endif