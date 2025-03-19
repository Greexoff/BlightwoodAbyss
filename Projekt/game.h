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
	void UpdateLocation();
	void InputHandle();
	bool isGameOver();
private:
	Issac Postac;
	vector <shared_ptr<Enemy>> CreateEnemy();
	void DeleteInactiveTears();
	char Direction='~';
	vector<shared_ptr<Enemy>> enemies;
	void MoveEnemies();
	int EnemyDirectionX;
	int EnemyDirectionY;
	vector<enemyTears>EnemyTears;
	void EnemyShootTears();
	constexpr static float enemyShootingGap = 1.5;
	double lastTearFired;
	void CollisionCheck();
	int amountofEnemies;


};