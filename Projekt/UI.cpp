#include "UI.h"

GameUI::GameUI()
{
	loadingScreenAssetsPath = fs::current_path() / "assets" / "background_loading_screen_assets";
	loadTexturesIntoMap();
	currentBackground = passCorrectTexture("backgroundLOADINGSTART.png");
}
GameUI::~GameUI()
{
	for (auto& i : loadedScreenTextures)
	{
		UnloadTexture(i.second);
	}
}
void GameUI::loadTexturesIntoMap()
{
	for (const auto& textureName : fs::directory_iterator(loadingScreenAssetsPath))
	{
		loadedScreenTextures.insert({ textureName.path().filename().string(), LoadTexture(textureName.path().string().c_str()) });
	}
}
Texture2D GameUI::passCorrectTexture(string textureName)
{
	for (auto& checking : loadedScreenTextures)
	{
		if (checking.first == textureName)
		{
			return checking.second;
		}
	}
}
void GameUI::setLoadingProgress(string backgroundName)
{
	currentBackground=passCorrectTexture(backgroundName);
}
void GameUI::DrawBackground()
{
	DrawTextureV(currentBackground, { 0,0 }, WHITE);
}