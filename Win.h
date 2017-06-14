#ifndef WIN_H
#define WIN_H

#include "GameState.h"

class Win : public GameState
{
public:
	Win();
	~Win();

	bool Init();
	void Enter();
	bool Render(float deltaTime);
	void Update(float deltaTime);
	void Exit(GameState *nextState);

	ID3DXSprite *sprite;

	IDirect3DTexture9 *background, *title, *text, *button[2], *buttonOver[2];

	int currentButton;

	D3DXVECTOR3 buttonCenter, titleCenter;

	float timer;
	bool canSwitch;
};

#endif