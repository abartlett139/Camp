#ifndef BARRACK_H
#define BARRACK_H

#include "Mesh.h"
#include "Player.h"

class Barrack
{
public:
	Barrack();
	~Barrack();

	void Initialize();
	void Render(D3DXVECTOR3 *pos);
	bool Collision(Player *player);

	D3DXMATRIX W, T, R, S;

	D3DXVECTOR3 min, max, MIN, MAX;

	D3DXVECTOR3 doorLeftTemp, doorRightTemp;
	D3DXVECTOR3 doorLeft, doorRight;

	Mesh barrackMesh;
};

#endif
