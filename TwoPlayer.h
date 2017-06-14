#include "MYCLIENT.h"


#ifndef TWOPLAYER_H
#define TWOPLAYER_H

#include "GameState.h"
#include "Player.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Barrack.h"
#include "StaticObject.h"
#include "Pickup.h"
#include <sstream>

class TwoPlayer : public GameState
{
public:
	TwoPlayer();
	~TwoPlayer();

	bool Init();
	void Enter();
	bool Render(float deltaTime);
	void Update(float deltaTime);
	void Exit(GameState *nextState);

	//	variables for lights
	D3DXVECTOR3 lightDir;
	D3DLIGHT9 light;
	D3DXCOLOR *lightColor;
	float skyboxAngle;

	//	p1 is environment camera
	Player p1, p2;

	//	environment & objects
	Skybox skybox;
	Terrain terrain;
	Barrack	barrack;
	StaticObject fence, barrel, pallet;
	Pickup health, armor, ammo;
	StaticObject tower;

	//	positions
	D3DXVECTOR3 barrackPos[7][7];

	//	stuff to draw player location to the screen
	ID3DXFont *font;
	std::stringstream locationStr;
	void DrawTextToScreen();
};

#endif