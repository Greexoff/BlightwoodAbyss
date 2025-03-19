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
	lastTearFired = 0.0;
	
}
Game::~Game() {

}
void Game::UpdateLocation() {
	for (auto & tears : Postac.tearsy)
	{		
		tears.UpdatePosition();
	}
	EnemyShootTears();
	MoveEnemies();
	for (auto& enTears : EnemyTears)
	{
		enTears.UpdatePosition(Postac.GetPlayerPosition());
	}
	CollisionCheck();
	DeleteInactiveTears();
	if (enemies.empty())
	{
		amountofEnemies++;
		enemies = CreateEnemy();
	}
}
void Game::Draw() {
	Postac.Draw();
	for (auto& tears : Postac.tearsy) {
		tears.Draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->Draw();
	}
	for (auto& enemTears : EnemyTears)
	{
		enemTears.Draw();
	}
}
void Game::InputHandle() {
	if (IsKeyDown(KEY_A))
	{
		Postac.moveLeft();
	}
	if (IsKeyDown(KEY_D))
	{
		Postac.moveRight();
	}
	if (IsKeyDown(KEY_W))
	{
		Postac.moveUp();
	}
	if (IsKeyDown(KEY_S))
	{
		Postac.moveDown();
	}
	if (IsKeyDown(KEY_UP))
	{
		Direction = 'u';
		Postac.shootTears(Direction);
	}
	if (IsKeyDown(KEY_DOWN))
	{
		Direction = 'd';
		Postac.shootTears(Direction);
	}
	if (IsKeyDown(KEY_LEFT))
	{
		Direction = 'l';
		Postac.shootTears(Direction);
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		Direction = 'r';
		Postac.shootTears(Direction);
	}
}
void Game::DeleteInactiveTears()
{
	for (auto it = Postac.tearsy.begin(); it != Postac.tearsy.end();)
	{
		if (!it->active)
		{
			it = Postac.tearsy.erase(it);
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
			if (Postac.GetPlayerPosition().x > enemy->position.x)
			{
				EnemyDirectionX = 1;
			}
			if (Postac.GetPlayerPosition().x < enemy->position.x)
			{
				EnemyDirectionX = -1;
			}
			if (Postac.GetPlayerPosition().y > enemy->position.y)
			{
				EnemyDirectionY = 1;
			}
			if (Postac.GetPlayerPosition().y < enemy->position.y)
			{
				EnemyDirectionY = -1;
			}
			enemy->Update(Postac.GetPlayerPosition(),EnemyDirectionX,EnemyDirectionY);
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
				EnemyTears.push_back(enemyTears({ monsterPtr->position.x + (monsterPtr->image.width / 4),monsterPtr->position.y + (monsterPtr->image.height / 4) }, 3, Postac.GetPlayerPosition()));//tu ta 3 zmienic zminna ktora bedzie zalezala od typu przeciwnika
			}
			lastTearFired = GetTime();
		}
	}

}
void Game::CollisionCheck()
{
	for (auto& tear : Postac.tearsy)
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
	for (auto& enemTear : EnemyTears)
	{
		if (CheckCollisionRecs(enemTear.getTearRect(), Postac.getPlayerRect()))
		{
			Postac.setPlayerHealth();
			enemTear.active = false;
		}
	}
	
	for (size_t i = 0; i < enemies.size(); i++)//sprobowac poprawic zeby nie miec 3petli wewnatrz siebie
	{
		for (size_t j = i + 1; j < enemies.size(); j++)
		{
			while(CheckCollisionRecs(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()))
			{
				if (!dynamic_pointer_cast<Monster3>(enemies[i]))
				{
					enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
					
				}
				if(!dynamic_pointer_cast<Monster3>(enemies[j]))
				{
					enemies[j]->UpdateColl(enemies[j]->getCollisionSide(enemies[j]->getEnemyRect(), enemies[i]->getEnemyRect()));
				}		
			}
		}
	}
}
bool Game::isGameOver()
{
	if (Postac.getPlayerHealth() == 0)
	{
		cout << "Koniec gry" << endl;
		return true;
	}
	else
	{
		return false;
	}
}