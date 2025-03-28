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
void Menu::insertUsername()
{
	
	int maxChars = 16;
	int key = GetCharPressed(); // Pobiera kod znaku, jeśli jakiś został naciśnięty
	while (key > 0) {
		if (username.length() < maxChars && key >= 32 && key <= 125) {
			username += (char)key;  // Dodajemy literę do napisu
		}
		key = GetCharPressed(); // Pobierz kolejną literę (jeśli użytkownik nacisnął kilka na raz)
	}

	// Obsługa klawisza Backspace
	if (IsKeyPressed(KEY_BACKSPACE) && !username.empty()) {
		username.pop_back();
	}
	DrawText(username.c_str(), 260, 265, 25, WHITE);
}
void Menu::DrawUsername()
{
	DrawText(username.c_str(), 260, 265, 25, WHITE);
}
void Menu::DrawPassword()
{
	DrawText(password.c_str(), 260, 265, 25, WHITE);
}
void Menu::insertPassword()
{

}