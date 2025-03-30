#pragma once

#include <raylib.h>
#include "tears.h"
#include <vector>
using namespace std;
class Character {
public:
	virtual ~Character();
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
	virtual void setPlayerDamage(float amount);
	virtual void setPlayerSpeed(float amount);
	virtual void setPlayerTearSpeed(float amount);
	virtual void setPlayerTearRate(float amount);
	virtual void setPlayerMaxHealth(float amount);
	virtual void setPlayerStartingPosition();
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
	FirstCharacter(Texture2D loadedImage);
	~FirstCharacter();
private:

};
class SecondCharacter : public Character
{
public:
	SecondCharacter(Texture2D loadedImage);
	~SecondCharacter();
private:

};
class ThirdCharacter : public Character
{
public:
	ThirdCharacter(Texture2D loadedImage);
	~ThirdCharacter();
private:

};