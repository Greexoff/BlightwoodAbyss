#include <iostream>
#include "raylib.h"
#include "typ.h"


Character::~Character() {
}
FirstCharacter::FirstCharacter(Texture2D loadedImage)
{
	image = loadedImage;
	setPlayerStartingPosition();
	playerHealth = 3;
	maxPlayerHealth = 3;
	playerSpeed = 6;
	playerDamage = 1;
	tearSpeed = 3;
	tearRate = 0.4;
	imageScale = 1;
}
FirstCharacter::~FirstCharacter()
{
}
SecondCharacter::SecondCharacter(Texture2D loadedImage)
{
	image = loadedImage;
	setPlayerStartingPosition();
	playerHealth = 1;
	maxPlayerHealth = 1;
	playerSpeed = 6.5;
	playerDamage = 5;
	tearSpeed = 5;
	tearRate = 0.3;
	imageScale = 1;
}
SecondCharacter::~SecondCharacter()
{
}
ThirdCharacter::ThirdCharacter(Texture2D loadedImage)
{
	image = loadedImage;
	setPlayerStartingPosition();
	playerHealth = 4;
	maxPlayerHealth = 4;
	playerSpeed = 4;
	playerDamage = 4;
	tearSpeed = 4;
	tearRate = 0.6;
	imageScale = 1;
}
ThirdCharacter::~ThirdCharacter()
{
}

void Character::setPlayerStartingPosition()
{
	position.x = (GetScreenWidth() - image.width* imageScale) / 2;
	position.y = (GetScreenHeight() - image.height* imageScale) / 2;
}
void Character::Draw() {
	DrawTextureEx(image, position,0,imageScale, WHITE);
}
void Character::movePlayer(int x, int y)
{
	position.x += x * playerSpeed;
	position.y += y * playerSpeed;
	if (position.y > GetScreenHeight()-170 - image.height* imageScale)
	{
		position.y = GetScreenHeight()-170 - image.height* imageScale;
	}
	if (position.y < 170)
	{
		position.y = 170;
	}
	if (position.x < 170)
	{
		position.x = 170;
	}
	if (position.x > GetScreenWidth()-170 - image.width* imageScale)
	{
		position.x = GetScreenWidth()-170 - image.width* imageScale;
	}
}
void Character::shootTears(int tearD_X, int tearD_Y, Texture2D loadedImage) {
	if (GetTime() - lastTearTime >= tearRate) {
		tearsy.push_back(Tears({ position.x + (image.width* imageScale / 4) + 5 * tearD_X, position.y + (image.height* imageScale / 4) + 5 * tearD_Y }, tearSpeed, tearD_X, tearD_Y, loadedImage));
		lastTearTime = GetTime();
	}
}
Rectangle Character::getPlayerRect()
{
	return { position.x,position.y,(float)image.width* imageScale,(float)image.height* imageScale };
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
	float healthBarWidth = image.width* imageScale;
	float healthBarHeight = 10;

	Vector2 healthBarPos = {position.x,position.y - 10 };
	float healthPercent = (float)playerHealth / maxPlayerHealth;
	float currentHealthWidth = (float)((healthBarWidth - 10) * healthPercent);
	DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, BLACK);
	DrawRectangle(healthBarPos.x + 5, healthBarPos.y + 2.5, currentHealthWidth, (healthBarHeight / 2), GREEN);
}
Vector2 Character::GetXYPlayerPoint()
{
	return { (position.x + (image.width* imageScale / 4)),(position.y + (image.width* imageScale / 4)) };
}
float Character::getPlayerDamage()
{
	return playerDamage;
}
void Character::setPlayerDamage(float amount)
{
	playerDamage +=amount;
}
void Character::setPlayerTearSpeed(float amount)
{
	tearSpeed +=amount;
}
void Character::setPlayerTearRate(float amount)
{
	if (tearRate <= 0.1)
	{
		tearRate -= (amount)/2;
	}
	else
	{
		tearRate -= amount;
	}
}
void Character::setPlayerSpeed(float amount)
{
	playerSpeed +=amount;
}
void Character::setPlayerMaxHealth(float amount)
{
	maxPlayerHealth +=amount;
}