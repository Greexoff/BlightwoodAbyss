
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
	int setAction=0;
	bool shouldEnd = false;
	while (!WindowShouldClose() && !shouldEnd)
	{
		BeginDrawing();
		if (Startingmenu.getMenuActive())
		{
			Startingmenu.DrawMenu();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				Startingmenu.setMenuActive(false);
				Loginmenu.setMenuActive(true);
			}
		}
		else
		{
			if (Loginmenu.getMenuActive())
			{
				Loginmenu.DrawMenu();
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
					}
					else
					{
						if (Loginmenu.checkIsLoginCorrect() && Loginmenu.checkIsPlayerInDataBase())
						{
							Loginmenu.setMenuActive(false);
							menu.setMenuActive(true);
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
					Loginmenu.switchMenuBackground("BackgroundSIGNUP.png");
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
					menu.DrawMenu();
					if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						setAction =menu.isButtonClicked();
					}
					switch (setAction)
					{
					case 1:
						menu.setMenuActive(false);
						Charactermenu.setMenuActive(true);
						game.setLastTimePlayerWasTouched();
						break;
					case 2:
						//tutaj rzeczy dla unlocked
						break;
					case 3:
						//tutaj dla highest score
						break;
					case 4:
						shouldEnd = true;
						break;
					default:
						break;
					}
					setAction = 0;
				}
				else
				{
					if (Charactermenu.getMenuActive())
					{ 
						Charactermenu.DrawMenu();
						game.DrawPlayerCharacterImage(Charactermenu.getPageNumber());
						if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
							setAction = Charactermenu.isButtonClicked();
						}
						switch (setAction)
						{
						case 1:
							game.setPlayerCharacter(Charactermenu.getPageNumber());
							Charactermenu.setMenuActive(false);
							game.setLastTimePlayerWasTouched();
							break;
						case 2:
							break;
						case 3:
							break;
						default:
							break;
							setAction = 0;
						}
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
			}
		}
		EndDrawing();
	}
	UnloadTexture(background);
	UnloadFont(font);
	CloseWindow();
}
/*
* Do zrobienia:
* !!!Przy wyborze postaci zrobic takie kolko, jezeli osiagniety limit po danej stronie to wracam do ostatniej postaci od drugiej strony (takie kolo jak w issacu)
* !!!Dokonczyc reszte menu i rozbic logike na poszczegolne klasy czy cos tak zeby to lepiej chodzilo i nie bylo w tym while 15 ifow w ifie IMPORTANT
* !!!Poprawic dzialanie calego wpisywania hasla
* !!/Dodac achievementy/unlockables(to chyba bedzie lepiej zrobic jako modul)
* !!/Sprawdzac czy to highest score gracza i jesli jest to zapisywac go do pliku DataBase.txt
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
