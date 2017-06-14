#include "TwoPlayer.h"



TwoPlayer::TwoPlayer()
{
}


TwoPlayer::~TwoPlayer()
{
}

bool TwoPlayer::Init()
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

	//	initialize the players
	p1.Initialize(true);
	p2.Initialize(false);
	p1.hud.enemyPos.push_back(&p2._pos);
	p1.hud.enemyActive.push_back(true);

	skyboxAngle=0;

	return true;
}

void TwoPlayer::Enter()
{
	//	set up client stuff
	client->GetReadyToDoStuff();
	//	if level has not be initilazed alread, initialize it
	if (!isInit)
		isInit = Init();

	//	start playing sounds for level
	SoundEngine->PlayMusic(Sound::GAMEBACKGROUND);

	//	set projection matrix
	D3DXMATRIX P;
	D3DXMatrixPerspectiveFovLH(&P, (float)MATH::fpuDiv((double)D3DX_PI, (double)4), (float)MATH::fpuDiv((double)screenWidth, (double)screenHeight), 1.0f, 5000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &P);
}

bool TwoPlayer::Render(float deltaTime)
{
	//	set player position to terrain height
	D3DXVECTOR3 pos;
	p1.getPosition(&pos);
	pos.y = (float)MATH::fpuAdd(terrain.getHeight(pos.x, pos.z), 25);
	p1.setPosition(&pos);

	//	set view matrix
	D3DXMATRIX V;
	p1.getViewMatrix(&V);
	Device->SetTransform(D3DTS_VIEW, &V);

	//	begin drawing scene
	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xFF000000, 1.0, 0);
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
				barrack.Collision(&p1);
			}
		}
	}
	fence.Render(&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 100.0f);
	for (int i = 0; i < 30; i++) {
		barrel.Render(&D3DXVECTOR3(sin(i * 10) * 1000.0f, 0.0f, cos(i * 10) * 1000.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 7.5f);
		barrel.Collision(&p1);
	}
	pallet.Render(&D3DXVECTOR3(245.0f, 5.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 1.0f), 45.0f, 20.0f);
	tower.Render(&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f);

	//	render pickup objects
	if (health.active)
		health.Render();
	if (armor.active)
		armor.Render();
	if (ammo.active)
		ammo.Render();

	//	render players
	p1.Render(deltaTime);
	p2.Render(deltaTime);

	//	draw player position text	(DEV ONLY)
	//	DrawTextToScreen();

	//	end and present the scene
	Device->EndScene();
	Device->Present(0, 0, 0, 0);


	return true;
}

void TwoPlayer::Update(float deltaTime)
{
	//-----------------------
	//		Player update
	//-----------------------
	skyboxAngle += deltaTime*.05f;
	p1.Update(deltaTime);
	//	compose and send player position message
	p1.fillCharBuff();
	send(client->ConnectSocket, p1.playerMessage, sizeof(p1.playerMessage), 0);
	
	//	recieve and translate client messages
	client->RecvMessages();
	while (!client->recvMessages.empty()) {
		char * str = client->recvMessages.back();
		if (str[0] == 'P') {
			p2.getFromBuff(client->recvMessages.back());
		}
		if (str[0] == 'B') {
			D3DXMATRIX W;
			memcpy((char*)&W, &str[1], sizeof(W));
			for (int i = 0; i < p2.weapon.bulletList.size(); i++) {
				if (!p2.weapon.bulletList[i]->active) {
					p2.weapon.bulletList[i]->Spawn(&W);
					break;
				}
			}
		}
		if (str[0] == 'D') {
			p2.dead = true;
		}
		client->recvMessages.pop_back();
	}

	//	update enemy hud
	if (p1.hud.enemyActive[0]) {
		p1.hud.enemyPos[0] = &p2._pos;
	}

	//---------------------
	//	Bullet collision
	//---------------------
	for (int j = 0; j < p2.weapon.bulletList.size(); j++) {
		if (p2.weapon.bulletList[j]->active) {
			if (p1.CheckCollision(&p2.weapon.bulletList[j]->min, &p2.weapon.bulletList[j]->max)) {
				p2.weapon.bulletList[j]->Reset();
				p1.RemoveHealth();
			}
		}
	}


	//	update pickup objects
	if (p1.health <= 10 && !health.active) {
		SoundEngine->PlayFX(Sound::HEARTBEAT);
		health.active = true;
		health.pos = D3DXVECTOR3(D3D::GetRandomFloat(-200, 0), 5.0f, -150.0f);
	}
	if (p1.armor <= 0 && !armor.active) {
		armor.active = true;
		armor.pos = D3DXVECTOR3(D3D::GetRandomFloat(-200, 200), 5.0f, 150.0f);
	}
	if (p1.ammo <= 0 && !ammo.active) {
		ammo.active = true;
		ammo.pos = D3DXVECTOR3(D3D::GetRandomFloat(0, 200), 5.0f, -150.0f);
	}
	//---------------------
	//	Health collision
	//---------------------
	if (health.active) {
		if (health.CheckCollision(p1._pos.x, p1._pos.z)) {
			health.active = false;
			p1.AddHealth();
		}
	}
	//---------------------
	//	Armor collision
	//---------------------
	if (armor.active) {
		if (armor.CheckCollision(p1._pos.x, p1._pos.z)) {
			armor.active = false;
			p1.AddArmor();
		}
	}
	//---------------------
	//	Ammo collision
	//---------------------
	if (ammo.active) {
		if (ammo.CheckCollision(p1._pos.x, p1._pos.z)) {
			ammo.active = false;
			p1.AddAmmo();
		}
	}




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
	//	switch state to lose or win based off which player is dead
	if (p1.dead) {
		send(client->ConnectSocket, "D", sizeof("D"), 0);	//	send dead message to server
		Exit(lose);
	}
	if (p2.dead) {
		Exit(win);
	}
}

void TwoPlayer::Exit(GameState *nextState)
{
	//	stop playing sounds for level
	SoundEngine->StopMusic();


	//	reset the projection and view matrices to the default
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_PROJECTION, &I);
	Device->SetTransform(D3DTS_VIEW, &I);

	//	reset game objects (positions, health, etc)
	p1.Reset();
	p2.Reset();


	//	shut down client stuff
	client->Reset();

	//	go to the next state
	previousState = currentState;
	currentState = nextState;
	currentState->Enter();
}





void TwoPlayer::DrawTextToScreen()
{
	locationStr.str("");
	locationStr << p1._pos.x;
	RECT pos;
	pos.top = 10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
	locationStr.str("");
	locationStr << p1._pos.y;

	pos.top = pos.bottom + 10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
	locationStr.str("");
	locationStr << p1._pos.z;

	pos.top = pos.bottom + 10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);



	locationStr.str("");
	locationStr << p2._pos.x;

	pos.top = 500;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
	locationStr.str("");
	locationStr << p2._pos.y;

	pos.top = pos.bottom + 10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
	locationStr.str("");
	locationStr << p2._pos.z;

	pos.top = pos.bottom + 10;
	pos.left = 10;
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_CALCRECT, D3D::WHITE);
	font->DrawText(NULL, locationStr.str().c_str(), -1, &pos, DT_LEFT, D3D::WHITE);
}
