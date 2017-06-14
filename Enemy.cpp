#include "Enemy.h"
#include "Sound.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

bool Enemy::Initialize(D3DXVECTOR3 * START)
{
	//	set active and health, and speed
	active = true;
	maxHealth = health = 5;
	speed = 95;
	angle = 0;
	//	set start position
	pos = startPos =  *START;
	currentState = DOWN;


	//	set scale
	D3DXMatrixScaling(&S, 3.5, 3.5, 3.5);


	//	set mesh
	enemyMesh.Initialize("models/enemy.x");

	//	initialize bullets
	for (int i = 0; i < 10; i++) {
		EnemyBullet *temp = new EnemyBullet();
		bulletList.push_back(temp);
	}
	for (int i = 0; i < bulletList.size(); i++)
		bulletList[i]->Initialize();

	return true;
}


void Enemy::Render(float deltaTime)
{
	//	set rotation and position and render
	D3DXMatrixRotationY(&R, angle);
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
	W = S*R*T;
	Device->SetTransform(D3DTS_WORLD, &W);
	enemyMesh.Render();

	//	render active bullets
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i]->active)
			bulletList[i]->Render(deltaTime);
	}

	//	transform the min and max bounding coordinates for collision detection
	D3DXVec3TransformCoord(&min, &enemyMesh.min, &(S*T));
	D3DXVec3TransformCoord(&max, &enemyMesh.max, &(S*T));
}

void Enemy::Update(float deltaTime, D3DXVECTOR3 *playerPos)
{
	//	fire rate control
	if (fireTime > 1) {
		fireTime = 0;
		canShoot = true;
	}
	else {
		fireTime += deltaTime;
	}

	//	check to see what state the enemy is in
	float x = pos.x - playerPos->x;
	float z = pos.z - playerPos->z;
	if (sqrt((x*x) + (z*z)) < 200) {
		currentState = PURSUE;
	}
	if (sqrt((x*x) + (z*z)) < 100) {
		currentState = FIRE;
	}
	if (health <= 0)
		currentState = DIE;



	//	state machine
	switch (currentState) {
	case UP:
		if(pos.z <= 975)
			pos.z += speed*deltaTime; 
		else {
			nextState = DOWN;
			nextAngle = 0;
			currentState = TURN;
		}
		break;
	case DOWN:
		if(pos.z >= -975)
			pos.z -= speed*deltaTime;
		else {
			nextState = UP;
			nextAngle = D3DX_PI;
			currentState = TURN;
		}
		break;
	case TURN:
		if (sqrt(nextAngle - angle)*(nextAngle - angle) > 0.1) {
			if (angle > nextAngle)
				angle -= deltaTime;
			else if (angle < nextAngle)
				angle += deltaTime;
		}
		else {
			angle = nextAngle;
			currentState = nextState;
		}
		break;
	case PURSUE:
		angle = atan2(x, z);
		pos.x -= speed*deltaTime*sin(angle);
		pos.z -= speed*deltaTime*cos(angle);
		break;
	case FIRE:
		angle = atan2(x, z);
		if (canShoot) {
			SoundEngine->PlayFX(Sound::ENEMYSHOT);
			CreateBullet();   
			canShoot = false;
		}
		break;
	case DIE:
		active = false;
		break;
	}
}

void Enemy::CreateBullet()
{
	//	find the next non-active bullet in the list and spawn it
	for (int i = 0; i < bulletList.size(); i++) {
		if (!bulletList[i]->active) {
			bulletList[i]->Spawn(&(R*T));
			break;
		}
	}
}


bool Enemy::CollisionDetection(D3DXVECTOR3 *MIN, D3DXVECTOR3 *MAX)
{
	//	check to see if object min and max are inside enemy's min and max
	if ((MAX->x < max.x) && (MIN->x > min.x) && (MAX->z < max.z) && (MIN->z > min.z))
		return true;

	return false;
}


void Enemy::Reset() {
	//	reset the enemy for next game/level
	pos = startPos;
	active = true;
	health = maxHealth;
	angle = 0;
	currentState = DOWN;
	for (int i = 0; i < bulletList.size(); i++)
		bulletList[i]->Reset();
}

