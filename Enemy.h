#ifndef ENEMY_H
#define ENEMY_H

#include "EnemyBullet.h"


class Enemy
{
public:
	Enemy();
	~Enemy();

	bool Initialize(D3DXVECTOR3 *START);
	void Render(float deltaTime);
	void Update(float deltaTime, D3DXVECTOR3 *playerPos);
	void CreateBullet();
	bool CollisionDetection(D3DXVECTOR3 *MIN, D3DXVECTOR3 *MAX);

	void Reset();

	D3DXVECTOR3 startPos;	//	keeps track of where the enemy started
	D3DXVECTOR3 pos;

	Mesh enemyMesh;
	std::vector<EnemyBullet*>bulletList;

	D3DXMATRIX W, R, S, T;
	D3DXVECTOR3 min, max;
	float angle;
	float speed;
	float fireTime;
	int health, maxHealth;
	bool active, canShoot;

	enum {UP, DOWN, TURN, PURSUE, FIRE, DIE};
	int currentState, nextState;
	float nextAngle;
};

#endif