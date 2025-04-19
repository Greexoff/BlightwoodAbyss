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
};//Aktualny stan rozgrywki (wykorzystywany rowniez w mainie)
struct ButtonData
{
	Rectangle rectangle;
	Vector2 position;
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
	Texture2D BackgroundTexture; 
	Rectangle ReturnToPrieviousMenuButton = { 292,810,488-292,1005 - 810 };//to raczej do usuniecia
};
class StartingMenu : public Menu
{
public:
	void Draw() override;
	StartingMenu();
	~StartingMenu();
private:
	string titleName;//przechowanie nazwy gry
	float titleFontSize;//przechowanie rozmiaru czcionki
	Rectangle ScreenBar;//przechowanie wymiarow na ktorych mozna wyswietlic tytul gry
};
class LoginMenu : public Menu
{
public:
	LoginMenu();
	~LoginMenu();
	void handleLoginMenuLogic(int& setAction, CurrentState& gameState);//Metoda wykonujaca odpowiednie dzialanie w zaleznosci od wybranej opcji
	void Draw() override;//Metoda wyswietlajaca informacje na ekranie
private:
	//|-----Zmienne------------------------------------------------------------------------------------------|
	fs::path data_basePath;//sciezka do bazy danych
	Rectangle LoginMenu_ConfirmArea, LoginMenu_UsernameBarArea, LoginMenu_PasswordBarArea, LoginMenu_SingupArea; //przechowanie prostokatow przyciskow
	Vector2 ConfirmPosition, UsernamePosition, PasswordPosition, SignupPosition;//przechowanie pozycji x i y przyciskow
	float UsernameTextFontSize, PasswordTextFontSize, ConfirmTextFontSize, SignupTextFontSize, InsertedDataFontSize;// przechowanie rozmiaru czcionki poszczegolnych elementow
	bool isSignupAreaActive;//zmienna do sprawdzenia czy wyswietla sie przycisk SignUp
	string username;//zmienna do przechowywania wpisywanej nazwy uzytkownika
	string password;//zmienna do przechowywania wpisywanego hasla uzytkownika
	bool userExist;
	int error;//zmienna do przechowywania typu bledu, ktory nalezy wyswietlic
	enum errorType {NO_ERROR, REGEX_ERROR, PASSWORD_ERROR, USERNAME_ERROR, NOT_EXIST_ERROR };//typy bledow
	atomic<bool> showError;//zmienna, ktora aktywuje wyswietlanie bledu jezeli taki istnieje
	long long errorDurationTime;//zmienna przechowujaca dlugosc wyswietlania bledu na ekranie
	enum Pressed { NOTHING, CONFIRM_BUTTON, USERNAME_BAR, PASSWORD_BAR, SIGNUP_BAR };//typ przycisnietego przycisku
	//|-----Metody-------------------------------------------------------------------------------------------|
	void setFontSizes();//Ustawianie rozmiaru czcionek poszczegolnych napisow
	void setBarAreas();//Ustawianie rozmiarow poszczegolnych napisow
	void setXYofTexts();//Ustawianie pozycji x i y poszczegolnych napisow na ekranie
	void showLoginError();//Metoda pod wątek wyświetlania błędów - ustawia wyświetlanie błędu na ekranie na określony czas
	void DrawingErrorSettingUp(string information);
	void insertData(string& name);//Ustawianie wartosci dla zmiennych username/password 
	void DrawError();//Ustawianie napisu, który zostanie wyświetlony i wywolanie funkcji rysujacej na ekranie
	void DrawLogin(string& name, int type);//Wyswietlanie username/password na ekranie !!!MOZLIWE ZE MOZNA USUNAC I WSTAWIC GAMEUI::DRAWTEXTONBAR
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
	void handleMainMenuLogic(int& setAction, CurrentState& gameState, bool& shouldEnd);//Ustawianie kolejnego etapu programu
	void Draw() override;//Metoda wyswietalajaca informacje na ekranie
private:
	//|---Zmienne------------------------------------------------------------------------------------|
	enum Pressed { NOTHING, NEWGAME_BUTTON, RULES_BUTTON, UNLOCKED_BUTTON, SCORE_BUTTON, EXIT_BUTTON };//typy przyciskow
	float buttonsFontSize;
	map<string, ButtonData>Buttons;//Mapa przyciskow - nazwy, rectangle i position (ButtonData)
	vector<string> ButtonNames;//Vector przechowujacy nazwy przyciskow
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
	void isButtonClicked(CurrentState& gameState);//Metoda wykonujaca odpowiednie dzialania na podstawie kliknietego przycisku
	void Draw() override;//Metoda wyświetlająca na ekranie
private:
	//|---Zmienne---------------------------------------------------------------------------------|
	enum CommentType{NOTHING, SWITCH_CHARACTER, SELECT_CHARACTER, STATS_CHARACTER, RETURN_BUTTON};
	Rectangle ArrowArea, ConfirmArea, CharacterInformationArea, ReturnArea;//Przechowanie polozen przyciskow, ktore wykonuja dane funkcje (+wyswietlaja informacje)
	Rectangle SmallerCommentsBar, BiggerCommentsBar;//Przechowanie rozmiarow tla na ktorym wyswietlane sa comments (bigger dla statystyk postaci)
	int pageNumber;//Przechowanie strony na której jest gracz (czyli identyfikacja, która postać jest wybrana)
	int leftSidePageLimit;//Przechowanie limitu lewostronnego
	int rightSidePageLimit;//Przechowanie limitu prawostronnego
	//|---Metody----------------------------------------------------------------------------------|
	void DrawComments(CommentType type);//Metoda wyświetlająca wlasciwe informacje w zaleznosci od typu komentarza
	void chooseExplanationType();//Metoda wybierajaca typ komentarza na podstawie polozenia kursora

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