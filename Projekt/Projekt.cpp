
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include "raylib.h"
#include "game.h"
#include "menuhub.h"
#include "UI.h"
#include "UserInfo.h"
#include "screenSettings.h"
import CharacterStats;

using namespace std;
namespace fs = filesystem;
int main()
{
	int monitor = GetCurrentMonitor();
	InitWindow(GetMonitorWidth(monitor),GetMonitorHeight(monitor), "Blightwood Abbys");
	ToggleFullscreen();
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
					game = new Game();
					game->setPlayerCharacter(csm->getPageNumber());
					game->setLastTimePlayerWasTouched();
				}
			}
			break;
		case MenuResult::START_GAME:
			game->InputHandle();
			game->Update();
			game->Draw();
			if (game->isGameOver())
			{
				Menu::setSelectedMenu(make_unique<AfterGameMenu>());
				if (auto* agm = dynamic_cast<AfterGameMenu*>(Menu::getSelectedMenu())) {
					agm->updatePlayerScoreInDataBase(game->playerTotalScore, UserInfo::GetInstance().getUsername());
				}
				delete game;
				game = nullptr;
				gameState = Menu::getSelectedMenu()->handleMenuLogic();
			}
			break;
		case MenuResult::EXIT:
			break;
		case MenuResult::AFTER_GAME:
			if (gameState == MenuResult::AFTER_GAME)
			{
				gameState = Menu::getSelectedMenu()->handleMenuLogic();
			}
			break;
		default:
			break;
		}
		EndDrawing();
	}
	CloseWindow();
}
/*|---TODO-----------------------------------------------------------------------|
* !!!! Podmienic dzialanie regexow, zeby pobierac tez wartosci trinketow
* !!!/Dobrze dzialajacy  rules, collectibles 
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali
* !Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
*/
