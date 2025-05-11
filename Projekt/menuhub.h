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
import CharacterStats;
using namespace std;
namespace fs = filesystem;

enum class MenuResult
{
	CONTINUE,
	START_GAME,
	EXIT,
	AFTER_GAME,
};
struct ButtonData
{
	Rectangle rectangle;
	Vector2 position;
};

class Menu
{
public:
	virtual void Draw()=0;
	virtual void LoadTextures(Texture2D correctTexture);
	virtual bool isReturnButtonClicked();
	virtual void ReturnToMenu();
	virtual MenuResult handleMenuLogic()=0;
	virtual ~Menu();
	static Menu* getSelectedMenu();
	static void setSelectedMenu(unique_ptr<Menu> newMenu);
protected:
	static unique_ptr<Menu> selectedMenu;
	fs::path data_basePath = fs::current_path() / "database" / "DataBase.txt";//sciezka do bazy danych
	Texture2D BackgroundTexture; 
	Rectangle ReturnToPrieviousMenuButton = { 292,810,488-292,1005 - 810 };
	Rectangle ReturnToMenuCommentBar = {50,50,300,300};
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
	Rectangle LoginMenu_ConfirmArea, LoginMenu_UsernameBarArea, LoginMenu_PasswordBarArea, LoginMenu_SingupArea; //przechowanie prostokatow przyciskow
	Vector2 ConfirmPosition, UsernamePosition, PasswordPosition, SignupPosition;//przechowanie pozycji x i y przyciskow
	float UsernameTextFontSize, PasswordTextFontSize, ConfirmTextFontSize, SignupTextFontSize, InsertedDataFontSize;// przechowanie rozmiaru czcionki poszczegolnych elementow
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
	float buttonsFontSize;
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
	fs::path CharImagePath;
	map<string,Texture2D> CharacterImages;
	enum CommentType{NOTHING, SWITCH_CHARACTER, SELECT_CHARACTER, STATS_CHARACTER, RETURN_BUTTON};
	Rectangle ArrowArea, ConfirmArea, CharacterInformationArea, ReturnArea;//Przechowanie polozen przyciskow, ktore wykonuja dane funkcje (+wyswietlaja informacje)
	Rectangle SmallerCommentsBar, BiggerCommentsBar;//Przechowanie rozmiarow tla na ktorym wyswietlane sa comments (bigger dla statystyk postaci)
	int pageNumber;//Przechowanie strony na której jest gracz (czyli identyfikacja, która postać jest wybrana)
	int leftSidePageLimit;//Przechowanie limitu lewostronnego
	int rightSidePageLimit;//Przechowanie limitu prawostronnego
	//|---Metody----------------------------------------------------------------------------------|
	void DrawComments(CommentType type);//Metoda wyświetlająca wlasciwe informacje w zaleznosci od typu komentarza
	void chooseExplanationType();//Metoda wybierajaca typ komentarza na podstawie polozenia kursora
	void DrawPlayerCharacterImage();
};
class RulesMenu : public Menu
{
public:
	RulesMenu();
	~RulesMenu();
	void Draw() override;
	MenuResult handleMenuLogic() override;
private:
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
	void handleScoresMenuLogic();
	MenuResult handleMenuLogic() override;
private:
	Rectangle prievousPageButton;
	Rectangle nextPageButton;
	vector<pair<string, int>>UsersScores;
	bool isNextPageButtonVisible;
	bool isPrievousPageButtonVisible;
	int maxPageNumber;
	int minPageNumber;
	int currentPageNumber;
	bool areUsersLoadedIntoVector;
	void LoadUsersScoresIntoVector();
	void DrawPlayersScores();
	void ArrowClicked(Rectangle bar, int action, bool visibility);
	void setButtonVisibility(bool& visibility, int extremePageNumber);
};
class AfterGameMenu : public Menu
{
public:
	AfterGameMenu();
	~AfterGameMenu();
	void Draw() override;
	MenuResult handleMenuLogic() override;
	void updatePlayerScoreInDataBase(int playerScore, string username);
private:
	enum Pressed {NOTHING, NEWGAME_BUTTON, MAINMENU_BUTTON, EXIT_BUTTON};
	map<string, ButtonData>Buttons;
	vector<string> ButtonNames;
	float buttonsFontSize;
	void setButtonPosition();
	void DrawButtons();
	bool isNewScoreHigher(int DataBaseScore, int currentScore);
};