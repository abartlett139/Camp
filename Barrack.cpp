#include "Barrack.h"



Barrack::Barrack()
{
}


Barrack::~Barrack()
{
}

void Barrack::Initialize()
{
	barrackMesh.Initialize("models/barrack.x");

}

void Barrack::Render(D3DXVECTOR3 * pos)
{
	D3DXMatrixScaling(&S, 50.0f, 50.0f, 50.0f);
	D3DXMatrixTranslation(&T, pos->x, pos->y, pos->z);
	W = S*T;
	Device->SetTransform(D3DTS_WORLD, &W);
	barrackMesh.Render();

	D3DXVec3TransformCoord(&min, &barrackMesh.min, &W);
	D3DXVec3TransformCoord(&max, &barrackMesh.max, &W);
	D3DXVec3TransformCoord(&doorLeft, &doorLeftTemp, &W);
	D3DXVec3TransformCoord(&doorRight, &doorRightTemp, &W);
}

bool Barrack::Collision(Player *player)
{
	if (player->_pos.x >= min.x && player->_pos.x <= max.x) {

		//	if player hits the front wall
		if (player->_pos.z >= min.z - 2 && player->_pos.z < max.z)
			player->_pos.z = min.z - 2;

		//	if player hits the back wall
		if (player->_pos.z <= max.z + 2 && player->_pos.z > min.z)
			player->_pos.z = max.z + 2;
	}

	if (player->_pos.z >= min.z && player->_pos.z <= max.z) {

		//	if player hits the left wall
		if (player->_pos.x >= min.x - 2 && player->_pos.x < max.x)
			player->_pos.x = min.x - 2;

		//	if player hits the right wall
		if (player->_pos.x <= max.x + 2 && player->_pos.x > min.x)
			player->_pos.x = max.x + 2;
	}

	return false;
}


