#include <iostream>
#include "raylib.h"
#include "typ.h"

Character::~Character() {
}
FirstCharacter::FirstCharacter(Texture2D loadedImage)
{
	characterName = "FirstCharacter";
	loadOwnStats();
	image = loadedImage;
	setPlayerStartingPosition();
	imageScale = 1;
}
FirstCharacter::~FirstCharacter()
{
}
SecondCharacter::SecondCharacter(Texture2D loadedImage)
{
	characterName = "SecondCharacter";
	loadOwnStats();
	image = loadedImage;
	setPlayerStartingPosition();
	imageScale = 1;
}
SecondCharacter::~SecondCharacter()
{
}
ThirdCharacter::ThirdCharacter(Texture2D loadedImage)
{
	characterName = "ThirdCharacter";
	loadOwnStats();
	image = loadedImage;
	setPlayerStartingPosition();
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
	position.x += x * stats.playerSpeed;
	position.y += y * stats.playerSpeed;
	if (position.y > GetScreenHeight()-170 - image.height* imageScale)
	{
		position.y = GetScreenHeight()-170 - image.height* imageScale;
	}
	if (position.y < (190-image.height*imageScale))
	{
		position.y = (190 - image.height * imageScale);
	}
	if (position.x < 160)
	{
		position.x = 160;
	}
	if (position.x > GetScreenWidth()-160 - image.width* imageScale)
	{
		position.x = GetScreenWidth()-160 - image.width* imageScale;
	}
}
void Character::shootTears(int tearD_X, int tearD_Y, Texture2D loadedImage) {
	if (GetTime() - lastTearTime >= stats.tearRate) {
		tearsy.push_back(Tears({ position.x + (image.width* imageScale / 4) + 5 * tearD_X, position.y + (image.height* imageScale / 4) + 5 * tearD_Y }, stats.tearSpeed, tearD_X, tearD_Y, loadedImage));
		lastTearTime = GetTime();
	}
}
Rectangle Character::getPlayerRect()
{
	return { position.x,position.y,(float)image.width* imageScale,(float)image.height* imageScale };
}
void Character::DrawPlayerHealthBar()
{
	float healthBarWidth = image.width* imageScale;
	float healthBarHeight = 10;

	Vector2 healthBarPos = {position.x,position.y - 10 };
	float healthPercent = (float)stats.playerHealth / stats.maxPlayerHealth;
	float currentHealthWidth = (float)((healthBarWidth - 10) * healthPercent);
	DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, BLACK);
	DrawRectangle(healthBarPos.x + 5, healthBarPos.y + 2.5, currentHealthWidth, (healthBarHeight / 2), GREEN);
}
Vector2 Character::GetXYPlayerPoint()
{
	return { (position.x + (image.width* imageScale / 4)),(position.y + (image.width* imageScale / 4)) };
}
void Character::loadOwnStats()
{
	for (const auto& it : CharacterData::GetAllStats())
	{
		if (it.first == characterName)
		{
			stats = it.second;
		}
	}
}

int Character::getPlayerMaxHealth()
{
	return stats.maxPlayerHealth;
}
int Character::getPlayerHealth()
{
	return stats.playerHealth;
}
float Character::getPlayerSpeed()
{
	return stats.playerSpeed;
}
float Character::getTearSpeed()
{
	return stats.tearSpeed;
}
float Character::getPlayerDamage()
{
	return stats.playerDamage;
}
float Character::getTearRate()
{
	return stats.tearRate;
}

void Character::setPlayerHealth(float amount)
{
	stats.playerHealth+=amount;
	if (stats.playerHealth >= stats.maxPlayerHealth)
	{
		stats.playerHealth = stats.maxPlayerHealth;
	}
}
void Character::setPlayerDamage(float amount)
{
	stats.playerDamage += amount;
}
void Character::setPlayerTearSpeed(float amount)
{
	stats.tearSpeed += amount;
}
void Character::setPlayerTearRate(float amount)
{
	if (stats.tearRate <= 0.1)
	{
		stats.tearRate -= (amount) / 2;
	}
	else
	{
		stats.tearRate -= amount;
	}
}
void Character::setPlayerSpeed(float amount)
{
	stats.playerSpeed += amount;
}
void Character::setPlayerMaxHealth(float amount)
{
	stats.maxPlayerHealth += amount;
}
