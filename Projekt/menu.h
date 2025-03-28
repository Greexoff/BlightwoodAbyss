#pragma once

#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

class Menu
{
public:
	Menu();
	void DrawLoginMenu();
	void insertUsername();
	void insertPassword();
	void setLoginMenuActive(bool value);
	void DrawUsername();
	void DrawPassword();
	bool getLoginMenuActive();
	int isButtonClicked();
	
private:
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR};
	bool LoginMenu_active;
	Texture2D LoginMenu_background;
	Rectangle LoginMenu_ConfirmArea;
	Rectangle LoginMenu_UsernameBarArea;
	Rectangle LoginMenu_PasswordBarArea;
	string username;
	string password;
	Font font = LoadFontEx("arial.ttf", 64, 0, 0);//zmienic na tego bahseita czy jakos tak
};