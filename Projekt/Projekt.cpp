
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include "raylib.h"
#include "game.h"
#include "menuhub.h"
string ScoreWithLeadingZeros(int number, int width)
{
	string scoreText = to_string(number);
	int leadingZeros = width - scoreText.length();
	return string(leadingZeros, '0') + scoreText;
}
using namespace std;
namespace fs = filesystem;
int main()
{
	int Width = 1536;
	int Height = 1024;
	InitWindow(Width, Height, "Survival Game");
	SetTargetFPS(60);
	Font font = LoadFontEx("bahnschrift.ttf", 80, 0, 0);
	Game game;
	string scoreText = "";
	bool shouldEnd = false;
	int setAction=0;
	StartingMenu startingTab;
	LoginMenu loginTab;
	MainMenu mainTab;
	CharacterSelectionMenu charTab;
	RulesMenu rulesTab;
	UnlockedItemsMenu unlockedTab;
	HighestScoreMenu scoresTab;
	CurrentState gameState = CurrentState::STARTING_MENU;
	while (!WindowShouldClose()&& !shouldEnd){
		BeginDrawing();
		switch (gameState)
		{
		case CurrentState::STARTING_MENU:
			startingTab.Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				gameState = CurrentState::LOGIN_MENU;
			}
			break;
		case CurrentState::LOGIN_MENU:
			loginTab.Draw();
			loginTab.handleLoginMenuLogic(setAction,gameState);
			break;
		case CurrentState::MAIN_MENU:
			mainTab.Draw();
			mainTab.handleMainMenuLogic(setAction, gameState, shouldEnd);
			break;
		case CurrentState::CHARACTER_SELECT_MENU:
			charTab.Draw();
			game.DrawPlayerCharacterImage(charTab.getPageNumber());
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && charTab.isButtonClicked()) {
				game.setPlayerCharacter(charTab.getPageNumber());
				gameState = CurrentState::GAMEPLAY;
				game.setLastTimePlayerWasTouched();
			}
			break;
		case CurrentState::RULES_MENU:
			rulesTab.Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && rulesTab.isReturnButtonClicked())
			{
				gameState = CurrentState::MAIN_MENU;
			}
			break;
		case CurrentState::UNLOCKED_ITEMS_MENU:
			unlockedTab.Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && unlockedTab.isReturnButtonClicked())
			{
				gameState = CurrentState::MAIN_MENU;
			}
			break;
		case CurrentState::SCORE_MENU:
			scoresTab.Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && scoresTab.isReturnButtonClicked()) {
				gameState = CurrentState::MAIN_MENU;
			}
			break;
		case CurrentState::GAMEPLAY:
			game.InputHandle();
			game.Update();
			game.DrawBackground();
			game.Draw();
			DrawTextEx(font, "SCORE:", { 60,30 }, 34, 2, GREEN);
			scoreText = ScoreWithLeadingZeros(game.playerTotalScore, 6);
			DrawTextEx(font, scoreText.c_str(), { 60,55 }, 34, 2, GREEN);
			if (game.isGameOver())
			{
				shouldEnd = true;
			}
			break;
		default:
			break;
		}
		EndDrawing();
	}
	UnloadFont(font);
	CloseWindow();
}
/*
* Do zrobienia:
* !!!Modele dla itemkow i PRZECIWNIKOW!!!
* !!!Return w character menu!!!
* !!!Dokonczyc highestscores(wyswieltanie), rules, collectibles
* !!/Dodac achievementy/unlockables(to chyba bedzie lepiej zrobic jako modul)
* !!/Sprawdzac czy to highest score gracza i jesli jest to zapisywac go do pliku DataBase.txt (np. dodac do highest scores funkcje getHighestScore(string username) i stad pobierac highest score gracza
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !/Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali? PARTIALLY DONE
* !Dodac klase UI obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, KONIECZNIE UI ZE STATSAMI
* 
* Raczej nie zrobie - animacje do wszystkiego
*/

/*
* Wykorzystane rzeczy z labów:
* <thread> wątek do zastopowania nowej fali. Mozna jeszcze cos przekminic ewentualnie
* <regex> logowanie sie do gierki
* <filesystem> ladowanie tekstur
* TBD <module>
* 
* 
*/
