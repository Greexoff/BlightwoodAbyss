#pragma once

#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

class Menu
{
public:
	Menu();
	bool LoginMenu_active;
	void DrawLoginMenu();
	bool isButtonClicked();
	
private:
	Texture2D LoginMenu_background;
	Rectangle LoginMenu_confirmArea;
};