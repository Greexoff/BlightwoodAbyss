
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include "raylib.h"
#include "game.h"
#include "menuhub.h"
#include "UI.h"
#include "screenSettings.h"
using namespace std;
namespace fs = filesystem;
int main()
{
	int monitor = GetCurrentMonitor();
	InitWindow(GetMonitorWidth(monitor),GetMonitorHeight(monitor), "Blightwood Abbys");
	ScreenSettings::GetInstance().ToggleFullScreenWindow();
	SetMousePosition(GetScreenWidth()*0.5f,GetScreenHeight()*0.5f);
	SetTargetFPS(60);
	Game* game = nullptr;
	Menu::setSelectedMenu(make_unique <StartingMenu>());
	MenuResult gameState = MenuResult::CONTINUE;

	while (!WindowShouldClose()&& gameState!=MenuResult::EXIT){
		if (IsKeyPressed(KEY_F11)) {
			ScreenSettings::GetInstance().ToggleFullScreenWindow();
		}
		BeginDrawing();
		switch (gameState)
		{
		case MenuResult::CONTINUE:
			gameState = Menu::getSelectedMenu()->handleMenuLogic();
			if (gameState == MenuResult::START_GAME)
			{
				if (auto* csm = dynamic_cast<CharacterSelectionMenu*>(Menu::getSelectedMenu())) {
					game = new Game(csm->getPageNumber());
					DisableCursor();
				}
			}
			break;
		case MenuResult::START_GAME:
			game->Update();
			game->Draw();
			if (game->isPostTabClosed())
			{
				Menu::setSelectedMenu(make_unique<MainMenu>());
				delete game;
				game = nullptr;
				gameState = Menu::getSelectedMenu()->handleMenuLogic();
			}
			break;
		case MenuResult::EXIT:
			break;
		default:
			break;
		}
		EndDrawing();
	}
	CloseWindow();
}
/*|---TODO-----------------------------------------------------------------------|
* !!!Dodac info o podnoszonym itemie i o odblokowywanym itemie (z kolorkami jak się uda
* !!Sprawdzić czy wszystkie wektory itp są usuwane w tych klasach w destruktorach
*/
