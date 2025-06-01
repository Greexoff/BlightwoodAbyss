#include "raylib.h"
#include "game.h"


using namespace std;

Game::Game(int CharacterType) {

	minMapLimit = ScreenSettings::GetInstance().getMinMapLimit();
	maxMapLimit = ScreenSettings::GetInstance().getMaxMapLimit();
	setPlayerCharacter(CharacterType);
	setItemProgress();
	amountofEnemies = 5;
	waveNumber = 1;
	enemyHittingGap = 1;
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
	drawStatsChanges = false;
	postTabClosed = false;
	setLastTimePlayerWasTouched();
	lastStatChangeTime = GetTime();
	statsThread=thread(&Game::thread_StatsChangesTimer, this);
}
Game::~Game() {
	enemies.clear();	
	statsChanges.clear();
	stopThread = true;
	if (statsThread.joinable())
	{
		statsThread.join();
	}
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
			if (waveNumber == 1 && getItemProgressWarden(itemName, ItemProgressAction::UPDATE_WARDEN))
			{
				tryUnlockItem(itemName, true);
			}
			itemName = "SpeedTrinket";
			if (waveNumber % 5 == 0&&getItemProgressWarden(itemName, ItemProgressAction::UPDATE_WARDEN))
			{
				tryUnlockItem(itemName, false);
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
		float stats_starting_y_pos = (GetScreenHeight() * 0.5f) - (140 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
		float stats_fontSize = 40 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
		GameUI::GetInstance().DrawCharacterStatsInGame(Player->getPlayerStats(), 10 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, stats_starting_y_pos, stats_fontSize,statsChanges,drawStatsChanges);
		if (itemProgressDrawCountdown)
		{
			drawItemInfoInGame();
		}
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
	position = getRandomPosition(texture);
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
	case 6:
		return make_shared<Monster6>(position, texture);
		break;
	default:
		return nullptr;
		break;
	}
}
void Game::statChange(shared_ptr<Items> item)
{
	lock_guard<mutex> lock(mtx);
	item->applyEffect(Player.get(), statsChanges);
	drawStatsChanges = true;
	newStatsChangeHappened = true;
}
void Game::thread_StatsChangesTimer()
{
	while (!stopThread)
	{
		if (newStatsChangeHappened.exchange(false))
		{
			lastStatChangeTime = GetTime();
		}
		double currTime = GetTime();
		if(currTime-lastStatChangeTime>2)
		{
			lock_guard<mutex> loct(mtx);
			statsChanges.clear();
			drawStatsChanges = false;
			lastStatChangeTime = currTime;
		}
	}
}
vector <shared_ptr<Enemy>> Game::CreateEnemy()
{
	vector<shared_ptr<Enemy>> enemies;
	random_device rd;
	mt19937 gen(rd());
	if (waveNumber % 5 == 0 && waveNumber<50)
	{
		uniform_int_distribution<int> typeDistribBosses(5,6);
		int type = typeDistribBosses(gen);
		enemies.push_back(createEnemyBasedOnType(type));
		if (getItemProgressWarden("SpeedTrinket", ItemProgressAction::UPDATE_WARDEN))
		{
			updateItemProgress("SpeedTrinket", GetTime(), ItemProgressAction::SET_VALUE);
		}
		return enemies;	
	}
	int poolOfEnemiesTypes = 3;
	int amountOfMiniBoss = 0;
	int amountOfBoss = 0;

	for (int i = 0; i < amountofEnemies; ++i) {
		if (waveNumber >= 5 && amountOfMiniBoss == 0)
		{
			poolOfEnemiesTypes = 4;
		}
		if (amountOfMiniBoss >= 6)
		{
			poolOfEnemiesTypes = 3;
		}
		if (waveNumber >= 50 && amountOfBoss <1)
		{
			poolOfEnemiesTypes = 6;
		}
		uniform_int_distribution<int> typeDistribEnemies(1, poolOfEnemiesTypes);
		int type = typeDistribEnemies(gen);
		if (type == 4)
		{
			amountOfMiniBoss++;
		}
		if (type == 5 || type == 6)
		{
			amountOfBoss++;
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
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			(*it)->shootTears(EnemyTears, Player.get());
			++it;
		}
	}

}
void Game::CollisionCheck()
{
	if (!items.empty())
	{
		for (auto it = items.begin(); it != items.end();)
		{
			if (CheckCollisionRecs(Player->getPlayerRect(), (*it)->getItemRect()))
			{
				if (Player->getPlayerHealth() == Player->getPlayerMaxHealth() && dynamic_pointer_cast<HeartContainer>(*it))
				{
					++it;
					continue;
				}
				statChange((*it));
				if (!dynamic_pointer_cast<HeartContainer>(*it) && !dynamic_pointer_cast<randomStatsItem>(*it))
				{
					if (getItemProgressWarden("TearSpeedTrinket", ItemProgressAction::UPDATE_WARDEN))
					{
						updateItemProgress("TearSpeedTrinket", 0, ItemProgressAction::SET_VALUE);
					}
				}
				it = items.erase((it));
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
						createRandomLoot((*it)->getEnemyPosition(), waveNumber % 5 == 0 && waveNumber<50);
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
				if (getItemProgressWarden("HealthTrinket", ItemProgressAction::UPDATE_WARDEN))
				{
					updateItemProgress("HealthTrinket", 0, ItemProgressAction::SET_VALUE);
				}

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
					if (getItemProgressWarden("HealthTrinket", ItemProgressAction::UPDATE_WARDEN))
					{
						updateItemProgress("HealthTrinket", 0, ItemProgressAction::SET_VALUE);
					}
				}
				enemTear.active = false;
			}
		}
	}
}
void Game::drawItemInfoInGame()
{
	for (auto& item : itemProgress)
	{
		if (item.displayWarden)
		{
			float y_pos = 200 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
			float gap = 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
			float x_pos = 540 * ScreenSettings::GetInstance().getScreenResolutionFactor().x;
			float biggerFont = 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
			float smallerFont = 50 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
			float text_y_pos = 0;
			Rectangle bar = { (GetScreenWidth() * 0.5f) - x_pos,(GetScreenHeight() * 0.5f) - y_pos,2 * x_pos,2 * y_pos };
			Rectangle Textbar = { bar.x,bar.y,bar.width * 0.6,bar.height };
			Rectangle ImageBar = { bar.x +Textbar.width,bar.y,bar.width * 0.4,bar.height };
			GameUI::GetInstance().DrawBlackBar(bar, 180);
			text_y_pos = Textbar.y + gap;
			GameUI::GetInstance().DrawTextRules(Textbar, biggerFont, "ITEM UNLOCKED", text_y_pos);
			GameUI::GetInstance().DrawTextRules(Textbar, biggerFont, item.itemDisplayName, text_y_pos);

			auto& image = LoadingTextures::GetInstance().passCorrectTexture(item.itemName + ".png", textureType::OBJECT_TEXTURE);
			float imageScale = 1.25 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;

			Vector2 imagePos = { (ImageBar.x + ImageBar.width * 0.4f) - (image.width * imageScale * 0.5f), (ImageBar.y + ImageBar.height * 0.5f) - (image.height * imageScale * 0.5f) };
			DrawTextureEx(image, imagePos, 0, imageScale, WHITE);
			GameUI::GetInstance().DrawTextRules(Textbar, smallerFont, item.itemDescription, text_y_pos);
			break;
		}
	}
}
void Game::setItemProgress()
{
	itemProgressDrawCountdown = false;
	float startingWave = 1;
	float expectedWave_HT = 10;
	float expectedWave_TST = 15;
	float startingTime = GetTime();
	float expectedTime_ST = 30.0;
	float expectedTime_TRT = 10.0;
	float expectedWave_AT = 50;
	itemProgress = {
		{"TearRateTrinket",startingTime,expectedTime_TRT,UnlockMethod::TIME_BASED_UNLOCK,false,	!UserInfo::GetInstance().getUserItemValue("TearRateTrinket"), "RIPPLEGILL", "REDUCES TIME BETWEEN SHOTS BY 0.15"},
		{"HealthTrinket",startingWave,expectedWave_HT,UnlockMethod::WAVES_BASED_UNLOCK,false,!UserInfo::GetInstance().getUserItemValue("HealthTrinket"), "HEARTHROOT", "INCREASES PLAYER'S MAX HEALTH BY 1" },
		{"SpeedTrinket",startingTime,expectedTime_ST,UnlockMethod::TIME_BASED_UNLOCK,false,!UserInfo::GetInstance().getUserItemValue("SpeedTrinket"),"SKYVEIL SPORE", "INCREASES PLAYER'S SPEED BY 1" },
		{"TearSpeedTrinket",startingWave,expectedWave_TST,UnlockMethod::WAVES_BASED_UNLOCK,false,!UserInfo::GetInstance().getUserItemValue("TearSpeedTrinket"), "BLINKSHROOM", "INCREASES PLAYER'S TEAR SPEED BY 0.5" },
		{"AllTrinket",startingWave,expectedWave_AT,UnlockMethod::WAVES_BASED_UNLOCK,false,!UserInfo::GetInstance().getUserItemValue("AllTrinket"), "NOCTILUMEN", "INCREASES EACH PLAYER'S STAT BY 0.5" },

	};
}
bool Game::getItemProgressWarden(string itemName, ItemProgressAction wardenType)
{
	for (auto& progress : itemProgress)
	{
		if (progress.itemName == itemName)
		{
			if (wardenType == ItemProgressAction::UPDATE_WARDEN)
			{
				return progress.updateWarden;
			}
			if (wardenType == ItemProgressAction::DISPLAY_WARDEN)
			{
				return progress.displayWarden;
			}
			break;
		}
	}
}
void Game::setItemProgressWarden(string itemName, bool value, ItemProgressAction wardenType)
{
	for (auto& progress : itemProgress)
	{
		if (progress.itemName == itemName)
		{
			if (wardenType == ItemProgressAction::UPDATE_WARDEN)
			{
				progress.updateWarden=value;
			}
			if (wardenType == ItemProgressAction::DISPLAY_WARDEN)
			{
				progress.displayWarden=value;
			}
			break;
		}
	}
}
void Game::updateItemProgress(string itemName, float value, ItemProgressAction action)
{
	for (auto& TrinketCriteria : itemProgress)
	{
		if (TrinketCriteria.itemName == itemName)
		{
			if (action == ItemProgressAction::INCREASE_VALUE)
			{
				TrinketCriteria.startingCondition += value;
			}
			if (action == ItemProgressAction::SET_VALUE)
			{
				TrinketCriteria.startingCondition = value;
			}
			break;
		}
	}
}
void Game::tryUnlockItem(string itemName,bool lessThan)
{
	for (auto& TrinketCriteria : itemProgress)
	{
		if (TrinketCriteria.itemName == itemName)
		{
			float unlockType = 0;
			bool UnlockCondition = lessThan;
			switch (TrinketCriteria.method)
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
			UnlockCondition ? UnlockCondition = (abs(unlockType - TrinketCriteria.startingCondition) < TrinketCriteria.endingCondition) : UnlockCondition = (abs(unlockType - TrinketCriteria.startingCondition) > TrinketCriteria.endingCondition);
			if (UnlockCondition)
			{
				bool itemUnlocked = true;
				UserInfo::GetInstance().updateUserItems(TrinketCriteria.itemName, itemUnlocked);
				setItemProgressWarden(TrinketCriteria.itemName, false, ItemProgressAction::UPDATE_WARDEN);
				setItemProgressWarden(TrinketCriteria.itemName, true, ItemProgressAction::DISPLAY_WARDEN);
			}
		}
	}
	
}
bool Game::isGameOver()
{
	if (Player->getPlayerHealth() <= 0)
	{
		if (IsCursorHidden())
		{
			EnableCursor();
		}
		return true;
	}
	else
	{
		return false;
	}
}
void Game::threadItemProgressUpdate(UnlockMethod method)
{
	for (auto& elements : itemProgress)
	{
		if (elements.method==method&&elements.updateWarden)
		{
			updateItemProgress(elements.itemName, 1, ItemProgressAction::INCREASE_VALUE);
			tryUnlockItem(elements.itemName, false);
		}
	}
}
void Game::threadDisableDisplayWarden(string itemName)
{
	if (getItemProgressWarden(itemName, ItemProgressAction::DISPLAY_WARDEN)) {
		itemProgressDrawCountdown = true;
		this_thread::sleep_for(chrono::seconds(5));
		setItemProgressWarden(itemName, false, ItemProgressAction::DISPLAY_WARDEN);
		itemProgressDrawCountdown = false;
	}
}
void Game::beginNewWave()
{
	disableEnemyTears();
	increasePlayerTotalScore(200 * waveNumber);
	waveNumber++;
	threadItemProgressUpdate(UnlockMethod::WAVES_BASED_UNLOCK);
	for (auto& elements : itemProgress)
	{
		threadDisableDisplayWarden(elements.itemName);
	}
	breakStartingTime = GetTime();
	startCountdown = true;
	this_thread::sleep_for(chrono::seconds(breakTime));
	startCountdown = false;
	if (!items.empty() && waveNumber % 5 != 0)
	{
		for (auto it = items.begin(); it != items.end();)
		{
			if (dynamic_pointer_cast<HeartContainer>(*it) || dynamic_pointer_cast<randomStatsItem>(*it))
			{
				it = items.erase((it));
			}
			else
			{
				++it;
			}
		}
	}
	if (amountofEnemies < 25)
	{
		amountofEnemies++;
	}
	if (waveNumber%50==0)
	{
		amountofEnemies = 5;
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
void Game::createRandomLoot(Vector2 enemyPos, bool condition)
{
	bool lootGenerated = false;
	shared_ptr<Items> Loot;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> typeDistrib(1, 6);
	uniform_int_distribution<int> randomDistrib(1, 10);
	if (condition)
	{
		while (!lootGenerated)
		{
			int type = typeDistrib(gen);
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
			case 6:
				if (UserInfo::GetInstance().getUserItemValue("AllTrinket"))
				{
					Loot = make_shared<AllTrinket>(LoadingTextures::GetInstance().passCorrectTexture("AllTrinket.png", textureType::OBJECT_TEXTURE), enemyPos);
					lootGenerated = true;
				}
				break;
			default:
				break;
			}
		}
		items.push_back(Loot);
	}
	else
	{
		int random = randomDistrib(gen);
		if (random == 9)
		{
			Loot = make_shared<HeartContainer>(LoadingTextures::GetInstance().passCorrectTexture("HeartContainer.png", textureType::OBJECT_TEXTURE), enemyPos);
			items.push_back(Loot);
		}
		if (random == 4)
		{
			Loot = make_shared<randomStatsItem>(LoadingTextures::GetInstance().passCorrectTexture("ChaosStone.png", textureType::OBJECT_TEXTURE), enemyPos);
			items.push_back(Loot);
		}
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
	if (waveNumber % 5 == 0 && waveNumber<=50)
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

	regex usersScoresRegex(R"(^(\w+),\s*(\w+),\s*Highest Score:\s*(\d+),(.*))");
	smatch match;
	string line;

	while (getline(DataBase, line))
	{
		if (regex_match(line, match, usersScoresRegex) && match[1] == username)
		{
			line.clear();
			if (stoi(match[3])<playerScore)
			{
				isHigher = true;
				string newScore = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(playerScore, 6, "");
				line = username + ", " + match[2].str() + ", Highest Score: " + newScore + ", ";
			}
			else
			{
				isHigher = false;
				line = username + ", " + match[2].str() + ", Highest Score: " + match[3].str() + ", ";
			}
			for (auto [trinketName, trinketValue] : UserInfo::GetInstance().getUserItems())
			{
				line += trinketName + ": " + to_string(trinketValue) + ", ";
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