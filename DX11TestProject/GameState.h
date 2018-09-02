#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

class CGameState
{
public:
	CGameState();
	~CGameState();

	bool Init();
	bool Update();
	bool Shutdown();
};

#endif