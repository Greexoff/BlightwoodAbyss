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
	void setMenuActive(bool value);
	bool getMenuActive();
private:
	bool Menu_active;
};
class LoginMenu : public Menu
{
public:
	LoginMenu();
	void DrawLoginMenu();
	void insertData(int setAction);
	void setMenuActive(bool value);
	void DrawUsername();
	void DrawPassword();
	bool checkIsLoginCorrect();
	bool getMenuActive();
	int isButtonClicked();
	void switchLoginMenuBackground(const char* background_file_name);
	bool isSignUpCardActive();
	bool checkIsPlayerInDataBase();
	void addPlayerToDataBase();
	void clearUsernameandPassword();
	void changeSignBarLevel(bool value);

private:
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };
	bool LoginMenu_active;
	Texture2D LoginMenu_background;
	Rectangle LoginMenu_ConfirmArea;
	Rectangle LoginMenu_UsernameBarArea;
	Rectangle LoginMenu_PasswordBarArea;
	Rectangle LoginMenu_SingupArea;
	bool areBarAreasActive;
	bool isSignupAreaActive;
	string username;
	string password;
	int fontsize;
	Font font = LoadFontEx("arial.ttf", 64, 0, 0);//zmienic na tego bahseita czy jakos tak
};