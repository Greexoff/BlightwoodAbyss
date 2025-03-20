#include <iostream>
#include "raylib.h"
#include "typ.h"

Character::Character()
{
	image = LoadTexture("graf.png");
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
void Character::moveDown() {
	position.y += playerSpeed;
	if (position.y>GetScreenHeight()-image.height)
	{
		position.y = GetScreenHeight() - image.height;
	}
}
void Character::moveUp()
{
	position.y -= playerSpeed;
	if (position.y < 0)
	{
		position.y = 0;
	}
}
void Character::moveLeft() {
	position.x -= playerSpeed;
	if (position.x < 0)
	{
		position.x = 0;
	}
}
void Character::moveRight() {
	position.x += playerSpeed;
	if (position.x > GetScreenWidth() - image.width)
	{
		position.x = GetScreenWidth() - image.width;
	}
}
void Character::shootTears(char pozycja) {
	if (GetTime() - lastTearTime >= 0.3) {
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
int Character::changePlayerHealth(int typeOfChange)
{
	if (playerHealth <= maxPlayerHealth)
	{
		return playerHealth += typeOfChange;
	}
	else
	{
		playerHealth = maxPlayerHealth;
		return playerHealth;
	}
}
