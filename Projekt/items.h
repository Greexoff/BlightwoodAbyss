#pragma once

#include <raylib.h>
#include "typ.h"
#include <string>

class Items
{
public:
	virtual ~Items();
	virtual void DrawItems();
	virtual Rectangle getItemRect();
	virtual void applyEffect(Character* player)=0;
	virtual void setPosition(Vector2 enemyPos);
protected:
	float imageScale;
	Vector2 position;
	Texture2D* image = nullptr;
};
class DamageTrinket : public Items
{
public:
	DamageTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player) override;
	~DamageTrinket();
private:

};
class TearRateTrinket : public Items
{
public:
	TearRateTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player) override;
	~TearRateTrinket();
private:

};
class SpeedTrinket : public Items
{
public:
	SpeedTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player) override;
	~SpeedTrinket();
private:

};
class HealthTrinket : public Items
{
public:
	HealthTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player) override;
	~HealthTrinket();
private:

};
class TearSpeedTrinket : public Items
{
public:
	TearSpeedTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player) override;
	~TearSpeedTrinket();
private:

};
class HeartContainer : public Items
{
public:
	HeartContainer(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player) override;
	~HeartContainer();
private:
};
