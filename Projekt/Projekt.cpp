
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

void DrawLoadingStartBackground()
{
	fs::path tmpPath = fs::current_path() / "assets" / "background_loading_screen_assets" / "backgroundLOADINGSTART.png";
	BeginDrawing();
	GameUI::GetInstance().DrawScaledBackgroundImage(LoadTexture(tmpPath.string().c_str()));
	EndDrawing();
}
void ToggleFullScreenWindow(int windowWidth, int windowHeight)
{
	int monitor = GetCurrentMonitor();
	if (!IsWindowFullscreen())
	{
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
		ScreenSettings::GetInstance().setResolutionFactor(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	}
	else
	{
		ToggleFullscreen();
		SetWindowSize(windowWidth, windowHeight);
		SetWindowPosition((GetMonitorWidth(monitor)-windowWidth)/ 2, (GetMonitorHeight(monitor)-windowHeight )/ 2);
		ScreenSettings::GetInstance().setResolutionFactor(windowWidth, windowHeight);
	}
}
using namespace std;
namespace fs = filesystem;
int main()
{
	int monitor = GetCurrentMonitor();
	InitWindow(GetMonitorWidth(monitor),GetMonitorHeight(monitor), "Blightwood Abbys");
	int WindowWidth = (int)GetMonitorWidth(monitor) * 0.66;
	int WindowHeight = (int)GetMonitorHeight(monitor) * 0.66;
	ToggleFullscreen();
	SetTargetFPS(60);

	DrawLoadingStartBackground();

	Game* game = nullptr;
	Menu::setSelectedMenu(make_unique <StartingMenu>());
	MenuResult gameState = MenuResult::CONTINUE;

	while (!WindowShouldClose()&& gameState!=MenuResult::EXIT){
		if (IsKeyPressed(KEY_F11)) {
			ToggleFullScreenWindow(WindowWidth, WindowHeight);
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
* !!!! Zamienic wszystkie stale liczby przy Draw na zalezne od GetScreenHeight() i GetScreenWidth() np. GetScreenHeight()*0,2 zamiast zwyklego 200
* !!!! Wprowdzic imageScale dla wszystkich backgroundow
* !!!/Dobrze dzialajacy  rules, collectibles 
* !!! uporządkować pozostałe pliku h
* !!! Sprawdzic czemu postac nie laduje sie na srodku, tylko troche na boku(pewnie image jeszcze nie jest zaladowany i dlatego)
* !!!Kolizje moze zrobic na nowo zeby to lepiej dzialalo
* !!!Metody fabrykujace, zmienic dzialanie gdzie startuja tearsy przy tworzeniu - da sie w paru miejscach wyjebac casy chyba
* !!sprobowac zrobic ten setw na tych napisach w highest scores
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali
* !Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
*/

/*|---Rzeczy-z-Labow------------------------------------------------------------|
* <thread> wątek do zastopowania nowej fali i obslugi bledow w LoginMenu
* <regex> logowanie sie do gierki
* <filesystem> ladowanie tekstur
* <module> strukutra CharacterStats
*/
