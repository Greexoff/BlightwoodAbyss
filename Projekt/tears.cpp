#include "tears.h"
#include <cmath>

Tears::Tears(Vector2 position, int tearSpeed, int tearDirection_X, int tearDirection_Y, Texture2D& loadedImage)
{
	image = &loadedImage;
	this->position = position;
	this->tearSpeed = tearSpeed;
	this->tearDirection_X = tearDirection_X;
	this->tearDirection_Y = tearDirection_Y;
	active = true;

}
Tears::~Tears()
{
}
enemyTears::enemyTears(Vector2 position, int speed, Vector2 Playerpos, Texture2D& loadedImage) : Tears(position, speed, directionX, directionY, loadedImage) {
	image = &loadedImage;
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
		DrawTextureV(*image, position, WHITE);
	}
}
void enemyTears::Draw() {
	if (active) {
		DrawTextureV(*image, position, WHITE);
	}
}
void Tears::UpdatePosition(Vector2 minMapLimit, Vector2 maxMapLimit) {
		position.y += tearSpeed*tearDirection_Y;
		position.x += tearSpeed * tearDirection_X;
		if (active) {
			if (position.y > maxMapLimit.y || position.y < minMapLimit.y || position.x > maxMapLimit.x || position.x < minMapLimit.x)
			{
				active = false;
			}
		}
}
void enemyTears::UpdatePosition(Vector2 Playerpos, Vector2 minMapLimit, Vector2 maxMapLimit)
{	
	position.x += TearsDirection.x * speed;
	position.y += TearsDirection.y * speed;
	if (active) {
		if (position.y > maxMapLimit.y || position.y < minMapLimit.y || position.x > maxMapLimit.x || position.x < minMapLimit.x)
		{
			active = false;
		}
	}
}
Rectangle Tears::getTearRect()
{
	return { position.x,position.y,(float)image->width,(float)image->height };
}
