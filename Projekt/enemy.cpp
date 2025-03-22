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
	enemyMaxHealth = enemyHealth = 6;
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
	enemyAttackSpeed = 4;
	enemyScore = 200;
}

Monster3::~Monster3() { UnloadTexture(image); }

//|---------------------------------------------------------------------------------------|
void Enemy::Draw() {
	DrawTextureV(image, position, WHITE);
}	
void Enemy::Update(Vector2 PlayerPosition, int directionX, int directionY, float enemySpeed) {
	if (position.x != PlayerPosition.x)
	{
		position.x+=directionX*enemySpeed;
	}
	if(position.y != PlayerPosition.y){
		position.y+=directionY*enemySpeed;
	}
}
void Enemy::UpdateColl(string CollisionSide)
{
	if (CollisionSide == "COLLISION_TOP")
	{
		position.y++;
	}
	if (CollisionSide == "COLLISION_BOTTOM")
	{
		position.y--;
	}
	if (CollisionSide == "COLLISION_LEFT")
	{
		position.x--;
	}
	if (CollisionSide == "COLLISION_RIGHT")
	{
		position.x++;
	}
}
Rectangle Enemy::getEnemyRect()
{
	return { position.x,position.y,(float)image.width,(float)image.height};
}
string Enemy::getCollisionSide(Rectangle enemy1, Rectangle enemy2)
{
	float dx = (enemy1.x + enemy1.width) - (enemy2.x + enemy2.width);
	float dy = (enemy1.y + enemy1.height) - (enemy2.y + enemy2.height);
	float totalWidth = (enemy1.width + enemy2.width);
	float totalHeight = (enemy1.height + enemy2.height);
	if (abs(dx) <= totalWidth && abs(dy) <= totalHeight) {
		float wy = totalWidth * dy;
		float hx = totalHeight * dx;

		if (wy > hx) {
			return (wy > -hx) ? "COLLISION_TOP" : "COLLISION_LEFT";
		}
		else {
			return (wy > -hx) ? "COLLISION_RIGHT" : "COLLISION_BOTTOM";
		}
	}
	return "COLLISION_MISSING";
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