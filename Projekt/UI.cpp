#include "UI.h"

GameUI::GameUI()
{
	MyOrange = { 255,110,37,255};
	MyYellow = { 218,208,0,255 };
	loadingScreenAssetsPath = fs::current_path() / "assets" / "background_loading_screen_assets";
	loadTexturesIntoMap();
	currentBackground = passCorrectTexture("backgroundLOADINGSTART.png"); \
	scoreText = "";
	fontPath = fs::current_path() / "assets" / "font_assets" / "VT323.ttf";
	font = LoadFontEx(fontPath.string().c_str(), 80, 0, 0);
}
GameUI& GameUI::GetInstance()
{
	static GameUI instance;
	return instance;
}
GameUI::~GameUI()
{
	for (auto& i : loadedScreenTextures)
	{
		UnloadTexture(i.second);
	}
	UnloadFont(font);
}
void GameUI::DrawTextWithOutline(const string& text, Vector2 position, float fontSize) {
	int spacing = (int)(fontSize / 20.0 + 0.5);
	int outlineSize = (int)(fontSize / 20.0 + 0.5);
	for (float dx = -outlineSize; dx <= outlineSize; dx++) {
		for (float dy = -outlineSize; dy <= outlineSize; dy++) {
			if (dx != 0 || dy != 0) {
				Vector2 offsetPos = { position.x + dx, position.y + dy };
				DrawTextEx(font, text.c_str(), offsetPos, fontSize, spacing, BLACK);
			}
		}
	}
	int shadowSize = (int)(fontSize / 50.0+0.5);
	Vector2 shadowPos = { position.x + shadowSize, position.y - shadowSize };
	DrawTextEx(font, text.c_str(), shadowPos, fontSize, spacing, MyYellow);
	DrawTextEx(font, text.c_str(), position, fontSize, spacing, MyOrange);
}
void GameUI::DrawCurrentScore(int currentPlayerScore, float fontSize)
{
	int width = 6;
	Vector2 labelPos = { 145, 42.5 };
	Vector2 scorePos = { 145, 92.5 };

	scoreText = to_string(currentPlayerScore);
	int leadingZeros = width - scoreText.length();
	string scoreWithZeros = string(leadingZeros, '0') + scoreText;

	DrawTextWithOutline("SCORE:", labelPos, fontSize);
	DrawTextWithOutline(scoreWithZeros, scorePos, fontSize);
}
Rectangle GameUI::setBarArea(float fontSize, string text, Vector2 textPosition, int type)
{
	float additionalSpace = 20;
	Vector2 measurements = MeasureTextBar(fontSize, text);
	switch (type)
	{
	case 1:
		return { textPosition.x - additionalSpace,textPosition.y - additionalSpace, measurements.x + additionalSpace * 2,measurements.y + additionalSpace * 2 };
		break;
	case 2:
		return { textPosition.x-additionalSpace,textPosition.y + measurements.y, measurements.x + additionalSpace * 2,measurements.y};
		break;
	default:
		return{ 0,0,0,0 };
		break;
	}
}
Vector2 GameUI::MeasureTextBar(float& fontSize, string text)
{
	Vector2 measurements = MeasureTextEx(font, text.c_str(), fontSize, (int)((fontSize / 20.0) + 0.5));
	while (measurements.x+30 > GetScreenWidth() || measurements.y+30 > GetScreenHeight())
	{
		fontSize -= 1;
		measurements = MeasureTextEx(font, text.c_str(), fontSize, (int)((fontSize / 20.0) + 0.5));
	}
	return measurements;
}
Vector2 GameUI::MeasureText(float fontSize, string text)
{
	return MeasureTextEx(font, text.c_str(), fontSize, (int)((fontSize / 20.0) + 0.5));
}
void GameUI::DrawBlackBar(Rectangle borders, unsigned char opacity)
{
	DrawRectangleRec(borders, { 0,0,0,opacity });
	DrawRectangleLinesEx(borders, 10, {0,0,0,opacity});
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
void GameUI::DisplayCharacterStats(float PlayerSpeed, float TearSpeed, float PlayerDamage, float TearRate)
{

}