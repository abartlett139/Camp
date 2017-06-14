#include "HUD.h"



HUD::HUD()
{
}


//	release all textures and sprite
HUD::~HUD()
{
	D3D::Release(healthBar);
	D3D::Release(healthBarFill);
	D3D::Release(armorBar);
	D3D::Release(armorBarFill);
	D3D::Release(ammoBar);
	D3D::Release(timeBar);
	D3D::Release(mapBar);
	D3D::Release(playerBar);
	D3D::Release(enemyBar);
	D3D::Release(healthIcon);
	D3D::Release(armorIcon);
	D3D::Release(ammoIcon);
	D3D::Release<ID3DXSprite*>(sprite);
}

void HUD::Init()
{
	//	create font
	D3DXCreateFont(Device, 28, 0, FW_THIN, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Impact"), &font);


	//	create sprite
	D3DXCreateSprite(Device, &sprite);

	//	load textures
	healthBar = D3D::LoadTexture("sprites/hud/healthBar.png");
	healthBarFill = D3D::LoadTexture("sprites/hud/healthFill.png");
	armorBar = D3D::LoadTexture("sprites/hud/armorBar.png");
	armorBarFill = D3D::LoadTexture("sprites/hud/armorFill.png");
	ammoBar = D3D::LoadTexture("sprites/hud/ammoBar.png");
	timeBar = D3D::LoadTexture("sprites/hud/timeBar.png");
	mapBar = D3D::LoadTexture("sprites/hud/mapBar.png");

	healthIcon = D3D::LoadTexture("sprites/hud/healthIcon.png");
	armorIcon = D3D::LoadTexture("sprites/hud/armorIcon.png");
	ammoIcon = D3D::LoadTexture("sprites/hud/ammoIcon.png");

	playerBar = D3D::LoadTexture("sprites/hud/playerIcon.png");
	enemyBar = D3D::LoadTexture("sprites/hud/enemyIcon.png");

	//	set rect positions
	timePos.top = 20;
	timePos.left = 1271;

	timeBarPos.top = 0;
	timeBarPos.left = 0;
	timeBarPos.right = 120;
	timeBarPos.bottom = 50;

	ammoPos.top = 20;
	ammoPos.left = 500;

	ammoBarPos.top = 0;
	ammoBarPos.left = 0;
	ammoBarPos.right = 59;
	ammoBarPos.bottom = 52;

	healthPos.top = 0;
	healthPos.left = 0;
	healthPos.right = 345;
	healthPos.bottom = 30;

	healthFillPos = healthPos;

	armorPos.top = 0;
	armorPos.left = 0;
	armorPos.right = 345;
	armorPos.bottom = 30;

	armorFillPos = armorPos;

	mapPos.top = 0;
	mapPos.bottom = 205;
	mapPos.right = 205;
	mapPos.left = 0;

	//	set level start time
	levelTimer = 0;
	ammoStr << 30;
}

void HUD::Render()
{
	//	draw all HUD sprites
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(healthBar, &healthPos, NULL, &D3DXVECTOR3(5.0f, 7.5f, 0.0f), D3D::WHITE);
	sprite->Draw(healthBarFill, &healthFillPos, NULL, &D3DXVECTOR3(5.0f, 7.5f, 0.0f), D3D::WHITE);
	sprite->Draw(healthIcon, NULL, NULL, &D3DXVECTOR3(360.0f, 5.0f, 0.0f), D3D::WHITE);

	sprite->Draw(armorBar, &armorPos, NULL, &D3DXVECTOR3(5.0f, 75.0f, 0.0f), D3D::WHITE);
	sprite->Draw(armorBarFill, &armorFillPos, NULL, &D3DXVECTOR3(5.0f, 75.0f, 0.0f), D3D::WHITE);
	sprite->Draw(armorIcon, NULL, NULL, &D3DXVECTOR3(360.0f, 50.0f, 0.0f), D3D::WHITE);

	sprite->Draw(ammoBar, &ammoBarPos, NULL, &D3DXVECTOR3((float)ammoPos.left - 16.5f, (float)ammoPos.top - 13.0f, 0.0f), D3D::WHITE);
	sprite->Draw(ammoIcon, NULL, NULL, &D3DXVECTOR3(550.0f, 5.0f, 0.0f), D3D::WHITE);


	sprite->Draw(timeBar, NULL, NULL, &D3DXVECTOR3((float)timePos.left - 62.0f, (float)timePos.top - 13.0f, 0.0f), D3D::WHITE);

	sprite->Draw(mapBar, &mapPos, NULL, &D3DXVECTOR3(1395.0f, 0.0f, 0.0f), D3D::WHITE);

	sprite->Draw(playerBar, NULL, &D3DXVECTOR3(10.0f, 10.0f, 0.0f), 
		&D3DXVECTOR3(
			(((playerPos.x + 1000.0f) / 10.0f) + 1400.0f) - 105.0f,	//	player map x position
			((-playerPos.z + 1000.0f) / 11.0f) - 155.0f,				//	player map y position
			0.0f),											//	player map z position
		D3D::WHITE);
	for (int i = 0; i < enemyPos.size(); i++) {
		if(enemyActive[i])
			sprite->Draw(enemyBar, NULL, &D3DXVECTOR3(10.0f, 10.0f, 0.0f),
				&D3DXVECTOR3(
					((enemyPos[i]->x + 1000.0f) / 10.0f) + 1400.0f , //	enemy map x position
					(-enemyPos[i]->z + 1000.0f) / 10.0f, 			//	enemy map y position
					0.0f),											//	enemy map z position
				D3D::WHITE);
	}

	sprite->End();

	DrawHUDText();
	
	
}

//	update for timer
void HUD::Update(float deltaTime)
{
	levelTimer += deltaTime;
}

//	update health hud
void HUD::UpdateHealth(float healthPercent)
{
	healthFillPos.right = 350 * healthPercent;
}

//	update armor hud
void HUD::UpdateArmor(float armorPercent)
{
	armorFillPos.right = 350 * armorPercent;
}

//	update ammo hud
void HUD::UpdateAmmo(int ammo)
{
	ammoStr.str("");
	ammoStr << ammo;
}

//	draw time string and ammo string
void HUD::DrawHUDText()
{
	timer.str("");
	timer << (int)levelTimer;
	font->DrawText(NULL, timer.str().c_str(), -1, &timePos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, timer.str().c_str(), -1, &timePos, DT_LEFT, D3D::WHITE);

	font->DrawTextA(NULL, ammoStr.str().c_str(), -1, &ammoPos, DT_CALCRECT, D3D::WHITE);
	font->DrawTextA(NULL, ammoStr.str().c_str(), -1, &ammoPos, DT_LEFT, D3D::WHITE);

}
