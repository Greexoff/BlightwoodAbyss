#include <iostream>
#include "raylib.h"
#include "typ.h"

Issac::Issac()
{
	image = LoadTexture("graf.png");
	position.x = (GetScreenWidth()-image.width)/2;
	position.y = (GetScreenHeight() - image.height) / 2;
	lastTearTime = 0;
	playerHealth = 3;
}
Issac::~Issac() {
	UnloadTexture(image);
}
void Issac::Draw() {
	DrawTextureV(image, position, WHITE);
}
void Issac::moveDown() {
	position.y += 4;
	if (position.y>GetScreenHeight()-image.height)
	{
		position.y = GetScreenHeight() - image.height;
	}
}
void Issac::moveUp()
{
	position.y -= 4;
	if (position.y < 0)
	{
		position.y = 0;
	}
}
void Issac::moveLeft() {
	position.x -= 4;
	if (position.x < 0)
	{
		position.x = 0;
	}
}
void Issac::moveRight() {
	position.x += 4;
	if (position.x > GetScreenWidth() - image.width)
	{
		position.x = GetScreenWidth() - image.width;
	}
}
void Issac::shootTears(char pozycja) {
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
Vector2 Issac::GetPlayerPosition()
{
	return { position.x,position.y };
}
Rectangle Issac::getPlayerRect()
{
	return { position.x,position.y,(float)image.width,(float)image.height };
}
int Issac::getPlayerHealth()
{
	return playerHealth;
}
int Issac::setPlayerHealth()
{
	return playerHealth--;
}