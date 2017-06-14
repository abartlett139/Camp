#include "MainMenu.h"



MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
	D3D::Release(background);
	D3D::Release(title);
	for (int i = 0; i < 6; i++) {
		D3D::Release(button[i]);
		D3D::Release(buttonOver[i]);
	}
	D3D::Release<ID3DXSprite*>(sprite);
}

bool MainMenu::Init()
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
	title = D3D::LoadTexture("sprites/titles/thecamp.png");

	button[0] = D3D::LoadTexture("sprites/buttons/oneplayer.png");
	button[1] = D3D::LoadTexture("sprites/buttons/twoplayer.png");
	button[2] = D3D::LoadTexture("sprites/buttons/instructions.png");
	button[3] = D3D::LoadTexture("sprites/buttons/options.png");
	button[4] = D3D::LoadTexture("sprites/buttons/backstory.png");
	button[5] = D3D::LoadTexture("sprites/buttons/credits.png");
	button[6] = D3D::LoadTexture("sprites/buttons/exit.png");
	
	buttonOver[0] = D3D::LoadTexture("sprites/buttons/over/oneplayer.png");
	buttonOver[1] = D3D::LoadTexture("sprites/buttons/over/twoplayer.png");
	buttonOver[2] = D3D::LoadTexture("sprites/buttons/over/instructions.png");
	buttonOver[3] = D3D::LoadTexture("sprites/buttons/over/options.png");
	buttonOver[4] = D3D::LoadTexture("sprites/buttons/over/backstory.png");
	buttonOver[5] = D3D::LoadTexture("sprites/buttons/over/credits.png");
	buttonOver[6] = D3D::LoadTexture("sprites/buttons/over/exit.png");

	return true;
}

void MainMenu::Enter()
{
	if (!isInit)
		isInit = Init();


	//	start playing sounds for level
	if (previousState == 0 || previousState == onePlayer || previousState == twoPlayer)
		SoundEngine->PlayMusic(Sound::MENUBACKGROUND);
}

bool MainMenu::Render(float deltaTime)
{
	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xFF000000, 1.0f, 0);
	Device->BeginScene();


	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	sprite->Draw(background, NULL, NULL, NULL, D3D::WHITE);
	sprite->Draw(title, NULL, &titleCenter, &D3DXVECTOR3(800, 100, 0) , D3D::WHITE);
	for (int i = 0; i < 7; i++) {
		if (i != currentButton) {
			sprite->Draw(button[i], NULL, &buttonCenter, &D3DXVECTOR3(800, (float)250+(100*i), 0), D3D::WHITE);
		}
		else {
			sprite->Draw(buttonOver[i], NULL, &buttonCenter, &D3DXVECTOR3(800, (float)250 + (100 * i), 0), D3D::WHITE);
		}
	}
	sprite->End();

	Device->EndScene();
	Device->Present(0, 0, 0, 0);
	return true;
}

void MainMenu::Update(float deltaTime)
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
			Exit(onePlayer);
			break;
		case 1:
			Exit(twoPlayer);
			break;
		case 2:
			Exit(instructions);
			break;
		case 3:
			Exit(options);
			break;
		case 4:
			Exit(backstory);
			break;
		case 5:
			Exit(credits);
			break;
		case 6:
			PostQuitMessage(0);
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
	if (currentButton >= 7)
		currentButton = 0;
	if (currentButton <= -1)
		currentButton = 6;
}


void MainMenu::Exit(GameState *nextState)
{
	//	stop playing sounds for level
	if (nextState == onePlayer || nextState == twoPlayer)
		SoundEngine->StopMusic();

	//	reset variables
	currentButton = 0;
	timer = 0;


	//	go to the next state
	previousState = currentState;
	currentState = nextState;
	currentState->Enter();
}
