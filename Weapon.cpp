#include "Weapon.h"


Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}

bool Weapon::Initialize()
{
	weaponMesh.Initialize("models/acr.x");
	for (int i = 0; i < 10; i++) {
		Bullet *temp = new Bullet();
		bulletList.push_back(temp);
	}

	for (int i = 0; i < bulletList.size(); i++)
		bulletList[i]->Initialize();

	return true;
}

bool Weapon::Render(float deltaTime)
{
	//	set position and render
	Device->GetTransform(D3DTS_WORLD, &T);
	D3DXMatrixTranslation(&O, 2, -2, 4);
	W = O*T;
	Device->SetTransform(D3DTS_WORLD, &W);
	weaponMesh.Render();

	//	render active bullets
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i]->active)
			bulletList[i]->Render(deltaTime);
	}
	return true;
}

D3DXMATRIX Weapon::CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++) {
		if (!bulletList[i]->active) {
			bulletList[i]->Spawn(&W);
			return W;
		}
	}
}

