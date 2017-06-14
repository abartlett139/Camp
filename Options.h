#ifndef OPTIONS_H
#define OPTIONS_H

#include "GameState.h"
#include "resource.h"

class Options : public GameState
{
public:
	Options();
	~Options();

	bool Init();
	void Enter();
	bool Render(float deltaTime);
	void Update(float deltaTime);
	void Exit(GameState *nextState);


	HWND dlgHwnd;
	static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	ID3DXSprite *sprite;

	IDirect3DTexture9 *background, *title, *button[4], *buttonOver[4];

	int currentButton;

	D3DXVECTOR3 buttonCenter, titleCenter;

	float timer;
	bool canSwitch;
};

#endif