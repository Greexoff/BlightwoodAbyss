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
private:
	Texture2D image;
	Texture2D imagePlayerHealthBar;
	Vector2 position;
	double lastTearTime;
	int playerHealth;
	int maxPlayerHealth;
	float playerSpeed;
	int tearSpeed;
};