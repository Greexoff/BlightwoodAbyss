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
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void shootTears(char);
	Vector2 GetPlayerPosition();
	vector<Tears> tearsy;
	Rectangle getPlayerRect();
	int getPlayerHealth();
	int changePlayerHealth(int typeOfChange);
private:
	Texture2D image;
	Vector2 position;
	double lastTearTime;
	int playerHealth;
	int maxPlayerHealth;
	float playerSpeed;
};