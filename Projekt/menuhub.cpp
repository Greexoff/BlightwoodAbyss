#include "menuhub.h"

unique_ptr<Menu> Menu::selectedMenu = nullptr;
Menu::~Menu()
{
}

void Menu::setButtonsAreas(Rectangle retButton, Rectangle leftArrow, Rectangle rightArrow)
{
	LeftArrow = leftArrow;
	RightArrow = rightArrow;
	ReturnButton = retButton;
}
void Menu::setAmountOfPages(int number)
{
	LeftSidePageLimit = 1;
	RightSidePageLimit=number;
}
void Menu::switchPage(int number)
{
	pageNumber += number;
}
void Menu::setButtonVisibility()
{
	if (LeftSidePageLimit == RightSidePageLimit)
	{
		isLeftArrowVisible = false;
		isRightArrowVisible = false;
	}
	else
	{
		isLeftArrowVisible = (pageNumber != LeftSidePageLimit);
		isRightArrowVisible = (pageNumber != RightSidePageLimit);
	}
}
bool Menu::getButtonVisibility(Page type)
{
	switch (type)
	{
	case Page::LEFT_SIDE_LIMIT:
		return isLeftArrowVisible;
		break;
	case Page::RIGHT_SIDE_LIMIT:
		return isRightArrowVisible;
		break;
	default:
		break;
	}
	return true;
}
int Menu::getPage(Page type)
{
	switch (type)
	{
	case Page::CURRENT_PAGE:
		return pageNumber;
		break;
	case Page::LEFT_SIDE_LIMIT:
		return LeftSidePageLimit;
		break;
	case Page::RIGHT_SIDE_LIMIT:
		return RightSidePageLimit;
		break;
	default:
		break;
	}
	return 0;
}
void Menu::setMenuElements()
{
	setButtonVisibility();
	Rectangle returnButton = { 413 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 912 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(930 - 413) * ScreenSettings::GetInstance().getScreenResolutionFactor().x ,(1030 - 912) * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	Rectangle leftArrowButton = { 989 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 912 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(1218 - 989) * ScreenSettings::GetInstance().getScreenResolutionFactor().x ,(1030 - 912) * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	Rectangle rightArrowButton = { 1277 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 912 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(1506 - 1277) * ScreenSettings::GetInstance().getScreenResolutionFactor().x ,(1030 - 912) * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	if (!getButtonVisibility(Page::LEFT_SIDE_LIMIT) && !getButtonVisibility(Page::RIGHT_SIDE_LIMIT))
	{
		returnButton = { 701 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 912 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(1218 - 701) * ScreenSettings::GetInstance().getScreenResolutionFactor().x ,(1030 - 912) * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	}

	setButtonsAreas(returnButton, leftArrowButton, rightArrowButton);
}
void Menu::drawMenuElements()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(LoadingTextures::GetInstance().passCorrectTexture("backgroundOPTIONS.png", textureType::BACKGROUND_TEXTURE));
	GameUI::GetInstance().DrawBlackBar(ReturnButton, 180);
	GameUI::GetInstance().DrawTextOnBar(ReturnButton, ReturnButton.height*0.5, "RETURN", ReturnButton.y + ReturnButton.height * 0.25);
	if (isLeftArrowVisible)
	{
	GameUI::GetInstance().DrawBlackBar(LeftArrow, 180);
	GameUI::GetInstance().DrawTextOnBar(LeftArrow, LeftArrow.height * 0.5, "<", LeftArrow.y + LeftArrow.height * 0.25);
	}
	if (isRightArrowVisible)
	{
	GameUI::GetInstance().DrawBlackBar(RightArrow, 180);
	GameUI::GetInstance().DrawTextOnBar(RightArrow, RightArrow.height*0.5, ">", RightArrow.y+RightArrow.height*0.25);
	}
}
Rectangle Menu::getButtonArea(ButtonType type)
{
	switch (type)
	{
	case ButtonType::RETURN_BUTTON:
		return ReturnButton;
		break;
	case ButtonType::LEFT_ARROW_BUTTON:
		return LeftArrow;
		break;
	case ButtonType::RIGHT_ARROW_BUTTON:
		return RightArrow;
		break;
	default:
		break;
	}
	return {};
}
void Menu::setSelectedMenu(unique_ptr<Menu> newMenu)
{
	selectedMenu = move(newMenu);
}
Menu* Menu::getSelectedMenu()
{
	return selectedMenu.get();
}
void Menu::CheckCollisions()
{
	Vector2 mousePos = GetMousePosition();
	if (getButtonVisibility(Page::LEFT_SIDE_LIMIT) && CheckCollisionPointRec(mousePos, LeftArrow) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		switchPage(-1);
	}
	if (getButtonVisibility(Page::RIGHT_SIDE_LIMIT) && CheckCollisionPointRec(mousePos, RightArrow) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		switchPage(1);
	}
	if (CheckCollisionPointRec(mousePos, ReturnButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		setSelectedMenu(make_unique<MainMenu>());
		return;
	}

}

StartingMenu::StartingMenu()
{
	titleFontSize = 200;
	titleName = "BLIGHTWOOD ABBYS";
}
StartingMenu::~StartingMenu()
{
}
void StartingMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(LoadingTextures::GetInstance().passCorrectTexture("backgroundSTARTING.png", textureType::BACKGROUND_TEXTURE));
	GameUI::GetInstance().DrawTextOnBar({ (float)GetScreenWidth()*0.2f,0,(float)GetScreenWidth()*0.6f,(float)GetScreenHeight() }, titleFontSize * ScreenSettings::GetInstance().getScreenResolutionFactor().y, titleName, GetScreenHeight()*0.3);
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
	setAction = 0;
	username = "";
	password = "";
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
	ReturnButtonFontSize= 75 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	UsernameTextFontSize = 150 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	PasswordTextFontSize = 150 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	ConfirmTextFontSize = 75 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	SignupTextFontSize = 75 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	InsertedDataFontSize = 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;

}
void LoginMenu::setBarAreas()
{
	Vector2 measureReturn = GameUI::GetInstance().MeasureText(ReturnButtonFontSize, "RETURN");
	Vector2 measureConfirm = GameUI::GetInstance().MeasureText(ConfirmTextFontSize, "CONFIRM");
	Vector2 measureUsername = GameUI::GetInstance().MeasureText(UsernameTextFontSize, "USERNAME");
	Vector2 measurePassword = GameUI::GetInstance().MeasureText(PasswordTextFontSize, "PASSWORD");
	Vector2 measureSignup = GameUI::GetInstance().MeasureText(ConfirmTextFontSize, "SIGNUP");
	UsernamePosition = { (float)(GetScreenWidth() * 0.5 - measureUsername.x * 0.5),(float)(GetScreenHeight() * 0.05) };
	ConfirmPosition = { (float)(GetScreenWidth() * 0.5 - measureConfirm.x * 0.5),0};
	PasswordPosition = { (float)(GetScreenWidth() * 0.5 - measurePassword.x * 0.5),0};
	SignupPosition = { (float)(GetScreenWidth() * 0.775 - measureSignup.x * 0.5),0};
	LoginMenu_UsernameBarArea=GameUI::GetInstance().setBarArea(UsernameTextFontSize, "USERNAME", UsernamePosition, 2, 10*ScreenSettings::GetInstance().getScreenResolutionFactor().x, 10 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
	PasswordPosition.y = LoginMenu_UsernameBarArea.y + LoginMenu_UsernameBarArea.height + GetScreenHeight()*0.01;
	LoginMenu_PasswordBarArea=	GameUI::GetInstance().setBarArea(PasswordTextFontSize, "PASSWORD", PasswordPosition, 2, 10 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 10 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
	ConfirmPosition.y = LoginMenu_PasswordBarArea.y + LoginMenu_PasswordBarArea.height + GetScreenHeight() * 0.125;
	SignupPosition.y = ConfirmPosition.y;
	ReturnPosition = SignupPosition;
	LoginMenu_ConfirmArea=GameUI::GetInstance().setBarArea(ConfirmTextFontSize, "CONFIRM", ConfirmPosition, 1, 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
	LoginMenu_SingupArea=GameUI::GetInstance().setBarArea(SignupTextFontSize, "SIGNUP", SignupPosition, 1, 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
	ReturnButtonArea = GameUI::GetInstance().setBarArea(ReturnButtonFontSize, "RETURN", ReturnPosition, 1, 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().x, 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
}
void LoginMenu::Draw()
{
	setFontSizes();
	setBarAreas();
	GameUI::GetInstance().DrawScaledBackgroundImage(LoadingTextures::GetInstance().passCorrectTexture("backgroundLOGIN.png", textureType::BACKGROUND_TEXTURE));
	GameUI::GetInstance().DrawBlackBar(LoginMenu_UsernameBarArea, 160);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_PasswordBarArea, 160);
	GameUI::GetInstance().DrawBlackBar(LoginMenu_ConfirmArea, 160);
	GameUI::GetInstance().DrawTextOnBar(LoginMenu_ConfirmArea, ConfirmTextFontSize, "CONFIRM", ConfirmPosition.y);
	GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, UsernameTextFontSize, "USERNAME", UsernamePosition.y-GetScreenHeight()*0.01);
	GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, PasswordTextFontSize, "PASSWORD", PasswordPosition.y - GetScreenHeight() * 0.01);
	if (isSignupAreaActive)
	{
		GameUI::GetInstance().DrawBlackBar(LoginMenu_SingupArea, 160);
		GameUI::GetInstance().DrawTextOnBar(LoginMenu_SingupArea, SignupTextFontSize, "SIGNUP", SignupPosition.y);
	}
	else
	{
		GameUI::GetInstance().DrawBlackBar(ReturnButtonArea, 160);
		GameUI::GetInstance().DrawTextOnBar(ReturnButtonArea, ReturnButtonFontSize, "RETURN", ReturnPosition.y);
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
	if (CheckCollisionPointRec(mousePos, ReturnButtonArea))
	{
		clearUsernameandPassword();
		isSignupAreaActive = true;
		return NOTHING;
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
	this_thread::sleep_for(chrono::seconds(errorDurationTime));
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
	ButtonNames = { "NEW GAME", "GAME RULES", "COLLECTION","HIGHEST SCORES","EXIT" };
	setButtonsPosition();
	baseButtonsFontSize = 180;
	buttonsFontSize = baseButtonsFontSize;
}
MainMenu::~MainMenu()
{
}
void MainMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(LoadingTextures::GetInstance().passCorrectTexture("backgroundMAIN.png", textureType::BACKGROUND_TEXTURE));
	
	for (const auto& [name, button_Data] : Buttons)
	{
		GameUI::GetInstance().DrawTextWithOutline(name, button_Data.position, buttonsFontSize);
	}
	buttonsFontSize = baseButtonsFontSize;
}
void MainMenu::setButtonsPosition()
{
	buttonsFontSize *= ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	float spacing = GetScreenHeight() / 7;
	for (size_t i=0;i<ButtonNames.size();i++)
	{
		const string& name = ButtonNames[i];
		Vector2 measurements = GameUI::GetInstance().MeasureText(buttonsFontSize, ButtonNames[i].c_str());
		Vector2 buttonPosition = { (GetScreenWidth() / 2) - (measurements.x / 2),(GetScreenHeight() / 6 + i *spacing- (measurements.y/2)) };
		Rectangle rect = { buttonPosition.x,buttonPosition.y+GetScreenHeight()*0.03,measurements.x,measurements.y-GetScreenHeight()*0.055};
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
	setButtonsPosition();
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
	setAreas();
	pageNumber = 0;
	leftSidePageLimit = -1;
	rightSidePageLimit = 1;
}
CharacterSelectionMenu::~CharacterSelectionMenu()
{
}
void CharacterSelectionMenu::Draw()
{
	GameUI::GetInstance().DrawScaledBackgroundImage(LoadingTextures::GetInstance().passCorrectTexture("backgroundCHAR.png", textureType::BACKGROUND_TEXTURE));
	DrawPlayerCharacterImage();
	chooseExplanationType();
}
void CharacterSelectionMenu::setAreas()
{
	Texture2D& characterTexture = LoadingTextures::GetInstance().passCorrectTexture("FirstCharacter.png", textureType::OBJECT_TEXTURE);
	textureScale = 2.8 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	ConfirmArea = { 894 * ScreenSettings::GetInstance().getScreenResolutionFactor().x ,584 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(characterTexture.width*textureScale),(characterTexture.height*textureScale)};
	ArrowArea = { 1028 * ScreenSettings::GetInstance().getScreenResolutionFactor().x,363 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(1247 - 1028) * ScreenSettings::GetInstance().getScreenResolutionFactor().x,(497 - 363) * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	CharacterInformationArea = { 320 * ScreenSettings::GetInstance().getScreenResolutionFactor().x,225 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,(593 - 320) * ScreenSettings::GetInstance().getScreenResolutionFactor().x,(464 - 225) * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	ReturnArea = { 1404 * ScreenSettings::GetInstance().getScreenResolutionFactor().x ,320 * ScreenSettings::GetInstance().getScreenResolutionFactor().y ,(1700 - 1404)* ScreenSettings::GetInstance().getScreenResolutionFactor().x ,(777 - 320)* ScreenSettings::GetInstance().getScreenResolutionFactor().y};
	SmallerCommentsBar = { 1304 * ScreenSettings::GetInstance().getScreenResolutionFactor().x,115 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,500 * ScreenSettings::GetInstance().getScreenResolutionFactor().x,300 * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
	BiggerCommentsBar = { 1304 * ScreenSettings::GetInstance().getScreenResolutionFactor().x,115 * ScreenSettings::GetInstance().getScreenResolutionFactor().y,500 * ScreenSettings::GetInstance().getScreenResolutionFactor().x,530 * ScreenSettings::GetInstance().getScreenResolutionFactor().y };
}
void CharacterSelectionMenu::DrawComments(CommentType type)
{
	switch (type)
	{
	case SWITCH_CHARACTER:
		GameUI::GetInstance().DrawComments("SWITCH CHARACTER", "CLICK THIS BUTTON TO CHANGE THE CHARACTER YOU WANT TO PLAY AS", SmallerCommentsBar);
		break;
	case SELECT_CHARACTER:
		GameUI::GetInstance().DrawComments("SELECT CHARACTER", "CLICK THIS BUTTON TO SELECT THE CHARACTER AND START A NEW GAME", SmallerCommentsBar);
		break;
	case STATS_CHARACTER:
		GameUI::GetInstance().DrawComments("CHARACTER STATS", "STATS OF SELECTED CHARACTER ARE SHOWN BELOW", BiggerCommentsBar);
		GameUI::GetInstance().DrawCharacterStatsInMenu(pageNumber, BiggerCommentsBar, 40*ScreenSettings::GetInstance().getScreenResolutionFactor().x, BiggerCommentsBar.y+(300 * ScreenSettings::GetInstance().getScreenResolutionFactor().y));
		break;
	case RETURN_BUTTON:
		GameUI::GetInstance().DrawComments("RETURN TO MENU", "CLICK THIS BUTTON TO RETURN TO MAIN MENU", SmallerCommentsBar);
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
	DrawTextureEx(LoadingTextures::GetInstance().passCorrectTexture(file_name, textureType::OBJECT_TEXTURE) , { ConfirmArea.x,ConfirmArea.y }, 0, textureScale, WHITE);
}
MenuResult CharacterSelectionMenu::handleMenuLogic()
{
	setAreas();
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
	setPagesContent();
	int amountOfPages = GameInfoPages.size() + CharInfoPages.size() + EnemyInfoPages.size() + ItemsInfoPages.size();
	setAmountOfPages(amountOfPages);
}

RulesMenu::~RulesMenu()
{
}
void RulesMenu::setPagesContent()
{
	GameInfoPages = 
	{
		{ "GAME RULES", {
		"THE OBJECTIVE OF THE GAME IS SURVIVING AS MANY WAVES OF ENEMIES AS POSSIBLE AND GETTING THE HIGHEST SCORE IN THE PROCESS.",
		"BEFORE STARTING THE GAME, YOU CHOOSE ONE OF THE AVAILABLE CHARACTERS. EACH CHARACTER HAVE DIFFERENT SET OF STATS.",
		"THE INITIAL WAVE CONTAINS 5 OPPONENTS, AND WITH EACH SUBSEQUENT WAVE THE NUMBER OF OPPONENTS INCREASES."}},

		{"GAME RULES", {
		"EVERY 5 WAVES YOU FACE A BOSS, WHO DROPS ITEMS AFTER YOU DEFEAT HIM.",
		"YOU CAN UNLOCK ITEMS BY COMPLETING THE QUESTS, WHICH ARE SHOWN ON THE PAGE DESCRIBING EACH ITEM.",
		"ALL CHARACTER, ENEMY AND ITEM STATS ARE AVAILABLE ON THE FOLLOWING PAGES."}}
	};
	CharInfoPages =
	{
		{"CHARACTER STATS", "FIRST CHARACTER", { "CHARACTER STATS ARE SHOWN BELOW:" }, "FirstCharacter.png", 0},
		{"CHARACTER STATS", "SECOND CHARACTER", { "CHARACTER STATS ARE SHOWN BELOW:" }, "SecondCharacter.png", -1},
		{"CHARACTER STATS", "THIRD CHARACTER", { "CHARACTER STATS ARE SHOWN BELOW:" }, "ThirdCharacter.png", 1},
	};

	EnemyInfoPages = 
	{
		{"ENEMY STATS", "FIRST ENEMY", "Monster1", { "THIS ENEMY WILL CONSTANTLY FOLLOW YOU ALTHOUGH THEY ARE RATHER SLOW.","THEY CANNOT SHOOT TEARS." }, "Enemy1.png"},
		{"ENEMY STATS", "SECOND ENEMY", "Monster2", { "THIS ENEMY WILL FOLLOW YOU, HOWEVER THEY CAN BE DESTROYED WITH SINGLE TEAR.","THIS ONE CANNOT SHOOT TEARS."}, "Enemy2.png"},
		{"ENEMY STATS", "THIRD ENEMY", "Monster3", { "THIS ENEMY CANNOT MOVE, HOWEVER THEY WILL SHOOT YOU FROM DISTANCE." }, "Enemy3.png"},
		{"ENEMY STATS", "FOURTH ENEMY", "Monster4", { "THIS ENEMY WILL BOTH FOLLOW YOU AND SHOOT TEARS AT YOU.","THERE CAN ONLY BE 6 OF THEM PER WAVE."}, "Enemy4.png"},
		{"ENEMY STATS", "FIFTH ENEMY", "Monster5", { "THIS BOSS WILL CHASE YOU AND SHOOT WITH VERY RAPID TEARS.","THEY HAVE HUGE HITBOX, SO IT WILL BE DIFFICULT TO MISS THEM." }, "Enemy5.png"},
	};
	ItemsInfoPages =
	{
		{"COLLECTIBLE ITEMS","DAMAGE TRINKET",{"INCREASES PLAYER'S DAMAGE BY 1.5","GAP","HOW TO UNLOCK:","ITEM AVAIABLE RIGHT AWAY!"}, "DamageTrinket.png"},
		{"COLLECTIBLE ITEMS","HEALTH TRINKET",{"INCREASES PLAYER'S MAX HEALTH BY 1","GAP","HOW TO UNLOCK:","SURVIVE 15 WAVES WITHOUT TAKING ANY DAMAGE"}, "HealthTrinket.png"},
		{"COLLECTIBLE ITEMS","SPEED TRINKET",{"INCREASES PLAYER'S SPEED BY 1.5" ,"GAP","HOW TO UNLOCK:","LAST 1 MINUTE DURING A BOSS FIGHT"}, "SpeedTrinket.png"},
		{"COLLECTIBLE ITEMS","TEAR RATE TRINKET",{"REDUCES TIME BETWEEN SHOTS BY 0.04 (MIN IS 0.1)","GAP","HOW TO UNLOCK:", "DEFEAT FIRST WAVE WITHIN 10 SECONDS"}, "TearRateTrinket.png"},
		{"COLLECTIBLE ITEMS","TEAR SPEED TRINKET",{"INCREASES PLAYER'S TEAR SPEED BY 0.5","GAP","HOW TO UNLOCK:", "SURVIVE 20 WAVES WITHOUT PICKING ANY ITEM"}, "TearSpeedTrinket.png"},
	};
}
void RulesMenu::Draw()
{
	drawMenuElements();
	GameUI::GetInstance().DrawBlackBar(GameInfoBar, 180);
	DrawRules(getPage(Page::CURRENT_PAGE));
}
MenuResult RulesMenu::handleMenuLogic()
{
	GameInfoBar = { GetScreenWidth() * 0.15f, GetScreenHeight() * 0.10f, GetScreenWidth() * 0.7f,GetScreenHeight() * 0.7f};
	setMenuElements();
	Draw();
	CheckCollisions();
	return MenuResult::CONTINUE;
}
void RulesMenu::DrawRules(int page)
{

	Rectangle imagesBar = { GameInfoBar.x,GameInfoBar.y,GameInfoBar.width * 0.4,GameInfoBar.height };
	Rectangle paragraphsWithImagesBar = { GameInfoBar.x + (GameInfoBar.width * 0.4),GameInfoBar.y,GameInfoBar.width * 0.6,GameInfoBar.height };
	float textureScale = 3.5 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;

	float y_pos = GameInfoBar.y + (20 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
	float smallerFont = 50 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	float mediumFont = 80 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	float biggerFont = 120 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;

	int GameInfoLimit = GameInfoPages.size();
	int CharInfoLimit = GameInfoPages.size() + CharInfoPages.size();
	int EnemyInfoLimit = CharInfoLimit + EnemyInfoPages.size();
	int ItemsInfoLimit = EnemyInfoLimit + ItemsInfoPages.size();

	if (page >= 1 && page <= GameInfoLimit)
	{
		auto& GameInfoPage = GameInfoPages[page - 1];
		GameUI::GetInstance().DrawTextRules(GameInfoBar, biggerFont, GameInfoPage.title, y_pos);
		for (const auto& line : GameInfoPage.paragraphs) {
			GameUI::GetInstance().DrawTextRules(GameInfoBar, smallerFont, line, y_pos);
		}
	}
	if(page > GameInfoPages.size() && page <= CharInfoLimit)
	{
		auto& CharPage = CharInfoPages[page - GameInfoLimit - 1];

		GameUI::GetInstance().DrawTextRules(GameInfoBar, biggerFont, CharPage.title, y_pos);

		auto& characterTexture = LoadingTextures::GetInstance().passCorrectTexture(CharPage.textureName, textureType::OBJECT_TEXTURE);
		Vector2 characterPos = { (imagesBar.x + imagesBar.width * 0.6f) - (characterTexture.width * textureScale * 0.5f),(imagesBar.y + imagesBar.height * 0.5f) - (characterTexture.height * textureScale * 0.5f) };
		DrawTextureEx(characterTexture, characterPos, 0, textureScale, WHITE);

		GameUI::GetInstance().DrawTextRules(paragraphsWithImagesBar, mediumFont, CharPage.characterName, y_pos);

		for (const auto& line : CharPage.paragraphs) {
			GameUI::GetInstance().DrawTextRules(paragraphsWithImagesBar, smallerFont, line, y_pos);
		}

		GameUI::GetInstance().DrawCharacterStatsInMenu(CharPage.pageNumber, paragraphsWithImagesBar, smallerFont, y_pos);
	}
	if (page > CharInfoLimit && page <= EnemyInfoLimit)
	{
		auto& EnemyPage = EnemyInfoPages[page - CharInfoLimit - 1];

		GameUI::GetInstance().DrawTextRules(GameInfoBar, biggerFont, EnemyPage.title, y_pos);

		auto& enemyTexture = LoadingTextures::GetInstance().passCorrectTexture(EnemyPage.textureName, textureType::OBJECT_TEXTURE);
		Vector2 enemyPos = { (imagesBar.x + imagesBar.width * 0.6f) - (enemyTexture.width * textureScale * 0.5f),(imagesBar.y + imagesBar.height * 0.5f) - (enemyTexture.height * textureScale * 0.5f) };
		DrawTextureEx(enemyTexture, enemyPos, 0, textureScale, WHITE);

		GameUI::GetInstance().DrawTextRules(paragraphsWithImagesBar, mediumFont, EnemyPage.enemyDescription, y_pos);
		for (const auto& line : EnemyPage.paragraphs) {
			GameUI::GetInstance().DrawTextRules(paragraphsWithImagesBar, smallerFont, line, y_pos);
		}

		GameUI::GetInstance().DrawEnemyStatsInMenu(EnemyPage.enemyName, paragraphsWithImagesBar, smallerFont, y_pos);
	}
	if (page > EnemyInfoLimit && page <= ItemsInfoLimit)
	{
		auto& ItemsPage = ItemsInfoPages[page - EnemyInfoLimit - 1];

		GameUI::GetInstance().DrawTextRules(GameInfoBar, biggerFont, ItemsPage.title, y_pos);

		auto& itemTexture = LoadingTextures::GetInstance().passCorrectTexture(ItemsPage.textureName, textureType::OBJECT_TEXTURE);
		Vector2 itemPos = { (imagesBar.x + imagesBar.width * 0.6f) - (itemTexture.width * textureScale * 0.5f),(imagesBar.y + imagesBar.height * 0.5f) - (itemTexture.height * textureScale * 0.5f) };
		DrawTextureEx(itemTexture, itemPos, 0, textureScale, WHITE);
		
		GameUI::GetInstance().DrawTextRules(paragraphsWithImagesBar, mediumFont, ItemsPage.itemName, y_pos);
		for (const auto& line : ItemsPage.paragraphs) {
			GameUI::GetInstance().DrawTextRules(paragraphsWithImagesBar, smallerFont, line, y_pos);
		}
	}
}

UnlockedItemsMenu::UnlockedItemsMenu()
{
}
UnlockedItemsMenu::~UnlockedItemsMenu()
{
}
void UnlockedItemsMenu::Draw()
{
	drawMenuElements();
}
MenuResult UnlockedItemsMenu::handleMenuLogic()
{
	setMenuElements();
	Draw();
	CheckCollisions();
	return MenuResult::CONTINUE;
}

HighestScoreMenu::HighestScoreMenu()
{
	areUsersLoadedIntoVector = false;
}
HighestScoreMenu::~HighestScoreMenu()
{
}
void HighestScoreMenu::Draw()
{
	drawMenuElements();
	DrawPlayersScores();
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
	if (UsersScores.size() % 5 == 0)
	{
		setAmountOfPages(UsersScores.size() / 5);
	}
	else
	{
		setAmountOfPages(((UsersScores.size() / 5) + 1));
	}
	areUsersLoadedIntoVector = true;
}
void HighestScoreMenu::DrawPlayersScores()
{
	int iteratorLimit = getPage(Page::CURRENT_PAGE) * 5;
	float gap = 100 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	string convertedData;
	int placeInList = 0;
	int Limit = iteratorLimit;
	if (getPage(Page::CURRENT_PAGE) == getPage(Page::RIGHT_SIDE_LIMIT))
	{
		Limit = UsersScores.size();
	}
	for (int i = iteratorLimit - 5; i < Limit; i++)
	{
		placeInList = i + 1;
		GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, 150*ScreenSettings::GetInstance().getScreenResolutionFactor().y, "HIGHEST SCORES", 30 * ScreenSettings::GetInstance().getScreenResolutionFactor().y);
		convertedData = GameUI::GetInstance().CreateTextWithLeadingZerosGameUI(UsersScores[i].second, 6, to_string(placeInList) + ". " + UsersScores[i].first +": ");
		GameUI::GetInstance().DrawTextOnBar({ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, 85 * ScreenSettings::GetInstance().getScreenResolutionFactor().y, convertedData, 150 * ScreenSettings::GetInstance().getScreenResolutionFactor().y + gap);
		gap += 100 * ScreenSettings::GetInstance().getScreenResolutionFactor().y;
	}
}
MenuResult HighestScoreMenu::handleMenuLogic()
{
	if (!areUsersLoadedIntoVector)
	{
		LoadUsersScoresIntoVector();
	}
	setMenuElements();
	Draw();
	CheckCollisions();
	return MenuResult::CONTINUE;
}
