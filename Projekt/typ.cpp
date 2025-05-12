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
}
ThirdCharacter::~ThirdCharacter()
{
}

void Character::setPlayerStartingPosition()
{
	position.x = (ScreenSettings::GetInstance().getMaxMapLimit().x - image.width * stats.imageScale) / 2;
	position.y = (ScreenSettings::GetInstance().getMaxMapLimit().y - image.height* stats.imageScale) / 2;
}
void Character::Draw() {
	DrawTextureEx(image, position,0, stats.imageScale, WHITE);
}
void Character::movePlayer(int x, int y, Vector2 minMapLimit, Vector2 maxMapLimit)
{
	position.x += x * stats.playerSpeed;
	position.y += y * stats.playerSpeed;

	float playerWidth = image.width * stats.imageScale;
	float playerHeight = image.height * stats.imageScale;
	Vector2 minLimit = { ScreenSettings::GetInstance().getMinMapWalls().x,ScreenSettings::GetInstance().getMinMapWalls().y };
	Vector2 maxLimit = { ScreenSettings::GetInstance().getMaxMapWalls().x,ScreenSettings::GetInstance().getMaxMapWalls().y };

	position.x = ScreenSettings::GetInstance().Clamp(position.x, minLimit.x, maxLimit.x - playerWidth);
	position.y = ScreenSettings::GetInstance().Clamp(position.y, minLimit.y-(playerHeight * 0.5), maxLimit.y- playerHeight);
}
void Character::shootTears(int tearD_X, int tearD_Y, Texture2D loadedImage) {
	if (GetTime() - lastTearTime >= stats.tearRate) {
		float characterCenterX = position.x + (image.width * stats.imageScale*0.5);
		float characterCenterY = position.y + (image.height * stats.imageScale*0.5);
		float tearOffsetX = (loadedImage.width* 0.5);
		float tearOffsetY = (loadedImage.height*0.5);
		tearsy.push_back(Tears({ characterCenterX - tearOffsetX, characterCenterY - tearOffsetY },stats.tearSpeed,	tearD_X, tearD_Y,loadedImage));
		lastTearTime = GetTime();
	}
}
Rectangle Character::getPlayerRect()
{
	return { position.x,position.y,(float)image.width* stats.imageScale,(float)image.height* stats.imageScale };
}
void Character::DrawPlayerHealthBar()
{
	float healthBarWidth = image.width* stats.imageScale;
	float healthBarHeight = 10;

	Vector2 healthBarPos = {position.x,position.y - 10 };
	float healthPercent = (float)stats.playerHealth / stats.maxPlayerHealth;
	float currentHealthWidth = (float)((healthBarWidth - 10) * healthPercent);
	DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, BLACK);
	DrawRectangle(healthBarPos.x + 5, healthBarPos.y + 2.5, currentHealthWidth, (healthBarHeight / 2), GREEN);
}
Vector2 Character::GetXYPlayerPoint()
{
	return { (position.x + (image.width* stats.imageScale / 2)),(position.y + (image.height* stats.imageScale / 2)) };
}
void Character::loadOwnStats()
{
	for (const auto& it : CharactersData::getInstance().getCharacterStats())
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
characterStats Character::getPlayerStats()
{
	return stats;
}
