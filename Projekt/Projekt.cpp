
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
	while (!WindowShouldClose())
	{
		game.InputHandle();
		game.Update();
		BeginDrawing();
		DrawTexture(background, 0, 0, WHITE);
		game.Draw();	
		DrawTextEx(font, "SCORE:", { 60,30 }, 34, 2, GREEN);
		string scoreText = ScoreWithLeadingZeros(game.playerTotalScore, 5);
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
