#pragma once
#include <iostream>
#include <iomanip>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>
import CharacterStats;
using namespace std;
namespace fs = filesystem;

class GameUI
{
public:
	//|---Singleton--------------------------------------------------------------------------|
	static GameUI& GetInstance();
	~GameUI();
	//|---Metody-----------------------------------------------------------------------------|
	void setLoadingProgress(string backgroundName);//DO USUNIECIA JAK JUZ OGRANE LOADINGSCRREN
	void DrawBackground();//DO USUNIECIA JAK JUZ OGRANE LOADINGSCRREN
	void DrawCurrentScore(int currentPlayerScore, float fontSize);//MAIN: Wyświetla aktualny score gracza
	void DrawTextWithOutline(const string& text, Vector2 position, float fontSize);//GENERAL: Wyświetla podany tekst w danym miejscu o podanym rozmiarze w pomarańczowo-zółtym stylu
	void DrawBlackBar(Rectangle border, unsigned char opacity);//GENERAL: Wyświetla ciemny pasek o podanych rozmiarach i przejrzystości wraz z czarną obramówką
	void DrawCharacterStatsInGame(float PlayerSpeed, float TearSpeed, float PlayerDamage, float TearRate, int playerMaxHealth, float x_pos, float starting_y_pos, float gap, float fontSize);//GAME: Wyświetla aktualne statystyki gracza w trakcie gry
	void DrawCharacterStatsInMenu(int pageNumber, Rectangle bar, float fontSize, float y_position);//CHAR_SELECT_MENU: Wyświetla domyślne statystyki postaci
	void DrawWaveNumber(int waveNumber, float fontSize);//MAIN: WYŚWIETLA NUMER AKTUALNEJ FALI 
	void DrawTextOnBar(Rectangle bar, float fontSize, const  string& text, float y_position);//GENERAL: Wyświetla tekst na podanym obszarze (wyśrodkowany), zmniejszając rozmiar/ rodzielając na części jezeli wykracza 
	void DrawData(string& name, Rectangle bar, float& fontSize);//LOGIN_MENU: Wyświetla dane wpisywane przez uzytkownika
	void DrawError(string information, Rectangle bar);//LOGIN_MENU: Wyświetla komunikaty błędów podczas logowania/tworzenia konta
	Rectangle setBarArea(float fontSize, string text, Vector2 textPosition, int type, float additional_x_space, float additional_y_space);// Ustawia obszar paska wraz z dodatkową przestrzenią po bokach
	Vector2 MeasureTextBar(float& fontSize, string text);//CHYBA DO USUNIECIA POTEM BO TAK TROCHE NWM PO CO 
	Vector2 MeasureText(float fontSize, string text);//GENERAL: Sprawdzanie długości tekstu dla wykorzystywanej czcionki (bez potrzeby przechowywania czcionki w innych klasach)
	string ConvertToString(float number, int prec);//Metoda konwertująca liczbe(float) na string o podanej precyzji
private:
	//|---Singleton--------------------------------------------------------------------------|
	GameUI();
	GameUI(const GameUI&) = delete;
	void operator=(const GameUI&) = delete;
	//|---Zmienne----------------------------------------------------------------------------|
	fs::path fontPath;//Sciezka do czcionki
	fs::path loadingScreenAssetsPath;//DO USUNIECIA JAK JUZ OGRANE LOADINGSCREEN
	Color MyOrange;//Specjalny pomarancz do napisu
	Color MyYellow;//Specjalna zolc do napisu
	Font font;//Zmienna przechowujaca czcionke
	Texture2D currentBackground;//DO USUNIECIA JAK JUZ OGRANE LOADINGSCREEN
	map<string, Texture2D> loadedScreenTextures;//DO USUNIECIA JAK JUZ OGRANE LOADINGSCRREN
	//|---Metody-----------------------------------------------------------------------------|
	void loadTexturesIntoMap();//DO USUNIECIA JAK JUZ OGRANE LOADINGSCRREN
	Texture2D passCorrectTexture(string textureName);//DO USUNIECIA JAK JUZ OGRANE LOADINGSCRREN
	vector<string> DivideTextIntoParts(const string& text, float fontSize, float maxWidth);//Metoda rozdzielająca napis na części

};