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
	string titleName;//przechowanie nazwy gry
	float titleFontSize;//przechowanie rozmiaru czcionki
};
class LoginMenu : public Menu
{
public:
	LoginMenu();
	~LoginMenu();

	MenuResult handleMenuLogic() override;//Metoda wykonujaca odpowiednie dzialanie w zaleznosci od wybranej opcji
	void Draw() override;//Metoda wyswietlajaca informacje na ekranie
private:
	//|-----Zmienne------------------------------------------------------------------------------------------|
	string username, password;
	Rectangle LoginMenu_ConfirmArea, LoginMenu_UsernameBarArea, LoginMenu_PasswordBarArea, LoginMenu_SingupArea, ReturnButtonArea; //przechowanie prostokatow przyciskow
	Vector2 ConfirmPosition, UsernamePosition, PasswordPosition, SignupPosition, ReturnPosition;//przechowanie pozycji x i y przyciskow
	float UsernameTextFontSize, PasswordTextFontSize, ConfirmTextFontSize, SignupTextFontSize, InsertedDataFontSize, ReturnButtonFontSize;// przechowanie rozmiaru czcionki poszczegolnych elementow
	bool isSignupAreaActive;//zmienna do sprawdzenia czy wyswietla sie przycisk SignUp
	bool userExist;
	int error;//zmienna do przechowywania typu bledu, ktory nalezy wyswietlic
	enum errorType {NO_ERROR, REGEX_ERROR, PASSWORD_ERROR, USERNAME_ERROR, IN_USE_ERROR };//typy bledow
	atomic<bool> showError;//zmienna, ktora aktywuje wyswietlanie bledu jezeli taki istnieje
	long long errorDurationTime;//zmienna przechowujaca dlugosc wyswietlania bledu na ekranie
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };//typ przycisnietego przycisku
	int setAction;
	//|-----Metody-------------------------------------------------------------------------------------------|
	void setFontSizes();//Ustawianie rozmiaru czcionek poszczegolnych napisow
	void setBarAreas();//Ustawianie rozmiarow poszczegolnych napisow
	void showLoginError();//Metoda pod wątek wyświetlania błędów - ustawia wyświetlanie błędu na ekranie na określony czas
	void insertData(string& name, string type);//Ustawianie wartosci dla zmiennych username/password 
	void ChooseErrorType();//Ustawianie napisu, który zostanie wyświetlony i wywolanie funkcji rysujacej na ekranie
	bool checkIsLoginCorrect();//Sprawdzanie czy kryteria (regex) username/password są spelnione 
	int isButtonClicked();//Sprawdzanie, czy któryś z przycisków został wciśnięty
	bool checkIsPlayerInDataBase();//Sprawdzanie, czy użytkownik znajduje się w DataBase
	void addPlayerToDataBase();//Dodawanie nowo stworzonego konta gracza, do DataBase
	void clearUsernameandPassword();//Czyszczenie zmiennych username/password, w momencie kiedy przycisnieto SignUp
	
};
class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();
	MenuResult handleMenuLogic() override;//Ustawianie kolejnego etapu programu
	void Draw() override;//Metoda wyswietalajaca informacje na ekranie
private:
	//|---Zmienne------------------------------------------------------------------------------------|
	enum Pressed { NOTHING, NEWGAME_BUTTON, RULES_BUTTON, UNLOCKED_BUTTON, SCORE_BUTTON, EXIT_BUTTON };//typy przyciskow
	float buttonsFontSize, baseButtonsFontSize;
	map<string, ButtonData>Buttons;//Mapa przyciskow - nazwy, rectangle i position (ButtonData)
	vector<string> ButtonNames;//Vector przechowujacy nazwy przyciskow
	int setAction;
	//|---Metody-------------------------------------------------------------------------------------|
	void setButtonsPosition();//Ustawianie pozycji przyciskow na ekranie
	int isButtonClicked();//Sprawdzanie, ktory przycisk zostal wcisniety
};
class CharacterSelectionMenu : public Menu
{
public:
	CharacterSelectionMenu();
	~CharacterSelectionMenu();
	int getPageNumber();//Metoda zwracajaca numer aktualnej strony (czyli ktora postac jest aktualnie wybrana)
	MenuResult handleMenuLogic() override;
	void Draw() override;//Metoda wyświetlająca na ekranie
private:
	//|---Zmienne---------------------------------------------------------------------------------|
	enum CommentType{NOTHING, SWITCH_CHARACTER, SELECT_CHARACTER, STATS_CHARACTER, RETURN_BUTTON};
	Rectangle ArrowArea, ConfirmArea, CharacterInformationArea, ReturnArea;//Przechowanie polozen przyciskow, ktore wykonuja dane funkcje (+wyswietlaja informacje)
	Rectangle SmallerCommentsBar, BiggerCommentsBar;//Przechowanie rozmiarow tla na ktorym wyswietlane sa comments (bigger dla statystyk postaci)
	int pageNumber;//Przechowanie strony na której jest gracz (czyli identyfikacja, która postać jest wybrana)
	int leftSidePageLimit;//Przechowanie limitu lewostronnego
	int rightSidePageLimit;//Przechowanie limitu prawostronnego
	float textureScale;
	//|---Metody----------------------------------------------------------------------------------|
	void DrawComments(CommentType type);//Metoda wyświetlająca wlasciwe informacje w zaleznosci od typu komentarza
	void chooseExplanationType();//Metoda wybierajaca typ komentarza na podstawie polozenia kursora
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
	};
	struct ItemsInfo
	{
		string title;
		string itemName;
		vector<string> paragraphs;
		string textureName;
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
};
class HighestScoreMenu : public Menu
{
public:
	HighestScoreMenu();
	~HighestScoreMenu();
	void Draw() override;	
	MenuResult handleMenuLogic() override;
private:
	vector<pair<string, int>>UsersScores;
	bool areUsersLoadedIntoVector;
	void LoadUsersScoresIntoVector();
	void DrawPlayersScores();
};
