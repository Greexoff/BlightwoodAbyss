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

void DamageTrinket::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float damage = 1.5;
	player->setPlayerDamage(damage);
	statsChanges["Damage"] = damage;
}
void TearRateTrinket::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float tearRate = 0.1;
	player->setPlayerTearRate(tearRate);
	statsChanges["TearRate"] = -1.0f*tearRate;
}
void SpeedTrinket::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float speed = 1.5;
	player->setPlayerSpeed(1.5);
	statsChanges["Speed"] = speed;
}
void HealthTrinket::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float health = 1;
	player->setPlayerMaxHealth(health);
	player->setPlayerHealth(player->getPlayerMaxHealth());
	statsChanges["MaxHealth"] = health;
}
void TearSpeedTrinket::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float tearSpeed = 0.5;
	player->setPlayerTearSpeed(tearSpeed);
	statsChanges["TearSpeed"] = tearSpeed;

}
void AllTrinket::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float stats = 0.5;
	float tearRate = 0.1;
	float health = 1;

	player->setPlayerDamage(stats);
	player->setPlayerTearRate(tearRate);
	player->setPlayerSpeed(stats);
	player->setPlayerMaxHealth(health);
	player->setPlayerHealth(player->getPlayerMaxHealth());
	player->setPlayerTearSpeed(stats);

	statsChanges["Damage"] = stats;
	statsChanges["TearRate"] = -1.0f*tearRate;
	statsChanges["Speed"] = stats;
	statsChanges["TearSpeed"] = stats;
	statsChanges["MaxHealth"] = health;
	statsChanges["Health"] = health;
}
void HeartContainer::applyEffect(Character* player, map<string, float>& statsChanges)
{
	float health = 1;
	player->setPlayerHealth(health);
	statsChanges["Health"] = health;
}
void randomStatsItem::applyEffect(Character* player, map<string, float>& statsChanges)
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

	statsChanges["Damage"] =damage;
	statsChanges["TearRate"] = -1.0f*tearRate;
	statsChanges["Speed"] =speed;
	statsChanges["TearSpeed"] =tearSpeed;
}
