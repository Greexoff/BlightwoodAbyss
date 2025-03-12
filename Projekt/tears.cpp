#include "tears.h"
#include <cmath>

Tears::Tears(Vector2 position, int speed, char direction)
{
	image = LoadTexture("tear.png");
	this->position = position;
	this->speed = speed;
	this->direction = direction;
	active = true;

}

enemyTears::enemyTears(Vector2 position, int speed, Vector2 Playerpos) : Tears(position, speed, direction) {
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
	switch (direction)
	{
	case 'u':
		position.y -= speed;
		if (active) {
			if (position.y > GetScreenHeight() || position.y < 0)
			{
				active = false;
			}
		}
		break;
	case 'd':
		position.y += speed;
		if (active) {
			if (position.y > GetScreenHeight() || position.y < 0)
			{
				active = false;
			}
		}
		break;
	case 'l':
		position.x -= speed;
		if (active) {
			if (position.x > GetScreenWidth() || position.x < 0)
			{
				active = false;
			}
		}
		break;
		case 'r':
			position.x += speed;
			if (active) {
				if (position.x > GetScreenWidth() || position.x < 0)
				{
					active = false;
				}
			}
			break;
		default:
		break;
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
