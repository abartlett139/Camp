#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include "Mesh.h"

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	bool Initialize();

	bool Render(float deltaTime);

	void Spawn(D3DXMATRIX *T);

	void Reset();

	Mesh bulletMesh;

	D3DXMATRIX W, T, O, R;

	D3DXVECTOR3 min, max;

	bool active;

	float speed, offset, life;
};

#endif

