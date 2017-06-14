#include "OnePlayer.h"



OnePlayer::OnePlayer()
{
}


OnePlayer::~OnePlayer()
{
}

bool OnePlayer::Init()
{
	D3DXCreateFont(Device, 20, 0, FW_THIN, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Impact"), &font);

	//	create directional light and enable it
	lightDir = D3DXVECTOR3(.5, -.5, .5);
	lightColor = new D3DXCOLOR(3.0f, 2.5f, 2.5f, 1.0f);
	light = D3D::InitDirectionalLight(&lightDir, lightColor);
	Device->SetLight(0, &light);
	Device->LightEnable(0, true);


	//	intialize the skybox
	skybox.generate();

	//	initialize the terrain
	terrain.Initialize(1024, 1024, 2, 0.05f);

	//	initialize other game objects
	barrack.Initialize();
	fence.Initialize("models/fence.x");
	barrel.Initialize("models/barrel.x");
	pallet.Initialize("models/pallet.x");
	tower.Initialize("models/tower.x");

	//	set barrack positions
	for (int i = -3; i < 4; i++) {
		for (int j = -3; j < 4; j++) {
			barrackPos[i + 3][j + 3] = { 300.0f*i, 2.0f, 300.0f*j };
		}
	}

	//	initialize pickups
	health.Initialize(Pickup::HEALTH);
	armor.Initialize(Pickup::ARMOR);
	ammo.Initialize(Pickup::AMMO);

	//	initialize enemies
	for (int i = -5; i < 6; i+=2) {
		Enemy *temp = new Enemy();
		temp->Initialize(&D3DXVECTOR3(150.0f *i, 2, 975));
		enemyList.push_back(temp);
		player.hud.enemyPos.push_back(&D3DXVECTOR3(150.0f *i, 2, 975));
		player.hud.enemyActive.push_back(true);
	}

	
	//	initialize the player
	player.Initialize(true);

	
	skyboxAngle=0;

	return true;
}

void OnePlayer::Enter()
{
	//	if level has not be initilazed alread, initialize it
	if (!isInit)
		isInit = Init();

	//	start playing sounds for level
	SoundEngine->PlayMusic(Sound::GAMEBACKGROUND);

	//	set projection matrix
	D3DXMATRIX P;
	D3DXMatrixPerspectiveFovLH(&P, (float)MATH::fpuDiv((double)D3DX_PI, (double)4), (float)MATH::fpuDiv((double)screenWidth, (double)screenHeight), 1.0f, 3000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &P);
}

bool OnePlayer::Render(float deltaTime)
{
	if (Device) {
		//	set player position to terrain height
		D3DXVECTOR3 pos;
		player.getPosition(&pos);
		pos.y = (float)MATH::fpuAdd(terrain.getHeight(pos.x, pos.z), 25);
		player.setPosition(&pos);

		//	set view matrix
		D3DXMATRIX V;
		player.getViewMatrix(&V);
		Device->SetTransform(D3DTS_VIEW, &V);


		//	begin drawing scene
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xFF000000, 1.0f, 0);
		Device->BeginScene();

		//	render skybox	-- should follow player -- and spin
		D3DXMATRIX W, T, R;
		D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
		D3DXMatrixRotationY(&R, skyboxAngle);
		W = R*T;
		Device->SetTransform(D3DTS_WORLD, &W);
		skybox.render();

		//	render terrain	--	always at 0,0,0
		D3DXMatrixTranslation(&W, 0.0f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &W);
		terrain.Render();

		//	render game objects
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++) {
				if (barrackPos[i][j] != D3DXVECTOR3(0.0f, barrackPos[i][j].y, 0.0f)) {
					barrack.Render(&barrackPos[i][j]);
					barrack.Collision(&player);
				}
			}
		}
		fence.Render(&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 100.0f);
		for (int i = 0; i < 30; i++) {
			barrel.Render(&D3DXVECTOR3(sin(i*10) * 1000.0f, 0.0f, cos(i*10) * 1000.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 7.5f);
			barrel.Collision(&player);
		}
		pallet.Render(&D3DXVECTOR3(245.0f, 5.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), 45.0f, 20.0f);
		tower.Render(&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f);



		//	render enemies
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i]->active)
				enemyList[i]->Render(deltaTime);
		}

		//	render pickup objects
		if (health.active)
			health.Render();
		if (armor.active)
			armor.Render();
		if (ammo.active)
			ammo.Render();


		//	render player
		player.Render(deltaTime);

		//	draw player position text (DEV ONLY)
		//	DrawTextToScreen();

		//	end and present the scene
		Device->EndScene();
		Device->Present(0, 0, 0, 0);

	}
	return true;
}

void OnePlayer::Update(float deltaTime)
{
	//-----------------------
	//		Player update
	//	Player bullet collsion
	//-----------------------
	skyboxAngle += deltaTime*.05f;
	player.Update(deltaTime);
	for (int i = 0; i < player.weapon.bulletList.size(); i++) {
		if (player.weapon.bulletList[i]->active) {
			for (int j = 0; j < enemyList.size(); j++) {
				if (enemyList[j]->active) {
					if (enemyList[j]->CollisionDetection(&player.weapon.bulletList[i]->min, &player.weapon.bulletList[i]->max)) {
						enemyList[j]->health--;
						player.weapon.bulletList[i]->Reset();
					}
				}
			}

		}
	}

	//-----------------------
	//		Enemy update
	//	Enemy bullet collsion
	//-----------------------
	bodyCount = 0;
	for (int i = 0; i < enemyList.size(); i++) {
		//check to see if the enemy is active
		if (enemyList[i]->active) {
			//	update the enemy
			enemyList[i]->Update(deltaTime, &player._pos);
			//	update enemy hud
			player.hud.enemyActive[i] = enemyList[i]->active;
			if (player.hud.enemyActive[i]) {
				player.hud.enemyPos[i] = &enemyList[i]->pos;
			}
			//	check to see if enemy bullets are active, and, if so, do they hit the player
			for (int j = 0; j < enemyList[i]->bulletList.size(); j++) {
				if (enemyList[i]->bulletList[j]->active) {
					if (player.CheckCollision(&enemyList[i]->bulletList[j]->min, &enemyList[i]->bulletList[j]->max)) {
						enemyList[i]->bulletList[j]->Reset();
						player.RemoveHealth();
					}
				}
			}
		}
		else {
			bodyCount++;
		}
	}

	//	update pickup objects
	if (player.health <= 10 && !health.active) {
		SoundEngine->PlayFX(Sound::HEARTBEAT);
		health.active = true;
		health.pos = D3DXVECTOR3(D3D::GetRandomFloat(-200, 0), 5.0f, -150.0f);
	}
	if (player.armor <= 0 && !armor.active) {
		armor.active = true;
		armor.pos = D3DXVECTOR3(D3D::GetRandomFloat(-200, 200), 5.0f, 150.0f);
	}
	if (player.ammo <= 0 && !ammo.active) {
		ammo.active = true;
		ammo.pos = D3DXVECTOR3(D3D::GetRandomFloat(0, 200), 5.0f, -150.0f);
	}
	//---------------------
	//	Health collision
	//---------------------
	if (health.active) {
		if (health.CheckCollision(player._pos.x, player._pos.z)) {
			health.active = false;
			player.AddHealth();
		}
	}
	//---------------------
	//	Armor collision
	//---------------------
	if (armor.active) {
		if (armor.CheckCollision(player._pos.x, player._pos.z)) {
			armor.active = false;
			player.AddArmor();
		}
	}
	//---------------------
	//	Ammo collision
	//---------------------
	if (ammo.active) {
		if (ammo.CheckCollision(player._pos.x, player._pos.z)) {
			ammo.active = false;
			player.AddAmmo();
		}
	}

	//	game object collision
	tower.Collision(&player);

	


	//	shortcut keys
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000f) {
		Exit(menu);
	}
	if (GetAsyncKeyState('1') & 0x8000f) {
		Exit(win);
	}
	if (GetAsyncKeyState('2') & 0x8000f) {
		Exit(lose);
	}


	//	if the player is dead, switch to lose state
	if (player.dead) {
		Exit(lose);
	}
	if (bodyCount >= 6) {
		Exit(win);
	}
}

void OnePlayer::Exit(GameState * nextState)
{
	//	stop playing sounds for level
	SoundEngine->StopMusic();

	//	reset the projection and view matrices to the default
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_PROJECTION, &I);
	Device->SetTransform(D3DTS_VIEW, &I);

	//	reset game objects (positions, health, etc)
	for (int i = 0; i < enemyList.size(); i++) {
		enemyList[i]->Reset();
	}
	player.Reset();
	health.active = false;
	armor.active = false;
	ammo.active = false;


	//	go to the next state
	previousState = currentState;
	currentState = nextState;
	currentState->Enter();
}












void OnePlayer::DrawTextToScreen()
{
	locationStr.str("");
	locationStr << player._pos.x;
	RECT pos;
	pos.top = 800;
	pos.left = 10;	
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
	locationStr.str("");
	locationStr << player._pos.y;

	pos.top = pos.bottom + 10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
	locationStr.str("");
	locationStr << player._pos.z;

	pos.top = pos.bottom+10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
}
