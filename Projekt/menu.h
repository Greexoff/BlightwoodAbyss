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
	void DrawMenu();
	void switchMenuBackground(const char* background_file_name);
	int isButtonClicked();
protected:
	bool Menu_active;
	Texture2D Menu_background;
	Font font;
private:
	enum Pressed { NOTHING, NEWGAME_BUTTON, UNLOCKED_BUTTON, SCORE_BUTTON, EXIT_BUTTON};
	Rectangle Menu_NewGameButton;
	Rectangle Menu_UnlockedItemsButton;
	Rectangle Menu_HighestScoreButton;
	Rectangle Menu_Exit;
};
class LoginMenu : public Menu
{
public:
	LoginMenu();
	~LoginMenu();
	void insertData(int setAction);
	void DrawUsername();
	void DrawPassword();
	bool checkIsLoginCorrect();
	int isButtonClicked();
	bool isSignUpCardActive();
	bool checkIsPlayerInDataBase();
	void addPlayerToDataBase();
	void clearUsernameandPassword();
	void changeSignBarLevel(bool value);

private:
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };
	Rectangle LoginMenu_ConfirmArea;
	Rectangle LoginMenu_UsernameBarArea;
	Rectangle LoginMenu_PasswordBarArea;
	Rectangle LoginMenu_SingupArea;
	bool areBarAreasActive;
	bool isSignupAreaActive;
	string username;
	string password;
	int fontsize;
};
class StartingMenu : public Menu
{
public:
	StartingMenu();
	~StartingMenu();
private:
};
class CharacterSelectMenu : public Menu
{
public:
	CharacterSelectMenu();
	~CharacterSelectMenu();
};