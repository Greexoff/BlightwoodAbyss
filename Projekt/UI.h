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
#include "Textures.h"
#include "screenSettings.h"
import CharacterStats;
using namespace std;
namespace fs = filesystem;

class GameUI
{
public:
	static GameUI& GetInstance();
	~GameUI();
	void DrawScaledBackgroundImage(Texture2D& image);
	void DrawTextWithOutline(const string& text, Vector2 position, float fontSize);
	void DrawBlackBar(Rectangle border, unsigned char opacity);
	void DrawCharacterStatsInGame(characterStats playerStats, float x_pos, float starting_y_pos, float fontSize, map<string,float>& statsChange, bool drawChanges);
	void DrawCharacterStatsInMenu(int pageNumber, Rectangle bar, float fontSize, float y_position);
	void DrawTextOnBar(Rectangle bar, float fontSize, const string& text, float y_position);
	void DrawTextRules(Rectangle bar, float fontSize, const string& text, float& y_position);
	void DrawData(string& name, Rectangle bar, float& fontSize);
	void DrawError(string information, Rectangle bar);
	void DrawComments(string comment1, string comment2, Rectangle Bar);
	void DrawEnemyStatsInMenu(string enemyName, Rectangle bar, float fontSize, float y_position);
	void DrawGameUI(const string& text, float fontSize, float y_pos);
	string CreateTextWithLeadingZerosGameUI(int number, int amountOfZeros, const string& text);
	Rectangle setBarArea(float fontSize, string text, Vector2 textPosition, int type, float additional_x_space, float additional_y_space);
	Vector2 MeasureText(float fontSize, string text);
	string ConvertToString(float number, int prec);
	string ConvertStatToString(float number, int prec);
private:
	GameUI();
	GameUI(const GameUI&) = delete;
	void operator=(const GameUI&) = delete;
	fs::path fontPath;
	Color MyOrange;
	Color MyYellow;
	Font *font=nullptr;
	vector<string> DivideTextIntoParts(const string& text, float fontSize, float maxWidth);
};