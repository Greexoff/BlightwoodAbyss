#pragma once

#include <raylib.h>
#include "tears.h"
#include <vector>
using namespace std;
class Issac {
public:
	Issac();
	~Issac();
	void Draw();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void shootTears(char);
	Vector2 GetPlayerPosition();
	vector<Tears> tearsy;
	Rectangle getPlayerRect();
private:
	Texture2D image;
	Vector2 position;
	double lastTearTime;

};