#pragma once
#include "typ.h"
#include "tears.h"
#include "enemy.h"
#include "items.h"
#include "UI.h"
#include <random>
#include <map>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>
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
	void DrawPlayerCharacterImage(int Character);
	int getWaveNumber();
private:
	fs::path backgroundPath;
	unique_ptr<Character> Player;
	string playerTearType;
	fs::path object_assets;
	map<string, Texture2D> loadedTextures;
	Texture2D passCorrectTexture(string textureName);
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
	void loadTextures();
	void increasePlayerTotalScore(int amount);
};