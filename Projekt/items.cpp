#include <iostream>
#include "raylib.h"
#include "items.h"

Items::~Items()
{

}
DamageTrinket::DamageTrinket(Texture2D loadedImage, Vector2 enemyPos)
{
	image = loadedImage;
	setPosition(enemyPos);
}
DamageTrinket::~DamageTrinket() {}
TearRateTrinket::TearRateTrinket(Texture2D loadedImage, Vector2 enemyPos)
{
	image = loadedImage;
	setPosition(enemyPos);
}
TearRateTrinket::~TearRateTrinket() {}
SpeedTrinket::SpeedTrinket(Texture2D loadedImage, Vector2 enemyPos)
{
	image = loadedImage;
	setPosition(enemyPos);
}
SpeedTrinket::~SpeedTrinket() {}
HealthTrinket::HealthTrinket(Texture2D loadedImage, Vector2 enemyPos)
{
	image = loadedImage;
	setPosition(enemyPos);
}
HealthTrinket::~HealthTrinket() {}
TearSpeedTrinket::TearSpeedTrinket(Texture2D loadedImage, Vector2 enemyPos)
{
	image = loadedImage;
	setPosition(enemyPos);
}
TearSpeedTrinket::~TearSpeedTrinket() {}

void Items::DrawItems()
{
	DrawTextureV(image, position, WHITE);
}
void Items::setPosition(Vector2 enemyPos)
{
	position.x = enemyPos.x - 3 * (image.width / 4);
	position.y = enemyPos.y - 3 * (image.height / 4);
}
Rectangle Items::getItemRect()
{
	return {position.x,position.y,(float)image.width,(float)image.height };
}
void DamageTrinket::applyEffect(Character* player)
{
	player->setPlayerDamage(1.5);
}
void TearRateTrinket::applyEffect(Character* player)
{
	player->setPlayerTearRate(0.04);
}
void SpeedTrinket::applyEffect(Character* player)
{
	player->setPlayerSpeed(1.5);
}
void HealthTrinket::applyEffect(Character* player)
{
	player->setPlayerMaxHealth(1);
}
void TearSpeedTrinket::applyEffect(Character* player)
{
	player->setPlayerTearSpeed(0.5);
}