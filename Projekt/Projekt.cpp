
#include <iostream>
#include <string>
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
	Font font = LoadFontEx("arial.ttf", 64, 0, 0);

	Game game;
	game.setPlayerCharacter(2);
	Menu menu;
	int setAction =0;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		if (menu.getLoginMenuActive())
		{
			menu.DrawLoginMenu();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				setAction = menu.isButtonClicked();
			}
			switch (setAction)
			{
			case 1:
				cout << "TU JEST BUTTON" << endl;
				menu.setLoginMenuActive(false);
				break;
			case 2:
				menu.insertUsername();
				break;
			case 3:
				menu.insertPassword();
			default:
				break;
			}
			menu.DrawUsername();
			menu.DrawPassword();
		}
		else
		{
			game.InputHandle();
			game.Update();
			//BeginDrawing();
			DrawTexture(background, 0, 0, WHITE);
			game.Draw();
			DrawTextEx(font, "SCORE:", { 60,30 }, 34, 2, GREEN);
			string scoreText = ScoreWithLeadingZeros(game.playerTotalScore, 6);
			DrawTextEx(font, scoreText.c_str(), { 60,55 }, 34, 2, GREEN);
			//EndDrawing();
			if (game.isGameOver())
			{
				break;
			}
		}
		EndDrawing();
	}
	UnloadTexture(background);
	CloseWindow();
}
/*
* Do zrobienia:
* DONE			Nowe postacie (jak mi sie bedzie chcialo)
* DONE			!Dodaj DMG do Character zeby wiecej zadawac dla bossa czy cos	
* PARTIALY DONE !Dodac itemki do podniesienia w czasie retreata miedzy falami (jak mi sie bedzie chcialo)
* !Dodac image do przedmiotow, nowe przedmioty i sprobawac zrobic ze przerwa miedzy falami jest przez 10 sekund, albo do momentu kiedy gracz wcisnie przycisk
* !^Mozna ewentualnie zrobic ze te przemioty pojawiaja sie tylko podczas tej przerwy miedzy falami
* !Poprawic dzialanie image przy inicjacji obiektu - stworzyc metody set na ustawianie i je polimorfowac?
* !Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali?
* !Dodac menu (najlepiej nowa klasa, moze modul?)
* !Dodac achievementy/unlockables(to chyba bedzie lepiej zrobic jako modul)
* !Dodac klase obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, wyglad menu, KONIECZNIE UI ZE STATSAMI
* !Wykorzystac filesystem do obslugi plikow przy LoadTexture()
* Raczej nie zrobie - animacje do wszystkiego
*/

/*
* Wykorzystane rzeczy z labów:
* <thread> wątek do zastopowania nowej fali. Mozna jeszcze cos przekminic ewentualnie
* TBD <filesystem>
* TBD <module>
* TBD ostatnia rzecz
* 
*/
