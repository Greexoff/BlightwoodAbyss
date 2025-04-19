#pragma once
#include <iostream>
#include <iomanip>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>
import CharacterStats;
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
	void DrawCharacterStatsInGame(float PlayerSpeed, float TearSpeed, float PlayerDamage, float TearRate, int playerMaxHealth, float x_pos, float starting_y_pos, float gap, float fontSize);
	void DrawCharacterStatsInMenu(int pageNumber, Rectangle bar, float fontSize, float y_position);
	Rectangle LOGINMENU_setBarArea(float fontSize, string text, Vector2 textPosition, int type, float additional_x_space, float additional_y_space);
	Vector2 MeasureTextBar(float& fontSize, string text);
	Vector2 MeasureText(float fontSize, string text);
	void DrawWaveNumber(int waveNumber, float fontSize);
	void DrawTextOnBar(Rectangle bar, float fontSize,const  string& text, float y_position);
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
	string ConvertToString(float number, int prec);
	vector<string> DivideTextIntoParts(const string& text, float fontSize, float maxWidth);
};