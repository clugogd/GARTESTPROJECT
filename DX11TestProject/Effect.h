#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "BaseObject.h"
class CEffect :
	public CBaseObject
{
public:
	CEffect();
	~CEffect();

	void Init() {}
	void Update() {}
	void Shutdown() {}
};

#endif