#pragma once
#include <iostream>
#include <vector>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>
using namespace std;

class ScreenSettings
{
public:
	static ScreenSettings& GetInstance();
	Vector2 getScreenResolutionFactor();
	void setResolutionFactor(int windowWidth, int windowHeight);
	~ScreenSettings();
	float Clamp(float value, float min, float max);
	void updateCamera(Vector2 playerPosition);
	Camera2D& getCamera();
	Vector2 getMinMapLimit();
	Vector2 getMaxMapLimit();
	float getBackgroundScale();
	Vector2 getMinMapWalls();
	Vector2 getMaxMapWalls();
	Vector2 getBaseResolution();
	void ToggleFullScreenWindow();
private:
	void setMapLimit(Vector2& mapLimit);
	float backgroundScale;
	Vector2 maxMapLimit, minMapLimit, minMapWalls,maxMapWalls;
	Vector2 resolutionFactor;
	Vector2 baseResolution;
	Camera2D camera;
	ScreenSettings();
	ScreenSettings(const ScreenSettings&) = delete;
	void operator=(const ScreenSettings&) = delete;
	void setBaseResolution();
	void setMapWalls();
};