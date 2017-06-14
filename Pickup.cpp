#include "Pickup.h"



Pickup::Pickup()
{
}


Pickup::~Pickup()
{
}

void Pickup::Initialize(int objectType)
{
	//	intialize the object based on what type of pickup it is
	switch (objectType) {
	case HEALTH:
		pickupMesh.Initialize("models/health.x");
		type = HEALTH;
		break;
	case ARMOR:
		pickupMesh.Initialize("models/armor.x");
		type = ARMOR;
		break;
	case AMMO:
		pickupMesh.Initialize("models/ammo.x");
		type = AMMO;
		break;
	default:break;
	}


}

void Pickup::Render()
{
	//	render
	D3DXMATRIX S;
	D3DXMatrixScaling(&S, 5, 5, 5);
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
	D3DXMATRIX P = S*T;
	Device->SetTransform(D3DTS_WORLD, &P);
	pickupMesh.Render();

	//	transform the min and max bounding coordinates for collision detection
	D3DXVec3TransformCoord(&min, &pickupMesh.min, &P);
	D3DXVec3TransformCoord(&max, &pickupMesh.max, &P);
}

bool Pickup::CheckCollision(float playerX, float playerZ)
{
	//	check distance between player and object
	float x = pos.x - playerX;
	float z = pos.z - playerZ;

	if (sqrt((x*x) + (z*z)) < 10)
		return true;


	return false;
}
