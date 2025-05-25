#include "raylib.h"
#include "game.h"
#include <thread>
#include <memory>

using namespace std;

Game::Game() {
	minMapLimit = ScreenSettings::GetInstance().getMinMapLimit();
	maxMapLimit = ScreenSettings::GetInstance().getMaxMapLimit();
	amountofEnemies = 5;
	waveNumber = 1;
	enemyShootingGap = 1.5;
	enemyHittingGap = 2;
	lastTearFired = 0.0;
	breakTime = 3;
	lastTimePlayerWasTouched = 0.0;	
	isCreatingNewWave = false;
	proceedCreatingEnemies = false;
	playerTotalScore = 0;
	enemies = CreateEnemy();
	startCountdown = false;
	breakStartingTime = 0;
}
Game::~Game() {
	enemies.clear();	
}
void Game::DrawBackground()
{
	ClearBackground(BLACK);
	DrawTextureEx(LoadingTextures::GetInstance().passCorrectTexture("backgroundOG.png", textureType::BACKGROUND_TEXTURE),minMapLimit, 0, ScreenSettings::GetInstance().getBackgroundScale(), WHITE);
}
void Game::setPlayerCharacter(int Character)
{
	switch (Character)
	{
	case 0:
		Player = make_unique<FirstCharacter>(LoadingTextures::GetInstance().passCorrectTexture("FirstCharacter.png", textureType::OBJECT_TEXTURE));
		playerTearType = "PlayerRedTear.png";
		break;
	case -1:
		Player = make_unique<SecondCharacter>(LoadingTextures::GetInstance().passCorrectTexture("SecondCharacter.png", textureType::OBJECT_TEXTURE));
		playerTearType = "PlayerGreenTear.png";
		break;
	case 1:
		Player = make_unique<ThirdCharacter>(LoadingTextures::GetInstance().passCorrectTexture("ThirdCharacter.png", textureType::OBJECT_TEXTURE));
		playerTearType = "PlayerBlueTear.png";
		break;
	default:
		Player = make_unique<FirstCharacter>(LoadingTextures::GetInstance().passCorrectTexture("FirstCharacter.png", textureType::OBJECT_TEXTURE));
		playerTearType = "PlayerRedTear.png";
		break;
	}
}
void Game::Update() {
	ScreenSettings::GetInstance().updateCamera(Player->GetXYPlayerPoint());
	if (proceedCreatingEnemies)
	{
		disablePlayerTears();
		enemies = CreateEnemy();
		lastTimePlayerWasTouched = GetTime();
		proceedCreatingEnemies = false;
	}
	for (auto & tears : Player->tearsy)
	{		
		tears.UpdatePosition(minMapLimit, maxMapLimit);
	}
	MoveEnemies();
	EnemyShootTears();
	for (auto& enTears : EnemyTears)
	{
		enTears.UpdatePosition(Player->GetXYPlayerPoint(), minMapLimit, maxMapLimit);
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
	BeginMode2D(ScreenSettings::GetInstance().getCamera());
	DrawBackground();
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
	EndMode2D();
	if (startCountdown)
	{
		DrawCountdownToNewWave();
	}
	DrawScoreAndWaveNumber();
	GameUI::GetInstance().DrawCharacterStatsInGame(Player->getPlayerStats(), 10, GetScreenHeight() * 0.4, 39*ScreenSettings::GetInstance().getScreenResolutionFactor().y);
}
void Game::InputHandle() {
	int moveX = 0;
	int moveY = 0;
	if (IsKeyDown(KEY_A)){moveX = -1;}
	if (IsKeyDown(KEY_D)){moveX = 1;}
	if (IsKeyDown(KEY_W)){moveY = -1;}
	if (IsKeyDown(KEY_S)){moveY = 1;}
	Player->movePlayer(moveX, moveY, minMapLimit, maxMapLimit);
	if (IsKeyDown(KEY_UP)) {
		Player->shootTears(0, -1, LoadingTextures::GetInstance().passCorrectTexture(playerTearType, textureType::OBJECT_TEXTURE));
	}
	if (IsKeyDown(KEY_DOWN)) {
		Player->shootTears(0, 1, LoadingTextures::GetInstance().passCorrectTexture(playerTearType, textureType::OBJECT_TEXTURE));
	}
	if (IsKeyDown(KEY_LEFT)) {
		Player->shootTears(-1, 0, LoadingTextures::GetInstance().passCorrectTexture(playerTearType, textureType::OBJECT_TEXTURE));
	}
	if (IsKeyDown(KEY_RIGHT)) {
		Player->shootTears(1, 0, LoadingTextures::GetInstance().passCorrectTexture(playerTearType, textureType::OBJECT_TEXTURE));
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
Vector2 Game::getRandomPosition(Texture2D& texture)
{
	Vector2 minLimit = ScreenSettings::GetInstance().getMinMapWalls();
	Vector2 maxLimit = ScreenSettings::GetInstance().getMaxMapWalls();

	int minX = static_cast<int>(minLimit.x + texture.width);
	int maxX = static_cast<int>(maxLimit.x - texture.width);
	int minY = static_cast<int>(minLimit.y + texture.height);
	int maxY = static_cast<int>(maxLimit.y - texture.height);
	Vector2 randomPos = { GetRandomValue(minX, maxX),GetRandomValue(minY, maxY) };

	return randomPos;
}
shared_ptr<Enemy> Game::createEnemyBasedOnType(int type)
{
	string textureName = "Enemy" + to_string(type) + ".png";
	Texture2D& texture = LoadingTextures::GetInstance().passCorrectTexture(textureName, textureType::OBJECT_TEXTURE);
	Vector2 position = getRandomPosition(texture);
	switch (type) {
	case 1:
		return make_shared<Monster1>(position, texture);
		break;
	case 2:
		return make_shared<Monster2>(position, texture);
		break;
	case 3:
		return make_shared<Monster3>(position, texture);
		break;
	case 4:
		return make_shared<Monster4>(position, texture);
		break;
	case 5:
		return make_shared<Monster5>(position, texture);
		break;
	default:
		return nullptr;
		break;
	}
}
vector <shared_ptr<Enemy>> Game::CreateEnemy()
{
	vector<shared_ptr<Enemy>> enemies;

	if (waveNumber % 5 == 0)
	{
		enemies.push_back(createEnemyBasedOnType(5));
		return enemies;
	}

	int poolOfEnemiesTypes = 3;
	int amountOfMiniBoss = 0;

	for (int i = 0; i < amountofEnemies; ++i) {
		if (waveNumber >= 5 && amountOfMiniBoss == 0)
		{
			poolOfEnemiesTypes = 4;
		}
		if (amountOfMiniBoss >= 6)
		{
			poolOfEnemiesTypes = 3;
		}
		int type = GetRandomValue(1, poolOfEnemiesTypes);
		if (type == 4)
		{
			amountOfMiniBoss++;
		}
		enemies.push_back(createEnemyBasedOnType(type));
	}
	return enemies;
}
void Game::MoveEnemies()
{
	for (auto& enemy : enemies) {
		enemy->Update(Player->GetXYPlayerPoint());
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
				EnemyTears.push_back(enemyTears(monsterPtr->getEnemyShootingPosition(2.8,2.8), enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), LoadingTextures::GetInstance().passCorrectTexture("Enemy3Tear.png", textureType::OBJECT_TEXTURE)));
			}
			if (auto monsterPtr = dynamic_pointer_cast<Monster4>(enem))
			{
				EnemyTears.push_back(enemyTears(monsterPtr->getEnemyShootingPosition(4.1,3.8), enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), LoadingTextures::GetInstance().passCorrectTexture("Enemy4Tear.png", textureType::OBJECT_TEXTURE)));
			}
			if (auto monsterPtr = dynamic_pointer_cast<Monster5>(enem))
			{
				EnemyTears.push_back(enemyTears(monsterPtr->getEnemyShootingPosition(4.5,4.25), enem->getEnemyAttackSpeed(), Player->GetXYPlayerPoint(), LoadingTextures::GetInstance().passCorrectTexture("Enemy5Tear.png", textureType::OBJECT_TEXTURE)));
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
				Player->setPlayerHealth(-1);

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
				Player->setPlayerHealth(-1);
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
				if (!dynamic_pointer_cast<Monster3>(enemies[i]) || !dynamic_pointer_cast<Monster3>(enemies[j]))
				{
					enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
				//	continue;
				//	enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
				}
				/*if (!dynamic_pointer_cast<Monster3>(enemies[j])) {
						enemies[j]->UpdateColl(enemies[j]->getCollisionSide(enemies[j]->getEnemyRect(), enemies[i]->getEnemyRect()));
				}*/
				//enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
			}
		}
	}
}
bool Game::isGameOver()
{
	if (Player->getPlayerHealth() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Game::beginNewWave()
{
	Player->setPlayerHealth(1);
	disableEnemyTears();
	increasePlayerTotalScore(200 * waveNumber);
	waveNumber++;
	breakStartingTime = GetTime();
	startCountdown = true;
	this_thread::sleep_for(chrono::seconds(breakTime));
	startCountdown = false;
	if (amountofEnemies < 20)
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
		Loot = make_unique<DamageTrinket>(LoadingTextures::GetInstance().passCorrectTexture("DamageTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
		break;
	case 2:
		Loot = make_unique<TearRateTrinket>(LoadingTextures::GetInstance().passCorrectTexture("TearRateTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
		break;
	case 3:
		Loot = make_unique<SpeedTrinket>(LoadingTextures::GetInstance().passCorrectTexture("SpeedTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
		break;
	case 4:
		Loot = make_unique<HealthTrinket>(LoadingTextures::GetInstance().passCorrectTexture("HealthTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
		break;
	case 5:
		Loot = make_unique<TearSpeedTrinket>(LoadingTextures::GetInstance().passCorrectTexture("TearSpeedTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
		break;
	default:
		break;
	}
	
}
void Game::setLastTimePlayerWasTouched()
{
	lastTimePlayerWasTouched = GetTime();
}
int Game::getWaveNumber()
{
	return waveNumber;
}
void Game::DrawCountdownToNewWave()
{
	double remainingTime =breakTime-(int)(GetTime()- breakStartingTime);
	string information = "NEW WAVE BEGINS IN: " + GameUI::GetInstance().ConvertToString((float)remainingTime, 0);
	if (waveNumber % 5 == 0)
	{
		information = "BOSS FIGHT BEGINS IN: " + GameUI::GetInstance().ConvertToString((float)remainingTime, 0);
	}
	GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() / 2 }, 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,information, GetScreenHeight()*0.2);
}
void Game::DrawScoreAndWaveNumber()
{
	string keepWaveNumberText = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(waveNumber, 3, "WAVE:");
	string keepPlayerScoreText = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(playerTotalScore, 6, "SCORE:");
	GameUI::GetInstance().DrawGameUI(keepPlayerScoreText + " " + keepWaveNumberText, 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, GetScreenHeight()*0.13);
}