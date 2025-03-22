#pragma once
#include "typ.h"
#include "tears.h"
#include "enemy.h"
#include <random>
#include <iostream>
#include <memory>
#include <thread>
using namespace std;

class Game {
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void InputHandle();
	bool isGameOver();
	int playerTotalScore;
private:
	Character Player;
	vector <shared_ptr<Enemy>> CreateEnemy();
	void DeleteInactiveTears();
	vector<shared_ptr<Enemy>> enemies;
	void MoveEnemies();
	vector<enemyTears>EnemyTears;
	void EnemyShootTears();
	double enemyShootingGap;
	double enemyHittingGap;
	double lastTearFired;
	double lastTimePlayerWasTouched;
	void CollisionCheck();
	int amountofEnemies;
	void beginNewWave();
	void disablePlayerTears();
	void disableEnemyTears();
	atomic<bool> isCreatingNewWave;
	atomic<bool> proceedCreatingEnemies;
	int waveNumber;
	void increasePlayerTotalScore(int amount);
};