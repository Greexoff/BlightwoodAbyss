#include "screenSettings.h"
ScreenSettings::ScreenSettings()
{
	backgroundScale = 1.5;
	camera.zoom = 1;
	camera.offset = { baseResolution.x / 2,baseResolution.y / 2 };
	camera.target = {0,0};
	resolutionFactor = { 1,1 };
	minMapLimit = { 0,0 };
	maxMapLimit = { 1536,1024 };
	setBaseResolution();
	setMapLimit(minMapLimit);
	setMapLimit(maxMapLimit);
	setMapWalls();
}
ScreenSettings& ScreenSettings::GetInstance()
{
	static ScreenSettings instance;
	return instance;
}
ScreenSettings::~ScreenSettings()
{
}
Vector2 ScreenSettings::getBaseResolution()
{
	return baseResolution;
}
Vector2 ScreenSettings::getScreenResolutionFactor()
{
	return resolutionFactor;
}
void ScreenSettings::setResolutionFactor(int windowWidth, int windowHeight)
{
	int monitor = GetCurrentMonitor();
	Vector2 sourceResolution = { GetMonitorWidth(monitor),GetMonitorHeight(monitor)};
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
	int monitor = GetCurrentMonitor();
	baseResolution = { (float)GetMonitorWidth(monitor),(float)GetMonitorHeight(monitor)};
}
void ScreenSettings::updateCamera(Vector2 playerPosition)
{
	float screenWidth = static_cast<float>(GetScreenWidth());
	float screenHeight = static_cast<float>(GetScreenHeight());

	camera.zoom = screenHeight / baseResolution.y;

	float viewWidth = screenWidth / camera.zoom;
	float viewHeight = screenHeight / camera.zoom;
	float minX = viewWidth*0.5;
	float maxX = maxMapLimit.x - (viewWidth*0.5);
	float minY = viewHeight*0.5;
	float maxY = maxMapLimit.y - (viewHeight*0.5);

	camera.target.x = Clamp(playerPosition.x, minX, maxX);
	camera.target.y = Clamp(playerPosition.y, minY, maxY);
	camera.offset = { screenWidth*(float)0.5, screenHeight*(float)0.5 };
}
Camera2D& ScreenSettings::getCamera()
{
	return camera;
}
Vector2 ScreenSettings::getMaxMapLimit()
{
	return maxMapLimit;
}
Vector2 ScreenSettings::getMinMapLimit()
{
	return minMapLimit;
}
float ScreenSettings::getBackgroundScale()
{
	return backgroundScale;
}
void ScreenSettings::setMapLimit(Vector2& mapLimit)
{
	mapLimit.x*=backgroundScale;
	mapLimit.y *= backgroundScale;
}
void ScreenSettings::setMapWalls()
{
	minMapWalls.x = minMapLimit.x + (170 * backgroundScale);
	minMapWalls.y = minMapLimit.y + (170 * backgroundScale);
	maxMapWalls.x = maxMapLimit.x - (170 * backgroundScale);
	maxMapWalls.y = maxMapLimit.y - (170 * backgroundScale);
}
Vector2 ScreenSettings::getMinMapWalls()
{
	return minMapWalls;
}
Vector2 ScreenSettings::getMaxMapWalls()
{
	return maxMapWalls;
}
void ScreenSettings::ToggleFullScreenWindow()
{
	int monitor = GetCurrentMonitor();
	Vector2 mousePos = GetMousePosition();
	if (IsWindowFullscreen())
	{
		int windowWidth = (int)GetMonitorWidth(monitor) * 0.66;
		int windowHeight = (int)GetMonitorHeight(monitor) * 0.66;
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
		SetWindowPosition((GetMonitorWidth(monitor) - windowWidth) / 2, (GetMonitorHeight(monitor) - windowHeight) / 2);
		setResolutionFactor(windowWidth, windowHeight);
		SetMousePosition(mousePos.x * resolutionFactor.x, mousePos.y * resolutionFactor.y);

	}
	else
	{
		ToggleFullscreen();
		SetWindowSize(baseResolution.x, baseResolution.y);
		SetMousePosition(mousePos.x / resolutionFactor.x, mousePos.y / resolutionFactor.y);
		setResolutionFactor(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	}
}