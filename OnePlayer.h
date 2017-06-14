#ifndef ONEPLAYER_H
#define ONEPLAYER_H

#include "GameState.h"
#include "Player.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Barrack.h"
#include "StaticObject.h"
#include "Enemy.h"
#include "Pickup.h"

#include <sstream>

class OnePlayer : public GameState
{
public:
	OnePlayer();
	~OnePlayer();

	bool Init();
	void Enter();
	bool Render(float deltaTime);
	void Update(float deltaTime);
	void Exit(GameState *nextState);

	//	variables for lights
	D3DXVECTOR3 lightDir;
	D3DLIGHT9 light;
	D3DXCOLOR *lightColor;


	Player player;	// also the camera

	//	enemy variables
	std::vector<Enemy*> enemyList;
	int bodyCount;
	float skyboxAngle;


	//	environment & objects
	Skybox skybox;
	Terrain terrain;
	Barrack	barrack;
	StaticObject fence, barrel, pallet;
	StaticObject tower;
	Pickup health, armor, ammo;

	//	positions
	D3DXVECTOR3 barrackPos[7][7];

	//	stuff to draw player location to the screen
	ID3DXFont *font;
	std::stringstream locationStr;
	void DrawTextToScreen();

	
};

#endif