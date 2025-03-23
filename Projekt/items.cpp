#include <iostream>
#include "raylib.h"
#include "items.h"

Items::Items()
{
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

TearRateTrinket::TearRateTrinket()
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