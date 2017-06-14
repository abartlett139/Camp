#ifndef LOSE_H
#define LOSE_H

#include "GameState.h"

class Lose : public GameState
{
public:
	Lose();
	~Lose();

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