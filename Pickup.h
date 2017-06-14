#ifndef PICKUP_H
#define PICKUP_H

#include "Mesh.h"


class Pickup
{
public:
	Pickup();
	~Pickup();

	void Initialize(int objectType);
	void Render();
	bool CheckCollision(float playerX, float playerZ);

	Mesh pickupMesh;

	D3DXMATRIX T;
	D3DXVECTOR3 pos, min, max;

	int type;
	enum { HEALTH, ARMOR, AMMO };

	bool active;
};

#endif