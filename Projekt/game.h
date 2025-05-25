#pragma once
#include "typ.h"
#include "tears.h"
#include "enemy.h"
#include "items.h"
#include "UI.h"
#include "Textures.h"
#include <random>
#include <map>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>
#include "screenSettings.h"
using namespace std;
namespace fs = filesystem;

class Game {
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void DrawBackground();
	void InputHandle();
	bool isGameOver();
	int playerTotalScore;
	void setPlayerCharacter(int Character);
	void setLastTimePlayerWasTouched();
	int getWaveNumber();
private:
	Vector2 minMapLimit, maxMapLimit;
	float backgroundScale;
	unique_ptr<Character> Player;
	string playerTearType;
	unique_ptr<Items> Loot;
	void createRandomLoot(Vector2 enemyPos);
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
	long long breakTime;
	void increasePlayerTotalScore(int amount);
	bool startCountdown;
	double breakStartingTime;
	void DrawCountdownToNewWave();
	void DrawScoreAndWaveNumber();
	Vector2 getRandomPosition(Texture2D& texture);
	shared_ptr<Enemy> createEnemyBasedOnType(int type);
};