#include "enemy.h"

using namespace std;
//		Konstruktory i Destruktory Klas Enemy i pochodnych
//|---------------------------------------------------------------------------------------|
Enemy::Enemy(Vector2 position)
{
	this->position = position;
	enemyAttackSpeed = 0;
	enemyHealth = 0;
	enemySpeed = 0;
	enemyScore = 0;
	image = LoadTexture("potwor.png");
	
}
Enemy::~Enemy()
{
	UnloadTexture(image);
}
Monster1::Monster1(Vector2 position)//Tank
	: Enemy(position) {
	image = LoadTexture("potwor1.png");
	enemyMaxHealth = enemyHealth = 4;
	enemySpeed = 1;
	enemyScore = 300;
}

Monster1::~Monster1() { UnloadTexture(image); }

Monster2::Monster2(Vector2 position)//Szybki 
	: Enemy(position) {
	image = LoadTexture("potwor2.png");
	enemyMaxHealth = enemyHealth = 1;
	enemySpeed = 2;
	enemyScore = 100;
}

Monster2::~Monster2() { UnloadTexture(image); }

Monster3::Monster3(Vector2 position)//Strzelajacy
	: Enemy(position) {
	image = LoadTexture("potwor3.png");
	enemyMaxHealth = enemyHealth = 2;
	enemyAttackSpeed = 5;
	enemyScore = 200;
}

Monster3::~Monster3() { UnloadTexture(image); }

Monster4::Monster4(Vector2 position)//Strzelajacy Mini-Boss
	: Enemy(position) {
	image = LoadTexture("potwor4.png");
	enemyMaxHealth = enemyHealth = 10;
	enemySpeed = 1;
	enemyAttackSpeed = 3;
	enemyScore = 400;
}
Monster4::~Monster4() { UnloadTexture(image); }

Monster5::Monster5(Vector2 position)//Boss
	: Enemy(position) {
	image = LoadTexture("potwor5.png");
	enemyMaxHealth = enemyHealth = 30;
	enemySpeed = 0.5;
	enemyAttackSpeed = 5;
	enemyScore = 800;
}
Monster5::~Monster5() { UnloadTexture(image); }
//|---------------------------------------------------------------------------------------|
void Enemy::Draw() {
	DrawTextureV(image, position, WHITE);
}	
void Enemy::Update(Vector2 PlayerPosition) {
	Vector2 dir;
	float dx = PlayerPosition.x - position.x;
	float dy = PlayerPosition.y - position.y; 
	float length = sqrt(dx * dx + dy * dy);

	if (length != 0) {
		dir.x = dx / length;
		dir.y = dy / length;
	}
	position.x += dir.x * enemySpeed;
	position.y += dir.y * enemySpeed;
	
}	
void Enemy::UpdateColl(Vector2 Direction)
{
	position.x += enemySpeed*Direction.x;
	position.y += enemySpeed*Direction.y;
	if (position.y > GetScreenHeight() - image.height)
	{
		position.y = GetScreenHeight() - image.height;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > GetScreenWidth() - image.width)
	{
		position.x = GetScreenWidth() - image.width;
	}
}
Rectangle Enemy::getEnemyRect()
{
	return { position.x,position.y,(float)image.width,(float)image.height};
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
	return enemyHealth;
}
int Enemy::setEnemyHealth()
{
	return enemyHealth--;
}
float Enemy::getEnemySpeed()
{
	return enemySpeed;
}
void Enemy::DrawEnemyHealthBar()
{
	float healthBarWidth = 60;
	float healthBarHeight = 10;

	Vector2 healthBarPos = { position.x ,position.y - 15 };
	float healthPercent = (float)enemyHealth / enemyMaxHealth;
	float currentHealthWidth = (float)((healthBarWidth-10) * healthPercent);
	DrawRectangle(healthBarPos.x, healthBarPos.y, healthBarWidth, healthBarHeight, BLACK);
	DrawRectangle(healthBarPos.x+5, healthBarPos.y+2.5, currentHealthWidth, (healthBarHeight/2), RED);
}
int Enemy::getEnemyAttackSpeed()
{
	return enemyAttackSpeed;
}
int Enemy::getEnemyScore()
{
	return enemyScore;
}
void Enemy::CheckOutofTheBorder()
{
	if (position.y > GetScreenHeight() - image.height)
	{
		position.y = GetScreenHeight() - image.height;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > GetScreenWidth() - image.width)
	{
		position.x = GetScreenWidth() - image.width;
	}
}