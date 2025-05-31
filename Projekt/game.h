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
#include "UserInfo.h"
#include "screenSettings.h"
using namespace std;
namespace fs = filesystem;
enum class UnlockMethod
{
	TIME_BASED_UNLOCK,
	WAVES_BASED_UNLOCK,
};
enum class ItemProgressAction
{
	INCREASE_VALUE,
	SET_VALUE,
	UPDATE_WARDEN,
	DISPLAY_WARDEN,
};
class Game {
public:
	Game();
	~Game();
	void Draw();
	void Update();
	void DrawBackground();
	void InputHandle();
	int playerTotalScore;
	void setPlayerCharacter(int Character);
	void setLastTimePlayerWasTouched();
	int getWaveNumber();
	bool isPostTabClosed();
private:
	struct unlockingItems
	{
		string itemName;
		float startingCondition;
		float endingCondition;
		UnlockMethod method;
		bool displayWarden;
		bool updateWarden;
		string itemDisplayName;
		string itemDescription;
	};
	bool itemProgressDrawCountdown;
	vector<unlockingItems> itemProgress;
	void updateItemProgress(string itemName, float value, ItemProgressAction action);
	void drawItemProgress();
	bool getItemProgressWarden(string itemName, ItemProgressAction wardenType);
	void setItemProgressWarden(string itemName, bool value, ItemProgressAction wardenType);
	void tryUnlockItem(string itemName, bool lessThan);
	void threadItemProgressUpdate(UnlockMethod method);
	void threadDisableDisplayWarden(string itemName);
	void setItemProgress();
	bool postTabClosed;
	bool isNewScoreHigher;
	bool wasDatabaseUpdated;
	bool updatePlayerInDataBase(int playerScore, string username, bool& flag);
	fs::path data_basePath = fs::current_path() / "database" / "DataBase.txt";
	void handlePostGameTab();
	bool isGameOver();
	Vector2 minMapLimit, maxMapLimit;
	float backgroundScale;
	unique_ptr<Character> Player;
	string playerTearType;
	void createRandomLoot(Vector2 enemyPos, bool condition);
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
	vector<shared_ptr<Items>> items;
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