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
	Vector2 labelPos = { 50, 42.5 };
	Vector2 scorePos = { 50, 92.5 };

	scoreText = to_string(currentPlayerScore);
	int leadingZeros = width - scoreText.length();
	string scoreWithZeros = string(leadingZeros, '0') + scoreText;

	DrawTextWithOutline("SCORE:", labelPos, fontSize);
	DrawTextWithOutline(scoreWithZeros, scorePos, fontSize);
}
Rectangle GameUI::LOGINMENU_setBarArea(float fontSize, string text, Vector2 textPosition, int type,float additional_x_space, float additional_y_space)
{
	float additionalSpace = 20;
	Vector2 measurements = MeasureTextBar(fontSize, text);
	switch (type)
	{
	case 1://Napis wewnatrz
		return { textPosition.x - additional_x_space,textPosition.y - additional_y_space, measurements.x + additional_x_space * 2,measurements.y + additional_y_space * 2 };
		break;
	case 2://Napis nad paskiem
		return { textPosition.x- additional_x_space,textPosition.y + measurements.y, measurements.x + additional_x_space * 2,measurements.y};
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
	DrawRectangleLinesEx(borders, 10, BLACK);
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
string GameUI::ConvertToString(float number, int prec)
{
	ostringstream text;
	text<<fixed<<setprecision(prec) << number;
	return text.str();
}
void GameUI::DrawCharacterStatsInGame(float PlayerSpeed, float TearSpeed, float PlayerDamage, float TearRate, int playerMaxHealth, float x_pos, float starting_y_pos, float gap, float fontSize)//to jakos lepiej zrobic
{
	GameUI::GetInstance().DrawTextWithOutline("CHARACTER STATS:",{ x_pos,starting_y_pos }, 30);
	GameUI::GetInstance().DrawTextWithOutline("MAX HEALTH: " + ConvertToString(playerMaxHealth, 0), { x_pos,starting_y_pos + (gap) }, 30);
	GameUI::GetInstance().DrawTextWithOutline("DAMAGE: " + ConvertToString(PlayerDamage,2), { x_pos,starting_y_pos + gap*2}, 30);
	GameUI::GetInstance().DrawTextWithOutline("SPEED: " + ConvertToString(PlayerSpeed, 2), { x_pos,starting_y_pos + (gap *3) }, 30);
	GameUI::GetInstance().DrawTextWithOutline("TEAR RATE: " + ConvertToString(TearRate, 2), { x_pos,starting_y_pos + (gap *4) }, 30);
	GameUI::GetInstance().DrawTextWithOutline("TEAR SPEED: " + ConvertToString(TearSpeed, 2), { x_pos,starting_y_pos + (gap *5) }, 30);
}
void GameUI::DrawCharacterStatsInMenu(int pageNumber, Rectangle bar, float fontSize, float y_position)
{
	float gap = 30;
	map<string, characterStats>stats = CharacterData::GetAllStats();
	string characterName;
	switch (pageNumber)
	{
	case 0:
		characterName = "FirstCharacter";
		break;
	case -1:
		characterName = "SecondCharacter";
		break;
	case 1:
		characterName = "ThirdCharacter";
		break;
	default:
		characterName = "FirstCharacter";
		break;
	}
	string maxHealthInfo = "MAX HEALTH: " + ConvertToString(stats[characterName].maxPlayerHealth, 0);
	string DamageInfo = "DAMAGE: " + ConvertToString(stats[characterName].playerDamage, 2);
	string SpeedInfo = "SPEED: " + ConvertToString(stats[characterName].playerSpeed, 2);
	string TearRateInfo = "TEAR RATE: " + ConvertToString(stats[characterName].tearRate, 2);
	string TearSpeedInfo = "TEAR SPEED: " + ConvertToString(stats[characterName].tearSpeed, 2);
	GameUI::GetInstance().DrawTextOnBar(bar, fontSize, maxHealthInfo, y_position);
	GameUI::GetInstance().DrawTextOnBar(bar, fontSize, DamageInfo, y_position+gap);
	GameUI::GetInstance().DrawTextOnBar(bar, fontSize, SpeedInfo, y_position+gap*2);
	GameUI::GetInstance().DrawTextOnBar(bar, fontSize, TearRateInfo, y_position+gap*3);
	GameUI::GetInstance().DrawTextOnBar(bar, fontSize, TearSpeedInfo, y_position+gap*4);
}
void GameUI::DrawWaveNumber(int waveNumber, float fontSize)
{
	GameUI::GetInstance().DrawTextWithOutline("WAVE: " + ConvertToString((float)waveNumber, 0), {(float)GetScreenWidth()-325,42.5,},fontSize);
}//tutaj zmienic to -325 na measureText?
void GameUI::DrawTextOnBar(Rectangle bar, float fontSize, const string& text, float y_position)
{
	float barBorders = 10;
	float lineWidthScale = 0.9;
	float minFontSize = 20;
	float gap = 2;

	vector<string> lines;
	bool textFits = false;
	float lineHeight;
	while (fontSize >= minFontSize)
	{
		float maxLineWidth = bar.width * lineWidthScale;
		lines = GameUI::GetInstance().DivideTextIntoParts(text, fontSize, maxLineWidth);

		lineHeight = GameUI::GetInstance().MeasureText(fontSize, "y").y;
		float totalHeight = lines.size() * lineHeight + (lines.size() - 1) * 2;

		if (y_position + totalHeight <= bar.y + bar.height - barBorders)
		{
			textFits = true;
			break;
		}

		fontSize -= 1;
	}

	if (!textFits)
	{
		float maxLineWidth = bar.width * lineWidthScale;
		lines = GameUI::GetInstance().DivideTextIntoParts(text, fontSize, maxLineWidth);
	}
	for (const string& line : lines)
	{
		Vector2 lineSize = GameUI::GetInstance().MeasureText(fontSize, line);
		float textX = bar.x + (bar.width - lineSize.x) / 2;
		GameUI::GetInstance().DrawTextWithOutline(line, { textX, y_position }, fontSize);
		y_position += lineHeight + gap;
	}
}
vector<string> GameUI::DivideTextIntoParts(const string& text, float fontSize, float maxWidth)
{
	vector<string> result;
	istringstream iss(text);
	string word, line;

	while (iss >> word)
	{
		string testLine;
		if (line.empty())
		{
			testLine = word;
		}
		else
		{
			testLine = line + " " + word;
		}
		Vector2 testSize = GameUI::GetInstance().MeasureText(fontSize, testLine);

		if (testSize.x <= maxWidth)
		{
			line = testLine;
		}
		else
		{
			if (!line.empty())
			{
				result.push_back(line);
				line = word;
			}
		}
	}

	if (!line.empty())
	{
		result.push_back(line);
	}
	return result;
}
