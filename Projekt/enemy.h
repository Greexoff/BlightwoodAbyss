#pragma once
#include "raylib.h"
#include <string>
#include <vector>


using namespace std;
class Enemy
{
public:
	Enemy(Vector2 position);
	void Update(Vector2 PlayerPosition, int directionX, int directionY, float enemySpeed);
	void Draw();
	virtual void DrawEnemyHealthBar();
	virtual ~Enemy();
	Texture2D image;
	Vector2 position;
	Rectangle getEnemyRect();
	void UpdateColl(string CollisionSide);
	string getCollisionSide(Rectangle enemy1, Rectangle enemy2);
	virtual int setEnemyHealth();
	virtual int getEnemyHealth();
	virtual float getEnemySpeed();
protected:
	int enemyHealth;
	Texture2D imageEnemyHealthBar;
	float enemySpeed;
private:
};
class Monster1 : public Enemy
{
public:
	Monster1(Vector2 postion);
	~Monster1();
private:
	
};
class Monster2 : public Enemy
{
public:
	Monster2(Vector2 postion);
	~Monster2();
private:
	
};
class Monster3 : public Enemy
{
public:
	Monster3(Vector2 postion);
	~Monster3();
private:
	
};
