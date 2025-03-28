#include "menu.h"

Menu::Menu()
{
	LoginMenu_active = true;
	LoginMenu_background=LoadTexture("backgroundLOGIN.png");
	LoginMenu_ConfirmArea = { 480,548,226,103 };
	LoginMenu_UsernameBarArea = {296, 185, 594, 92};
	LoginMenu_PasswordBarArea = {296, 413, 594, 92};
	username = "";
	password = "";
	fontsize = 55;
}
void Menu::DrawLoginMenu()
{
	DrawTexture(LoginMenu_background, 0, 0, WHITE);
}
int Menu::isButtonClicked()
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
	return NOTHING;
}
void Menu::setLoginMenuActive(bool value)
{
	LoginMenu_active = value;
}
bool Menu::getLoginMenuActive()
{
	return LoginMenu_active;
}
void Menu::insertData(int setAction)
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
void Menu::DrawUsername()
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

void Menu::DrawPassword()
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