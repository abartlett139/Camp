#include "Bullet.h"



Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

bool Bullet::Initialize()
{
	speed = 200;

	bulletMesh.Initialize("models/bullet.x");
	return true;
}



bool Bullet::Render(float deltaTime)
{
	//	set offset, rotation, position, and render
	offset += speed*deltaTime;
	D3DXMatrixRotationYawPitchRoll(&R, 0, D3DX_PI / 4, D3DX_PI / 3);
	D3DXMatrixTranslation(&O, 0, .5, offset);
	W = R*O*T;
	Device->SetTransform(D3DTS_WORLD, &W);
	bulletMesh.Render();

	//	transform the min and max bounding coordinates for collision detection
	D3DXVec3TransformCoord(&min, &bulletMesh.min, &W);
	D3DXVec3TransformCoord(&max, &bulletMesh.max, &W);

	life += deltaTime;
	if (life >= 2) {
		Reset();
	}

	return true;
}

void Bullet::Spawn(D3DXMATRIX * t)
{
	//	set position vector based of weapon
	T = *t;
	//	activates bullet
	active = true;
	speed = 200;
	life = 0;
}

void Bullet::Reset() {
	//	reset bullet stats
	active = false;
	offset = 0;
	speed = 0;
	life = 0;
	min = max = D3DXVECTOR3(0, 0, 0);
}
