#pragma once
#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

enum class CurrentState
{
	STARTING_MENU,
	LOGIN_MENU,
	MAIN_MENU,
	CHARACTER_SELECT_MENU,
	UNLOCKED_ITEMS_MENU,
	SCORE_MENU,
	GAMEPLAY,
};
class Menu
{
public:
	Menu();
	~Menu();
	void DrawMenu();
	void switchMenuBackground(const char* background_file_name);
	int isButtonClicked();
	void handleMainMenuLogic(int& setAction, CurrentState& gameState, bool shouldEnd);
protected:
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
	void handleLoginMenuLogic(int& setAction, CurrentState& gameState);
private:
	void insertData(string& name);
	void DrawLogin(string name, string location);
	bool checkIsLoginCorrect();
	int isButtonClicked();
	bool isSignUpCardActive();
	bool checkIsPlayerInDataBase();
	void addPlayerToDataBase();
	void clearUsernameandPassword();
	void changeSignBarLevel(bool value);
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };
	Rectangle LoginMenu_ConfirmArea;
	Rectangle LoginMenu_UsernameBarArea;
	Rectangle LoginMenu_PasswordBarArea;
	Rectangle LoginMenu_SingupArea;
	bool areBarAreasActive;
	bool isSignupAreaActive;
	string username;
	string password;
	bool userExist;
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
	int getPageNumber();
	bool isButtonClicked();

private:
	enum Pressed { NOTHING, CONFIRM_BUTTON, ARROW_LEFT, ARROW_RIGHT };
	Vector2 ArrowLeft_p1;
	Vector2 ArrowLeft_p2;
	Vector2 ArrowLeft_p3;
	Vector2 ArrowRight_p1; 
	Vector2 ArrowRight_p2;
	Vector2 ArrowRight_p3;
	Rectangle ConfirmArea;
	int pageNumber;
	int leftSidePageLimit;
	int rightSidePageLimit;

};
class UnlockedItemsMenu : public Menu
{
public:
	UnlockedItemsMenu();
	~UnlockedItemsMenu();
private:
};
class HighestScoreMenu : public Menu
{
public:
	HighestScoreMenu();
	~HighestScoreMenu();
private:

};