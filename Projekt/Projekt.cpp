
#include <iostream>
#include "raylib.h"
#include "game.h"

using namespace std;
int main()
{
	int Width = 1186;
	int Height = 738;

	InitWindow(Width, Height, "Survival Game");
	Texture2D background = LoadTexture("background.png");
	SetTargetFPS(60);

	Game game;
	while (!WindowShouldClose())
	{
		game.InputHandle();
		game.Update();
		BeginDrawing();
		DrawTexture(background, 0, 0, WHITE);
		game.Draw();
		EndDrawing();
		if (game.isGameOver())
		{
			break;
		}
	}
	UnloadTexture(background);
	CloseWindow();
}
