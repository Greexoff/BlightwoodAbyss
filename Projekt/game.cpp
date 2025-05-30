#include "raylib.h"
#include "game.h"
#include <thread>
#include <memory>

using namespace std;

Game::Game() {
	minMapLimit = ScreenSettings::GetInstance().getMinMapLimit();
	maxMapLimit = ScreenSettings::GetInstance().getMaxMapLimit();
	setItemProgress();
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
	wasDatabaseUpdated = false;
	isNewScoreHigher = false;
	postTabClosed = false;
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
	if (!isGameOver())
	{
		InputHandle();
		if (proceedCreatingEnemies)
		{
			disablePlayerTears();
			enemies = CreateEnemy();
			lastTimePlayerWasTouched = GetTime();
			proceedCreatingEnemies = false;
		}
		MoveEnemies();
		EnemyShootTears();
		CollisionCheck();
		if (enemies.empty() && !isCreatingNewWave)
		{
			string itemName = "TearRateTrinket";
			if (waveNumber == 1)
			{
				tryUnlockItem(itemName, itemProgress[itemName], true, UnlockMethod::TIME_BASED_UNLOCK);
			}
			itemName = "SpeedTrinket";
			if (waveNumber % 5 == 0) {
				tryUnlockItem(itemName, itemProgress[itemName], false, UnlockMethod::TIME_BASED_UNLOCK);
			}
			isCreatingNewWave = true;
			thread t(&Game::beginNewWave, this);
			t.detach();
		}
	}
	for (auto& tears : Player->tearsy)
	{
		tears.UpdatePosition(minMapLimit, maxMapLimit);
	}
	for (auto& enTears : EnemyTears)
	{
		enTears.UpdatePosition(Player->GetXYPlayerPoint(), minMapLimit, maxMapLimit);
	}
	DeleteInactiveTears();
}
void Game::Draw() {
	BeginMode2D(ScreenSettings::GetInstance().getCamera());
	DrawBackground();
	for (auto& loot : items)
	{
		loot->DrawItems();
	}
	for (auto& enemy : enemies)
	{
		enemy->Draw();
	}
	for (auto& enemTears : EnemyTears)
	{
		enemTears.Draw();
	}
	if (!isGameOver())
	{
		Player->DrawPlayerHealthBar();
		Player->Draw();
		for (auto& tears : Player->tearsy) {
			tears.Draw();
		}
		for (auto& enemy : enemies)
		{
			enemy->DrawEnemyHealthBar();
		}
		EndMode2D();
		if (startCountdown)
		{
			DrawCountdownToNewWave();
		}
		DrawScoreAndWaveNumber();
		GameUI::GetInstance().DrawCharacterStatsInGame(Player->getPlayerStats(), 10, GetScreenHeight() * 0.4, 39 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
	}
	else
	{
		EndMode2D();
		handlePostGameTab();
	}
	
	
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
		itemProgress["SpeedTrinket"].first = GetTime();
		return enemies;	}

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
	if (!items.empty())
	{
		//TUTAJ JESZCZE JAKAS FUNKCJA DO WYSWIETLANIA NAPISU ZE PODNIESIONO X I CO TO DAJE JAK JUZ BEDE MIAL INTERFEJS GRAFICZNY TO TA METODE NA PUBLIC  CZY CUS
		for (auto it = items.begin(); it != items.end();)
		{
			if (CheckCollisionRecs(Player->getPlayerRect(), (*it)->getItemRect()))
			{
				(*it)->applyEffect(Player.get());
				it=items.erase((it));
				itemProgress["TearSpeedTrinket"].first = 0;
			}
			else
			{
				++it;
			}
		}
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
						//if (waveNumber % 5 == 0)
					//	{
							createRandomLoot((*it)->getEnemyPosition());
						//}
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
		for (size_t i = 0; i < enemies.size(); i++)
		{
			for (size_t j = i + 1; j < enemies.size(); j++)
			{
				if ((CheckCollisionRecs(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()))) {
					if (!dynamic_pointer_cast<Monster3>(enemies[i]))
					{
						enemies[i]->UpdateColl(enemies[i]->getCollisionSide(enemies[i]->getEnemyRect(), enemies[j]->getEnemyRect()));
					}
					if (!dynamic_pointer_cast<Monster3>(enemies[j])) {
						enemies[j]->UpdateColl(enemies[j]->getCollisionSide(enemies[j]->getEnemyRect(), enemies[i]->getEnemyRect()));
					}
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
				itemProgress["HealthTrinket"].first = 0;

			}
			else
			{
				++it;
			}
		}
		for (auto& enemTear : EnemyTears)
		{
			if (CheckCollisionRecs(enemTear.getTearRect(), Player->getPlayerRect()))
			{
				if (GetTime() - lastTimePlayerWasTouched > enemyHittingGap) {
					Player->setPlayerHealth(-1);
					lastTimePlayerWasTouched = GetTime();
					itemProgress["HealthTrinket"].first = 0;
				}
				enemTear.active = false;
			}
		}
	}
}
void Game::setItemProgress()
{
	int startingWave = 1;
	int expectedWave_HT = 10;
	int expectedWave_TST = 15;
	double startingTime = GetTime();
	double expectedTime_ST = 30.0;
	double expectedTime_TRT = 10.0;
	itemProgress["TearRateTrinket"] = {startingTime,expectedTime_TRT};
	itemProgress["HealthTrinket"] = {startingWave,expectedWave_HT};
	itemProgress["SpeedTrinket"] = {startingTime,expectedTime_ST};
	itemProgress["TearSpeedTrinket"] = {startingWave,expectedWave_TST};
}
void Game::tryUnlockItem(string itemName,pair<float,float> itemConditions,bool lessThan, UnlockMethod method)
{
	float unlockType = 0;
	bool UnlockCondition = lessThan;
	switch (method)
	{
	case UnlockMethod::WAVES_BASED_UNLOCK:
		unlockType = 0;
		break;
	case UnlockMethod::TIME_BASED_UNLOCK:
		unlockType = GetTime();
		break;
	default:
		break;
	}
	UnlockCondition ? UnlockCondition = (abs(unlockType - itemConditions.first )< itemConditions.second) : UnlockCondition = (abs(unlockType - itemConditions.first) > itemConditions.second);
	if (UnlockCondition)
	{
		bool itemUnlocked = true;
		UserInfo::GetInstance().updateUserItems(itemName, itemUnlocked);
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
	string itemName = "HealthTrinket";
	tryUnlockItem(itemName, itemProgress[itemName], false, UnlockMethod::WAVES_BASED_UNLOCK);
	itemProgress[itemName].first++;
	itemName="TearSpeedTrinket";
	tryUnlockItem(itemName, itemProgress[itemName], false, UnlockMethod::WAVES_BASED_UNLOCK);
	itemProgress[itemName].first++;
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
	bool lootGenerated = false;
	shared_ptr<Items> Loot;
	while (!lootGenerated)
	{
		int type = GetRandomValue(1, 5);
		switch (type)
		{
		case 1:
			if (UserInfo::GetInstance().getUserItemValue("DamageTrinket"))
			{
				Loot = make_shared<DamageTrinket>(LoadingTextures::GetInstance().passCorrectTexture("DamageTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
				lootGenerated = true;
			}
			break;
		case 2:
			if (UserInfo::GetInstance().getUserItemValue("TearRateTrinket"))
			{
				Loot = make_shared<TearRateTrinket>(LoadingTextures::GetInstance().passCorrectTexture("TearRateTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
				lootGenerated = true;
			}
			break;
		case 3:
			if (UserInfo::GetInstance().getUserItemValue("SpeedTrinket"))
			{
				Loot = make_shared<SpeedTrinket>(LoadingTextures::GetInstance().passCorrectTexture("SpeedTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
				lootGenerated = true;
			}
			break;
		case 4:
			if (UserInfo::GetInstance().getUserItemValue("HealthTrinket"))
			{
				Loot = make_shared<HealthTrinket>(LoadingTextures::GetInstance().passCorrectTexture("HealthTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
				lootGenerated = true;
			}
			break;
		case 5:
			if (UserInfo::GetInstance().getUserItemValue("TearSpeedTrinket"))
			{
				Loot = make_shared<TearSpeedTrinket>(LoadingTextures::GetInstance().passCorrectTexture("TearSpeedTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
				lootGenerated = true;
			}
			break;
		default:
			break;
		}
	}
	items.push_back(Loot);
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
void Game::handlePostGameTab()
{
	if (!wasDatabaseUpdated)
	{
		isNewScoreHigher = updatePlayerInDataBase(playerTotalScore, UserInfo::GetInstance().getUsername(), wasDatabaseUpdated);
	}
	Rectangle borders = { GetScreenWidth() * 0.2,GetScreenHeight() * 0.2,GetScreenWidth() * 0.6,GetScreenHeight() * 0.6 };
	GameUI::GetInstance().DrawBlackBar(borders, 100);
	GameUI::GetInstance().DrawTextOnBar(borders, 150* ScreenSettings::GetInstance().getScreenResolutionFactor().y, "GAME OVER!", borders.y + (30 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
	GameUI::GetInstance().DrawTextOnBar(borders, 75 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, "YOUR SCORE:", borders.y + (200 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
	string scoreText = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(playerTotalScore, 6, "");
	GameUI::GetInstance().DrawTextOnBar(borders, 75 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, scoreText, borders.y + (280 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
	if (isNewScoreHigher)
	{
		GameUI::GetInstance().DrawTextOnBar(borders, 100 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, "NEW PERSONAL BEST!", borders.y + (360 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
	}
	Vector2 measureReturnText = GameUI::GetInstance().MeasureText(80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, "RETURN TO MENU");
	Rectangle returnButton =GameUI::GetInstance().setBarArea(80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, "RETURN TO MENU", { (borders.x+(borders.width*0.5f))-(measureReturnText.x*0.5f), borders.y + (500 * ScreenSettings::GetInstance().getScreenResolutionFactor().y)}, 1, 10, 10);
	GameUI::GetInstance().DrawBlackBar(returnButton, 100);
	GameUI::GetInstance().DrawTextOnBar(returnButton, 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, "RETURN TO MENU", returnButton.y+(10 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, returnButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		postTabClosed = true;
	}

}
bool Game::isPostTabClosed()
{
	return postTabClosed;
}
bool Game::updatePlayerInDataBase(int playerScore, string username, bool& flag)
{
	flag = true;
	bool isHigher = true;
	ifstream DataBase(data_basePath);
	ofstream UpdatedDataBase("temp.txt");
	if (!DataBase.is_open() || !UpdatedDataBase.is_open())
	{
		return false;
	}

	regex usersScoresRegex(R"(^(\w+),(\w+),Highest Score:\s*(\d+),(.*))");
	smatch match;
	string line;

	while (getline(DataBase, line))
	{
		if (regex_match(line, match, usersScoresRegex) && match[1] == username)
		{
			if (stoi(match[3])<playerScore)
			{
				isHigher = true;
				string newScore = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(playerScore, 6, "");
				line = match[1].str() + "," + match[2].str() + ",Highest Score: " + newScore + ", ";
			}
			else
			{
				isHigher = false;
			}
			for (auto [trinketName, trinketValue] : UserInfo::GetInstance().getUserItems())
			{
				line += trinketName + ": " + to_string(trinketValue) + ",";
			}
		}
		UpdatedDataBase << line << "\n";
	}
	DataBase.close();
	UpdatedDataBase.close();
	fs::remove(data_basePath);
	fs::rename("temp.txt", data_basePath.c_str());
	return isHigher;
}