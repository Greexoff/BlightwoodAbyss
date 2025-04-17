
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include "raylib.h"
#include "game.h"
#include "menuhub.h"
#include "UI.h"

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
	StartingMenu* startingTab = nullptr;
	LoginMenu* loginTab = nullptr;
	MainMenu* mainTab = nullptr;
	RulesMenu* rulesTab = nullptr;
	UnlockedItemsMenu* unlockedTab = nullptr;
	HighestScoreMenu* scoresTab = nullptr;
	CharacterSelectionMenu* charTab = nullptr;
	Game* game = nullptr;
	string scoreText = "";
	bool shouldEnd = false;
	int setAction=0;
	int loadingStage = 0;
	CurrentState gameState = CurrentState::LOADING;
	while (!WindowShouldClose()&& !shouldEnd){
		BeginDrawing();
		switch (gameState)
		{
		case CurrentState::LOADING:
			switch (loadingStage) {
			case 0:
				GameUI::GetInstance().setLoadingProgress("backgroundLOADINGSTART.png");
				break;
			case 1:
				startingTab = new StartingMenu();
				GameUI::GetInstance().setLoadingProgress("backgroundLOADINGLOGIN.png");
				break;
			case 2:
				loginTab = new LoginMenu();
				GameUI::GetInstance().setLoadingProgress("backgroundLOADINGMENU.png");
				break;
			case 3:
				mainTab = new MainMenu();
				rulesTab = new RulesMenu();
				unlockedTab = new UnlockedItemsMenu();
				scoresTab = new HighestScoreMenu();
				GameUI::GetInstance().setLoadingProgress("backgroundLOADINGTABS.png");
				break;
			case 4:
				charTab = new CharacterSelectionMenu();
				GameUI::GetInstance().setLoadingProgress("backgroundLOADINGCHAR.png");
				break;
			case 5:
				game = new Game();
				GameUI::GetInstance().setLoadingProgress("backgroundLOADINGFULL.png");
				break;
			case 6:
				gameState = CurrentState::STARTING_MENU;
				break;
			}
			GameUI::GetInstance().DrawBackground();
			loadingStage++;
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
			game->DrawPlayerCharacterImage(charTab->getPageNumber());
			charTab->showExplanations();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				charTab->isButtonClicked(gameState);
				if (gameState == CurrentState::GAMEPLAY) {
					game->setPlayerCharacter(charTab->getPageNumber());
					game->setLastTimePlayerWasTouched();
				}
			}
			break;
		case CurrentState::RULES_MENU:
			rulesTab->Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && rulesTab->isReturnButtonClicked())
			{
				gameState = CurrentState::MAIN_MENU;
			}
			break;
		case CurrentState::UNLOCKED_ITEMS_MENU:
			unlockedTab->Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && unlockedTab->isReturnButtonClicked())
			{
				gameState = CurrentState::MAIN_MENU;
			}
			break;
		case CurrentState::SCORE_MENU:
			scoresTab->Draw();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && scoresTab->isReturnButtonClicked()) {
				gameState = CurrentState::MAIN_MENU;
			}
			break;
		case CurrentState::GAMEPLAY:
			game->InputHandle();
			game->Update();
			game->DrawBackground();
			game->Draw();
			GameUI::GetInstance().DrawCurrentScore(game->playerTotalScore, 60);
			if (game->isGameOver())
			{
				shouldEnd = true;
			}
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
	delete game;
}
/*
* Do zrobienia:
* !!!Kolizje moze zrobic na nowo zeby to lepiej dzialalo
* !!!Metody fabrykujace, zmienic dzialanie gdzie startuja tearsy przy tworzeniu
* !!!Dokonczyc highestscores(wyswieltanie), rules, collectibles
* !!/Sprawdzac czy to highest score gracza i jesli jest to zapisywac go do pliku DataBase.txt (np. dodac do highest scores funkcje getHighestScore(string username) i stad pobierac highest score gracza
* !!Przy spawnowaniu potworow robic checking czy sie nie pojawia na graczu, albo na innym juz istniejacym potworze, jezeli tak to jeszcze raz losowanie/przesuwanie az bedzie w innej pozycji
* !/Dodac jakies itemki, ktore wypadaja po mniejszych wrogach, ale daja mniejsze staty, a te dla bossa zwiekszyć?
* !Stworzyc mechanizm, ktory ulepsza potwory po jakiejs fali? PARTIALLY DONE
* !Dodac klase UI obslugujaca tworzenie tekstur potrzebny - score (prawie done), odliczanie do nastepnej fali, licznik fali, KONIECZNIE UI ZE STATSAMI
* 
* 
*/

/*
* Wykorzystane rzeczy z labów:
* <thread> wątek do zastopowania nowej fali. Mozna jeszcze cos przekminic ewentualnie
* <regex> logowanie sie do gierki
* <filesystem> ladowanie tekstur
* <module> strukutra CharacterStats
* 
* 
*/
