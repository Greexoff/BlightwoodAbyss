#pragma once
#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>

using namespace std;
namespace fs = filesystem;

class GameUI
{
public:
	static GameUI& GetInstance();
	~GameUI();
	void setLoadingProgress(string backgroundName);
	void DrawBackground();
	void DrawCurrentScore(int currentPlayerScore, float fontSize);
	void DrawTextWithOutline(const string& text, Vector2 position, float fontSize);
	void DrawBlackBar(Rectangle border, unsigned char opacity);
	Rectangle setBarArea(float fontSize, string text, Vector2 textPosition, int type);
	Vector2 MeasureTextBar(float& fontSize, string text);
	Vector2 MeasureText(float fontSize, string text);
private:
	GameUI();
	GameUI(const GameUI&) = delete;
	void operator=(const GameUI&) = delete;
	Color MyOrange;
	Color MyYellow;
	fs::path fontPath;
	Font font;
	string scoreText;
	Texture2D currentBackground;
	void loadTexturesIntoMap();
	Texture2D passCorrectTexture(string textureName);
	fs::path loadingScreenAssetsPath;
	map<string, Texture2D> loadedScreenTextures;
};