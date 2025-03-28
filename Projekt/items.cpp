#include <iostream>
#include "raylib.h"
#include "items.h"

Items::Items(Vector2 enemyPos)
{
	image = LoadTexture("DamageTrinket.png");
	position.x = enemyPos.x - 3 * (image.width / 4);
	position.y = enemyPos.y - 3 * (image.height / 4);
}
Items::~Items()
{

}
DamageTrinket::DamageTrinket(Vector2 enemyPos) : Items(enemyPos)
{
	image = LoadTexture("DamageTrinket.png");
}
DamageTrinket::~DamageTrinket() { UnloadTexture(image); }
TearRateTrinket::TearRateTrinket(Vector2 enemyPos) : Items(enemyPos)
{
	image = LoadTexture("TearRateTrinket.png");
}
TearRateTrinket::~TearRateTrinket() { UnloadTexture(image); }
SpeedTrinket::SpeedTrinket(Vector2 enemyPos) : Items(enemyPos)
{
	image = LoadTexture("SpeedTrinket.png");
}
SpeedTrinket::~SpeedTrinket() { UnloadTexture(image); }
HealthTrinket::HealthTrinket(Vector2 enemyPos) : Items(enemyPos)
{
	image = LoadTexture("HealthTrinket.png");
}
HealthTrinket::~HealthTrinket() { UnloadTexture(image); }
TearSpeedTrinket::TearSpeedTrinket(Vector2 enemyPos) : Items(enemyPos)
{
	image = LoadTexture("TearSpeedTrinket.png");
}
TearSpeedTrinket::~TearSpeedTrinket() { UnloadTexture(image); }

void Items::DrawItems()
{
	DrawTextureV(image, position, WHITE);
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