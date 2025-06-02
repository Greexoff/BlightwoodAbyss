#pragma once
#include <iostream>
#include <vector>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>
#include "UI.h"
#include "Textures.h"
#include "UserInfo.h"
#include "screenSettings.h"
import CharacterStats;
using namespace std;
namespace fs = filesystem;

enum class MenuResult
{
	CONTINUE,
	START_GAME,
	EXIT,
};
struct ButtonData
{
	Rectangle rectangle;
	Vector2 position;
};
enum class ButtonType
{
	RETURN_BUTTON,
	LEFT_ARROW_BUTTON,
	RIGHT_ARROW_BUTTON,
};
enum class Page
{
	LEFT_SIDE_LIMIT,
	RIGHT_SIDE_LIMIT,
	CURRENT_PAGE,
};
class Menu
{
public:
	virtual MenuResult handleMenuLogic()=0;
	virtual ~Menu();
	static Menu* getSelectedMenu();
	static void setSelectedMenu(unique_ptr<Menu> newMenu);
protected:
	virtual void Draw() = 0;
	static unique_ptr<Menu> selectedMenu;
	fs::path data_basePath = fs::current_path() / "database" / "DataBase.txt";
	virtual void setButtonsAreas(Rectangle retButton, Rectangle leftArrow, Rectangle rightArrow);
	virtual void CheckCollisions();
	virtual Rectangle getButtonArea(ButtonType type);
	virtual void drawMenuElements();
	virtual void switchPage(int number);
	virtual void setAmountOfPages(int number);
	virtual void setButtonVisibility();
	virtual int getPage(Page type);
	virtual bool getButtonVisibility(Page type);
	virtual void setMenuElements();
private:
	Rectangle ReturnButton, LeftArrow, RightArrow;
	bool isRightArrowVisible = false;
	bool isLeftArrowVisible= false;
	int pageNumber = 1, LeftSidePageLimit=1, RightSidePageLimit=1;
};
class StartingMenu : public Menu
{
public:
	void Draw() override;
	MenuResult handleMenuLogic() override;
	StartingMenu();
	~StartingMenu();
private:
	string titleName;
	float titleFontSize;
};
class LoginMenu : public Menu
{
public:
	LoginMenu();
	~LoginMenu();

	MenuResult handleMenuLogic() override;
	void Draw() override;
private:
	string username, password;
	Rectangle LoginMenu_ConfirmArea, LoginMenu_UsernameBarArea, LoginMenu_PasswordBarArea, LoginMenu_SingupArea, ReturnButtonArea; 
	Vector2 ConfirmPosition, UsernamePosition, PasswordPosition, SignupPosition, ReturnPosition;
	float UsernameTextFontSize, PasswordTextFontSize, ConfirmTextFontSize, SignupTextFontSize, InsertedDataFontSize, ReturnButtonFontSize;
	bool isSignupAreaActive;
	bool userExist;
	int error;
	enum errorType {NO_ERROR, REGEX_ERROR, PASSWORD_ERROR, USERNAME_ERROR, IN_USE_ERROR };
	atomic<bool> showError;
	long long errorDurationTime;
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };
	int setAction;
	void setFontSizes();
	void setBarAreas();
	void showLoginError();
	void insertData(string& name, string type);
	void ChooseErrorType();
	bool checkIsLoginCorrect();
	int isButtonClicked();
	bool checkIsPlayerInDataBase();
	void addPlayerToDataBase();
	void clearUsernameandPassword();
	
};
class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();
	MenuResult handleMenuLogic() override;
	void Draw() override;
private:
	enum Pressed { NOTHING, NEWGAME_BUTTON, RULES_BUTTON, UNLOCKED_BUTTON, SCORE_BUTTON, EXIT_BUTTON };
	float buttonsFontSize, baseButtonsFontSize;
	map<string, ButtonData>Buttons;
	vector<string> ButtonNames;
	int setAction;
	void setButtonsPosition();
	int isButtonClicked();
};
class CharacterSelectionMenu : public Menu
{
public:
	CharacterSelectionMenu();
	~CharacterSelectionMenu();
	int getPageNumber();
	MenuResult handleMenuLogic() override;
	void Draw() override;
private:
	enum CommentType{NOTHING, SWITCH_CHARACTER, SELECT_CHARACTER, STATS_CHARACTER, RETURN_BUTTON};
	Rectangle ArrowArea, ConfirmArea, CharacterInformationArea, ReturnArea;
	Rectangle SmallerCommentsBar, BiggerCommentsBar;
	int pageNumber;
	int leftSidePageLimit;
	int rightSidePageLimit;
	float textureScale;
	void DrawComments(CommentType type);
	void chooseExplanationType();
	void DrawPlayerCharacterImage();
	void setAreas();
};
class RulesMenu : public Menu
{
public:
	RulesMenu();
	~RulesMenu();
	void Draw() override;
	MenuResult handleMenuLogic() override;
private:
	struct ItemsInfo
	{
		string title;
		string itemName;
		vector<string> paragraphs;
		string textureName;
		float textureScale;
	};
	struct GameInfo
	{
		string title;
		vector<string> paragraphs;
	};
	struct CharacterInfo
	{
		string title;
		string characterName;
		vector<string> paragraphs;
		string textureName;
		int pageNumber;
	};
	struct EnemyInfo
	{
		string title;
		string enemyDescription;
		string enemyName;
		vector<string> paragraphs;
		string textureName;
		float textureScale;
	};
	Rectangle GameInfoBar;
	vector<GameInfo> GameInfoPages;
	vector<CharacterInfo> CharInfoPages;
	vector<EnemyInfo> EnemyInfoPages;
	vector<ItemsInfo> ItemsInfoPages;
	void DrawRules(int page);
	void setPagesContent();
};
class UnlockedItemsMenu : public Menu
{
public:
	UnlockedItemsMenu();
	~UnlockedItemsMenu();
	void Draw() override;
	MenuResult handleMenuLogic() override;
private:
	struct ItemsInfo
	{
		string itemName;
		vector<string> paragraphs1;
		vector<string> paragraphs2;
		string textureName;
		string itemNameInFile;
	};
	vector<ItemsInfo> ItemsInfoPages;
	Rectangle ItemsInfoBar;
	void DrawPage(int page);
	void DrawLockedBar(float beginBarHeight, float endBarHeight);
	void setPageContent();
};
class HighestScoreMenu : public Menu
{
public:
	HighestScoreMenu();
	~HighestScoreMenu();
	void Draw() override;	
	MenuResult handleMenuLogic() override;
private:
	Rectangle ScoresInfoBar;
	vector<pair<string, int>>UsersScores;
	bool areUsersLoadedIntoVector;
	void LoadUsersScoresIntoVector();
	void DrawPlayersScores();
};
