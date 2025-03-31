#include "menu.h"

Menu::Menu()
{
	Menu_background = LoadTexture("backgroundMENU.png");
	font = LoadFontEx("bahnschrift.ttf", 55, 0, 0);
	Menu_NewGameButton = {434, 123, 761-434, 198-123};
	Menu_UnlockedItemsButton = { 365, 267, 848 - 365, 322 - 267 };
	Menu_HighestScoreButton = {384, 401, 822-384, 480-401};
	Menu_Exit = {538, 543, 653-538, 598-543};
}
Menu::~Menu()
{
	UnloadTexture(Menu_background);
	UnloadFont(font);
}
void Menu::DrawMenu()
{
	DrawTexture(Menu_background, 0, 0, WHITE);
}
void Menu::switchMenuBackground(const char* background_file_name)
{
	Menu_background = LoadTexture(background_file_name);
}
int Menu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, Menu_NewGameButton))
	{
		cout << "Kliknieto nowa giera" << endl;
		return NEWGAME_BUTTON;
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
void Menu::handleMainMenuLogic(int& setAction, CurrentState& gameState, bool& shouldEnd)
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
			gameState = CurrentState::UNLOCKED_ITEMS_MENU;
			break;
		case 3:
			gameState = CurrentState::SCORE_MENU;
			break;
		case 4:
			shouldEnd = true;
			break;
		default:
			break;
		}
		setAction = 0;
}

LoginMenu::LoginMenu()
{
	areBarAreasActive = true;
	Menu_background=LoadTexture("backgroundLOGIN.png");
	LoginMenu_ConfirmArea = { 480,548,226,103 };
	LoginMenu_UsernameBarArea = {296, 185, 594, 92};
	LoginMenu_PasswordBarArea = {296, 413, 594, 92};
	LoginMenu_SingupArea = { 537,678,649-537, 705- 678 };
	username = "";
	password = "";
	userExist = false;
	fontsize = 55;
	isSignupAreaActive = true;
}
LoginMenu::~LoginMenu()
{
	UnloadTexture(Menu_background);
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
	int maxFontSize = 55;
	int minFontSize = 10;
	int spacing = 2;
	int position=0;
	if (location == "upper")
	{
		position = 235;
	}
	if (location == "lower")
	{
		position = 465;
	}
		Vector2 textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
		while (textSize.x > 545 && fontsize > minFontSize)
		{
			fontsize -= 1;
			textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
		}
		while (textSize.x < 545 && fontsize < maxFontSize)
		{
			fontsize += 1;
			textSize = MeasureTextEx(font, name.c_str(), fontsize, spacing);
			if (textSize.x > 545)
			{
				fontsize -= 1;
				break;
			}
		}
		float y_position = position - (textSize.y / 2);

		DrawTextEx(font, name.c_str(), { 320, y_position }, fontsize, spacing, WHITE);
}
bool LoginMenu::checkIsLoginCorrect()
{
	regex username_regex("^[a-zA-Z0-9]{3,15}$");
	regex password_regex("^[a-zA-Z0-9]{4,20}$");
	if (regex_match(username, username_regex) && regex_match(password,password_regex))
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
	ifstream file("DataBase.txt");
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
	ofstream file("DataBase.txt", ios::app);
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
		if (!checkIsLoginCorrect()) return;

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
		switchMenuBackground("backgroundSIGNUP.png");
		changeSignBarLevel(false);
		setAction = 0;
		break;
	default:
		break;
	}
	DrawLogin(username, "upper");
	DrawLogin(password, "lower");
}

StartingMenu::StartingMenu()
{
	Menu_background = LoadTexture("backgroundSTARTING.png");
}
StartingMenu::~StartingMenu()
{
	UnloadTexture(Menu_background);
}	 

CharacterSelectMenu::CharacterSelectMenu()
{
	Menu_background = LoadTexture("backgroundCHAR.png");
	ArrowLeft_p1 = { 68, 232 };
	ArrowLeft_p2= { 308, 58 };
	ArrowLeft_p3= { 308, 405 };
	ArrowRight_p1= { 1100, 237 };
	ArrowRight_p2= { 859, 65 };
	ArrowRight_p3= { 859, 412 };
	ConfirmArea={ 480,548,226,103 };
	pageNumber = 0;
	leftSidePageLimit = -1;
	rightSidePageLimit = 1;
}
CharacterSelectMenu::~CharacterSelectMenu()
{
	UnloadTexture(Menu_background);
}
bool CharacterSelectMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos,ConfirmArea))
	{
		cout << "Kliknieto przycisk" << endl;
		return true;
	}
	if (CheckCollisionPointTriangle(mousePos, ArrowLeft_p1, ArrowLeft_p2, ArrowLeft_p3))
	{
		cout << "Kilknieto LeftArrow" << endl;
		pageNumber--;
		if (pageNumber < leftSidePageLimit)
		{
			pageNumber = rightSidePageLimit;
		}
	}
	if (CheckCollisionPointTriangle(mousePos, ArrowRight_p1, ArrowRight_p2, ArrowRight_p3))
	{
		cout << "Kliknieto RightArrow" << endl;
		pageNumber++;
		if (pageNumber > rightSidePageLimit)
		{
			pageNumber = leftSidePageLimit;
		}
	}

	return false;
}
int CharacterSelectMenu::getPageNumber()
{
	return pageNumber;
}

UnlockedItemsMenu::UnlockedItemsMenu()
{
	Menu_background = LoadTexture("backgroundUNLOCKED.png");
}
UnlockedItemsMenu::~UnlockedItemsMenu()
{
	UnloadTexture(Menu_background);
}

HighestScoreMenu::HighestScoreMenu()
{
	Menu_background = LoadTexture("backgroundSCORE.png");
	playerScoresSaved = false;
}
HighestScoreMenu::~HighestScoreMenu()
{
	UnloadTexture(Menu_background);
}
void HighestScoreMenu::handleScoreMenu()
{
	if (!playerScoresSaved)
	{
		getPlayerScores();
	}
	DrawPlayerScores();
}
void HighestScoreMenu::getPlayerScores()
{
	ifstream file("DataBase.txt");
	string line;
	regex filePattern(R"(([^,]+),([^,]+),Highest Score: (\d+))");
	smatch match;

	if (!file) {
		cout << "Nie mozna otworzyc pliku" << endl;
	}

	while (getline(file, line)) {
		if (regex_match(line, match, filePattern)) {
			string username = match[1];
			int highestScore = stoi(match[3]);
			playerScores.insert({ highestScore,username });
		}
	}
	playerScoresSaved = true;
}
void HighestScoreMenu::DrawPlayerScores()
{
}

