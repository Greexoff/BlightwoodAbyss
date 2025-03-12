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
Monster1::Monster1(Vector2 position)
	: Enemy(position) {
	image = LoadTexture("potwor1.png");
}

Monster1::~Monster1() { UnloadTexture(image); }

Monster2::Monster2(Vector2 position)
	: Enemy(position) {
	image = LoadTexture("potwor2.png");
}

Monster2::~Monster2() { UnloadTexture(image); }

Monster3::Monster3(Vector2 position)
	: Enemy(position) {
	image = LoadTexture("potwor3.png");
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
Rectangle Enemy::getEnemyRect()
{
	return { position.x,position.y,(float)image.width,(float)image.height};
}
