#include "screenSettings.h"
ScreenSettings::ScreenSettings()
{
	camera.zoom = 1;
	camera.offset = { baseResolution.x / 2,baseResolution.y / 2 };
	camera.target = {0,0};
	resolutionFactor = { 1,1 };
	setBaseResolution();
}
ScreenSettings& ScreenSettings::GetInstance()
{
	static ScreenSettings instance;
	return instance;
}
ScreenSettings::~ScreenSettings()
{
}
Vector2 ScreenSettings::getScreenResolutionFactor()
{
	return resolutionFactor;
}
void ScreenSettings::setResolutionFactor(int windowWidth, int windowHeight)
{
	Vector2 sourceResolution = { 1920,1080 };
	resolutionFactor.x = windowWidth / sourceResolution.x;
	resolutionFactor.y = windowHeight / sourceResolution.y;
}
float ScreenSettings::Clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}
void ScreenSettings::setBaseResolution()
{
	baseResolution = { 1920,1080 };
}
void ScreenSettings::updateCamera(Vector2 playerPosition)
{
	camera.target = playerPosition;
	camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
	camera.zoom = GetScreenHeight() / baseResolution.y;
}
Camera2D& ScreenSettings::getCamera()
{
	return camera;
}
