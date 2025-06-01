#pragma once

#include <raylib.h>
#include "typ.h"
#include <string>
#include <random>

class Items
{
public:
	virtual ~Items();
	virtual void DrawItems();
	virtual Rectangle getItemRect();
	virtual void applyEffect(Character* player, map<string,float>& statsChanges)=0;
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
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~DamageTrinket();
private:

};
class TearRateTrinket : public Items
{
public:
	TearRateTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~TearRateTrinket();
private:

};
class SpeedTrinket : public Items
{
public:
	SpeedTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~SpeedTrinket();
private:

};
class HealthTrinket : public Items
{
public:
	HealthTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~HealthTrinket();
private:

};
class TearSpeedTrinket : public Items
{
public:
	TearSpeedTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~TearSpeedTrinket();
private:

};
class AllTrinket : public Items
{
public:
	AllTrinket(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~AllTrinket();
private:
};
class HeartContainer : public Items
{
public:
	HeartContainer(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~HeartContainer();
private:
};
class randomStatsItem : public Items
{
public:
	randomStatsItem(Texture2D& loadedImage, Vector2 enemyPos);
	void applyEffect(Character* player, map<string, float>& statsChanges) override;
	~randomStatsItem();
private:
};
