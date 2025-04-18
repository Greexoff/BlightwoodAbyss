#include "menuhub.h"


Menu::~Menu()
{
}
void Menu::LoadTextures(fs::path filePath)
{
	fs::path wholePath = background_assets_path / filePath;
	BackgroundTexture=LoadTexture(wholePath.string().c_str());
}
bool Menu::isReturnButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, ReturnToPrieviousMenuButton))
	{
		return true;
	}
	return false;
}

StartingMenu::StartingMenu()
{
	LoadTextures("backgroundSTARTING.png");
	titleFontSize = 200;
	titleName = "BLIGHTWOOD ABBYS";
	setTitlePosition();
}
StartingMenu::~StartingMenu()
{
	UnloadTexture(BackgroundTexture);
}

void StartingMenu::setTitlePosition()
{
	Vector2 measurements = GameUI::GetInstance().MeasureTextBar(titleFontSize, titleName.c_str());
	titleNamePosition = { (GetScreenWidth() / 2) - (measurements.x / 2),(GetScreenHeight() / 4) - (measurements.y / 2) };

}
void StartingMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
	GameUI::GetInstance().DrawTextWithOutline(titleName, titleNamePosition, titleFontSize);
}

LoginMenu::LoginMenu()
{
	data_basePath = fs::current_path() / "database"/"DataBase.txt";
	LoadTextures("backgroundSTARTING.png");
	username = "";
	password = "";
	UsernameTextFontSize = 150;
	PasswordTextFontSize = 150;
	ConfirmTextFontSize = 75;
	SignupTextFontSize=70;
	setXYofTexts();
	setBarAreas();
	userExist = false;
	fontsize = 80;
	isSignupAreaActive = true;
	errorType = 0;
}
LoginMenu::~LoginMenu()
{
	UnloadTexture(BackgroundTexture);
}
void LoginMenu::setXYofTexts()
{
	ConfirmPosition = { 575,850 };
	UsernamePosition = { 450,76 };
	PasswordPosition = { 450,420 };
	SignupPosition = { 1095,855 };
}
void LoginMenu::setBarAreas()
{
	LoginMenu_UsernameBarArea=GameUI::GetInstance().setBarArea(UsernameTextFontSize, "USERNAME", UsernamePosition, 2, 30, 30);
	LoginMenu_PasswordBarArea=	GameUI::GetInstance().setBarArea(PasswordTextFontSize, "PASSWORD", PasswordPosition, 2, 30, 30);
	LoginMenu_ConfirmArea=GameUI::GetInstance().setBarArea(ConfirmTextFontSize, "CONFIRM", ConfirmPosition, 1, 30, 30);
	LoginMenu_SingupArea=GameUI::GetInstance().setBarArea(SignupTextFontSize, "SIGNUP", SignupPosition, 1, 30, 30);
}
void LoginMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_UsernameBarArea, 160);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_PasswordBarArea, 160);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_ConfirmArea, 160);
	GameUI::GetInstance().DrawTextWithOutline("USERNAME", UsernamePosition, UsernameTextFontSize);
	GameUI::GetInstance().DrawTextWithOutline("PASSWORD", PasswordPosition, PasswordTextFontSize);
	GameUI::GetInstance().DrawTextWithOutline("CONFIRM", ConfirmPosition, ConfirmTextFontSize);
	if (isSignupAreaActive)
	{
		GameUI::GetInstance().DrawBlackBar(LoginMenu_SingupArea, 160);
		GameUI::GetInstance().DrawTextWithOutline("SIGNUP", SignupPosition, SignupTextFontSize);
	}
}
int LoginMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, LoginMenu_ConfirmArea))
	{
		return CONFIRM_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_UsernameBarArea))
	{
		return USERNAME_BAR;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_PasswordBarArea))
	{;
		return PASSWORD_BAR;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_SingupArea) && isSignupAreaActive)
	{
		return SIGNUP_BAR;
	}
	return NOTHING;
}
void LoginMenu::insertData(string& name)
{
	int key = GetCharPressed();
	while (key > 0) {
		if (key >= 32 && key <= 125) {
			name += (char)key;
		}
		key = GetCharPressed();
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !name.empty() || IsKeyPressedRepeat(KEY_BACKSPACE) && !name.empty()) {
		name.pop_back();
	}
}
void LoginMenu::DrawLogin(string& name, int type)
{
	int maxFontSize = 80;
	int minFontSize = 40;
	int spacing = 2;
	int position = 0;
	Rectangle Area;

	switch (type)
	{
	case 1:
		Area = LoginMenu_UsernameBarArea;
		break;
	case 2:
		Area = LoginMenu_PasswordBarArea;
		break;
	default:
		break;
	}

	position = Area.y + (Area.height / 2);

	float textStartX = Area.x + 30;
	float maxTextEndX = Area.x + Area.width - 60;
	Area.x = textStartX;
	Vector2 textSize = GameUI::GetInstance().MeasureText(fontsize, name.c_str());
	while ((textStartX + textSize.x > maxTextEndX) && fontsize > minFontSize)
	{
		fontsize -= 1;
		textSize = GameUI::GetInstance().MeasureText(fontsize, name.c_str());
	}
	while (fontsize == minFontSize && (textStartX + textSize.x > maxTextEndX) && !name.empty())
	{
		name.pop_back();
		textSize = GameUI::GetInstance().MeasureText(fontsize, name.c_str());
	}
	while ((textStartX + textSize.x < maxTextEndX) && fontsize < maxFontSize)
	{
		fontsize += 1;
		textSize = GameUI::GetInstance().MeasureText(fontsize, name.c_str());
		if (textStartX + textSize.x > maxTextEndX)
		{
			fontsize -= 1;
			break;
		}
	}
	float y_position = position - (textSize.y / 2);
	GameUI::GetInstance().DrawTextWithOutline(name.c_str(), { textStartX, y_position }, fontsize);
}
bool LoginMenu::checkIsLoginCorrect()
{
	regex DataRegex("^[a-zA-Z0-9]{4,20}$");
	if (regex_match(username, DataRegex) && regex_match(password, DataRegex))
	{
		return true;
	}
	else
	{
		thread displayErrorThread(&LoginMenu::showLoginError, this);
		displayErrorThread.detach();
		errorType = 1;
		return false;
	}
}
void LoginMenu::changeSignBarLevel(bool value)
{
	isSignupAreaActive = value;
}
bool LoginMenu::checkIsPlayerInDataBase()
{
	ifstream file(data_basePath.string());
	if (!file.is_open())
	{
		return false;
	}
	regex userRegex(R"(^(\w+),(\w+),Highest Score:\s*(\d+)$)");
	smatch match;
	string line;
	userExist = false;

	while (getline(file, line))
	{
		if (regex_match(line, match, userRegex) && username == match[1])
		{
			userExist = true;
			if (password == match[2])
			{
				return true;
			}
			return false;
		}
	}
	return false;
}
void LoginMenu::addPlayerToDataBase()
{
	ofstream file(data_basePath.string(), ios::app);
	string line;
	if (!file.is_open())
	{
	}
	else
	{
		file << endl << username << "," << password << ",Highest Score: 000000";
	}

}
void LoginMenu::clearUsernameandPassword()
{
	username = "";
	password = "";
}
void LoginMenu::handleLoginMenuLogic(int& setAction, CurrentState& gameState)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		setAction = isButtonClicked();
	}
	bool userValid;
	switch (setAction)
	{
	case 1:
		if (!checkIsLoginCorrect())
		{
			setAction = 0;
			break;
		}

		userValid = checkIsPlayerInDataBase();

		if (isSignupAreaActive)
		{
			if (userValid)
			{
				gameState = CurrentState::MAIN_MENU;
			}
			else
			{
				if (userExist)
				{
					thread displayErrorThread(&LoginMenu::showLoginError, this);
					displayErrorThread.detach();
					errorType = 2;
				}
				else
				{
					thread displayErrorThread(&LoginMenu::showLoginError, this);
					displayErrorThread.detach();
					errorType = 3;
				}
			}
		}
		else 
		{
			if (userExist)
			{
				thread displayErrorThread(&LoginMenu::showLoginError, this);
				displayErrorThread.detach();
				errorType = 4;
			}
			else
			{
				addPlayerToDataBase();
				gameState = CurrentState::MAIN_MENU;
			}
		}
		setAction = 0;
		break;
	case 2:
		insertData(username);
		break;
	case 3:
		insertData(password);
		break;
	case 4:
		clearUsernameandPassword();
		changeSignBarLevel(false);
		setAction = 0;
		break;
	default:
		break;
	}
	if (showError)
	{
		DrawError();
	}
	DrawLogin(username, 1);
	DrawLogin(password, 2);
}
void LoginMenu::showLoginError()
{
	errorStartTime = GetTime();
	showError = true;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	showError = false;
}
void LoginMenu::DrawingErrorSettingUp(string information)
{
	float fontSize = 50;
	float spacing = 2;
	Vector2 textSize = GameUI::GetInstance().MeasureText(fontSize, information.c_str());
	float barCenterX= LoginMenu_PasswordBarArea.x + LoginMenu_PasswordBarArea.width / 2.0;
	float textX = barCenterX - textSize.x / 2.0;
	GameUI::GetInstance().DrawTextWithOutline(information.c_str(), { textX, (LoginMenu_PasswordBarArea.y + LoginMenu_PasswordBarArea.height+30) }, fontSize);
}
void LoginMenu::DrawError()
{
	switch (errorType)
	{
	case 1://Nie pasuje do regexa
		DrawingErrorSettingUp("Error: Data does not meet criteria");
		break;
	case 2://Bledne Haslo
		DrawingErrorSettingUp("Error: Password is invalid");
		break;
	case 3://Nie ma uzytkownika o podanej nazwie
		DrawingErrorSettingUp("Error: Username is invalid");
		break;
	case 4://Juz istnieje taki uzytkownik
		DrawingErrorSettingUp("Error: Username is already in use");
		break;
	default:
		break;
	}
}

MainMenu::MainMenu()
{
	LoadTextures("backgroundSTARTING.png");
	ButtonNames = { "New Game", "Game Rules", "Collection","Highest Scores","Exit" };
	buttonsFontSize = 120;
	setButtonsPosition();

}
MainMenu::~MainMenu()
{
	UnloadTexture(BackgroundTexture);
}
void MainMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
	for (const auto& it : Buttons)
	{
		GameUI::GetInstance().DrawTextWithOutline(it.first, it.second.position, buttonsFontSize);
	}
}
void MainMenu::setButtonsPosition()
{
	float spacing = GetScreenHeight() / 8;
	for (size_t i=0;i<ButtonNames.size();i++)
	{
		const string& name = ButtonNames[i];
		Vector2 measurements = GameUI::GetInstance().MeasureTextBar(buttonsFontSize, ButtonNames[i].c_str());
		Vector2 buttonPosition = { (GetScreenWidth() / 2) - (measurements.x / 2),(GetScreenHeight() / 6 + i *spacing) - (measurements.y / 2) };
		Rectangle rect=GameUI::GetInstance().setBarArea(buttonsFontSize, ButtonNames[i], buttonPosition, 1,30, 30);
		Buttons[name] = {rect, buttonPosition};
	}
}
int MainMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, Buttons["New Game"].rectangle))
	{
		return NEWGAME_BUTTON;
	}
	if(CheckCollisionPointRec(mousePos, Buttons["Game Rules"].rectangle))
	{
		return RULES_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Buttons["Collection"].rectangle))
	{
		return UNLOCKED_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Buttons["Highest Scores"].rectangle))
	{
		return SCORE_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Buttons["Exit"].rectangle))
	{
		return EXIT_BUTTON;
	}
	return NOTHING;
}
void MainMenu::handleMainMenuLogic(int& setAction, CurrentState& gameState, bool& shouldEnd)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		setAction = isButtonClicked();
	}
	switch (setAction)
	{
	case 1:
		gameState = CurrentState::CHARACTER_SELECT_MENU;
		break;
	case 2:
		gameState = CurrentState::RULES_MENU;
		break;
	case 3:
		gameState = CurrentState::UNLOCKED_ITEMS_MENU;
		break;
	case 4:
		gameState = CurrentState::SCORE_MENU;
		break;
	case 5:
		shouldEnd = true;
		break;
	default:
		break;
	}
	setAction = 0;
}

CharacterSelectionMenu::CharacterSelectionMenu()
{
	LoadTextures("backgroundCHAR.png");
	commsAssetsPath= fs::current_path() / "assets" / "comments_assets";
	LoadCommsTextures();
	ArrowArea = {66,593,219-66,701-593};
	ConfirmArea = { 628,541,796-628,778-541 };
	CharacterInformationArea = {620,294,790-620,473-294};
	ReturnArea = {0,0,100,100};
	SmallerCommentsBar = {1035,100,300,300};
	BiggerCommentsBar = { 1035,100,300,500 };
	pageNumber = 0;
	leftSidePageLimit = -1;
	rightSidePageLimit = 1;
}//ZROBIC OD NOWA TE COMMENTSY TAK ZEBY DZIALALY
CharacterSelectionMenu::~CharacterSelectionMenu()
{
	UnloadTexture(BackgroundTexture);
	for (auto& i : loadedComms)
	{
		UnloadTexture(i.second);
	}
}
void CharacterSelectionMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
}
void CharacterSelectionMenu::LoadCommsTextures()
{
	for (const auto& textureName : fs::directory_iterator(commsAssetsPath))
	{
		loadedComms.insert({ textureName.path().filename().string(), LoadTexture(textureName.path().string().c_str()) });
	}
}
Texture2D CharacterSelectionMenu::passCorrectTexture(string textureName)
{
	for (auto& checking : loadedComms)
	{
		if (checking.first == textureName)
		{
			return checking.second;
		}
	}
}
void CharacterSelectionMenu::DrawComments(CommentType type)
{
	switch (type)
	{
	case SWITCH_CHARACTER:
		GameUI::GetInstance().DrawBlackBar(SmallerCommentsBar, 180);
		GameUI::GetInstance().DrawTextWithOutline("SWITCH CHARACTER", { SmallerCommentsBar.x,SmallerCommentsBar.y+(SmallerCommentsBar.height / 4) }, 35);
		break;
	case SELECT_CHARACTER:
		GameUI::GetInstance().DrawBlackBar(SmallerCommentsBar, 180);
		GameUI::GetInstance().DrawTextWithOutline("SELECT CHARACTER", { SmallerCommentsBar.x,SmallerCommentsBar.y + (SmallerCommentsBar.height / 4) }, 35);
		break;
	case STATS_CHARACTER:
		GameUI::GetInstance().DrawBlackBar(BiggerCommentsBar, 180);
		GameUI::GetInstance().DrawTextWithOutline("CHARACTERS STATS", { SmallerCommentsBar.x ,SmallerCommentsBar.y + (SmallerCommentsBar.height / 4) }, 35);
		break;
	case RETURN_BUTTON:
		GameUI::GetInstance().DrawBlackBar(SmallerCommentsBar, 180);
		GameUI::GetInstance().DrawTextWithOutline("RETURN TO MENU", { SmallerCommentsBar.x,SmallerCommentsBar.y + (SmallerCommentsBar.height / 4) },35);
		break;
	default:
		break;
	}
}
void CharacterSelectionMenu::isButtonClicked(CurrentState& gameState)
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, ConfirmArea))
	{
		gameState = CurrentState::GAMEPLAY;
	}
	if (CheckCollisionPointRec(mousePos, ArrowArea))
	{
		pageNumber++;
		if (pageNumber > rightSidePageLimit)
		{
			pageNumber = leftSidePageLimit;
		}
	}
	if (CheckCollisionPointRec(mousePos, ReturnArea))
	{
		gameState = CurrentState::MAIN_MENU;
	}
}
/*void CharacterSelectionMenu::DrawCharacterStats(string characterName)
{
	CharacterData::LoadStatsOnce();
	const auto& statsMap = CharacterData::GetAllStats();

	auto it = statsMap.find(characterName);
	if (it == statsMap.end()) return;

	const characterStats& stats = it->second;

	Vector2 basePos = { 1263, 383 };
	float spacing = 42.5;
	float fontSize = 30.0;
	GameUI::GetInstance().DrawTextWithOutline(stats.playerDamage.c_str(), { basePos.x, basePos.y + 0 * spacing }, fontSize);
	GameUI::GetInstance().DrawTextWithOutline(stats.playerSpeed.c_str(), { basePos.x, basePos.y + 1 * spacing }, fontSize);
	GameUI::GetInstance().DrawTextWithOutline(stats.tearRate.c_str(), { basePos.x, basePos.y + 2 * spacing }, fontSize);
	GameUI::GetInstance().DrawTextWithOutline(stats.tearSpeed.c_str(), { basePos.x, basePos.y + 3 * spacing }, fontSize);
	GameUI::GetInstance().DrawTextWithOutline(stats.maxPlayerHealth.c_str(), { basePos.x, basePos.y + 4 * spacing }, fontSize);
}
void CharacterSelectionMenu::GetCharacterStats(int CurrentPage)
{
	switch (CurrentPage)
	{
	case 0:
		DrawCharacterStats("FirstCharacter");
		break;
	case -1:
		DrawCharacterStats("SecondCharacter");
		break;
	case 1:
		DrawCharacterStats("ThirdCharacter");
		break;
	default:
		break;
	}
}*/
void CharacterSelectionMenu::showExplanations()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, ConfirmArea))
	{
		DrawComments(CommentType::SELECT_CHARACTER);
	}
	if (CheckCollisionPointRec(mousePos, ArrowArea))
	{
		DrawComments(CommentType::SWITCH_CHARACTER);
	}
	if (CheckCollisionPointRec(mousePos, CharacterInformationArea))
	{
		DrawComments(CommentType::STATS_CHARACTER);

	}
	if (CheckCollisionPointRec(mousePos, ReturnArea))
	{
		DrawComments(CommentType::RETURN_BUTTON);
	}
}
int CharacterSelectionMenu::getPageNumber()
{
	return pageNumber;
}

RulesMenu::RulesMenu()
{
	LoadTextures("backgroundOPTIONS.png");
}
RulesMenu::~RulesMenu()
{
	UnloadTexture(BackgroundTexture);
}
void RulesMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
}

UnlockedItemsMenu::UnlockedItemsMenu()
{
	LoadTextures("backgroundOPTIONS.png");
}
UnlockedItemsMenu::~UnlockedItemsMenu()
{
	UnloadTexture(BackgroundTexture);
}
void UnlockedItemsMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
}

HighestScoreMenu::HighestScoreMenu()
{
	LoadTextures("backgroundOPTIONS.png");
}
HighestScoreMenu::~HighestScoreMenu()
{
	UnloadTexture(BackgroundTexture);
}
void HighestScoreMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
}