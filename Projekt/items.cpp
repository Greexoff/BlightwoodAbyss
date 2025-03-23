#include <iostream>
#include "raylib.h"
#include "items.h"

Items::Items()
{
	image = LoadTexture("DamageTrinket.png");
	position.x = (GetScreenWidth() - image.width) / 2;
	position.y = (GetScreenHeight() - image.height) / 2;
}
Items::~Items()
{

}
DamageTrinket::DamageTrinket()
{
	image = LoadTexture("DamageTrinket.png");
}
DamageTrinket::~DamageTrinket() { UnloadTexture(image); }
TearRateTrinket::TearRateTrinket() : Items()
{
	image = LoadTexture("TearRateTrinket.png");
}
TearRateTrinket::~TearRateTrinket() { UnloadTexture(image); }
SpeedTrinket::SpeedTrinket()
{
	image = LoadTexture("SpeedTrinket.png");
}
SpeedTrinket::~SpeedTrinket() { UnloadTexture(image); }
HealthTrinket::HealthTrinket()
{
	image = LoadTexture("HealthTrinket.png");
}
HealthTrinket::~HealthTrinket() { UnloadTexture(image); }
TearSpeedTrinket::TearSpeedTrinket()
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
	player->setPlayerDamage(1);
}
void TearRateTrinket::applyEffect(Character* player)
{
	player->setPlayerTearRate(1);
}
void SpeedTrinket::applyEffect(Character* player)
{
	player->setPlayerSpeed(1);
}
void HealthTrinket::applyEffect(Character* player)
{
	player->setPlayerMaxHealth(1);
}
void TearSpeedTrinket::applyEffect(Character* player)
{
	player->setPlayerTearSpeed(1);
}