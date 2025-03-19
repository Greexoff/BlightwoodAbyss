#include "enemy.h"

using namespace std;
//		Konstruktory i Destruktory Klas Enemy i pochodnych
//|---------------------------------------------------------------------------------------|
Enemy::Enemy(Vector2 position)
{
	this->position = position;
	
}
Enemy::~Enemy()
{
	UnloadTexture(image);
}
Monster1::Monster1(Vector2 position)//tank na 4 hity?
	: Enemy(position) {
	image = LoadTexture("potwor1.png");
	enemyHealth = 4;
}

Monster1::~Monster1() { UnloadTexture(image); }

Monster2::Monster2(Vector2 position)//szybki na strzala?
	: Enemy(position) {
	image = LoadTexture("potwor2.png");
	enemyHealth = 1;
}

Monster2::~Monster2() { UnloadTexture(image); }

Monster3::Monster3(Vector2 position)// strzelajacy na 2 hity?
	: Enemy(position) {
	image = LoadTexture("potwor3.png");
	enemyHealth = 2;
}

Monster3::~Monster3() { UnloadTexture(image); }
//|---------------------------------------------------------------------------------------|
void Enemy::Draw() {
	DrawTextureV(image, position, WHITE);
}	
void Enemy::Update(Vector2 PlayerPosition,int directionX, int directionY) {//jeszcze dodac jako parametr speed (*razy speed, gdzie speed to pole prywatne klasy enemy)
	if (position.x != PlayerPosition.x)
	{
		position.x+=directionX;
	}
	if(position.y != PlayerPosition.y){
		position.y+=directionY;
	}
}
//Do ogranięcia po wątkach
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
	
}
int Enemy::getEnemyHealth()
{
	return enemyHealth;
}
int Enemy::setEnemyHealth()
{
	return enemyHealth--;
}
