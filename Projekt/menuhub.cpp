#include "menuhub.h"

unique_ptr<Menu> Menu::selectedMenu = nullptr;
Menu::~Menu()
{

}
void Menu::LoadTextures(Texture2D correctTexture)
{
	BackgroundTexture=correctTexture;
}
bool Menu::isReturnButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, ReturnToPrieviousMenuButton))
	{
		GameUI::GetInstance().DrawReturnToMenuComment(ReturnToMenuCommentBar, 130);
		return true;
	}
	return false;
}
void Menu::ReturnToMenu()
{
	if (isReturnButtonClicked() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Menu::setSelectedMenu(make_unique <MainMenu>());
	}
}

void Menu::setSelectedMenu(unique_ptr<Menu> newMenu)
{
	selectedMenu = move(newMenu);
}
Menu* Menu::getSelectedMenu()
{
	return selectedMenu.get();
}

StartingMenu::StartingMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundSTARTING.png", textureType::BACKGROUND_TEXTURE));
	titleFontSize = 200;
	titleName = "BLIGHTWOOD ABBYS";
	ScreenBar={ (float)GetScreenWidth()/8,0,(float)GetScreenWidth()-2*(float)GetScreenWidth()/8,(float)GetScreenHeight()};
}
StartingMenu::~StartingMenu()
{
}
void StartingMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
	GameUI::GetInstance().DrawTextOnBar(ScreenBar, titleFontSize, titleName, ScreenBar.y+GetScreenHeight()*0.2);
}
MenuResult StartingMenu::handleMenuLogic()
{
	Draw();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Menu::setSelectedMenu(make_unique <LoginMenu>());
	}
	return MenuResult::CONTINUE;
}

LoginMenu::LoginMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundSTARTING.png", textureType::BACKGROUND_TEXTURE));
	setAction = 0;
	username = "";
	password = "";
	setFontSizes();
	setXYofTexts();
	setBarAreas();
	userExist = false;
	isSignupAreaActive = true;
	error = errorType::NO_ERROR;
	showError = false;
	errorDurationTime = 3;
}
LoginMenu::~LoginMenu()
{
}
void LoginMenu::setFontSizes()
{
	UsernameTextFontSize = 150;
	PasswordTextFontSize = 150;
	ConfirmTextFontSize = 75;
	SignupTextFontSize = 75;
	InsertedDataFontSize = 80;
}
void LoginMenu::setXYofTexts()
{
	ConfirmPosition = { (float)(GetScreenWidth()*0.4),(float)(GetScreenHeight()*0.85)};
	UsernamePosition = {(float)(GetScreenWidth()*0.3),(float)(GetScreenHeight()*0.07)};
	PasswordPosition = {(float)(GetScreenWidth()*0.3),(float)(GetScreenHeight()*0.4)};
	SignupPosition = { (float)(GetScreenWidth()*0.7),(float)(GetScreenHeight()*0.85)};
}
void LoginMenu::setBarAreas()
{
	LoginMenu_UsernameBarArea=GameUI::GetInstance().setBarArea(UsernameTextFontSize, "USERNAME", UsernamePosition, 2, 10, 10);
	LoginMenu_PasswordBarArea=	GameUI::GetInstance().setBarArea(PasswordTextFontSize, "PASSWORD", PasswordPosition, 2, 10, 10);
	LoginMenu_ConfirmArea=GameUI::GetInstance().setBarArea(ConfirmTextFontSize, "CONFIRM", ConfirmPosition, 1, 30, 30);
	LoginMenu_SingupArea=GameUI::GetInstance().setBarArea(SignupTextFontSize, "SIGNUP", SignupPosition, 1, 30, 30);
}
void LoginMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_UsernameBarArea, 160);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_PasswordBarArea, 160);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_ConfirmArea, 160);
	GameUI::GetInstance().DrawTextOnBar(LoginMenu_ConfirmArea, ConfirmTextFontSize, "CONFIRM", ConfirmPosition.y);
	GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, UsernameTextFontSize, "USERNAME", GetScreenHeight() / 8);
	GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, PasswordTextFontSize, "PASSWORD", GetScreenHeight() / 2- GetScreenHeight() / 8);

	if (isSignupAreaActive)
	{
		GameUI::GetInstance().DrawBlackBar(LoginMenu_SingupArea, 160);
		GameUI::GetInstance().DrawTextOnBar(LoginMenu_SingupArea, SignupTextFontSize, "SIGNUP", SignupPosition.y);

	}
}
int LoginMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, LoginMenu_ConfirmArea))
	{
		return CONFIRM_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_UsernameBarArea))
	{
		return USERNAME_BAR;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_PasswordBarArea))
	{;
		return PASSWORD_BAR;
	}
	if (CheckCollisionPointRec(mousePos, LoginMenu_SingupArea) && isSignupAreaActive)
	{
		return SIGNUP_BAR;
	}
	return NOTHING;
}
void LoginMenu::insertData(string& name, string type)
{
	int key = GetCharPressed();
	while (key > 0) {
		if (key >= 32 && key <= 125) {
			name += (char)key;
		}
		key = GetCharPressed();
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !name.empty() || IsKeyPressedRepeat(KEY_BACKSPACE) && !name.empty()) {
		name.pop_back();
	}
	if (type == "username")
	{
		UserInfo::GetInstance().setUsername(name);
	}
	if (type == "password")
	{
		UserInfo::GetInstance().setPassword(name);
	}
}
bool LoginMenu::checkIsLoginCorrect()
{
	regex DataRegex("^[a-zA-Z0-9]{4,20}$");
	if (regex_match(username, DataRegex) && regex_match(password, DataRegex))
	{
		return true;
	}
	else
	{
		thread displayErrorThread(&LoginMenu::showLoginError, this);
		displayErrorThread.detach();
		error = errorType::REGEX_ERROR;
		return false;
	}
}
bool LoginMenu::checkIsPlayerInDataBase()
{
	ifstream file(data_basePath.string());
	if (!file.is_open())
	{
		return false;
	}
	regex userRegex(R"(^(\w+),(\w+),.*)");
	smatch match;
	string line;
	userExist = false;

	while (getline(file, line))
	{
		if (regex_match(line, match, userRegex) && username == match[1])
		{
			userExist = true;
			if (password == match[2])
			{
				//JAKIES GOWNO DO LADOWANIA VALUE TRINKETOW
			/*int iterator = 0;//zamienic 0 na pierwszy potrzebny match 
				for (const auto& i : jakasZmiennaKtoraPrzechowujeIloscTrinketow)
				{
					int tmpIt = iterator;
					UserInfo::GetInstance().addUserItems(match[tmpIt].str(), stob(match[tmpIt++]));
					iterator++;
				}*/
				return true;
			}
			return false;
		}
	}
	return false;
}
void LoginMenu::addPlayerToDataBase()
{
	ofstream file(data_basePath.string(), ios::app);
	string line;
	if (!file.is_open()){}
	else
	{
		file << endl << username << "," << password << ",Highest Score: 000000,";
	}

}
void LoginMenu::clearUsernameandPassword()
{
	username = "";
	password = "";
}
MenuResult LoginMenu::handleMenuLogic()
{
	Draw();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		setAction = isButtonClicked();
	}
	bool userValid;
	switch (setAction)
	{
	case 1:
		if (!checkIsLoginCorrect())
		{
			setAction = 0;
			break;
		}

		userValid = checkIsPlayerInDataBase();

		if (isSignupAreaActive)
		{
			if (userValid)
			{
				Menu::setSelectedMenu(make_unique <MainMenu>());
				return MenuResult::CONTINUE;
			}
			else
			{
				if (userExist)
				{
					thread displayErrorThread(&LoginMenu::showLoginError, this);
					displayErrorThread.detach();
					error = errorType::PASSWORD_ERROR;
				}
				else
				{
					thread displayErrorThread(&LoginMenu::showLoginError, this);
					displayErrorThread.detach();
					error = errorType::USERNAME_ERROR;
				}
			}
		}
		else 
		{
			if (userExist)
			{
				thread displayErrorThread(&LoginMenu::showLoginError, this);
				displayErrorThread.detach();
				error = errorType::IN_USE_ERROR;
			}
			else
			{
				addPlayerToDataBase();
				Menu::setSelectedMenu(make_unique <MainMenu>());
				return MenuResult::CONTINUE;
			}
		}
		setAction = 0;
		break;
	case 2:
		insertData(username, "username");
		break;
	case 3:
		insertData(password, "password");
		break;
	case 4:
		clearUsernameandPassword();
		isSignupAreaActive = false;
		setAction = 0;
		break;
	default:
		break;
	}
	if (showError)
	{
		ChooseErrorType();
	}
	GameUI::GetInstance().DrawData(username, LoginMenu_UsernameBarArea, InsertedDataFontSize);
	GameUI::GetInstance().DrawData(password, LoginMenu_PasswordBarArea, InsertedDataFontSize);
	return MenuResult::CONTINUE;
}
void LoginMenu::showLoginError()
{
	showError = true;
	std::this_thread::sleep_for(std::chrono::seconds(errorDurationTime));
	showError = false;
}
void LoginMenu::ChooseErrorType()
{
	vector<pair<string, errorType>> errors = {
	{"Error: Data does not meet criteria", errorType::REGEX_ERROR},
	{ "Error: Password is invalid", errorType::PASSWORD_ERROR},
	{ "Error: Username is invalid", errorType::USERNAME_ERROR },
	{"Error: Username is already in use", errorType::IN_USE_ERROR} };
	for (const auto& [errorInfo, type] : errors)
	{
		if (error == type) {
			GameUI::GetInstance().DrawError(errorInfo, LoginMenu_PasswordBarArea);
		}
	}
}

MainMenu::MainMenu()
{
	setAction = 0;
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundSTARTING.png", textureType::BACKGROUND_TEXTURE));
	ButtonNames = { "NEW GAME", "GAME RULES", "COLLECTION","HIGHEST SCORES","EXIT" };
	buttonsFontSize = 180;
	setButtonsPosition();

}
MainMenu::~MainMenu()
{
}
void MainMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
	for (const auto& [name, button_Data] : Buttons)
	{
		GameUI::GetInstance().DrawTextWithOutline(name, button_Data.position, buttonsFontSize);
	}
}
void MainMenu::setButtonsPosition()
{
	float spacing = GetScreenHeight() / 7;
	for (size_t i=0;i<ButtonNames.size();i++)
	{
		const string& name = ButtonNames[i];
		Vector2 measurements = GameUI::GetInstance().MeasureTextBar(buttonsFontSize, ButtonNames[i].c_str());
		Vector2 buttonPosition = { (GetScreenWidth() / 2) - (measurements.x / 2),(GetScreenHeight() / 6 + i *spacing- (measurements.y/2)) };
		Rectangle rect = { buttonPosition.x,buttonPosition.y + GetScreenHeight() * 0.01,measurements.x,measurements.y - GetScreenHeight() * 0.05 };
		Buttons[name] = {rect, buttonPosition};
	}
}
int MainMenu::isButtonClicked()
{
	Vector2 mousePos = GetMousePosition();

	if (CheckCollisionPointRec(mousePos, Buttons["NEW GAME"].rectangle))
	{
		return NEWGAME_BUTTON;
	}
	if(CheckCollisionPointRec(mousePos, Buttons["GAME RULES"].rectangle))
	{
		return RULES_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Buttons["COLLECTION"].rectangle))
	{
		return UNLOCKED_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Buttons["HIGHEST SCORES"].rectangle))
	{
		return SCORE_BUTTON;
	}
	if (CheckCollisionPointRec(mousePos, Buttons["EXIT"].rectangle))
	{
		return EXIT_BUTTON;
	}
	return NOTHING;
}
MenuResult MainMenu::handleMenuLogic()
{
	Draw();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		setAction = isButtonClicked();
	}
	switch (setAction)
	{
	case 1:
		Menu::setSelectedMenu(make_unique <CharacterSelectionMenu>());
		break;
	case 2:
		Menu::setSelectedMenu(make_unique <RulesMenu>());
		break;
	case 3:
		Menu::setSelectedMenu(make_unique <UnlockedItemsMenu>());
		break;
	case 4:
		Menu::setSelectedMenu(make_unique <HighestScoreMenu>());
		break;
	case 5:
		return MenuResult::EXIT;
		break;
	default:
		break;
	}
	setAction = 0;
	return MenuResult::CONTINUE;
}

CharacterSelectionMenu::CharacterSelectionMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundCHAR.png", textureType::BACKGROUND_TEXTURE));
	ArrowArea = {66,593,219-66,701-593};
	ConfirmArea = { 628,541,796-628,778-541 };
	CharacterInformationArea = {620,294,790-620,473-294};
	ReturnArea = {0,0,100,100};
	SmallerCommentsBar = {1035,100,300,300};
	BiggerCommentsBar = { 1035,100,300,500 };
	pageNumber = 0;
	leftSidePageLimit = -1;
	rightSidePageLimit = 1;
}
CharacterSelectionMenu::~CharacterSelectionMenu()
{
}
void CharacterSelectionMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
	DrawPlayerCharacterImage();
	chooseExplanationType();
}
void CharacterSelectionMenu::DrawComments(CommentType type)
{
	switch (type)
	{
	case SWITCH_CHARACTER:
		GameUI::GetInstance().DrawBlackBar(SmallerCommentsBar, 180);
		GameUI::GetInstance().DrawTextOnBar(SmallerCommentsBar, 55, "SWITCH CHARACTER", SmallerCommentsBar.y+20);
		GameUI::GetInstance().DrawTextOnBar(SmallerCommentsBar, 30, "CLICK THIS BUTTON TO CHANGE THE CHARACTER YOU WANT TO PLAY AS", SmallerCommentsBar.y+150);
		break;
	case SELECT_CHARACTER:
		GameUI::GetInstance().DrawBlackBar(SmallerCommentsBar, 180);
		GameUI::GetInstance().DrawTextOnBar(SmallerCommentsBar, 55, "SELECT CHARACTER", SmallerCommentsBar.y + 20);
		GameUI::GetInstance().DrawTextOnBar(SmallerCommentsBar, 30, "CLICK THIS BUTTON TO SELECT THE CHARACTER AND START A NEW GAME", SmallerCommentsBar.y + 150);
		break;
	case STATS_CHARACTER:
		GameUI::GetInstance().DrawBlackBar(BiggerCommentsBar, 180);
		GameUI::GetInstance().DrawTextOnBar(BiggerCommentsBar, 55, "CHARACTER STATS", BiggerCommentsBar.y + 20);
		GameUI::GetInstance().DrawTextOnBar(BiggerCommentsBar, 30, "STATS OF SELECTED CHARACTER ARE SHOWN BELOW", BiggerCommentsBar.y + 150);
		GameUI::GetInstance().DrawCharacterStatsInMenu(pageNumber, BiggerCommentsBar, 30, BiggerCommentsBar.y+300);
		break;
	case RETURN_BUTTON:
		GameUI::GetInstance().DrawReturnToMenuComment(SmallerCommentsBar, 180);
		break;
	default:
		break;
	}
}
void CharacterSelectionMenu::chooseExplanationType()
{
	vector < pair<Rectangle, CommentType>>comments = { { ConfirmArea, CommentType::SELECT_CHARACTER }, {ArrowArea,CommentType::SWITCH_CHARACTER},{CharacterInformationArea,CommentType::STATS_CHARACTER},{ReturnArea,CommentType::RETURN_BUTTON } };
	Vector2 mousePos = GetMousePosition();
	for (const auto& [area, type] : comments)
	{
		if (CheckCollisionPointRec(mousePos, area))
		{
			DrawComments(type);
			break;
		}
	}
}
int CharacterSelectionMenu::getPageNumber()
{
	return pageNumber;
}
void CharacterSelectionMenu::DrawPlayerCharacterImage()
{
	string file_name = "";
	switch (pageNumber)
	{
	case 0:
		file_name = "FirstCharacter.png";
		break;
	case -1:
		file_name = "SecondCharacter.png";
		break;
	case 1:
		file_name = "ThirdCharacter.png";
		break;
	default:
		file_name = "FirstCharacter.png";
		break;
	}
	DrawTextureEx(LoadingTextures::GetInstance().passCorrectTexture(file_name, textureType::OBJECT_TEXTURE), {628,535}, 0, 2, WHITE);
}
MenuResult CharacterSelectionMenu::handleMenuLogic()
{
	Draw();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Vector2 mousePos = GetMousePosition();
		if (CheckCollisionPointRec(mousePos, ConfirmArea))
		{
			return MenuResult::START_GAME;
		}
		if (CheckCollisionPointRec(mousePos, ArrowArea))
		{
			pageNumber++;
			if (pageNumber > rightSidePageLimit)
			{
				pageNumber = leftSidePageLimit;
			}
		}
		if (CheckCollisionPointRec(mousePos, ReturnArea))
		{
			Menu::setSelectedMenu(make_unique <MainMenu>());
		}
	}
	return MenuResult::CONTINUE;

}

RulesMenu::RulesMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundOPTIONS.png", textureType::BACKGROUND_TEXTURE));
}
RulesMenu::~RulesMenu()
{
}
void RulesMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
}
MenuResult RulesMenu::handleMenuLogic()
{
	Draw();
	ReturnToMenu();
	return MenuResult::CONTINUE;
}

UnlockedItemsMenu::UnlockedItemsMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundOPTIONS.png", textureType::BACKGROUND_TEXTURE));
}
UnlockedItemsMenu::~UnlockedItemsMenu()
{
}
void UnlockedItemsMenu::Draw()
{
	DrawTexture(BackgroundTexture, 0, 0, WHITE);
}
MenuResult UnlockedItemsMenu::handleMenuLogic()
{
	Draw();
	ReturnToMenu();
	return MenuResult::CONTINUE;
}

HighestScoreMenu::HighestScoreMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundOPTIONS.png", textureType::BACKGROUND_TEXTURE));
	currentPageNumber = maxPageNumber = minPageNumber = 1;
	prievousPageButton = {1083,862,1218-1083,1001-862};
	nextPageButton = { 1256,862,1391-1256 ,1001 - 862 };
	areUsersLoadedIntoVector = false;
	isNextPageButtonVisible = false;
	isPrievousPageButtonVisible = false;
}
HighestScoreMenu::~HighestScoreMenu()
{
}
void HighestScoreMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
	if (isNextPageButtonVisible)
	{
		GameUI::GetInstance().DrawBlackBar(nextPageButton, 160);
		GameUI::GetInstance().DrawTextOnBar(nextPageButton, 75, "-->",nextPageButton.y+nextPageButton.height/4);
	}
	if (isPrievousPageButtonVisible)
	{
		GameUI::GetInstance().DrawBlackBar(prievousPageButton, 160);
		GameUI::GetInstance().DrawTextOnBar(prievousPageButton, 75, "<--", prievousPageButton.y+prievousPageButton.height/4);
	}
}
void HighestScoreMenu::LoadUsersScoresIntoVector()
{
	ifstream DataBase(data_basePath.string());
	if (!DataBase.is_open())
	{
		return;
	}
	regex userRegex(R"(^(\w+),(\w+),Highest Score:\s*(\d+),.*)");
	smatch match;
	string line;

	while (getline(DataBase, line))
	{
		if (regex_match(line, match, userRegex))
		{
			UsersScores.push_back({ match[1],stoi(match[3]) });
		}
	}
	auto comparator = [](pair<string, int>a, pair<string, int>b) {return a.second > b.second; };
	sort(UsersScores.begin(), UsersScores.end(),comparator);
	if (UsersScores.size() % 10 == 0)
	{
		maxPageNumber = UsersScores.size() / 10;
	}
	else
	{
		maxPageNumber = ((UsersScores.size() / 10) + 1);
	}
	areUsersLoadedIntoVector = true;
}
void HighestScoreMenu::DrawPlayersScores()
{
	int iteratorLimit = currentPageNumber * 10;
	float gap = 90;
	string convertedData;
	int placeInList = 0;
	int Limit = iteratorLimit;
	if (currentPageNumber == maxPageNumber)
	{
		Limit = UsersScores.size();
	}
	for (int i = iteratorLimit - 10; i < Limit; i++)
	{
		placeInList = i + 1;
		GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, 50, "HIGHEST SCORES", 20);
		convertedData = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(UsersScores[i].second, 6, to_string(placeInList) + ". " + UsersScores[i].first +": ");
		GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, 50, convertedData, 20 + gap);
		gap += 90;
	}
}
void HighestScoreMenu::ArrowClicked(Rectangle bar, int action, bool visibility)
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, bar)&& visibility)
	{
		currentPageNumber+=action;
	}
}
void HighestScoreMenu::setButtonVisibility(bool& visibilty, int extremePageNumber)
{

	if (currentPageNumber ==extremePageNumber)
	{
		visibilty = false;
	}
	else
	{
		visibilty = true;
	}
}
MenuResult HighestScoreMenu::handleMenuLogic()
{
	Draw();
	setButtonVisibility(isPrievousPageButtonVisible, minPageNumber);
	setButtonVisibility(isNextPageButtonVisible, maxPageNumber);
	if (!areUsersLoadedIntoVector)
	{
		LoadUsersScoresIntoVector();
	}
	DrawPlayersScores();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		ArrowClicked(nextPageButton, 1, isNextPageButtonVisible);
		ArrowClicked(prievousPageButton, -1, isPrievousPageButtonVisible);
	}
	ReturnToMenu();
	return MenuResult::CONTINUE;
}

AfterGameMenu::AfterGameMenu()
{
	LoadTextures(LoadingTextures::GetInstance().passCorrectTexture("backgroundSTARTING.png", textureType::BACKGROUND_TEXTURE));
	ButtonNames = { "NEW GAME", "MAIN MENU","EXIT" };
	buttonsFontSize = 120;
	setButtonPosition();
}
AfterGameMenu::~AfterGameMenu()
{

}
void AfterGameMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(BackgroundTexture);
	DrawButtons();
}
void AfterGameMenu::setButtonPosition()
{
	float spacing = GetScreenHeight() / 8;
	for (size_t i = 0; i < ButtonNames.size(); i++)
	{
		const string& name = ButtonNames[i];
		Vector2 measurements = GameUI::GetInstance().MeasureTextBar(buttonsFontSize, ButtonNames[i].c_str());
		Vector2 buttonPosition = { (GetScreenWidth() / 2) - (measurements.x / 2),(GetScreenHeight() / 6 + i * spacing) - (measurements.y / 2) };
		Rectangle rect = GameUI::GetInstance().setBarArea(buttonsFontSize, ButtonNames[i], buttonPosition, 1, 0, 0);
		Buttons[name] = { rect, buttonPosition };
	}
}
void AfterGameMenu::DrawButtons()
{
	for (const auto& [name, button_Data] : Buttons)
	{
		GameUI::GetInstance().DrawTextWithOutline(name, button_Data.position, buttonsFontSize);
	}
}
MenuResult AfterGameMenu::handleMenuLogic()
{
	Draw();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mousePos = GetMousePosition();
		if (CheckCollisionPointRec(mousePos, Buttons["NEW GAME"].rectangle))
		{
			Menu::setSelectedMenu(make_unique<CharacterSelectionMenu>());
			return MenuResult::CONTINUE;
		}
		if (CheckCollisionPointRec(mousePos, Buttons["MAIN MENU"].rectangle))
		{
			Menu::setSelectedMenu(make_unique<MainMenu>());
			return MenuResult::CONTINUE;
		}
		if (CheckCollisionPointRec(mousePos, Buttons["EXIT"].rectangle))
		{
			return MenuResult::EXIT;
		}
	}
	return MenuResult::AFTER_GAME;
}
void AfterGameMenu::updatePlayerScoreInDataBase(int playerScore, string username)
{
	ifstream DataBase(data_basePath);
	ofstream UpdatedDataBase("temp.txt");
	if (!DataBase.is_open() || !UpdatedDataBase.is_open()) 
	{
		return;
	}

	regex usersScoresRegex(R"(^(\w+),(\w+),Highest Score:\s*(\d+),(.*))");
	smatch match;
	string line;

	while (getline(DataBase, line))
	{
		if (regex_match(line, match, usersScoresRegex) && match[1] == username)
		{
			if (isNewScoreHigher(stoi(match[3]), playerScore))
			{
				string newScore = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(playerScore, 6, "");
				line = match[1].str() + "," + match[2].str() + ",Highest Score: " + newScore + "," + match[4].str();
			}
		}
		UpdatedDataBase << line<<"\n";
	}
	DataBase.close();
	UpdatedDataBase.close();
	fs::remove(data_basePath);
	fs::rename("temp.txt", data_basePath.c_str());
}
bool AfterGameMenu::isNewScoreHigher(int DataBaseScore, int currentScore)
{
	if (currentScore > DataBaseScore)
	{
		return true;
	}
	return false;
}