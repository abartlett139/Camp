#ifndef HUD_H
#define HUD_H

#include "D3D.h"
#include <sstream>
#include <chrono>

class HUD
{
public:
	HUD();
	~HUD();

	void Init();
	void Render();
	void Update(float deltaTime);
	void UpdateHealth(float health);
	void UpdateArmor(float armor);
	void UpdateAmmo(int ammo);
	void DrawHUDText();

	ID3DXSprite *sprite;

	IDirect3DTexture9 *healthBar, *healthBarFill, *armorBar, *armorBarFill, *ammoBar, *timeBar, *mapBar, *playerBar, *enemyBar;
	IDirect3DTexture9 *healthIcon, *armorIcon, *ammoIcon;
	RECT healthPos, healthFillPos, armorPos, armorFillPos, ammoPos, ammoBarPos, timePos, timeBarPos, mapPos;
	D3DXVECTOR3 healthbarCenter, playerPos;
	std::vector<D3DXVECTOR3*> enemyPos;
	std::vector<bool> enemyActive;

	ID3DXFont *font;

	std::stringstream timer, ammoStr;
	float levelTimer;
	
};

#endif