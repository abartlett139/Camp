#ifndef WEAPON_H
#define WEAPON_H

#include "Bullet.h"

class Weapon
{
public:
	Weapon();
	~Weapon();

	bool Initialize();

	bool Render(float deltaTime);

	D3DXMATRIX CreateBullet();

	Mesh weaponMesh;

	D3DXMATRIX W, T, O;

	//Bullet bullet;
	std::vector<Bullet*>bulletList;
};

#endif
