#pragma once

#include <raylib.h>
#include "tears.h"
#include <vector>
using namespace std;
class Character {
public:
	Character();
	~Character();
	virtual void Draw();
	virtual void DrawPlayerHealthBar();
	virtual void movePlayer(int x, int y);
	virtual void shootTears(int tearD_X, int tearD_Y);
	vector<Tears> tearsy;
	virtual Rectangle getPlayerRect();
	virtual int getPlayerHealth();
	virtual int reducePlayersHealth();
	virtual int increasePlayersHealth();
	virtual	Vector2 GetXYPlayerPoint();
	virtual float getPlayerDamage();
protected:
	Texture2D image;
	Vector2 position;
	double lastTearTime;
	int playerHealth;
	int maxPlayerHealth;
	float playerSpeed;
	float tearSpeed;
	float playerDamage;
	float tearRate;
};
class FirstCharacter : public Character
{
public:
	FirstCharacter();
	~FirstCharacter();
private:

};
class SecondCharacter : public Character
{
public:
	SecondCharacter();
	~SecondCharacter();
private:

};
class ThirdCharacter : public Character
{
public:
	ThirdCharacter();
	~ThirdCharacter();
private:

};