#pragma once
#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
using namespace std;
namespace fs = filesystem;

enum class CurrentState
{
	STARTING_MENU,
	LOGIN_MENU,
	MAIN_MENU,
	CHARACTER_SELECT_MENU,
	RULES_MENU,
	UNLOCKED_ITEMS_MENU,
	SCORE_MENU,
	GAMEPLAY,
};

class Menu
{
public:
	virtual void Draw();
	virtual void LoadTextures(fs::path filePath);
	virtual bool isReturnButtonClicked();
	virtual ~Menu();
protected:
	fs::path background_assets_path = fs::current_path() / "assets" / "background_assets";
	fs::path fontPath = fs::current_path() / "assets" / "font_assets"/ "VT323.ttf";
	Font font = LoadFontEx(fontPath.string().c_str(), 80, 0, 0);
	Texture2D BackgroundTexture; 
	Rectangle ReturnToPrieviousMenuButton = { 292,810,488-292,1005 - 810 };
};
class StartingMenu : public Menu
{
public:
	StartingMenu();
	~StartingMenu();
private:
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
	fs::path data_basePath = fs::current_path() / "DataBase.txt";
	bool isSignupAreaActive;
	string username;
	string password;
	bool userExist;
	int fontsize;
};
class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();
	void handleMainMenuLogic(int& setAction, CurrentState& gameState, bool& shouldEnd);
private:
	enum Pressed { NOTHING, NEWGAME_BUTTON, RULES_BUTTON, UNLOCKED_BUTTON, SCORE_BUTTON, EXIT_BUTTON };
	Rectangle Menu_NewGameButton;
	Rectangle Menu_RulesButton;
	Rectangle Menu_UnlockedItemsButton;
	Rectangle Menu_HighestScoreButton;
	Rectangle Menu_Exit;
	int isButtonClicked();
};
class CharacterSelectionMenu : public Menu
{
public:
	CharacterSelectionMenu();
	~CharacterSelectionMenu();
	int getPageNumber();
	void showExplanations();
	void isButtonClicked(CurrentState& gameState);
private:
	fs::path commsAssetsPath;
	void LoadCommsTextures();
	map<string, Texture2D> loadedComms;
	Texture2D passCorrectTexture(string textureName);
	void DrawComments(Texture2D texture);
	Rectangle ArrowArea;
	Rectangle ConfirmArea;
	Rectangle CharacterInformationArea;
	Rectangle ReturnArea;
	int pageNumber;
	int leftSidePageLimit;
	int rightSidePageLimit;
};
class RulesMenu : public Menu
{
public:
	RulesMenu();
	~RulesMenu();
private:
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