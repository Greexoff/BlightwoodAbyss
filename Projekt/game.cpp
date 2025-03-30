#include "raylib.h"
#include "game.h"
#include <thread>
#include <memory>

using namespace std;

Game::Game() {
	textureNames = {
	"potwor1.png", "potwor2.png", "potwor3.png", "potwor4.png", "potwor5.png",
	"DamageTrinket.png", "TearRateTrinket.png", "SpeedTrinket.png", "HealthTrinket.png", "TearSpeedTrinket.png", "tear.png", "EnemyTears.png", "FirstCharacter.png", "SecondCharacter.png"};
	loadTexturesIntoVector();
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
	for (int i = 0; i < loadImages.size(); i++)
	{
		UnloadTexture(loadImages[i]);
	}
}
void Game::setPlayerCharacter(int Character)
{
	switch (Character)
	{
	case 1:
		Player = make_unique<FirstCharacter>(loadImages[12]);
		break;
	case 2:
		Player = make_unique<SecondCharacter>(loadImages[13]);
		break;
	case 3:
		Player = make_unique<ThirdCharacter>(loadImages[12]);//zamienic na 14 jak bede mial texture
		break;
	default:
		Player = make_unique<FirstCharacter>(loadImages[12]);
		break;
	}
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
		Player->shootTears(0, -1, loadImages[10]);
	}
	if (IsKeyDown(KEY_DOWN)) {
		Player->shootTears(0, 1, loadImages[10]);
	}
	if (IsKeyDown(KEY_LEFT)) {
		Player->shootTears(-1, 0, loadImages[10]);
	}
	if (IsKeyDown(KEY_RIGHT)) {
		Player->shootTears(1, 0, loadImages[10]);
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
		Vector2 position = { GetRandomValue(100,GetScreenWidth() - 100), GetRandomValue(100,GetScreenHeight() - 100) };
		enemiesy.push_back(make_shared<Monster5>(position, loadImages[4]));
		return enemiesy;
	}
	int amountOfMiniBoss=0;
	for (int i = 0; i < amountofEnemies; i++) {
		Vector2 position = { GetRandomValue(100,GetScreenWidth() - 100), GetRandomValue(100,GetScreenHeight() - 100) };
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
			enemiesy.push_back(make_shared<Monster1>(position, loadImages[0]));
			break;
		case 2:
			enemiesy.push_back(make_shared<Monster2>(position, loadImages[1]));
			break;
		case 3:
			enemiesy.push_back(make_shared<Monster3>(position, loadImages[2]));
			break;
		case 4:
			enemiesy.push_back(make_shared<Monster4>(position, loadImages[3]));
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
void Game::EnemyShootTears()
{
	if (!enemies.empty()) {
		if (GetTime() - lastTearFired >= enemyShootingGap)
		{
			int randomInd = GetRandomValue(0, enemies.size() - 1);
			shared_ptr <Enemy> enem = enemies[randomInd];
			if (auto monsterPtr = dynamic_pointer_cast<Monster3>(enem))
			{
				EnemyTears.push_back(enemyTears({ monsterPtr->position.x + (monsterPtr->image.width / 4),monsterPtr->position.y + (monsterPtr->image.height / 4) }, enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), loadImages[11]));
			}
			if (auto monsterPtr = dynamic_pointer_cast<Monster4>(enem))
			{
				EnemyTears.push_back(enemyTears({ monsterPtr->position.x + (monsterPtr->image.width / 4),monsterPtr->position.y + (monsterPtr->image.height / 4) }, enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(),loadImages[11]));
			}
			if (auto monsterPtr = dynamic_pointer_cast<Monster5>(enem))
			{
				EnemyTears.push_back(enemyTears({ monsterPtr->position.x + (monsterPtr->image.width / 4),monsterPtr->position.y + (monsterPtr->image.height / 4) }, enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(),loadImages[11]));
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
							createRandomLoot((*it)->getEnemyPosition());
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
				Player->reducePlayersHealth();

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
				Player->reducePlayersHealth();
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
				if (!dynamic_pointer_cast<Monster3>(enemies[i]))
				{
					enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
				}
				if(!dynamic_pointer_cast<Monster3>(enemies[j])){ 
						enemies[j]->UpdateColl(enemies[j]->getCollisionSide(enemies[j]->getEnemyRect(), enemies[i]->getEnemyRect()));
				}
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
	Player->increasePlayersHealth();
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
		Loot = make_unique<DamageTrinket>(loadImages[5], enemyPos);
		break;
	case 2:
		Loot = make_unique<TearRateTrinket>(loadImages[6], enemyPos);
		break;
	case 3:
		Loot = make_unique<SpeedTrinket>(loadImages[7], enemyPos);
		break;
	case 4:
		Loot = make_unique<HealthTrinket>(loadImages[8], enemyPos);
		break;
	case 5:
		Loot = make_unique<TearSpeedTrinket>(loadImages[9], enemyPos);
		break;
	default:
		break;
	}
	
}
void Game::setLastTimePlayerWasTouched()
{
	lastTimePlayerWasTouched = GetTime();
}
void Game::loadTexturesIntoVector()
{
	for (const auto& textureFile : textureNames) {
		loadImages.push_back(LoadTexture(textureFile));
	}
}
