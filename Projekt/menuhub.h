#pragma once
#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>
#include "UI.h"
import CharacterStats;
using namespace std;
namespace fs = filesystem;

enum class CurrentState
{
	LOADING,
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
	virtual void Draw()=0;
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
	void Draw() override;
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
	void Draw() override;
private:
	void setBarAreas();
	void setXYofTexts();
	void showLoginError();
	int errorType;
	atomic<bool> showError = false;
	atomic<double> errorStartTime = 0.0;
	void DrawingErrorSettingUp(string information);
	void insertData(string& name);
	void DrawError();
	void DrawLogin(string name, int type);
	bool checkIsLoginCorrect();
	int isButtonClicked();
	bool checkIsPlayerInDataBase();
	void addPlayerToDataBase();
	void clearUsernameandPassword();
	void changeSignBarLevel(bool value);
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };
	Rectangle LoginMenu_ConfirmArea, LoginMenu_UsernameBarArea, LoginMenu_PasswordBarArea, LoginMenu_SingupArea;
	Vector2 ConfirmPosition, UsernamePosition, PasswordPosition, SignupPosition;
	float UsernameTextFontSize, PasswordTextFontSize, ConfirmTextFontSize, SignupTextFontSize;
	fs::path data_basePath;
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
	void Draw() override;
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
	void Draw() override;
private:
	fs::path commsAssetsPath;
	fs::path characterStatsPath;
	map<string, characterStats> allCharacterStats;
	void LoadCommsTextures();
	map<string, Texture2D> loadedComms;
	Texture2D passCorrectTexture(string textureName);
	void DrawComments(Texture2D texture);
	void GetCharacterStats(int currentPage);
	void DrawCharacterStats(string characterName);
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
	void Draw() override;
private:
};
class UnlockedItemsMenu : public Menu
{
public:
	UnlockedItemsMenu();
	~UnlockedItemsMenu();
	void Draw() override;
private:
};
class HighestScoreMenu : public Menu
{
public:
	HighestScoreMenu();
	~HighestScoreMenu();
	void Draw() override;
private:
};