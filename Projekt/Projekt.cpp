
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
			case 3:
				menu.insertData(setAction);
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
			DrawTexture(background, 0, 0, WHITE);
			game.Draw();
			DrawTextEx(font, "SCORE:", { 60,30 }, 34, 2, GREEN);
			string scoreText = ScoreWithLeadingZeros(game.playerTotalScore, 6);
			DrawTextEx(font, scoreText.c_str(), { 60,55 }, 34, 2, GREEN);
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
* !!!Zamienic menu na moduł
* !!!Poprawic dzialanie calego wpisywania hasla
* !!!Dodac baze danych txt i odczytywac oraz w osobnej funkcji po kliknieciu confirm ma byc checking username i password z regexem
* !!!Dodac mechanizm zeby protekcja zaczynala sie w momencie, kiedy rozpocznie sie rozgrywka, a nie kiedy bedzie InitWindow
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !!Zaczac powoli robic textury dla wszystkich rzeczy, w szczegolnosci lepsze menu i jakis background, tak zeby bylo pod mini-sprawko
* !Mozna ewentualnie zrobic ze te przemioty pojawiaja sie tylko podczas tej przerwy miedzy falami (raczej nie)
* !Poprawic dzialanie image przy inicjacji obiektu - stworzyc metody set na ustawianie i je polimorfowac?
* !Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali?
* !Dodac klase UI obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, KONIECZNIE UI ZE STATSAMI
* !Wykorzystac filesystem do obslugi plikow przy LoadTexture()
* Raczej nie zrobie - animacje do wszystkiego
* Dodac achievementy/unlockables(to chyba bedzie lepiej zrobic jako modul)
*/

/*
* Wykorzystane rzeczy z labów:
* <thread> wątek do zastopowania nowej fali. Mozna jeszcze cos przekminic ewentualnie
* TBD <filesystem>
* TBD <module>
* TBD ostatnia rzecz
* 
*/
