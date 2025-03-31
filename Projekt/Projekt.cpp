
#include <iostream>
#include <string>
#include <regex>
#include "raylib.h"
#include "game.h"
#include "menu.h"
string ScoreWithLeadingZeros(int number, int width)
{
	string scoreText = to_string(number);
	int leadingZeros = width - scoreText.length();
	return string(leadingZeros, '0') + scoreText;
}
using namespace std;
int main()
{
	int Width = 1186;
	int Height = 738;
	InitWindow(Width, Height, "Survival Game");
	Texture2D background = LoadTexture("backgroundOG.png");
	SetTargetFPS(60);
	Font font = LoadFontEx("bahnschrift.ttf", 55, 0, 0);

	Game game;
	StartingMenu Startingmenu;
	LoginMenu Loginmenu;
	Menu menu;
	CharacterSelectMenu Charactermenu;
	UnlockedItemsMenu unlockmenu;
	HighestScoreMenu scoremenu;
	int setAction=0;
	bool shouldEnd = false;
	string scoreText = "";
	CurrentState gameState = CurrentState::STARTING_MENU;
	while (!WindowShouldClose() && !shouldEnd)
	{
		BeginDrawing();
		switch (gameState)
		{
		case CurrentState::STARTING_MENU:
			Startingmenu.DrawMenu();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				gameState = CurrentState::LOGIN_MENU;
			}
			break;
		case CurrentState::LOGIN_MENU:
			Loginmenu.DrawMenu();
			Loginmenu.handleLoginMenuLogic(setAction, gameState);
			break;
		case CurrentState::MAIN_MENU:
			menu.DrawMenu();
			menu.handleMainMenuLogic(setAction, gameState, shouldEnd);
			break;
		case CurrentState::CHARACTER_SELECT_MENU:
			Charactermenu.DrawMenu();
			game.DrawPlayerCharacterImage(Charactermenu.getPageNumber());
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Charactermenu.isButtonClicked()) {
				game.setPlayerCharacter(Charactermenu.getPageNumber());
				gameState = CurrentState::GAMEPLAY;
				game.setLastTimePlayerWasTouched();
			}
			break;
		case CurrentState::UNLOCKED_ITEMS_MENU:
			unlockmenu.DrawMenu();
			//tutaj jeszcze jakas zabawa ze laduje tylko te tekstury ktore masz odblokowane
			break;
		case CurrentState::SCORE_MENU:
			scoremenu.DrawMenu();
			scoremenu.handleScoreMenu();
			//tutaj algorytm sortowania po high score z pliku + jeszcze wypadaloby poprawic sytuacje ze typek sie wpisze o tym samym nicku ale to juz na poziomie add player
			break;
		case CurrentState::GAMEPLAY:
			game.InputHandle();
			game.Update();
			DrawTexture(background, 0, 0, WHITE);
			game.Draw();
			DrawTextEx(font, "SCORE:", { 60,30 }, 34, 2, GREEN);
			scoreText = ScoreWithLeadingZeros(game.playerTotalScore, 6);
			DrawTextEx(font, scoreText.c_str(), { 60,55 }, 34, 2, GREEN);
			if (game.isGameOver())
			{
				shouldEnd=true;
			}
			break;
		default:
			break;
		}
		EndDrawing();
	}
	UnloadTexture(background);
	UnloadFont(font);
	CloseWindow();
}
/*
* Do zrobienia:
* !!!Dokonczyc highestscores(wyswieltanie)
* !!!Dodac jakies reguły gry i moze zrobic strony (znowu strzalki) i na pierwszej glowne zasady, na drugiej opis postaci, opis przeciwnikow, opis itemow
* !!!Zrobic te unlocked
* !!/Dodac achievementy/unlockables(to chyba bedzie lepiej zrobic jako modul)
* !!/Sprawdzac czy to highest score gracza i jesli jest to zapisywac go do pliku DataBase.txt (np. dodac do highest scores funkcje getHighestScore(string username) i stad pobierac highest score gracza
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !/Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali? PARTIALLY DONE
* !Dodac klase UI obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, KONIECZNIE UI ZE STATSAMI
* !Wykorzystac filesystem do obslugi plikow przy LoadTexture()
* 
* Raczej nie zrobie - animacje do wszystkiego
*/

/*
* Wykorzystane rzeczy z labów:
* <thread> wątek do zastopowania nowej fali. Mozna jeszcze cos przekminic ewentualnie
* <regex> logowanie sie do gierki
* TBD <filesystem>
* TBD <module>
* 
* 
*/
