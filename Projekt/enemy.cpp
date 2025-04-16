#include "enemy.h"

using namespace std;
//		Konstruktory i Destruktory Klas Enemy i pochodnych
//|---------------------------------------------------------------------------------------|
Enemy::~Enemy()
{
}
Monster1::Monster1(Vector2 position, Texture2D loadedImage)//Tank
{
	image = loadedImage;
	this->position = position;
	enemyMaxHealth = enemyHealth=4;
	enemySpeed = 1;
	enemyAttackSpeed = 0;
	enemyScore = 300;
	imageScale = 1;
}
Monster1::~Monster1() {}

Monster2::Monster2(Vector2 position, Texture2D loadedImage)//Szybki 
{
	image = loadedImage;
	this->position = position;
	enemyMaxHealth = enemyHealth = 1;
	enemySpeed = 2.5;
	enemyAttackSpeed = 0;
	enemyScore = 100;
	imageScale = 1.05;
}
Monster2::~Monster2() {}

Monster3::Monster3(Vector2 position, Texture2D loadedImage)//Strzelajacy
{
	image = loadedImage;
	this->position = position;
	enemyMaxHealth = enemyHealth = 2;
	enemySpeed = 0;
	enemyAttackSpeed = 5;
	enemyScore = 200;
	imageScale = 0.8;
}
Monster3::~Monster3() {}

Monster4::Monster4(Vector2 position, Texture2D loadedImage)//Strzelajacy Mini-Boss
{
	image = loadedImage;
	this->position = position;
	enemyMaxHealth = enemyHealth = 10;
	enemySpeed = 0.5;
	enemyAttackSpeed = 3;
	enemyScore = 400;
	imageScale = 1.4;
}
Monster4::~Monster4() {}

Monster5::Monster5(Vector2 position, Texture2D loadedImage)//Boss
{
	image = loadedImage;
	this->position = position;
	enemyMaxHealth = enemyHealth = 50;
	enemySpeed = 0.2;
	enemyAttackSpeed = 8;
	enemyScore = 800;
	imageScale = 1.6;
}
Monster5::~Monster5() {}
//|---------------------------------------------------------------------------------------|
void Enemy::Draw() {
	DrawTextureEx(image, position, 0, imageScale, WHITE);
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
	position.x += dir.x * enemySpeed;
	position.y += dir.y * enemySpeed;
	
}	
void Enemy::UpdateColl(Vector2 Direction)
{
	position.x += enemySpeed*Direction.x;
	position.y += enemySpeed*Direction.y;
	if (position.y > GetScreenHeight()-250 - image.height*imageScale)
	{
		position.y = GetScreenHeight() - 250 - image.height * imageScale;
	}
	if (position.y < 250)
	{
		position.y = 250;
	}
	if (position.x < 250)
	{
		position.x = 250;
	}
	if (position.x > GetScreenWidth()-250 - image.width * imageScale)
	{
		position.x = GetScreenWidth() - 250 - image.width * imageScale;
	}
}
Rectangle Enemy::getEnemyRect()
{
	return { position.x,position.y,(((float)image.width - 5 )*imageScale),(((float)image.height - 5)*imageScale)};
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
int Enemy::setEnemyHealth(int playerDamage)
{
	return enemyHealth-=playerDamage;
}
float Enemy::getEnemySpeed()
{
	return enemySpeed;
}
void Enemy::DrawEnemyHealthBar()
{
	float healthBarWidth = image.width*imageScale;
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
	if (position.y > GetScreenHeight() - 250 -image.height*imageScale)
	{
		position.y = GetScreenHeight() - 250-image.height * imageScale;
	}
	if (position.y < 250)
	{
		position.y = 250;
	}
	if (position.x < 250)
	{
		position.x = 250;
	}
	if (position.x > GetScreenWidth() - 250-image.height * imageScale)
	{
		position.x = GetScreenWidth() - 250-image.height * imageScale;
	}
}
Vector2 Enemy::getEnemyPosition(float divideBy)
{
	return { position.x+(image.width*imageScale/divideBy),position.y+(image.height*imageScale/divideBy) };
}
