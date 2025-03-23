
#include <iostream>
#include <string>
#include "raylib.h"
#include "game.h"

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
	game.setPlayerCharacter(1);
	while (!WindowShouldClose())
	{
		game.InputHandle();
		game.Update();
		BeginDrawing();
		DrawTexture(background, 0, 0, WHITE);
		game.Draw();	
		DrawTextEx(font, "SCORE:", { 60,30 }, 34, 2, GREEN);
		string scoreText = ScoreWithLeadingZeros(game.playerTotalScore, 6);
		DrawTextEx(font, scoreText.c_str(), { 60,55 }, 34, 2, GREEN);
		EndDrawing();
		if (game.isGameOver())
		{
			break;
		}
	}
	UnloadTexture(background);
	CloseWindow();
}
/*
* Do zrobienia:
* DONE		Nowe postacie (jak mi sie bedzie chcialo)
* DONE		!Dodaj DMG do Character zeby wiecej zadawac dla bossa czy cos	
* !Dodac itemki do podniesienia w czasie retreata miedzy falami (jak mi sie bedzie chcialo)
* !Dodac menu (najlepiej nowa klasa, moze modul?)
* !Dodac klase obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, wyglad menu, 
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
