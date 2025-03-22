#include <iostream>
#include "raylib.h"
#include "typ.h"

Character::Character()
{
	image = LoadTexture("graf.png");
	position.x = (GetScreenWidth()-image.width)/2;
	position.y = (GetScreenHeight() - image.height) / 2;
	lastTearTime = 0;
	playerHealth = 300;
	maxPlayerHealth = 3;
	playerSpeed = 4.5;
	tearSpeed = 3;
}
Character::~Character() {
	UnloadTexture(image);
}
void Character::Draw() {
	DrawTextureV(image, position, WHITE);
}
void Character::movePlayer(int x, int y)
{
	position.x += x * playerSpeed;
	position.y += y * playerSpeed;
	if (position.y > GetScreenHeight() - image.height)
	{
		position.y = GetScreenHeight() - image.height;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > GetScreenWidth() - image.width)
	{
		position.x = GetScreenWidth() - image.width;
	}
}
void Character::shootTears(int tearD_X, int tearD_Y) {
	if (GetTime() - lastTearTime >= 0.4) {
		tearsy.push_back(Tears({ position.x + (image.width / 4) + 5 * tearD_X, position.y + (image.height / 4) + 5 * tearD_Y }, tearSpeed, tearD_X, tearD_Y));
		lastTearTime = GetTime();
	}
}
Rectangle Character::getPlayerRect()
{
	return { position.x,position.y,(float)image.width,(float)image.height };
}
int Character::getPlayerHealth()
{
	return playerHealth;
}
int Character::reducePlayersHealth()
{
	return playerHealth--;
}
int Character::increasePlayersHealth()
{
	if (playerHealth < maxPlayerHealth)
	{
		return playerHealth++;
	}
	else
	{
		return playerHealth;
	}
}
void Character::DrawPlayerHealthBar()
{
	float healthBarWidth = 100;
	float healthBarHeight = 10;

	Vector2 healthBarPos = {position.x,position.y - 10 };
	float healthPercent = (float)playerHealth / maxPlayerHealth;
	float currentHealthWidth = (float)((healthBarWidth - 10) * healthPercent);
	DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, BLACK);
	DrawRectangle(healthBarPos.x + 5, healthBarPos.y + 2.5, currentHealthWidth, (healthBarHeight / 2), GREEN);
}
Vector2 Character::GetXYPlayerPoint()
{
	return { (position.x + (image.width / 4)),(position.y + (image.width / 4)) };
}

