#include "MYCLIENT.h"
#include "GameState.h"
#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::Initialize(bool isPlayer1)
{
	//	if the player is p1 (not a 2nd player in 2p mode), true (init hud)
	isP1 = isPlayer1;
	if (isP1)
		hud.Init();
	//	walk and strafe speed
	speed = 100;
	//	obviously not dead
	dead = false;
	//	find a random position to start
	setPosition(&D3DXVECTOR3(D3D::GetRandomFloat(-1000.0f, 1000.0f), 0.0f, -1000.0f));

	//	set states
	healthMax = health = 20;
	armorMax = armor = 10;
	ammoMax = ammo = 30;
	//	initialize model
	playerMesh.Initialize("models/player.x");
	//	initialize weapon
	weapon.Initialize();
}

void Player::Render(float deltaTime)
{

	//	get the inverse of the view matrix -- that is the position and rotation
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	Device->SetTransform(D3DTS_WORLD, &T);
	weapon.Render(deltaTime);
	//	set the scaling matrix
	D3DXMatrixScaling(&S, 3.5, 3.5, 3.5);
	//	set player position and render player and weapon
	D3DXMATRIX O, R;
	D3DXMatrixTranslation(&O, 0, 2.0f, -1.5f);
	D3DXMatrixRotationY(&R, D3DX_PI);
	W = R*S*O*T;
	Device->SetTransform(D3DTS_WORLD, &W);
	playerMesh.Render();


	//	set translation matrix for player's position
	D3DXMatrixTranslation(&T, _pos.x, _pos.y, _pos.z);

	//	transform the min and max bounding coordinates for collision detection
	D3DXVec3TransformCoord(&min, &playerMesh.min, &(S*T));
	D3DXVec3TransformCoord(&max, &playerMesh.max, &(S*T));

	//	if the player is p1, render the hud
	if(isP1)
		hud.Render();
}

void Player::Update(float deltaTime)
{
	//	timer control for firing
	if (fireTime > .1) {
		fireTime = 0;
		if (ammo > 0)
			canShoot = true;
	}
	else
		fireTime += deltaTime;

	//	update HUD
	if (isP1) {
		hud.Update(deltaTime);
		hud.playerPos = _pos;
	}

	//	keep player in bounds
	if (_pos.z <= -1000)
		_pos.z = -1000;
	if (_pos.z >= 1000)
		_pos.z = 1000;
	if (_pos.x <= -1000)
		_pos.x = -1000;
	if (_pos.x >= 1000)
		_pos.x = 1000;

	//	keep track of position
	prevPos = _pos;

	//-------------------
	//	Keyboard Input
	//-------------------
	if (GetAsyncKeyState('W') & 0x8000f)
		walk(speed * deltaTime);
	if (GetAsyncKeyState('S') & 0x8000f)
		walk(-speed * deltaTime);
	if (GetAsyncKeyState('A') & 0x8000f)
		strafe(-speed * deltaTime);
	if (GetAsyncKeyState('D') & 0x8000f)
		strafe(speed * deltaTime);
	if (GetAsyncKeyState(VK_UP) & 0x8000f) 
		pitch(-1.5f * deltaTime);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		pitch(1.5f * deltaTime);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		yaw(-1.5f * deltaTime);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		yaw(1.5f * deltaTime);
	if (GetAsyncKeyState(VK_SPACE) & 0x8000f && canShoot) {
		D3DXMATRIX bulletMatrix = weapon.CreateBullet();
		//	SEND BULLET MESSAGE
		if (currentState == twoPlayer) {
			char bulletMessage[120] = "";
			int currentPos = 0;
			bulletMessage[currentPos] = 'B';
			currentPos += sizeof(char);
			memcpy(&bulletMessage[currentPos], (char*)&bulletMatrix, sizeof(bulletMatrix));
			send(client->ConnectSocket, bulletMessage, sizeof(bulletMessage), 0);
		}
		ammo--;
		hud.UpdateAmmo(ammo);
		canShoot = false;
		SoundEngine->PlayFX(Sound::FIRE);
	}


	//-------------------
	//	Controller Input
	//-------------------
	if (controller->isConnected) {
		if (controller->leftX)
			strafe(speed * controller->leftX * deltaTime);
		if (controller->leftY)
			walk(speed * controller->leftY * deltaTime);
		if (controller->rightX)
			yaw(1.5f * controller->rightX * deltaTime);
		if (controller->rightY)
			pitch(1.5f *controller->rightY * deltaTime);
		if (controller->isPressed(XINPUT_GAMEPAD_A) && canShoot) {
			weapon.CreateBullet();
			ammo--;
			hud.UpdateAmmo(ammo);
			SoundEngine->PlayFX(Sound::FIRE);
			canShoot = false;
		}
	}
}

bool Player::CheckCollision(D3DXVECTOR3 * MIN, D3DXVECTOR3 * MAX)
{
	//	check to see if object min and max are inside player's min and max
	if ((MAX->x < max.x) && (MIN->x > min.x) && (MAX->z < max.z) && (MIN->z > min.z))
		return true;

	return false;
}

void Player::RemoveHealth()
{
	//	play hit sound
	SoundEngine->PlayFX(Sound::HIT);
	//remove armor/health
	if (armor > 0) {
		armor--;
		hud.UpdateArmor(MATH::fpuDiv((double)armor, (double)armorMax));
	}
	else if (health > 0) {
		health--;
		hud.UpdateHealth(MATH::fpuDiv((double)health, (double)healthMax));
	}
	//check to see if player is dead
	if (health <= 0) {
		SoundEngine->PlayFX(Sound::DIE);
		dead = true;
	}
}

void Player::AddHealth()
{
	//	refill health and update HUD
	SoundEngine->PlayFX(Sound::HEARTBEAT);
	health = healthMax;
	hud.UpdateHealth(1.0f);
}

void Player::AddArmor()
{
	//	refill armor and update HUD
	SoundEngine->PlayFX(Sound::ARMOR);
	armor = armorMax;
	hud.UpdateArmor(1.0f);
}

void Player::AddAmmo()
{
	//	refill ammo and update HUD
	SoundEngine->PlayFX(Sound::RELOAD);
	ammo = ammoMax;
	hud.UpdateAmmo(ammo);
}

void Player::Reset()
{
	//	reset stats
	health = healthMax;
	armor = armorMax;
	ammo = ammoMax;
	fireTime = 0;

	//	if first player, update hud
	if (isP1) {
		hud.UpdateHealth(1.0f);
		hud.UpdateArmor(1.0f);
		hud.UpdateAmmo(ammoMax);
		hud.levelTimer = 0;
	}

	//reset bullets
	for (int i = 0; i < weapon.bulletList.size(); i++) {
		weapon.bulletList[i]->Reset();
	}

	//reset position and angle/vectors
	setPosition(&D3DXVECTOR3(D3D::GetRandomFloat(-1000, 1000), 0, -1000));
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	not dead anymore
	dead = false;
}


//	put player's position information into a buffer to send to server
void Player::fillCharBuff()
{
	int currentPos = 0;
	//	'P' indicates a position message
	playerMessage[currentPos] = 'P';
	currentPos += sizeof(char);
	//	position vector
	memcpy(&playerMessage[currentPos], (char*)&_pos, sizeof(_pos));
	currentPos += sizeof(_pos);
	//	look vector
	memcpy(&playerMessage[currentPos], (char*)&_look, sizeof(_look));
	currentPos += sizeof(_look);
	//	right vector
	memcpy(&playerMessage[currentPos], (char*)&_right, sizeof(_right));
	currentPos += sizeof(_right);
	//	up vector
	memcpy(&playerMessage[currentPos], (char*)&_up, sizeof(_up));
	currentPos += sizeof(_up);

	playerMessage[currentPos] = '\0';
}


//	update player positon based on buffer recieved from server
void Player::getFromBuff(char* c)
{
	int currentPos = 1;
	//	position vector
	memcpy((char*)&_pos, &c[currentPos], sizeof(_pos));
	currentPos += sizeof(_pos);
	//	look vector
	memcpy((char*)&_look, &c[currentPos], sizeof(_look));
	currentPos += sizeof(_look);
	//	right vector
	memcpy((char*)&_right, &c[currentPos], sizeof(_right));
	currentPos += sizeof(_right);
	//	up vector
	memcpy((char*)&_up, &c[currentPos], sizeof(_up));
	currentPos += sizeof(_up);

}