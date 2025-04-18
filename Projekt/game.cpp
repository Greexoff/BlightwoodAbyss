#include "raylib.h"
#include "game.h"
#include <thread>
#include <memory>

using namespace std;

Game::Game() {
	object_assets = fs::current_path() / "assets" / "object_assets";
	backgroundPath = fs::current_path() / "assets" / "background_assets"/ "backgroundOG.png";
	loadTextures();
	amountofEnemies = 5;
	waveNumber = 1;
	enemyShootingGap = 1.5;
	enemyHittingGap = 2;
	lastTearFired = 0.0;
	lastTimePlayerWasTouched = 0.0;	
	isCreatingNewWave = false;
	proceedCreatingEnemies = false;
	playerTotalScore = 0;
	enemies = CreateEnemy();
}
Game::~Game() {
	enemies.clear();
	for (auto& i : loadedTextures)
	{
		UnloadTexture(i.second);
	}
	
}
void Game::DrawBackground()
{
	DrawTexture(passCorrectTexture("backgroundOG.png"), 0, 0, WHITE);
}
void Game::setPlayerCharacter(int Character)
{
	switch (Character)
	{
	case 0:
		Player = make_unique<FirstCharacter>(passCorrectTexture("FirstCharacter.png"));
		playerTearType = "PlayerRedTear.png";
		break;
	case -1:
		Player = make_unique<SecondCharacter>(passCorrectTexture("SecondCharacter.png"));
		playerTearType = "PlayerGreenTear.png";
		break;
	case 1:
		Player = make_unique<ThirdCharacter>(passCorrectTexture("ThirdCharacter.png"));
		playerTearType = "PlayerBlueTear.png";
		break;
	default:
		Player = make_unique<FirstCharacter>(passCorrectTexture("FirstCharacter.png"));
		playerTearType = "PlayerRedTear.png";
		break;
	}
}
void Game::DrawPlayerCharacterImage(int Character)
{
	string file_name = "";
	switch (Character)
	{
	case 0:
		file_name = "FirstCharacter.png";
		break;
	case -1:
		file_name = "SecondCharacter.png";
		break;
	case 1:
		file_name = "ThirdCharacter.png";
		break;
	default:
		file_name = "FirstCharacter.png";
		break;
	}
	DrawTextureEx(passCorrectTexture(file_name), { 628,535 }, 0, 2, WHITE);
}
void Game::Update() {
	if (proceedCreatingEnemies)
	{
		disablePlayerTears();
		enemies = CreateEnemy();
		lastTimePlayerWasTouched = GetTime();
		proceedCreatingEnemies = false;
	}
	for (auto & tears : Player->tearsy)
	{		
		tears.UpdatePosition();
	}
	MoveEnemies();
	EnemyShootTears();
	for (auto& enTears : EnemyTears)
	{
		enTears.UpdatePosition(Player->GetXYPlayerPoint());
	}
	CollisionCheck();
	DeleteInactiveTears();
	if (enemies.empty() && !isCreatingNewWave)
	{
		isCreatingNewWave = true;
		thread t(&Game::beginNewWave,this);
		t.detach();
	}
}
void Game::Draw() {
	if (Loot != nullptr)
	{
		Loot->DrawItems();
	}
	Player->DrawPlayerHealthBar();
	Player->Draw();	
	for (auto& tears : Player->tearsy) {
		tears.Draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->Draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->DrawEnemyHealthBar();
	}
	for (auto& enemTears : EnemyTears)
	{
		enemTears.Draw();
	}
	GameUI::GetInstance().DisplayCharacterStats(Player->getPlayerSpeed(), Player->getTearSpeed(), Player->getPlayerDamage(), Player->getTearRate());
}
void Game::InputHandle() {
	int moveX = 0;
	int moveY = 0;
	if (IsKeyDown(KEY_A)){moveX = -1;}
	if (IsKeyDown(KEY_D)){moveX = 1;}
	if (IsKeyDown(KEY_W)){moveY = -1;}
	if (IsKeyDown(KEY_S)){moveY = 1;}
	Player->movePlayer(moveX, moveY);
	if (IsKeyDown(KEY_UP)) {
		Player->shootTears(0, -1, passCorrectTexture(playerTearType));
	}
	if (IsKeyDown(KEY_DOWN)) {
		Player->shootTears(0, 1, passCorrectTexture(playerTearType));
	}
	if (IsKeyDown(KEY_LEFT)) {
		Player->shootTears(-1, 0, passCorrectTexture(playerTearType));
	}
	if (IsKeyDown(KEY_RIGHT)) {
		Player->shootTears(1, 0, passCorrectTexture(playerTearType));
	}
}
void Game::DeleteInactiveTears()
{
	for (auto it = Player->tearsy.begin(); it != Player->tearsy.end();)
	{
		if (!it->active)
		{
			it = Player->tearsy.erase(it);
		}
		else
		{
			++it;
		}
	}
	for (auto it = EnemyTears.begin(); it != EnemyTears.end();)
	{
		if (!it->active)
		{
			it = EnemyTears.erase(it); 
		}
		else
		{
			++it;
		}
	}
}
vector <shared_ptr<Enemy>> Game::CreateEnemy()
{
	vector<shared_ptr<Enemy>> enemiesy;
	int poolOfEnemiesTypes = 3;
	if (waveNumber % 5 == 0)
	{
		Vector2 position = { GetRandomValue(300,GetScreenWidth() - 300), GetRandomValue(300,GetScreenHeight() - 300) };
		enemiesy.push_back(make_shared<Monster5>(position, passCorrectTexture("Enemy5.png")));
		return enemiesy;
	}
	int amountOfMiniBoss=0;
	for (int i = 0; i < amountofEnemies; i++) {
		Vector2 position = { GetRandomValue(300,GetScreenWidth() - 300), GetRandomValue(300,GetScreenHeight() - 300) };
		if (waveNumber >= 5 && amountOfMiniBoss==0)
		{
			poolOfEnemiesTypes = 4;
		}
		if (amountOfMiniBoss >= 3)
		{
			poolOfEnemiesTypes = 3;
		}
		int type = GetRandomValue(1, poolOfEnemiesTypes);
		switch (type) {
		case 1:
			enemiesy.push_back(make_shared<Monster1>(position, passCorrectTexture("Enemy1.png")));
			break;
		case 2:
			enemiesy.push_back(make_shared<Monster2>(position, passCorrectTexture("Enemy2.png")));
			break;
		case 3:
			enemiesy.push_back(make_shared<Monster3>(position, passCorrectTexture("Enemy3.png")));
			break;
		case 4:
			enemiesy.push_back(make_shared<Monster4>(position, passCorrectTexture("Enemy4.png")));
			amountOfMiniBoss++;
			break;
		default:
			break;
		}

	}
	return enemiesy;
}
void Game::MoveEnemies()
{
	for (auto& enemy : enemies) {
		if (!dynamic_pointer_cast<Monster3>(enemy)) {
			enemy->Update(Player->GetXYPlayerPoint());
		}
	}
}
void Game::EnemyShootTears()//Tutaj jeszcze zrobic jakas metode fabrykujaca i w tym getEnemyPosition zmienic dzialanie tego, tak zeby te strzaly lecialy fajnie ze srodka
{
	if (!enemies.empty()) {
		if (GetTime() - lastTearFired >= enemyShootingGap)
		{
			int randomInd = GetRandomValue(0, enemies.size() - 1);
			shared_ptr <Enemy> enem = enemies[randomInd];
			if (auto monsterPtr = dynamic_pointer_cast<Monster3>(enem))
			{
				EnemyTears.push_back(enemyTears(monsterPtr->getEnemyPosition(4.0), enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), passCorrectTexture("Enemy3Tear.png")));
			}
			if (auto monsterPtr = dynamic_pointer_cast<Monster4>(enem))
			{
				EnemyTears.push_back(enemyTears(monsterPtr->getEnemyPosition(4.0), enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), passCorrectTexture("Enemy4Tear.png")));
			}
			if (auto monsterPtr = dynamic_pointer_cast<Monster5>(enem))
			{
				EnemyTears.push_back(enemyTears(monsterPtr->getEnemyPosition(2.0), enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), passCorrectTexture("Enemy5Tear.png")));
			}
			lastTearFired = GetTime();
		}
	}

}
void Game::CollisionCheck()
{
	if (Loot != nullptr && CheckCollisionRecs(Player->getPlayerRect(), Loot->getItemRect()))
	{
		//TUTAJ JESZCZE JAKAS FUNKCJA DO WYSWIETLANIA NAPISU ZE PODNIESIONO X I CO TO DAJE JAK JUZ BEDE MIAL INTERFEJS GRAFICZNY TO TA METODE NA PUBLIC  CZY CUS
		Loot->applyEffect(Player.get());
		Loot.reset();
	}
	if (!enemies.empty()) {
		for (auto& tear : Player->tearsy)
		{
			auto it = enemies.begin();
			while (it != enemies.end())
			{
				if (CheckCollisionRecs((*it)->getEnemyRect(), tear.getTearRect()))
				{
					(*it)->setEnemyHealth(Player->getPlayerDamage());
					if ((*it)->getEnemyHealth() <= 0)
					{
						increasePlayerTotalScore((*it)->getEnemyScore());
						if (waveNumber % 5 == 0)
						{
							createRandomLoot((*it)->getEnemyPosition(2.0));
						}
						it = enemies.erase(it);
					}
					else
					{
						++it;
					}
					tear.active = false;
				}
				else
				{
					++it;
				}

			}
		}
		auto it = enemies.begin();
		while (it != enemies.end())
		{
			if (GetTime() - lastTimePlayerWasTouched > enemyHittingGap && CheckCollisionRecs((*it)->getEnemyRect(), Player->getPlayerRect()))
			{
				Player->setPlayerHealth(-1);

				lastTimePlayerWasTouched = GetTime();
			}
			else
			{
				++it;
			}
		}
	}
	for (auto& enemTear : EnemyTears)
	{
		if (CheckCollisionRecs(enemTear.getTearRect(), Player->getPlayerRect()))
		{
			if (GetTime() - lastTimePlayerWasTouched > enemyHittingGap) {
				Player->setPlayerHealth(-1);
				lastTimePlayerWasTouched = GetTime();
			}
			enemTear.active = false;
		}
	}
	for (size_t i = 0; i < enemies.size(); i++)
	{
		for (size_t j = i + 1; j < enemies.size(); j++)
		{
			if ((CheckCollisionRecs(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()))) {
				if (!dynamic_pointer_cast<Monster3>(enemies[i]) || !dynamic_pointer_cast<Monster3>(enemies[j]))
				{
					enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
				//	continue;
				//	enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
				}
				/*if (!dynamic_pointer_cast<Monster3>(enemies[j])) {
						enemies[j]->UpdateColl(enemies[j]->getCollisionSide(enemies[j]->getEnemyRect(), enemies[i]->getEnemyRect()));
				}*/
				//enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
			}
		}
	}
}
bool Game::isGameOver()
{
	if (Player->getPlayerHealth() == 0)
	{
		cout << "Koniec gry" << endl;
		return true;
	}
	else
	{
		return false;
	}
}
void Game::beginNewWave()
{
	Player->setPlayerHealth(1);
	disableEnemyTears();
	increasePlayerTotalScore(200 * waveNumber);
	this_thread::sleep_for(chrono::seconds(3));
	waveNumber++;
	if (waveNumber < 11)
	{
		amountofEnemies++;
	}
	proceedCreatingEnemies = true;
	isCreatingNewWave = false;
}
void Game::disableEnemyTears()
{
	for (auto& enemTears : EnemyTears)
	{
		enemTears.active = false;
	}
}
void Game::disablePlayerTears()
{
	for (auto& playerTears : Player->tearsy)
	{
		playerTears.active = false;
	}
}
void Game::increasePlayerTotalScore(int amount)
{
	playerTotalScore += amount;
}
void Game::createRandomLoot(Vector2 enemyPos)
{
	int type=GetRandomValue(1,5);
	switch (type)
	{
	case 1:
		Loot = make_unique<DamageTrinket>(passCorrectTexture("DamageTrinket.png"), enemyPos);
		break;
	case 2:
		Loot = make_unique<TearRateTrinket>(passCorrectTexture("TearRateTrinket.png"), enemyPos);
		break;
	case 3:
		Loot = make_unique<SpeedTrinket>(passCorrectTexture("SpeedTrinket.png"), enemyPos);
		break;
	case 4:
		Loot = make_unique<HealthTrinket>(passCorrectTexture("HealthTrinket.png"), enemyPos);
		break;
	case 5:
		Loot = make_unique<TearSpeedTrinket>(passCorrectTexture("TearSpeedTrinket.png"), enemyPos);
		break;
	default:
		break;
	}
	
}
void Game::setLastTimePlayerWasTouched()
{
	lastTimePlayerWasTouched = GetTime();
}
void Game::loadTextures()
{
	loadedTextures.insert({ backgroundPath.filename().string(),LoadTexture(backgroundPath.string().c_str())});
	for (const auto& textureName : fs::directory_iterator(object_assets))
	{
		loadedTextures.insert({ textureName.path().filename().string(), LoadTexture(textureName.path().string().c_str()) });
	}
}
Texture2D Game::passCorrectTexture(string textureName)
{
	for (auto& checking : loadedTextures)
	{
		if (checking.first == textureName)
		{
			return checking.second;
		}
	}
}
