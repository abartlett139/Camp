#include "MYCLIENT.h"
#include "Options.h"

Options::Options()
{
}


Options::~Options()
{
	D3D::Release(background);
	D3D::Release(title);
	for (int i = 0; i < 6; i++) {
		D3D::Release(button[i]);
		D3D::Release(buttonOver[i]);
	}
	D3D::Release<ID3DXSprite*>(sprite);
}

bool Options::Init()
{
	//	create sprite
	D3DXCreateSprite(Device, &sprite);

	//	set center of textures
	buttonCenter = D3DXVECTOR3(440, 60, 0);
	titleCenter = D3DXVECTOR3(800, 100, 0);



	//	set current button
	currentButton = 0;

	//	set the timer
	timer = 0;
	canSwitch = false;

	//	load textures
	background = D3D::LoadTexture("sprites/background.png");
	title = D3D::LoadTexture("sprites/titles/options.png");

	button[0] = D3D::LoadTexture("sprites/buttons/fxvolume.png");
	button[1] = D3D::LoadTexture("sprites/buttons/musicvolume.png");
	button[2] = D3D::LoadTexture("sprites/buttons/sethostipaddress.png");
	button[3] = D3D::LoadTexture("sprites/buttons/mainmenu.png");
	buttonOver[0] = D3D::LoadTexture("sprites/buttons/over/fxvolume.png");
	buttonOver[1] = D3D::LoadTexture("sprites/buttons/over/musicvolume.png");
	buttonOver[2] = D3D::LoadTexture("sprites/buttons/over/sethostipaddress.png");
	buttonOver[3] = D3D::LoadTexture("sprites/buttons/over/mainmenu.png");

	
	//	create dialog box for ip address
	dlgHwnd = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);


	return true;
}

void Options::Enter()
{
	if (!isInit)
		isInit = Init();


	//	start playing sounds for level
	if (previousState == 0 || previousState == onePlayer || previousState == twoPlayer)
		SoundEngine->PlayMusic(Sound::MENUBACKGROUND);

}

bool Options::Render(float deltaTime)
{
	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xFF000000, 1.0f, 0);
	Device->BeginScene();


	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	sprite->Draw(background, NULL, NULL, NULL, D3D::WHITE);
	sprite->Draw(title, NULL, &titleCenter, &D3DXVECTOR3(800, 100, 0), D3D::WHITE);
	for (int i = 0; i < 4; i++) {
		if (i != currentButton) {
			sprite->Draw(button[i], NULL, &buttonCenter, &D3DXVECTOR3(800, (float)300 + (100 * i), 0), D3D::WHITE);
		}
		else {
			sprite->Draw(buttonOver[i], NULL, &buttonCenter, &D3DXVECTOR3(800, (float)300 + (100 * i), 0), D3D::WHITE);
		}
	}
	sprite->End();

	Device->EndScene();
	Device->Present(0, 0, 0, 0);


	return true;
}

void Options::Update(float deltaTime)
{
	//	timer control for button switching
	if (timer >= .5f) {
		timer = 0;
		canSwitch = true;
	}
	else {
		timer += deltaTime;
	}

	//-------------------
	//	Change States
	//-------------------
	if (((GetAsyncKeyState(VK_RETURN) && 0x8000) || controller->isPressed(XINPUT_GAMEPAD_A)) && canSwitch) {
		SoundEngine->PlayFX(Sound::ENEMYSHOT);
		canSwitch = false;
		timer = 0;
		switch (currentButton) {
		case 0:
			//	adjust fx volume
			SoundEngine->fxVolume += .2;
			if (SoundEngine->fxVolume >= 2.5f)
				SoundEngine->fxVolume = 0;
			SoundEngine->PlayFX(Sound::FIRE);
			break;
		case 1:
			//	adjust music volume
			SoundEngine->musicVolume += .2;
			if (SoundEngine->musicVolume >= 2.5f)
				SoundEngine->musicVolume = 0;
			SoundEngine->ChangeMusicVolume();
			break;
		case 2:
			//	display and enable dialog window for setting the IP address
			if(!IsWindowVisible(dlgHwnd))
				ShowWindow(dlgHwnd, SW_SHOW);
			if (!IsWindowEnabled(dlgHwnd))
				EnableWindow(dlgHwnd, TRUE);
			break;
		case 3:
			Exit(menu);
			break;
		default:
			break;
		}
	}

	//-------------------
	//	Keyboard Input
	//-------------------
	if (GetAsyncKeyState(VK_DOWN) && 0x8000 && canSwitch) {
		SoundEngine->PlayFX(Sound::RELOAD);
		currentButton += 1;
		canSwitch = false;
		timer = 0;
	}
	if (GetAsyncKeyState(VK_UP) && 0x8000 && canSwitch) {
		SoundEngine->PlayFX(Sound::RELOAD);
		currentButton -= 1;
		canSwitch = false;
		timer = 0;
	}

	//-------------------
	//	Controller Input
	//-------------------
	if (controller->isConnected) {
		if (controller->leftY && canSwitch) {
			SoundEngine->PlayFX(Sound::RELOAD);
			currentButton -= controller->leftY / abs(controller->leftY);
			canSwitch = false;
			timer = 0;
		}
	}

	//	keep current button in range
	if (currentButton >= 4)
		currentButton = 0;
	if (currentButton <= -1)
		currentButton = 3;
}


void Options::Exit(GameState *nextState)
{
	//	stop playing sounds for level
	if(nextState == onePlayer || nextState == twoPlayer)
		SoundEngine->StopMusic();

	//	reset variables
	currentButton = 0;
	timer = 0;

	EndDialog(dlgHwnd, FALSE);

	//	go to the next state
	previousState = currentState;
	currentState = nextState;
	currentState->Enter();
}


