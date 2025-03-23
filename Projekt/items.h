#pragma once

#include <raylib.h>

class Items
{
public:
	Items();
	 ~Items();
	virtual void DrawItems();
protected:
	Vector2 position;
	Texture2D image;
};
class DamageTrinket : public Items
{
public:
	DamageTrinket();
	~DamageTrinket();
private:

};
class TearRateTrinket : public Items
{
public:
	TearRateTrinket();
	~TearRateTrinket();
private:

};
class SpeedTrinket : public Items
{
public:
	SpeedTrinket();
	~SpeedTrinket();
private:

};
class HealthTrinket : public Items
{
public:
	HealthTrinket();
	~HealthTrinket();
private:

};
class TearSpeedTrinket : public Items
{
public:
	TearSpeedTrinket();
	~TearSpeedTrinket();
private:

};
