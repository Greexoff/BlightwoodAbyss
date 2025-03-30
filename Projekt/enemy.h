#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <cmath>


using namespace std;
class Enemy
{
public:
	virtual void Update(Vector2 PlayerPosition);
	virtual void Draw();
	virtual void DrawEnemyHealthBar();
	virtual ~Enemy();
	//Texture2D image;
	//Vector2 position;
	virtual Rectangle getEnemyRect();
	virtual void UpdateColl(Vector2 Direction);
	virtual Vector2 getCollisionSide(Rectangle enemy1, Rectangle enemy2);
	virtual int setEnemyHealth(int playerDamage);
	virtual int getEnemyHealth();
	virtual float getEnemySpeed();
	virtual int getEnemyAttackSpeed();
	virtual int getEnemyScore();
	virtual void CheckOutofTheBorder();
	virtual Vector2 getEnemyPosition(float divideBy);
protected:
	vector<Texture2D> loadImages;
	Texture2D image;
	Vector2 position;
	int enemyHealth;
	float enemySpeed;
	int enemyAttackSpeed;
	int enemyScore;
	int enemyMaxHealth;
private:
};
class Monster1 : public Enemy
{
public:
	Monster1(Vector2 postion, Texture2D loadedImage);
	~Monster1();
private:
	
};
class Monster2 : public Enemy
{
public:
	Monster2(Vector2 postion, Texture2D loadedImage);
	~Monster2();
private:
	
};
class Monster3 : public Enemy
{
public:
	Monster3(Vector2 postion, Texture2D loadedImage);
	~Monster3();
private:
	
};
class Monster4 : public Enemy
{
public:
	Monster4(Vector2 postion, Texture2D loadedImage);
	~Monster4();
private:

};
class Monster5 : public Enemy
{
public:
	Monster5(Vector2 postion, Texture2D loadedImage);
	~Monster5();
private:

};