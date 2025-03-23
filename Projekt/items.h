#pragma once

#include <raylib.h>
#include "typ.h"
#include <string>

class Items
{
public:
	Items();
	 ~Items();
	virtual void DrawItems();
	virtual Rectangle getItemRect();
	virtual void applyEffect(Character* player)=0;
protected:
	Vector2 position;
	Texture2D image;
};
class DamageTrinket : public Items
{
public:
	DamageTrinket();
	void applyEffect(Character* player) override;
	~DamageTrinket();
private:

};
class TearRateTrinket : public Items
{
public:
	TearRateTrinket();
	void applyEffect(Character* player) override;
	~TearRateTrinket();
private:

};
class SpeedTrinket : public Items
{
public:
	SpeedTrinket();
	void applyEffect(Character* player) override;
	~SpeedTrinket();
private:

};
class HealthTrinket : public Items
{
public:
	HealthTrinket();
	void applyEffect(Character* player) override;
	~HealthTrinket();
private:

};
class TearSpeedTrinket : public Items
{
public:
	TearSpeedTrinket();
	void applyEffect(Character* player) override;
	~TearSpeedTrinket();
private:

};
