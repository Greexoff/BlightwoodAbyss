#include "screenSettings.h"
ScreenSettings::ScreenSettings()
{
	resolutionFactor = { 1,1 };
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