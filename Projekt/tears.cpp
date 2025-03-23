#include "tears.h"
#include <cmath>

Tears::Tears(Vector2 position, int tearSpeed, int tearDirection_X, int tearDirection_Y)
{
	image = LoadTexture("tear.png");
	this->position = position;
	this->tearSpeed = tearSpeed;
	this->tearDirection_X = tearDirection_X;
	this->tearDirection_Y = tearDirection_Y;
	active = true;

}
Tears::~Tears()
{
}
enemyTears::enemyTears(Vector2 position, int speed, Vector2 Playerpos) : Tears(position, speed, directionX, directionY) {
	image = LoadTexture("EnemyTears.png");
	this->position = position;
	this->speed = speed;
	active = true;
	Vector2 directionVec = { Playerpos.x - position.x, Playerpos.y - position.y };
	float length = sqrt(directionVec.x * directionVec.x + directionVec.y * directionVec.y);
	TearsDirection.x = directionVec.x / length;
	TearsDirection.y = directionVec.y / length;
}
void Tears::Draw() {
	if (active) {
		DrawTextureV(image, position, WHITE);
	}
}
void enemyTears::Draw() {
	if (active) {
		DrawTextureV(image, position, WHITE);
	}
}
void Tears::UpdatePosition() {
		position.y += tearSpeed*tearDirection_Y;
		position.x += tearSpeed * tearDirection_X;
		if (active) {
			if (position.y > GetScreenHeight() || position.y < 0 || position.x > GetScreenWidth() || position.x < 0)
			{
				active = false;
			}
		}
}
void enemyTears::UpdatePosition(Vector2 Playerpos)
{	
	position.x += TearsDirection.x * speed;
	position.y += TearsDirection.y * speed;
}
Rectangle Tears::getTearRect()
{
	return { position.x,position.y,(float)image.width,(float)image.height };
}
