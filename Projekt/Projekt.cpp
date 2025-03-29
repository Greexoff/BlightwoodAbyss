
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
	Font font = LoadFontEx("arial.ttf", 64, 0, 0);

	Game game;
	game.setPlayerCharacter(2);
	LoginMenu Loginmenu;
	Menu menu;
	int setAction =0;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		if (Loginmenu.getMenuActive())
		{
			Loginmenu.DrawLoginMenu();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				setAction = Loginmenu.isButtonClicked();
			}
			switch (setAction)
			{
			case 1:
				if (Loginmenu.isSignUpCardActive() && Loginmenu.checkIsLoginCorrect())
				{
					Loginmenu.addPlayerToDataBase();
					Loginmenu.setMenuActive(false);
					menu.setMenuActive(true);
					Loginmenu.switchLoginMenuBackground("BackgroundLOGGEDIN.png");
					game.setLastTimePlayerWasTouched();//to trzeba bedzie przeniesc do ostatniej mozliwej funkcji z menu
				}
				else
				{
					if (Loginmenu.checkIsLoginCorrect() && Loginmenu.checkIsPlayerInDataBase())
					{
						Loginmenu.setMenuActive(false);
						menu.setMenuActive(true);
						Loginmenu.switchLoginMenuBackground("BackgroundLOGGEDIN.png");
						game.setLastTimePlayerWasTouched();//to trzeba bedzie przeniesc do ostatniej mozliwej funkcji z menu
					}
				}
				setAction = 0;
				break;
			case 2:
			case 3:
				Loginmenu.insertData(setAction);
				break;
			case 4:		
				Loginmenu.clearUsernameandPassword();
				Loginmenu.switchLoginMenuBackground("BackgroundSIGNUP.png");
				Loginmenu.changeSignBarLevel(false);
				setAction = 0;
				break;
			default:
				break;
			}
			Loginmenu.DrawUsername();
			Loginmenu.DrawPassword();
		}
		else
		{
			if (menu.getMenuActive())
			{
				//tutaj rzeczy dla main menu
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
		}
		EndDrawing();
	}
	UnloadTexture(background);
	CloseWindow();
}
/*
* Do zrobienia:
* !!!Zrobic klase pochodna Starting menu, zeby byl jakis fajny obrazek i po przycisnieciu dowolnego przycisku jest switch do LoginMenu
* !!!Poprawic dzialanie calego wpisywania hasla
* !!!Stworzyc cala klase Menu glownego, na tej samej zasadzie co login menu zrobic przyciski - nowa gra, ustawienia, highest scores, achievementy?
* !!/Po nowej grze znow nowe podmenu do wyboru klasy postaci wraz ze statystykami charactera
* !!/Sprawdzac czy to highest score gracza i jesli jest to zapisywac go do pliku DataBase.txt
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !!Zaczac powoli robic textury dla wszystkich rzeczy, w szczegolnosci lepsze menu i jakis background, tak zeby bylo pod mini-sprawko
* !/Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !Poprawic dzialanie image przy inicjacji obiektu - stworzyc metody set na ustawianie i je polimorfowac?
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali? PARTIALLY DONE
* !Dodac klase UI obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, KONIECZNIE UI ZE STATSAMI
* !Wykorzystac filesystem do obslugi plikow przy LoadTexture()
* Raczej nie zrobie - animacje do wszystkiego
* Dodac achievementy/unlockables(to chyba bedzie lepiej zrobic jako modul)
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
