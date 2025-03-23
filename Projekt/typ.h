#pragma once

#include <raylib.h>
#include "tears.h"
#include <vector>
using namespace std;
class Character {
public:
	Character();
	~Character();
	void Draw();
	void DrawPlayerHealthBar();
	void movePlayer(int x, int y);
	void shootTears(int tearD_X, int tearD_Y);
	vector<Tears> tearsy;
	Rectangle getPlayerRect();
	int getPlayerHealth();
	int reducePlayersHealth();
	int increasePlayersHealth();
	Vector2 GetXYPlayerPoint();
	float getPlayerDamage();
protected:
	Texture2D image;
	Texture2D imagePlayerHealthBar;
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