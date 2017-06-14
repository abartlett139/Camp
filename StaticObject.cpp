#include "StaticObject.h"



StaticObject::StaticObject()
{
}


StaticObject::~StaticObject()
{
}

void StaticObject::Initialize(char * fileName)
{
	objectMesh.Initialize(fileName);
}

void StaticObject::Render(D3DXVECTOR3 * pos, D3DXVECTOR3 *rot, float angle, float scale)
{
	D3DXMatrixTranslation(&T, pos->x / scale, pos->y / scale, pos->z / scale);
	D3DXMatrixRotationAxis(&R, rot, angle);
	D3DXMatrixScaling(&S, scale, scale, scale);
	W = R*T*S;
	Device->SetTransform(D3DTS_WORLD, &W);
	objectMesh.Render();

	D3DXVec3TransformCoord(&min, &objectMesh.min, &W);
	D3DXVec3TransformCoord(&max, &objectMesh.max, &W);
}

void StaticObject::Collision(Player * player)
{
	if (player->_pos.x >= min.x && player->_pos.x <= max.x) {

		//	if player hits the front of the object
		if (player->_pos.z >= min.z - 2 && player->_pos.z < max.z)
			player->_pos.z = player->prevPos.z;
			//player->_pos.z = min.z - 2;

		//	if player hits the back of the object
		if (player->_pos.z <= max.z + 2 && player->_pos.z > min.z)
			player->_pos.z = player->prevPos.z;
			//player->_pos.z = max.z + 2;
	}

	if (player->_pos.z >= min.z && player->_pos.z <= max.z) {

		//	if player hits the left side of the object
		if (player->_pos.x >= min.x - 2 && player->_pos.x < max.x)
			player->_pos.x = player->prevPos.x;
			//player->_pos.x = min.x - 2;

		//	if player hits the right side of the object
		if (player->_pos.x <= max.x + 2 && player->_pos.x > min.x)
			player->_pos.x = player->prevPos.x;
			//player->_pos.x = max.x + 2;
	}
}

/*void StaticObject::Collision(Enemy * enemy)
{
	if (enemy->pos.x >= min.x && enemy->pos.x <= max.x) {

		//	if enemy hits the front of the object
		if (enemy->pos.z >= min.z - 2 && enemy->pos.z < max.z)
			enemy->pos.z = enemy->prevPos.z;
		//enemy->_pos.z = min.z - 2;

		//	if enemy hits the back of the object
		if (enemy->pos.z <= max.z + 2 && enemy->pos.z > min.z)
			enemy->pos.z = enemy->prevPos.z;
		//enemy->_pos.z = max.z + 2;
	}

	if (enemy->pos.z >= min.z && enemy->pos.z <= max.z) {

		//	if enemy hits the left side of the object
		if (enemy->pos.x >= min.x - 2 && enemy->pos.x < max.x)
			enemy->pos.x = enemy->prevPos.x;
		//enemy->_pos.x = min.x - 2;

		//	if enemy hits the right side of the object
		if (enemy->pos.x <= max.x + 2 && enemy->pos.x > min.x)
			enemy->pos.x = enemy->prevPos.x;
		//enemy->_pos.x = max.x + 2;
	}
}*/
