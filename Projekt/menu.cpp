#include "menu.h"
Menu::Menu()
{
	Menu_active = false;
}
bool Menu::getMenuActive()
{
	if (Menu_active)
	{
		return true;
	}
	return false;
}
void Menu::setMenuActive(bool value)
{
	Menu_active = value;
}
LoginMenu::LoginMenu()
{
	LoginMenu_active = true;//to bede raczej musial zmienic jak chce powitalny screen jeszcze zrobic
	areBarAreasActive = true;
	LoginMenu_background=LoadTexture("backgroundLOGIN.png");
	LoginMenu_ConfirmArea = { 480,548,226,103 };
	LoginMenu_UsernameBarArea = {296, 185, 594, 92};
	LoginMenu_PasswordBarArea = {296, 413, 594, 92};
	LoginMenu_SingupArea = { 537,678,649-537, 705- 678 };
	username = "";
	password = "";
	fontsize = 55;
	isSignupAreaActive = true;
}
void LoginMenu::DrawLoginMenu()
{
	DrawTexture(LoginMenu_background, 0, 0, WHITE);
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
void LoginMenu::setMenuActive(bool value)
{
	LoginMenu_active = value;
}
bool LoginMenu::getMenuActive()
{
	return LoginMenu_active;
}
void LoginMenu::insertData(int setAction)
{
	int key = GetCharPressed();
	if (setAction == 2)
	{
		while (key > 0) {
			if (key >= 32 && key <= 125) {
				username += (char)key;
			}
			key = GetCharPressed();
		}
		if (IsKeyPressed(KEY_BACKSPACE) && !username.empty()) {
			username.pop_back();
		}
	}
	if (setAction == 3)
	{
		while (key > 0) {
			if (key >= 32 && key <= 125) {
				password += (char)key;
			}
			key = GetCharPressed();
		}
		if (IsKeyPressed(KEY_BACKSPACE) && !password.empty()) {
			password.pop_back();
		}
	}
}
void LoginMenu::DrawUsername()
{
	int maxFontSize = 55;
	int minFontSize = 10;
	int spacing = 2;

	Vector2 textSize = MeasureTextEx(font, username.c_str(), fontsize, spacing);
	while (textSize.x > 545 && fontsize > minFontSize)
	{
		fontsize -= 1;
		textSize = MeasureTextEx(font, username.c_str(), fontsize, spacing);
	}
	while (textSize.x < 545 && fontsize < maxFontSize)
	{
		fontsize += 1;
		textSize = MeasureTextEx(font, username.c_str(), fontsize, spacing);
		if (textSize.x > 545)
		{
			fontsize -= 1;
			break;
		}
	}
	float y_position = 235 - (textSize.y / 2);

	DrawTextEx(font, username.c_str(), { 320, y_position }, fontsize, spacing, WHITE);
}
void LoginMenu::DrawPassword()
{
	int maxFontSize = 55;
	int minFontSize = 10;
	int spacing = 2;

	Vector2 textSize = MeasureTextEx(font, password.c_str(), fontsize, spacing);
	while (textSize.x > 545 && fontsize > minFontSize)
	{
		fontsize -= 1;
		textSize = MeasureTextEx(font, password.c_str(), fontsize, spacing);
	}
	while (textSize.x < 545 && fontsize < maxFontSize)
	{
		fontsize += 1;
		textSize = MeasureTextEx(font, password.c_str(), fontsize, spacing);
		if (textSize.x > 545)
		{
			fontsize -= 1;
			break;
		}
	}
	float y_position = 465 - (textSize.y / 2);

	DrawTextEx(font, password.c_str(), { 320, y_position }, fontsize, spacing, WHITE);
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
void LoginMenu::switchLoginMenuBackground(const char* background_file_name)
{
	LoginMenu_background = LoadTexture(background_file_name);
}
void LoginMenu::changeSignBarLevel(bool value)
{
	isSignupAreaActive = value;
}
bool LoginMenu::isSignUpCardActive()
{
	if (isSignupAreaActive)
	{
		return false;
	}
	return true;
}
bool LoginMenu::checkIsPlayerInDataBase()
{
	regex checkInDataBase_regex(username + "," + password);
	ifstream file("DataBase.txt");
	string line;
	if (!file.is_open())
	{
		cout << "Nie mozna otworzyc pliku" << endl;
		return false;
	}
	while (getline(file, line))
	{
		if (regex_search(line, checkInDataBase_regex))
		{
			cout << "Jest taki gracz pozdro" << endl;
			return true;
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