#include "EnemyBullet.h"



EnemyBullet::EnemyBullet()
{
}


EnemyBullet::~EnemyBullet()
{
}

bool EnemyBullet::Initialize()
{
	speed = 200;

	bulletMesh.Initialize("models/bullet.x");
	return true;
}



bool EnemyBullet::Render(float deltaTime)
{
	//	set offset, rotation, position, and render
	offset += speed*deltaTime;
	D3DXMatrixRotationYawPitchRoll(&R, 0, D3DX_PI / 4, D3DX_PI / 3);
	D3DXMatrixTranslation(&O, 0, 20, -offset);
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

void EnemyBullet::Spawn(D3DXMATRIX * t)
{
	T = *t;
	active = true;
	speed = 200;
	life = 0;
}

void EnemyBullet::Reset() {
	active = false;
	offset = 0;
	speed = 0;
	life = 0;
	min = max = D3DXVECTOR3(0, 0, 0);
}
