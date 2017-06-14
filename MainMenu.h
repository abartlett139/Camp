#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameState.h"

class MainMenu : public GameState
{
public:
	MainMenu();
	~MainMenu();

	bool Init();
	void Enter();
	bool Render(float deltaTime);
	void Update(float deltaTime);
	void Exit(GameState *nextState);

	ID3DXSprite *sprite;

	IDirect3DTexture9 *background, *title, *button[7], *buttonOver[7];

	int currentButton;

	D3DXVECTOR3 buttonCenter, titleCenter;

	float timer;
	bool canSwitch;
};

#endif