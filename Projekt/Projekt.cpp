
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include "raylib.h"
#include "game.h"
#include "menuhub.h"
#include "UI.h"
#include "UserInfo.h"
import CharacterStats;

void DrawLoadingStartBackground()
{
	fs::path tmpPath = fs::current_path() / "assets" / "background_loading_screen_assets" / "backgroundLOADINGSTART.png";
	BeginDrawing();
	DrawTextureV(LoadTexture(tmpPath.string().c_str()), {0,0}, WHITE);
	EndDrawing();
}
using namespace std;
namespace fs = filesystem;
int main()
{
	int Width = 1536;
	int Height = 1024;
	InitWindow(Width, Height, "Survival Game");
	SetTargetFPS(60);

	DrawLoadingStartBackground();
	CharacterData::LoadStatsOnce();

	Game* game = nullptr;
	Menu::setSelectedMenu(make_unique <StartingMenu>());

	string keepWaveNumberText = "";
	string keepPlayerScoreText = "";

	MenuResult gameState = MenuResult::CONTINUE;

	while (!WindowShouldClose()&& gameState!=MenuResult::EXIT){
		BeginDrawing();
		if (gameState == MenuResult::CONTINUE)
		{
			gameState = Menu::getSelectedMenu()->handleMenuLogic();
			if (gameState == MenuResult::START_GAME)
			{
				if (auto* csm = dynamic_cast<CharacterSelectionMenu*>(Menu::getSelectedMenu())) {
					game = new Game();
					game->setPlayerCharacter(csm->getPageNumber());
					game->setLastTimePlayerWasTouched();
				}
			}
		}
		if (gameState == MenuResult::START_GAME)
		{
			game->InputHandle();
			game->Update();
			game->DrawBackground();
			game->Draw();
			keepWaveNumberText = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(game->getWaveNumber(), 3, "WAVE:");
			keepPlayerScoreText = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(game->playerTotalScore, 6, "SCORE:");
			GameUI::GetInstance().DrawGameUI(keepPlayerScoreText + " " + keepWaveNumberText, 60, GetScreenHeight() - 30);
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
		}
		if (gameState == MenuResult::AFTER_GAME)
		{
			gameState = Menu::getSelectedMenu()->handleMenuLogic();
		}
		EndDrawing();
	}
	delete Menu::getSelectedMenu();
	CloseWindow();
}
/*|---TODO-----------------------------------------------------------------------|
* !!!/Dobrze dzialajacy  rules, collectibles 
* !!! uporządkować pozostałe pliku h
* !!! Sprawdzic czemu postac nie laduje sie na srodku, tylko troche na boku(pewnie image jeszcze nie jest zaladowany i dlatego)
* !!!Kolizje moze zrobic na nowo zeby to lepiej dzialalo
* !!!Metody fabrykujace, zmienic dzialanie gdzie startuja tearsy przy tworzeniu - da sie w paru miejscach wyjebac casy chyba
* !!sprobowac zrobic ten setw na tych napisach w highest scores
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali
* !Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !ladowanie statystyk potworow z pliku tekstowego
*/

/*|---Rzeczy-z-Labow------------------------------------------------------------|
* <thread> wątek do zastopowania nowej fali i obslugi bledow w LoginMenu
* <regex> logowanie sie do gierki
* <filesystem> ladowanie tekstur
* <module> strukutra CharacterStats
*/
