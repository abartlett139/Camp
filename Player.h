#ifndef PLAYER_H
#define PLAYER_H

#include "D3D.h"
#include "Mesh.h"
#include "Weapon.h"
#include "Camera.h"
//#include "Controller.h"
#include "HUD.h"


class Player : public Camera
{
public:
	

	Player();
	~Player();

	void Initialize(bool isPlayerOne);
	void Render(float deltaTime);
	void Update(float deltaTime);
	bool CheckCollision(D3DXVECTOR3 *MIN, D3DXVECTOR3 *MAX);
	void RemoveHealth();
	void AddHealth();
	void AddArmor();
	void AddAmmo();
	void Reset();


	double fireTime;
	bool canShoot = false;

	bool dead;

	float speed;
	int health, ammo, armor;
	int healthMax, ammoMax, armorMax;
	Weapon weapon;

	D3DXMATRIX W, T, S;
	Mesh playerMesh;

	D3DXVECTOR3 min, max, velocity, prevPos;



	//	multiplayer only
	char playerMessage[120];
	void fillCharBuff();
	void getFromBuff(char* c);
	bool isP1;

	HUD hud;
};

#endif