#pragma once
#include <iostream>
#include <vector>
#include <raylib.h>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <thread>
using namespace std;

class ScreenSettings
{
public:
	static ScreenSettings& GetInstance();
	Vector2 getScreenResolutionFactor();
	void setResolutionFactor(int windowWidth, int windowHeight);
	~ScreenSettings();
private:
	Vector2 resolutionFactor;
	ScreenSettings();
	ScreenSettings(const ScreenSettings&) = delete;
	void operator=(const ScreenSettings&) = delete;
};