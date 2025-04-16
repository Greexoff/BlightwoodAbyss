#pragma once
#include <iostream>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>

using namespace std;
namespace fs = filesystem;

class GameUI
{
public:
	GameUI();
	~GameUI();
	void setLoadingProgress(string backgroundName);
	void DrawBackground();
private:
	Texture2D currentBackground;
	void loadTexturesIntoMap();
	Texture2D passCorrectTexture(string textureName);
	fs::path loadingScreenAssetsPath;
	map<string, Texture2D> loadedScreenTextures;
};