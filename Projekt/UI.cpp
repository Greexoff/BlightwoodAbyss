#include "UI.h"

GameUI:: GameUI()
{
	MyOrange = { 255,110,37,255};
	MyYellow = { 218,208,0,255 };
	font = &LoadingTextures::GetInstance().getFont();
}
GameUI& GameUI::GetInstance()
{
	static GameUI instance;
	return instance;
}
GameUI::~GameUI()
{
}

//|---Drawing----------------------------------------------------------------------|
void GameUI::DrawScaledBackgroundImage(Texture2D& image)
{
	ClearBackground(BLACK);
	Rectangle source = { 0,0,image.width,image.height };
	Rectangle destination = { 0,0,GetScreenWidth(),GetScreenHeight() };
	DrawTexturePro(image, source, destination, { 0,0 }, 0, WHITE);
}
void GameUI::DrawTextWithOutline(const string& text, Vector2 position, float fontSize) {
	int spacing = (int)(fontSize / 20.0 + 0.5);
	int outlineSize = (int)(fontSize / 20.0 + 0.5);
	for (float dx = -outlineSize; dx <= outlineSize; dx++) {
		for (float dy = -outlineSize; dy <= outlineSize; dy++) {
			if (dx != 0 || dy != 0) {
				Vector2 offsetPos = { position.x + dx, position.y + dy };
				DrawTextEx(*font, text.c_str(), offsetPos, fontSize, spacing, BLACK);
			}
		}
	}
	int shadowSize = (int)(fontSize / 50.0+0.5);
	Vector2 shadowPos = { position.x + shadowSize, position.y - shadowSize };
	DrawTextEx(*font, text.c_str(), shadowPos, fontSize , spacing, MyYellow);
	DrawTextEx(*font, text.c_str(), position, fontSize, spacing, MyOrange);
}
void GameUI::DrawComments(string comment1, string comment2, Rectangle bar)
{
	GameUI::GetInstance().DrawBlackBar(bar, 180);
	GameUI::GetInstance().DrawTextOnBar(bar, 62 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, comment1, bar.y + (25 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
	GameUI::GetInstance().DrawTextOnBar(bar, 40 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, comment2, bar.y + (150 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
}
void GameUI::DrawBlackBar(Rectangle borders, unsigned char opacity)
{
	DrawRectangleRounded(borders, 0.25, 16, { 0,0,0,opacity });
	DrawRectangleRoundedLinesEx(borders, 0.25, 16, 10 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, BLACK);
}
void GameUI::DrawCharacterStatsInGame(characterStats playerStats, float x_pos, float starting_y_pos, float fontSize)
{
	vector<string> statsParagraphs;
	statsParagraphs = {
		{"Character Stats:"},
		{"Current Health: " + ConvertToString(playerStats.playerHealth, 0)},
		{"Max Health: " + ConvertToString(playerStats.maxPlayerHealth, 0)},
		{"Damage: " + ConvertToString(playerStats.playerDamage,2)},
		{"Speed: " + ConvertToString(playerStats.playerSpeed, 2)},
		{"Tear Rate: " + ConvertToString(playerStats.tearRate, 2)},
		{"Tear Speed: " + ConvertToString(playerStats.tearSpeed, 2)},
	};
	for(int i =0;i<statsParagraphs.size();i++)
	{ 
		GameUI::GetInstance().DrawTextWithOutline(statsParagraphs[i], {x_pos,starting_y_pos+(fontSize*i)}, fontSize);
	}
}
void GameUI::DrawCharacterStatsInMenu(int pageNumber, Rectangle bar, float fontSize, float y_position)
{
	float gap = fontSize;
	int it = 0;
	map<string, characterStats>stats = CharactersData::getInstance().getCharacterStats();
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
	vector<string> playerInfo;
	playerInfo = {
		{"MAX HEALTH: " + ConvertToString(stats[characterName].maxPlayerHealth, 0)},
		{"DAMAGE: " + ConvertToString(stats[characterName].playerDamage, 2)},
		{"SPEED: " + ConvertToString(stats[characterName].playerSpeed, 2)},
		{"TEAR RATE: " + ConvertToString(stats[characterName].tearRate, 2)},
		{"TEAR SPEED: " + ConvertToString(stats[characterName].tearSpeed, 2)},
	};
	for (const auto& playerStats : playerInfo)
	{
		GameUI::GetInstance().DrawTextOnBar(bar, fontSize, playerStats, y_position + gap*it);
		it++;
	}
}
void GameUI::DrawEnemyStatsInMenu(string enemyName, Rectangle bar, float fontSize, float y_position)
{
	float gap = fontSize;
	int it = 0;
	map<string, enemyStats>stats = CharactersData::getInstance().getEnemyStats();
	vector<string> enemyInfo = {
		{"MAX HEALTH: " + ConvertToString(stats[enemyName].maxEnemyHealth, 2)},
		{"SPEED: " + ConvertToString(stats[enemyName].enemySpeed, 2)},
		{"TEAR SPEED: " + ConvertToString(stats[enemyName].enemyAttackSpeed, 2)},
		{"SCORE: " + ConvertToString(stats[enemyName].enemyScore, 0)},
	};
	for (const auto& enemyStat : enemyInfo)
	{
		GameUI::GetInstance().DrawTextOnBar(bar, fontSize, enemyStat, y_position + gap * it);
		it++;
	}
}
void GameUI::DrawTextOnBar(Rectangle bar, float fontSize, const string& text, float y_position)
{
	float barBorders = 10;
	float lineWidthScale = 0.9;
	float minFontSize = 10;
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

		fontSize--;
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
void GameUI::DrawTextRules(Rectangle bar, float fontSize, const string& text, float& y_position)
{
	float additionalGapAtEnd = 20 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	if (text == "GAP")
	{
		y_position += additionalGapAtEnd;
		return;
	}
	float barBorders = 10;
	float lineWidthScale = 0.9;
	float minFontSize = 10;
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

		fontSize--;
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
	y_position += additionalGapAtEnd;
}
void GameUI::DrawData(string& name, Rectangle bar, float& fontSize)
{
	int maxFontSize = 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	int minFontSize = 40 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	int spacing = 2 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	int position = bar.y + (bar.height / 2);
	float barBorder = 20 * ScreenSettings::GetInstance().getScreenResolutionFactor().x;
	float textStartX = bar.x + barBorder;
	float maxTextEndX = bar.x + bar.width - barBorder;
	Vector2 textSize = GameUI::GetInstance().MeasureText(fontSize, name.c_str());
	while ((textStartX + textSize.x > maxTextEndX) && fontSize > minFontSize)
	{
		fontSize -= 1;
		textSize = GameUI::GetInstance().MeasureText(fontSize, name.c_str());
	}
	while ((textStartX + textSize.x > maxTextEndX) && !name.empty())
	{
		name.pop_back();
		textSize = GameUI::GetInstance().MeasureText(fontSize, name.c_str());
	}
	while ((textStartX + textSize.x < maxTextEndX) && fontSize < maxFontSize)
	{
		fontSize += 1;
		textSize = GameUI::GetInstance().MeasureText(fontSize, name.c_str());
		if (textStartX + textSize.x > maxTextEndX)
		{
			fontSize -= 1;
			break;
		}
	}
	float y_position = position - (textSize.y / 2);
	GameUI::GetInstance().DrawTextWithOutline(name.c_str(), { textStartX, y_position }, fontSize);
}
void GameUI::DrawError(string information, Rectangle bar)
{
	float fontSize = 50 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	float spacing = 2;
	Vector2 textSize = GameUI::GetInstance().MeasureText(fontSize, information.c_str());
	float barCenterX = bar.x + bar.width / 2.0;
	float textX = barCenterX - textSize.x / 2.0;
	GameUI::GetInstance().DrawTextWithOutline(information.c_str(), { textX, (float)(bar.y + bar.height + GetScreenHeight()*0.02) }, fontSize);
}
void GameUI::DrawGameUI(const string& text, float fontSize, float y_pos)
{
	Vector2 measurements = GameUI::GetInstance().MeasureText(fontSize, text);
	Vector2 ui_pos = { (float)GetScreenWidth() / 2 - measurements.x / 2, y_pos-measurements.y };
	GameUI::DrawTextWithOutline(text, ui_pos, fontSize);
}

//|---Inne-------------------------------------------------------------------------|
string GameUI::ConvertToString(float number, int prec)
{
	ostringstream text;
	text << fixed << setprecision(prec) << number;
	return text.str();
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
Rectangle GameUI::setBarArea(float fontSize, string text, Vector2 textPosition, int type, float additional_x_space, float additional_y_space)
{
	Vector2 measurements = MeasureText(fontSize, text);
	switch (type)
	{
	case 1://Napis wewnatrz
		return { textPosition.x - additional_x_space,textPosition.y - additional_y_space, measurements.x + additional_x_space * 2,measurements.y + additional_y_space * 2 };
		break;
	case 2://Napis nad paskiem
		return { textPosition.x - additional_x_space,textPosition.y + measurements.y-additional_y_space, measurements.x + additional_x_space * 2,measurements.y+additional_y_space };
		break;
	default:
		return{ 0,0,0,0 };
		break;
	}
}
Vector2 GameUI::MeasureTextBar(float& fontSize, string text)
{
	Vector2 measurements = MeasureTextEx(*font, text.c_str(), fontSize, (int)((fontSize / 20.0) + 0.5));
	while (measurements.x + 30 > GetScreenWidth() || measurements.y + 30 > GetScreenHeight())
	{
		fontSize -= 1;
		measurements = MeasureTextEx(*font, text.c_str(), fontSize, (int)((fontSize / 20.0) + 0.5));
	}
	return measurements;
}
Vector2 GameUI::MeasureText(float fontSize, string text)
{
	return MeasureTextEx(*font, text.c_str(), fontSize, (int)((fontSize / 20.0) + 0.5));
}
string GameUI::CreateTextWithLeadingZerosGameUI(int number, int amountOfZeros, const string& text)
{
	string numberText = to_string(number);
	int leadingZeros = amountOfZeros - numberText.length();
	string numberWithZeros = string(leadingZeros, '0') + numberText;
	return text + numberWithZeros;
}