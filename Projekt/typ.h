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
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void shootTears(char);
	Vector2 GetPlayerPosition();
	vector<Tears> tearsy;
	Rectangle getPlayerRect();
	int getPlayerHealth();
	int reducePlayersHealth();
	int increasePlayersHealth();
private:
	Texture2D image;
	Texture2D imagePlayerHealthBar;
	Vector2 position;
	double lastTearTime;
	int playerHealth;
	int maxPlayerHealth;
	float playerSpeed;
};