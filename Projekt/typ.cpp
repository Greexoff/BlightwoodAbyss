#include <iostream>
#include "raylib.h"
#include "typ.h"

Character::Character()
{
	image = LoadTexture("graf.png");
	imagePlayerHealthBar = LoadTexture("1H.png");
	position.x = (GetScreenWidth()-image.width)/2;
	position.y = (GetScreenHeight() - image.height) / 2;
	lastTearTime = 0;
	playerHealth = 3;
	maxPlayerHealth = 3;
	playerSpeed = 4.5;
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
void Character::shootTears(char pozycja) {
	if (GetTime() - lastTearTime >= 0.5) {
		switch (pozycja)
		{
		case 'u':
			tearsy.push_back(Tears({ position.x +(image.width/4)-3,position.y-(image.height/4)-3}, 3,'u'));
			lastTearTime = GetTime();
			break;
		case 'd':
			tearsy.push_back(Tears({ position.x + (image.width / 4) - 3,position.y + (image.height / 4) + 3 }, 3,'d'));
			lastTearTime = GetTime();
			break;
		case 'l':
			tearsy.push_back(Tears({ position.x-(image.width/4)-3,position.y + (image.height / 4)-3}, 3,'l'));
			lastTearTime = GetTime();
			break;
		case 'r':
			tearsy.push_back(Tears({ position.x + (image.width / 4)+3,position.y + (image.height / 4) - 3 }, 3,'r'));
			lastTearTime = GetTime();
			break;
		}
	}
}
Vector2 Character::GetPlayerPosition()
{
	return { position.x,position.y };
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
	int i = 0;
	while (i != playerHealth)
	{
		DrawTexture(imagePlayerHealthBar, position.x + 10 * i, position.y - 11, WHITE);
		i++;
	}
}
