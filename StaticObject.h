#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "Mesh.h"
#include "Player.h"
#include "Enemy.h"

class StaticObject
{
public:
	StaticObject();
	~StaticObject();

	void Initialize(char *fileName);
	void Render(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot, float angle, float scale);
	void Collision(Player *player);
	void Collision(Enemy *enemy);

	D3DXMATRIX W, T, R, S;

	D3DXVECTOR3 min, max;

	Mesh objectMesh;
};

#endif