#pragma once
#include "raylib.h"

class Tears {
public:
	Tears(Vector2 position, int speed, int tearDirection_X, int tearDirection_Y);
	void UpdatePosition();
	virtual void Draw();
	bool active;
	Rectangle getTearRect();
protected:
	Texture2D image; 
	Vector2 position;
	int tearSpeed;
	int tearDirection_X;
	int tearDirection_Y;
};
class enemyTears : public Tears
{
public:
	enemyTears(Vector2 position, int speed, Vector2 Playerpos);
	void UpdatePosition(Vector2 Playerpos);
	void Draw() override;
	bool active;
private:
	Vector2 TearsDirection;
	int speed;
	int directionX;
	int directionY;
};