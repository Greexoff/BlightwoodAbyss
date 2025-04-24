#include "enemy.h"

using namespace std;
//		Konstruktory i Destruktory Klas Enemy i pochodnych
//|---------------------------------------------------------------------------------------|
Enemy::~Enemy()
{
}
Monster1::Monster1(Vector2 position, Texture2D loadedImage)//Tank
{
	enemyName = "Monster1";
	image = loadedImage;
	this->position = position;
	loadEnemyStats();
}
Monster1::~Monster1() {}

Monster2::Monster2(Vector2 position, Texture2D loadedImage)//Szybki 
{
	enemyName = "Monster2";
	image = loadedImage;
	this->position = position;
	loadEnemyStats();
}
Monster2::~Monster2() {}

Monster3::Monster3(Vector2 position, Texture2D loadedImage)//Strzelajacy
{
	enemyName = "Monster3";
	image = loadedImage;
	this->position = position;
	loadEnemyStats();
}
Monster3::~Monster3() {}

Monster4::Monster4(Vector2 position, Texture2D loadedImage)//Strzelajacy Mini-Boss
{
	enemyName = "Monster4";
	image = loadedImage;
	this->position = position;
	loadEnemyStats();
}
Monster4::~Monster4() {}

Monster5::Monster5(Vector2 position, Texture2D loadedImage)//Boss
{
	enemyName = "Monster5";
	image = loadedImage;
	this->position = position;
	loadEnemyStats();
}
Monster5::~Monster5() {}
//|---------------------------------------------------------------------------------------|
void Enemy::Draw() {
	DrawTextureEx(image, position, 0, stats.imageScale, WHITE);
}	
void Enemy::Update(Vector2 PlayerPosition) {
	Vector2 dir = {0,0};
	float dx = PlayerPosition.x - position.x;
	float dy = PlayerPosition.y - position.y; 
	float length = sqrt(dx * dx + dy * dy);

	if (length != 0) {
		dir.x = dx / length;
		dir.y = dy / length;
	}
	position.x += dir.x * stats.enemySpeed;
	position.y += dir.y * stats.enemySpeed;
	
}	
void Enemy::UpdateColl(Vector2 Direction)
{
	position.x += stats.enemySpeed *Direction.x;
	position.y += stats.enemySpeed *Direction.y;
	if (position.y > GetScreenHeight()-GetScreenHeight()*0.25 - image.height * stats.imageScale)
	{
		position.y = GetScreenHeight() - GetScreenHeight() * 0.25 - image.height * stats.imageScale;
	}
	if (position.y < GetScreenHeight() * 0.25)
	{
		position.y = GetScreenHeight() * 0.25;
	}
	if (position.x < GetScreenHeight() * 0.25)
	{
		position.x = GetScreenHeight() * 0.25;
	}
	if (position.x > GetScreenWidth()- GetScreenHeight() * 0.25 - image.width * stats.imageScale)
	{
		position.x = GetScreenWidth() - GetScreenHeight() * 0.25 - image.width * stats.imageScale;
	}
}
Rectangle Enemy::getEnemyRect()
{
	return { position.x,position.y,(((float)image.width - 5 )* stats.imageScale),(((float)image.height - 5)* stats.imageScale)};
}
Vector2 Enemy::getCollisionSide(Rectangle enemy1, Rectangle enemy2)
{
	float dx = (enemy1.x + enemy1.width) - (enemy2.x + enemy2.width);
	float dy = (enemy1.y + enemy1.height) - (enemy2.y + enemy2.height);
	float totalWidth = (enemy1.width + enemy2.width);
	float totalHeight = (enemy1.height + enemy2.height);
	Vector2 Directions = { 0,0 };
	if (abs(dx) <= totalWidth && abs(dy) <= totalHeight) {
		float wy = totalWidth * dy;
		float hx = totalHeight * dx;
		if (wy > hx) {
			return (wy > -hx) ? Directions = {0, 1} : Directions = {-1,0};
		}
		else {
			return (wy > -hx) ? Directions = {1, 0} : Directions = {0, -1};
		}
	}
	return Directions;
}
int Enemy::getEnemyHealth()
{
	return stats.enemyHealth;
}
int Enemy::setEnemyHealth(int playerDamage)
{
	return stats.enemyHealth-=playerDamage;
}
float Enemy::getEnemySpeed()
{
	return stats.enemySpeed;
}
void Enemy::DrawEnemyHealthBar()
{
	float healthBarWidth = image.width* stats.imageScale;
	float healthBarHeight = 10;

	Vector2 healthBarPos = { position.x ,position.y - 15 };
	float healthPercent = (float)stats.enemyHealth / stats.maxEnemyHealth;
	float currentHealthWidth = (float)((healthBarWidth-10) * healthPercent);
	DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, BLACK);
	DrawRectangle(healthBarPos.x+5, healthBarPos.y+2.5, currentHealthWidth, (healthBarHeight/2), RED);
}
int Enemy::getEnemyAttackSpeed()
{
	return stats.enemyAttackSpeed;
}
int Enemy::getEnemyScore()
{
	return stats.enemyScore;
}
void Enemy::CheckOutofTheBorder()
{
	if (position.y > GetScreenHeight() - GetScreenHeight() * 0.25 -image.height* stats.imageScale)
	{
		position.y = GetScreenHeight() - GetScreenHeight() * 0.25 -image.height * stats.imageScale;
	}
	if (position.y < GetScreenHeight() * 0.25)
	{
		position.y = GetScreenHeight() * 0.25;
	}
	if (position.x < GetScreenHeight() * 0.25)
	{
		position.x = GetScreenHeight() * 0.25;
	}
	if (position.x > GetScreenWidth() - GetScreenHeight() * 0.25 -image.height * stats.imageScale)
	{
		position.x = GetScreenWidth() - GetScreenHeight() * 0.25 -image.height * stats.imageScale;
	}
}
Vector2 Enemy::getEnemyPosition(float divideBy)
{
	return { position.x+(image.width* stats.imageScale /divideBy),position.y+(image.height* stats.imageScale /divideBy) };
}
void Enemy::loadEnemyStats()
{
	for (const auto& [Name, enemyStats] : CharactersData::getInstance().getEnemyStats())
	{
		if (Name == enemyName)
		{
			stats = enemyStats;
		}
	}
}
