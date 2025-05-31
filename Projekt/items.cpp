#include <iostream>
#include "raylib.h"
#include "items.h"

Items::~Items()
{

}
DamageTrinket::DamageTrinket(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 1.1f;
	image = &loadedImage;
	setPosition(enemyPos);
}
DamageTrinket::~DamageTrinket() {}
TearRateTrinket::TearRateTrinket(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 1.1f;
	image = &loadedImage;
	setPosition(enemyPos);
}
TearRateTrinket::~TearRateTrinket() {}
SpeedTrinket::SpeedTrinket(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 1.1f;
	image = &loadedImage;
	setPosition(enemyPos);
}
SpeedTrinket::~SpeedTrinket() {}
HealthTrinket::HealthTrinket(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 1.1f;
	image = &loadedImage;
	setPosition(enemyPos);
}
HealthTrinket::~HealthTrinket() {}
TearSpeedTrinket::TearSpeedTrinket(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale =1.1f;
	image = &loadedImage;
	setPosition(enemyPos);
}
TearSpeedTrinket::~TearSpeedTrinket() {}
AllTrinket::AllTrinket(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 1.2f;
	image = &loadedImage;
	setPosition(enemyPos);
}
AllTrinket::~AllTrinket() {}
HeartContainer::HeartContainer(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 0.7f;
	image = &loadedImage;
	setPosition(enemyPos);
}
HeartContainer::~HeartContainer() {}
randomStatsItem::randomStatsItem(Texture2D& loadedImage, Vector2 enemyPos)
{
	imageScale = 0.7f;
	image = &loadedImage;
	setPosition(enemyPos);
}
randomStatsItem::~randomStatsItem() {}
void Items::DrawItems()
{
	DrawTextureEx(*image, position, 0, imageScale, WHITE);
}
void Items::setPosition(Vector2 enemyPos)
{
	position.x = enemyPos.x - ((image->width*imageScale)/ 2);
	position.y = enemyPos.y - ((image->height*imageScale) / 2);
}
Rectangle Items::getItemRect()
{
	return {position.x,position.y,(float)image->width,(float)image->height };
}
void DamageTrinket::applyEffect(Character* player)
{
	player->setPlayerDamage(1.5);
}
void TearRateTrinket::applyEffect(Character* player)
{
	player->setPlayerTearRate(0.1);
}
void SpeedTrinket::applyEffect(Character* player)
{
	player->setPlayerSpeed(1.5);
}
void HealthTrinket::applyEffect(Character* player)
{
	player->setPlayerMaxHealth(1);
	player->setPlayerHealth(player->getPlayerMaxHealth());
}
void TearSpeedTrinket::applyEffect(Character* player)
{
	player->setPlayerTearSpeed(0.5);
}
void AllTrinket::applyEffect(Character* player)
{
	player->setPlayerDamage(0.5);
	player->setPlayerTearRate(0.1);
	player->setPlayerSpeed(0.5);
	player->setPlayerMaxHealth(1);
	player->setPlayerHealth(player->getPlayerMaxHealth());
	player->setPlayerTearSpeed(0.5);
}
void HeartContainer::applyEffect(Character* player)
{
	player->setPlayerHealth(1);
}
void randomStatsItem::applyEffect(Character* player)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> normalDistrib(-0.3f, 0.4f);
	uniform_real_distribution<float> tearRateDistrib(-0.12f, 0.1f);

	float damage = normalDistrib(gen);
	float tearRate = tearRateDistrib(gen);
	float speed = normalDistrib(gen);
	float tearSpeed = normalDistrib(gen);
	player->setPlayerDamage(damage);
	player->setPlayerTearRate(tearRate);
	player->setPlayerSpeed(speed);
	player->setPlayerTearSpeed(tearSpeed);
}
