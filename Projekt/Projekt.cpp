
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include "raylib.h"
#include "game.h"
#include "menuhub.h"
#include "UI.h"
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

	StartingMenu* startingTab = nullptr;
	LoginMenu* loginTab = nullptr;
	MainMenu* mainTab = nullptr;
	RulesMenu* rulesTab = nullptr;
	UnlockedItemsMenu* unlockedTab = nullptr;
	HighestScoreMenu* scoresTab = nullptr;
	CharacterSelectionMenu* charTab = nullptr;
	AfterGameMenu* afterTab = nullptr;
	Game* game = nullptr;
	string scoreText = "";
	bool shouldEnd = false;
	int setAction=0;
	string keepWaveNumberText = "";
	string keepPlayerScoreText = "";
	CurrentState gameState = CurrentState::LOADING;
	while (!WindowShouldClose()&& !shouldEnd){
		BeginDrawing();
		switch (gameState)
		{
		case CurrentState::LOADING:
			startingTab = new StartingMenu();
			loginTab = new LoginMenu();
			mainTab = new MainMenu();
			rulesTab = new RulesMenu();
			unlockedTab = new UnlockedItemsMenu();
			scoresTab = new HighestScoreMenu();
			charTab = new CharacterSelectionMenu();
			gameState = CurrentState::STARTING_MENU;
			break;
		case CurrentState::STARTING_MENU:
			startingTab->Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				gameState = CurrentState::LOGIN_MENU;
			}
			break;
		case CurrentState::LOGIN_MENU:
			loginTab->Draw();
			loginTab->handleLoginMenuLogic(setAction,gameState);
			break;
		case CurrentState::MAIN_MENU:
			mainTab->Draw();
			mainTab->handleMainMenuLogic(setAction, gameState, shouldEnd);
			break;
		case CurrentState::CHARACTER_SELECT_MENU:
			charTab->Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				charTab->isButtonClicked(gameState);
				if (gameState == CurrentState::GAMEPLAY) {
					game = new Game();
					game->setPlayerCharacter(charTab->getPageNumber());
					game->setLastTimePlayerWasTouched();
				}
			}
			break;
		case CurrentState::RULES_MENU:
			rulesTab->Draw();
			rulesTab->ReturnToMenu(gameState);
			break;
		case CurrentState::UNLOCKED_ITEMS_MENU:
			unlockedTab->Draw();
			unlockedTab->ReturnToMenu(gameState);
			break;
		case CurrentState::SCORE_MENU:
			scoresTab->Draw();
			scoresTab->handleScoresMenuLogic();
			scoresTab->ReturnToMenu(gameState);
			break;
		case CurrentState::GAMEPLAY:
			game->InputHandle();
			game->Update();
			game->DrawBackground();
			game->Draw();
			keepWaveNumberText =GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(game->getWaveNumber(), 3, "WAVE");
			keepPlayerScoreText =GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(game->playerTotalScore, 6, "SCORE");
			GameUI::GetInstance().DrawGameUI(keepPlayerScoreText + " "+ keepWaveNumberText, 60, GetScreenHeight()-30);
			if (game->isGameOver())
			{
				afterTab = new AfterGameMenu();
				delete game;
				gameState = CurrentState::AFTERGAME_MENU;
			}
			break;
		case CurrentState::AFTERGAME_MENU:
			afterTab->Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				afterTab->isButtonClicked(gameState);
			}
			break;
		case CurrentState::END:
			shouldEnd = true;
			break;
		default:
			break;
		}
		EndDrawing();
	}
	CloseWindow();
	delete startingTab;
	delete loginTab;
	delete mainTab;
	delete rulesTab;
	delete unlockedTab;
	delete scoresTab;
	delete charTab;
}
/*|---TODO-----------------------------------------------------------------------|
* !!!!sprobowac zrobic ten setw na tych napisach w highest scores
* !!!!GameUI todo:Sprawdzac czy to highest score gracza i jesli jest to zapisywac go do pliku DataBase.txt        Dobrze dzialajacy loadingScreen
* !!!/rules, collectibles
* * !!!Oczyścić main i uporządkować pozostałe pliku h
* !!!ladowanie statystyk potworow z pliku tekstowego
* !!!Kolizje moze zrobic na nowo zeby to lepiej dzialalo
* !!!Metody fabrykujace, zmienic dzialanie gdzie startuja tearsy przy tworzeniu
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
