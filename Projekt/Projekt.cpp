
#include <iostream>
#include "raylib.h"
#include "game.h"

using namespace std;
int main()
{
	int Width = 1186;
	int Height = 738;

	InitWindow(Width, Height, "Tower Defense");
	Texture2D background = LoadTexture("background.png");
	SetTargetFPS(60);

	Game game;
	while (!WindowShouldClose())
	{
		game.InputHandle();
		game.UpdateLocation();
		BeginDrawing();
		DrawTexture(background, 0, 0, WHITE);
		game.Draw();
		EndDrawing();
	}
	UnloadTexture(background);
	CloseWindow();
}
