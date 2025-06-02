#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <random>
#include "screenSettings.h"
#include "tears.h"
#include "typ.h"
#include "Textures.h"
import CharacterStats;


using namespace std;
class Enemy
{
public:
	virtual void Update(Vector2 PlayerPosition);
	virtual void Draw();
	virtual void DrawEnemyHealthBar();
	virtual void shootTears(vector<enemyTears>& enemTears, Character* player);
	virtual ~Enemy();
	virtual Rectangle getEnemyRect();
	virtual void UpdateColl(Vector2 Direction);
	virtual Vector2 getCollisionSide(Rectangle enemy1, Rectangle enemy2);
	virtual int setEnemyHealth(float playerDamage);
	virtual int getEnemyHealth();
	virtual float getEnemySpeed();
	virtual int getEnemyAttackSpeed();
	virtual int getEnemyScore();
	virtual Vector2 getEnemyShootingPosition();
	virtual Vector2 getEnemyPosition();
	virtual void loadEnemyStats();
protected:
	void generateDelayOnShooting();
	Texture2D* image=nullptr;
	Vector2 position;
	enemyStats stats;
	string enemyName;
	double lastTearFiredTime=GetTime();
	Vector2 enemyShootingPos;
	float minShootingDelay=0.1, maxShootingDelay=8;
	float nextShootDelay;
private:
};
class Monster1 : public Enemy
{
public:
	Monster1(Vector2 postion, Texture2D& loadedImage);
	~Monster1();
private:
	
};
class Monster2 : public Enemy
{
public:
	Monster2(Vector2 postion, Texture2D& loadedImage);
	~Monster2();
private:
	
};
class Monster3 : public Enemy
{
public:
	Monster3(Vector2 postion, Texture2D& loadedImage);
	~Monster3();
private:
	
};
class Monster4 : public Enemy
{
public:
	Monster4(Vector2 postion, Texture2D& loadedImage);
	~Monster4();
private:

};
class Monster5 : public Enemy
{
public:
	Monster5(Vector2 postion, Texture2D& loadedImage);
	~Monster5();
private:
};
class Monster6 : public Enemy
{
public:
	Monster6(Vector2 postion, Texture2D& loadedImage);
	~Monster6();
	void Update(Vector2 PlayerPosition) override;
private:
	float teleportBackoff;
	float timeBetweenTeleport, lastTeleportTime;
};