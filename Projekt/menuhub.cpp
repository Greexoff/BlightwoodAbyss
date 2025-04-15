#include "menuhub.h"


Menu::~Menu()
{
	UnloadFont(font);
}
void Menu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
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
		cout << "Kliknieto powrot" << endl;
		return true;
	}
	return false;
}

StartingMenu::StartingMenu()
{
	LoadTextures("backgroundSTARTING.png");
}
StartingMenu::~StartingMenu()
{
	UnloadTexture(BackgroundTexture);
}

LoginMenu::LoginMenu()
{
	LoadTextures("backgroundLOGIN.png");
	LoginMenu_ConfirmArea = { 520,796,883-520,975-796 };
	LoginMenu_UsernameBarArea = { 382, 216, 1032-382, 354-216 };
	LoginMenu_PasswordBarArea = { 382, 495, 1032-382, 633-495 };
	LoginMenu_SingupArea = {1050,807,1318-1050,975 -807 };
	username = "";
	password = "";
	userExist = false;
	fontsize = 80;
	isSignupAreaActive = true;
}
LoginMenu::~LoginMenu()
{
	UnloadTexture(BackgroundTexture);
}
int LoginMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, LoginMenu_ConfirmArea))
	{
		cout << "Kliknieto przycisk" << endl;
		return CONFIRM_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_UsernameBarArea))
	{
		cout << "Kilknieto username" << endl;
		return USERNAME_BAR;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_PasswordBarArea))
	{
		cout << "Kliknieto password" << endl;
		return PASSWORD_BAR;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_SingupArea) && isSignupAreaActive)
	{
		cout << "Kliknieto Sign-up" << endl;
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
void LoginMenu::DrawLogin(string name, string location)
{
	int maxFontSize = 80;
	int minFontSize = 20;
	int spacing = 2;
	int position = 0;
	if (location == "upper")
	{
		position = 290;
	}
	if (location == "lower")
	{
		position = 570;
	}
	Vector2 textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
	while (textSize.x > LoginMenu_UsernameBarArea.width-40 && fontsize > minFontSize)
	{
		fontsize -= 1;
		textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
	}
	while (fontsize == minFontSize && textSize.x > LoginMenu_UsernameBarArea.width - 40 && !name.empty())
	{
		name.pop_back();
		textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
	}
	while (textSize.x < LoginMenu_UsernameBarArea.width-40 && fontsize < maxFontSize)
	{
		fontsize += 1;
		textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
		if (textSize.x > LoginMenu_UsernameBarArea.width - 40)
		{
			fontsize -= 1;
			break;
		}
	}
	float y_position = position - (textSize.y / 2);

	DrawTextEx(font, name.c_str(), { LoginMenu_UsernameBarArea.x+30, y_position }, fontsize, spacing, WHITE);
}
bool LoginMenu::checkIsLoginCorrect()
{
	regex username_regex("^[a-zA-Z0-9]{3,15}$");
	regex password_regex("^[a-zA-Z0-9]{4,20}$");
	if (regex_match(username, username_regex) && regex_match(password, password_regex))
	{
		return true;
	}
	else
	{
		cout << "Niepoprawne dane" << endl;
		return false;
	}
}
void LoginMenu::changeSignBarLevel(bool value)
{
	isSignupAreaActive = value;
}
bool LoginMenu::isSignUpCardActive()
{
	if (isSignupAreaActive)
	{
		return true;
	}
	return false;
}
bool LoginMenu::checkIsPlayerInDataBase()
{
	ifstream file(data_basePath.string());
	if (!file.is_open())
	{
		cout << "Nie mozna otworzyc pliku" << endl;
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
		cout << "Nie mozna otworzyc pliku" << endl;
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
		}

		userValid = checkIsPlayerInDataBase();

		if (isSignUpCardActive()) // Logowanie
		{
			if (userValid)
			{
				cout << "Uzytkownik zalogowany pomyslnie" << endl;
				gameState = CurrentState::MAIN_MENU;
			}
			else
			{
				if (userExist)
				{
					cout << "Bledne haslo" << endl;
				}
				else
				{
					cout << "Nie ma uzytkownika o podanej nazwie" << endl;
				}
			}
		}
		else // Rejestracja
		{
			if (userExist)
			{
				cout << "Juz istnieje uzytkownik o takiej nazwie" << endl;
			}
			else
			{
				addPlayerToDataBase();
				cout << "Rejestracja zakonczona sukcesem" << endl;
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
		LoadTextures("backgroundSIGNUP.png");
		changeSignBarLevel(false);
		setAction = 0;
		break;
	default:
		break;
	}
	DrawLogin(username, "upper");
	DrawLogin(password, "lower");
}

MainMenu::MainMenu()
{
	LoadTextures("backgroundMENU.png");
	Menu_NewGameButton = { 470, 120, 952 - 470, 216 - 120 };
	Menu_RulesButton = { 410,276,1017 - 410,372 - 276 };
	Menu_UnlockedItemsButton = { 410, 429, 1017 - 410, 525 - 429 };
	Menu_HighestScoreButton = { 260,582, 1167-260, 678-582 };
	Menu_Exit = { 598, 735, 840-598, 831-735 };

}
MainMenu::~MainMenu()
{
	UnloadTexture(BackgroundTexture);
}
int MainMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, Menu_NewGameButton))
	{
		cout << "Kliknieto nowa giera" << endl;
		return NEWGAME_BUTTON;
	}
	
	if(CheckCollisionPointRec(mousePos, Menu_RulesButton))
	{
		cout << "Kliknieto rules" << endl;
		return RULES_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Menu_UnlockedItemsButton))
	{
		cout << "Kilnieto unlocked items" << endl;
		return UNLOCKED_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Menu_HighestScoreButton))
	{
		cout << "Kliknieto Highest Score" << endl;
		return SCORE_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Menu_Exit))
	{
		cout << "Kliknieto Exit" << endl;
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
	pageNumber = 0;
	leftSidePageLimit = -1;
	rightSidePageLimit = 1;
}
CharacterSelectionMenu::~CharacterSelectionMenu()
{
	UnloadTexture(BackgroundTexture);
	for (auto& i : loadedComms)
	{
		UnloadTexture(i.second);
	}
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
void CharacterSelectionMenu::DrawComments(Texture2D texture)
{
	DrawTextureEx(texture, { 1035, 100 }, 0, 1, WHITE);
}
void CharacterSelectionMenu::isButtonClicked(CurrentState& gameState)
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, ConfirmArea))
	{
		cout << "Kliknieto przycisk" << endl;
		gameState = CurrentState::GAMEPLAY;
	}
	if (CheckCollisionPointRec(mousePos, ArrowArea))
	{
		cout << "Kliknieto RightArrow" << endl;
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
void CharacterSelectionMenu::showExplanations()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, ConfirmArea))
	{
		cout << "Kliknieto przycisk" << endl;
		DrawComments(passCorrectTexture("NewGameInfo.png"));
	}
	if (CheckCollisionPointRec(mousePos, ArrowArea))
	{
		cout<<"Tutaj wybor postaci"<<endl;
		DrawComments(passCorrectTexture("SwitchInfo.png"));
	}
	if (CheckCollisionPointRec(mousePos, CharacterInformationArea))
	{
		cout << "Tu statsy postaci" << endl;
		DrawComments(passCorrectTexture("CharacterInfo.png"));
	}
	if (CheckCollisionPointRec(mousePos, ReturnArea))
	{
		cout << "Tu jest powrot" << endl;
		DrawComments(passCorrectTexture("ReturnInfo.png"));
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

UnlockedItemsMenu::UnlockedItemsMenu()
{
	LoadTextures("backgroundOPTIONS.png");
}
UnlockedItemsMenu::~UnlockedItemsMenu()
{
	UnloadTexture(BackgroundTexture);
}

HighestScoreMenu::HighestScoreMenu()
{
	LoadTextures("backgroundOPTIONS.png");
}
HighestScoreMenu::~HighestScoreMenu()
{
	UnloadTexture(BackgroundTexture);
}