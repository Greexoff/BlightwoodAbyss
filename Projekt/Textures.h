#pragma once
#include <iostream>
#include <raylib.h>
#include <string>
#include <filesystem>
#include <map>
#include <string>

using namespace std;
namespace fs = filesystem;
enum class textureType
{
	BACKGROUND_TEXTURE,
	OBJECT_TEXTURE,
	FONT_TEXTURE,
};
class LoadingTextures
{
public:
	static LoadingTextures& GetInstance();
	~LoadingTextures();
	Texture2D& passCorrectTexture(string textureName, textureType typeOfTexture);
	Font& getFont();
private:
	LoadingTextures();
	LoadingTextures(const LoadingTextures&) = delete;
	void operator=(const LoadingTextures&) = delete;
	fs::path BackgroundTexturesPath;
	fs::path ObjectTexturesPath;
	fs::path FontPath;
	map<string, Texture2D> BackgroundTextures;
	map<string, Texture2D> ObjectTextures;
	Font font;
	void setPathsToDirectories();
	void loadTextures(fs::path directory_path, map<string,Texture2D>& mapTextures);
	void loadFont(fs::path directory_path);
	void unloadTextures(map<string, Texture2D>& mapTextures);
};