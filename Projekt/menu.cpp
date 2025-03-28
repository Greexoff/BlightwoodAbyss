#include "menu.h"

Menu::Menu()
{
	LoginMenu_active = true;
	LoginMenu_background=LoadTexture("backgroundLOGIN.png");
	LoginMenu_confirmArea = { 480,548,226,103 };
}
void Menu::DrawLoginMenu()
{
	DrawTexture(LoginMenu_background, 0, 0, WHITE);
}
bool Menu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, LoginMenu_confirmArea))
	{
			cout << "Kliknieto przycisk" << endl;
			return true;
	}
	return false;
}