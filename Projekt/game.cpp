#include <iostream>
#include <memory>
#include "raylib.h"
#include "game.h"

using namespace std;

Game::Game() {
	amountofEnemies = 5;
	enemies = CreateEnemy();
	EnemyDirectionX = 0;
	EnemyDirectionY = 0;
	enemyShootingGap = 1.5;
	enemyHittingGap = 3;
	lastTearFired = 0.0;
	lastTimePlayerWasTouched = 0.0;
	
}
Game::~Game() {

}
void Game::Update() {
	for (auto & tears : Player.tearsy)
	{		
		tears.UpdatePosition();
	}
	EnemyShootTears();
	MoveEnemies();
	for (auto& enTears : EnemyTears)
	{
		enTears.UpdatePosition(Player.GetPlayerPosition());
	}
	CollisionCheck();
	DeleteInactiveTears();
	beginNextWave();
}
void Game::Draw() {
	Player.Draw();
	Player.DrawPlayerHealthBar();
	for (auto& tears : Player.tearsy) {
		tears.Draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->Draw();
		enemy->DrawEnemyHealthBar();
	}
	for (auto& enemTears : EnemyTears)
	{
		enemTears.Draw();
	}
}
void Game::InputHandle() {
	if (IsKeyDown(KEY_A))
	{
		Player.moveLeft();
	}
	if (IsKeyDown(KEY_D))
	{
		Player.moveRight();
	}
	if (IsKeyDown(KEY_W))
	{
		Player.moveUp();
	}
	if (IsKeyDown(KEY_S))
	{
		Player.moveDown();
	}
	if (IsKeyDown(KEY_UP))
	{
		Direction = 'u';
		Player.shootTears(Direction);
	}
	if (IsKeyDown(KEY_DOWN))
	{
		Direction = 'd';
		Player.shootTears(Direction);
	}
	if (IsKeyDown(KEY_LEFT))
	{
		Direction = 'l';
		Player.shootTears(Direction);
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		Direction = 'r';
		Player.shootTears(Direction);
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
		Vector2 position = { GetRandomValue(100,1036), GetRandomValue(100,638) };
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
	for (auto& enemy : enemies)
	{
		if (!dynamic_pointer_cast<Monster3>(enemy)) {//tutaj mozna zrobic thread? zeby wykonywal jednoczesnie sprawdzanie dla x i dla y
			if (Player.GetPlayerPosition().x > enemy->position.x)
			{
				EnemyDirectionX = 1;
			}
			if (Player.GetPlayerPosition().x < enemy->position.x)
			{
				EnemyDirectionX = -1;
			}
			if (Player.GetPlayerPosition().y > enemy->position.y)
			{
				EnemyDirectionY = 1;
			}
			if (Player.GetPlayerPosition().y < enemy->position.y)
			{
				EnemyDirectionY = -1;
			}
			enemy->Update(Player.GetPlayerPosition(),EnemyDirectionX,EnemyDirectionY,enemy->getEnemySpeed());
		}
	}
}
void Game::EnemyShootTears()
{
	if (!enemies.empty()) {
		double currTime = GetTime();
		if (currTime - lastTearFired >= enemyShootingGap)
		{
			int randomInd = GetRandomValue(0, enemies.size() - 1);
			shared_ptr <Enemy> enem = enemies[randomInd];
			if (auto monsterPtr = dynamic_pointer_cast<Monster3>(enem))
			{
				EnemyTears.push_back(enemyTears({ monsterPtr->position.x + (monsterPtr->image.width / 4),monsterPtr->position.y + (monsterPtr->image.height / 4) }, 3, Player.GetPlayerPosition()));//tu ta 3 zmienic zminna ktora bedzie zalezala od typu przeciwnika
			}
			lastTearFired = GetTime();
		}
	}

}
void Game::CollisionCheck()
{
	for (auto& tear : Player.tearsy)
	{
		if (enemies.empty()) {
			break;
		}
		auto it = enemies.begin();
		while (it != enemies.end())
		{
			if (CheckCollisionRecs((*it)->getEnemyRect(), tear.getTearRect()))
			{
				(*it)->setEnemyHealth();
				if ((*it)->getEnemyHealth() == 0)
				{
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
	double currentTime = GetTime();
	auto it = enemies.begin();
	while (it != enemies.end())
	{	
		if (CheckCollisionRecs((*it)->getEnemyRect(), Player.getPlayerRect())&&currentTime-lastTimePlayerWasTouched>enemyHittingGap)
		{
			Player.reducePlayersHealth();

			lastTimePlayerWasTouched = GetTime();
		}
		else
		{
			++it;
		}
	}
	for (auto& enemTear : EnemyTears)
	{
		if (CheckCollisionRecs(enemTear.getTearRect(), Player.getPlayerRect()))
		{
			if (currentTime - lastTimePlayerWasTouched > enemyHittingGap) {
				Player.reducePlayersHealth();
				lastTimePlayerWasTouched = GetTime();
			}
			enemTear.active = false;
		}
	}
	
	for (size_t i = 0; i < enemies.size(); i++)//sprobowac poprawic zeby nie miec 3petli wewnatrz siebie, jeszcze poprawic calosc bo nwm czy chce zeby to tak funkcjonowało
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
void Game::beginNextWave()
{
	if (enemies.empty())
	{
		//Tutaj jakas funkcja do wyswietlania komunikatu ze ukonczono fale; Do wykminienia jak robic przerwe miedzy falami
			Player.increasePlayersHealth();
			for (auto& enemTears : EnemyTears)
			{
				enemTears.active = false;
			}
			for (auto& playerTears : Player.tearsy)
			{
				playerTears.active = false;
			}
			amountofEnemies++;
			enemies = CreateEnemy();

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