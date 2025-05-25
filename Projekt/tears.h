#pragma once
#include "raylib.h"
#include "screenSettings.h"

class Tears {
public:
	Tears(Vector2 position, int speed, int tearDirection_X, int tearDirection_Y, Texture2D& loadedImage);
	~Tears();
	void UpdatePosition(Vector2 minMapLimit, Vector2 maxMapLimit);
	virtual void Draw();
	bool active;
	Rectangle getTearRect();
protected:
	Texture2D* image=nullptr; 
	Vector2 position;
	int tearSpeed;
	int tearDirection_X;
	int tearDirection_Y;
};
class enemyTears : public Tears
{
public:
	enemyTears(Vector2 position, int speed, Vector2 Playerpos, Texture2D& loadedImage);
	void UpdatePosition(Vector2 Playerpos,Vector2 minMapLimit, Vector2 maxMapLimit);
	void Draw() override;
	bool active;
private:
	Vector2 TearsDirection;
	int speed;
	int directionX;
	int directionY;
};