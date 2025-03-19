#pragma once
#include "typ.h"
#include "tears.h"
#include "enemy.h"
#include <random>
using namespace std;

class Game {
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void InputHandle();
	bool isGameOver();
	void beginNextWave();
private:
	Character Player;
	vector <shared_ptr<Enemy>> CreateEnemy();
	void DeleteInactiveTears();
	char Direction='~';
	vector<shared_ptr<Enemy>> enemies;
	void MoveEnemies();
	int EnemyDirectionX;
	int EnemyDirectionY;
	vector<enemyTears>EnemyTears;
	void EnemyShootTears();
	double enemyShootingGap;
	double enemyHittingGap;
	double lastTearFired;
	double lastTimePlayerWasTouched;
	void CollisionCheck();
	int amountofEnemies;


};