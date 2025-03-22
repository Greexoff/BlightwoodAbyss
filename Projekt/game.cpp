#include "raylib.h"
#include "game.h"
#include <thread>

using namespace std;

Game::Game() {
	amountofEnemies = 5;
	enemies = CreateEnemy();
	enemyShootingGap = 1.5;
	enemyHittingGap = 5;
	lastTearFired = 0.0;
	lastTimePlayerWasTouched = 0.0;	
	isCreatingNewWave = false;
	proceedCreatingEnemies = false;
	waveNumber = 1;
	playerTotalScore = 0;
}
Game::~Game() {
	enemies.clear();
}
void Game::Update() {
	if (proceedCreatingEnemies)
	{
		disablePlayerTears();
		enemies = CreateEnemy();
		lastTimePlayerWasTouched = GetTime();
		proceedCreatingEnemies = false;
	}
	for (auto & tears : Player.tearsy)
	{		
		tears.UpdatePosition();
	}
	MoveEnemies();
	EnemyShootTears();
	for (auto& enTears : EnemyTears)
	{
		enTears.UpdatePosition(Player.GetXYPlayerPoint());
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
	Player.DrawPlayerHealthBar();
	Player.Draw();	
	for (auto& tears : Player.tearsy) {
		tears.Draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->DrawEnemyHealthBar();
		enemy->Draw();
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
	Player.movePlayer(moveX, moveY);
	if (IsKeyDown(KEY_UP)) {
		Player.shootTears(0, -1);
	}
	if (IsKeyDown(KEY_DOWN)) {
		Player.shootTears(0, 1);
	}
	if (IsKeyDown(KEY_LEFT)) {
		Player.shootTears(-1, 0);
	}
	if (IsKeyDown(KEY_RIGHT)) {
		Player.shootTears(1, 0);
	}
}
void Game::DeleteInactiveTears()
{
	for (auto it = Player.tearsy.begin(); it != Player.tearsy.end();)
	{
		if (!it->active)
		{
			it = Player.tearsy.erase(it);
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

	for (int i = 0; i < amountofEnemies; i++) {
		Vector2 position = { GetRandomValue(100,GetScreenWidth()-100), GetRandomValue(100,GetScreenHeight()-100) };
		int type = GetRandomValue(1,3);

		switch (type) {
		case 1:
			enemiesy.push_back(make_shared<Monster1>(position));  
			break;
		case 2:
			enemiesy.push_back(make_shared<Monster2>(position)); 
			break;
		case 3:
			enemiesy.push_back(make_shared<Monster3>(position));  
			break;
		}
	}
	return enemiesy;
}
void Game::MoveEnemies()
{
	for (auto& enemy : enemies) {
		int enemyDirectionX = 0;
		int enemyDirectionY = 0;
		if (!dynamic_pointer_cast<Monster3>(enemy)) {
			enemyDirectionX = (Player.GetXYPlayerPoint().x> enemy->position.x) ? 1 : (Player.GetXYPlayerPoint().x< enemy->position.x) ? -1 : 0;
			enemyDirectionY = (Player.GetXYPlayerPoint().y> enemy->position.y) ? 1 : (Player.GetXYPlayerPoint().y < enemy->position.y) ? -1 : 0;
			enemy->Update(Player.GetXYPlayerPoint(), enemyDirectionX, enemyDirectionY, enemy->getEnemySpeed());
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
				EnemyTears.push_back(enemyTears({ monsterPtr->position.x + (monsterPtr->image.width / 4),monsterPtr->position.y + (monsterPtr->image.height / 4) }, enem->getEnemyAttackSpeed(), Player.GetXYPlayerPoint()));//tu ta 3 zmienic zminna ktora bedzie zalezala od typu przeciwnika
			}
			lastTearFired = GetTime();
		}
	}

}
void Game::CollisionCheck()
{
	if (!enemies.empty()) {
		for (auto& tear : Player.tearsy)
		{
			auto it = enemies.begin();
			while (it != enemies.end())
			{
				if (CheckCollisionRecs((*it)->getEnemyRect(), tear.getTearRect()))
				{
					(*it)->setEnemyHealth();
					if ((*it)->getEnemyHealth() == 0)
					{
						increasePlayerTotalScore((*it)->getEnemyScore());
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
			if (CheckCollisionRecs((*it)->getEnemyRect(), Player.getPlayerRect()) && GetTime() - lastTimePlayerWasTouched > enemyHittingGap)
			{
				Player.reducePlayersHealth();

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
		if (CheckCollisionRecs(enemTear.getTearRect(), Player.getPlayerRect()))
		{
			if (GetTime() - lastTimePlayerWasTouched > enemyHittingGap) {
				Player.reducePlayersHealth();
				lastTimePlayerWasTouched = GetTime();
			}
			enemTear.active = false;
		}
	}
	
	for (size_t i = 0; i < enemies.size(); i++)
	{
		for (size_t j = i + 1; j < enemies.size(); j++)
		{
			if (!dynamic_pointer_cast<Monster3>(enemies[i]) || !dynamic_pointer_cast<Monster3>(enemies[j]))
			{
				if ((CheckCollisionRecs(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()))) {
				enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
				enemies[j]->UpdateColl(enemies[j]->getCollisionSide(enemies[j]->getEnemyRect(), enemies[i]->getEnemyRect()));
			}
			}
		}
	}
}
bool Game::isGameOver()
{
	if (Player.getPlayerHealth() == 0)
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
	Player.increasePlayersHealth();
	disableEnemyTears();
	disablePlayerTears();
	increasePlayerTotalScore(200 * waveNumber);
	this_thread::sleep_for(chrono::seconds(5));
	waveNumber++;
	amountofEnemies++;
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
	for (auto& playerTears : Player.tearsy)
	{
		playerTears.active = false;
	}
}
void Game::increasePlayerTotalScore(int amount)
{
	playerTotalScore += amount;
}
