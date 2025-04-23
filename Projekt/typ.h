#pragma once

#include <raylib.h>
#include "tears.h"
#include <vector>
#include <string>
#include <map>
import CharacterStats;
using namespace std;
class Character {
public:
	virtual ~Character();
	virtual void Draw();
	virtual void DrawPlayerHealthBar();
	virtual void movePlayer(int x, int y);
	virtual void shootTears(int tearD_X, int tearD_Y, Texture2D loadedImage);
	vector<Tears> tearsy;
	virtual Rectangle getPlayerRect();
	virtual	Vector2 GetXYPlayerPoint();
	virtual void setPlayerDamage(float amount);
	virtual void setPlayerSpeed(float amount);
	virtual void setPlayerTearSpeed(float amount);
	virtual void setPlayerTearRate(float amount);
	virtual void setPlayerMaxHealth(float amount);
	virtual void setPlayerHealth(float amount);
	virtual void setPlayerStartingPosition();
	virtual int getPlayerMaxHealth();
	virtual int getPlayerHealth();
	virtual float getPlayerSpeed();
	virtual float getTearSpeed();
	virtual float getPlayerDamage();
	virtual float getTearRate();
	virtual void loadOwnStats();
protected:
	characterStats stats;
	string characterName;
	Texture2D image;
	Vector2 position;
	double lastTearTime;
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