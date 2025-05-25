#include "Textures.h"

LoadingTextures::LoadingTextures()
{
	setPathsToDirectories();
	loadTextures(BackgroundTexturesPath,BackgroundTextures);
	loadTextures(ObjectTexturesPath,ObjectTextures);
	loadFont(FontPath);
}
LoadingTextures& LoadingTextures::GetInstance()
{
	static LoadingTextures instance;
	return instance;
}
LoadingTextures::~LoadingTextures()
{
	unloadTextures(BackgroundTextures);
	unloadTextures(ObjectTextures);
	UnloadFont(font);
}
void LoadingTextures::setPathsToDirectories()
{
	BackgroundTexturesPath=fs::current_path()/"assets"/"background_assets";
	ObjectTexturesPath=fs::current_path() / "assets" / "object_assets";
	FontPath=fs::current_path() / "assets" / "font_assets";
}
void LoadingTextures::loadTextures(fs::path directoryPath, map<string, Texture2D>& mapTextures)
{
	for (const auto& texture : fs::directory_iterator(directoryPath))
	{
		mapTextures.insert({ texture.path().filename().string(), LoadTexture(texture.path().string().c_str()) });
	}
}
void LoadingTextures::loadFont(fs::path directoryPath)
{
	fs::path wholePath = directoryPath / "VT323.ttf";
	font = LoadFontEx(wholePath.string().c_str(), 80, 0, 0);
}
void LoadingTextures::unloadTextures(map<string, Texture2D>& mapTextures)
{
	for (auto& i : mapTextures)
	{
		UnloadTexture(i.second);
	}
	mapTextures.clear();
}
Texture2D& LoadingTextures::passCorrectTexture(string textureName, textureType typeOfTexture)
{
	map<string, Texture2D>* selectedMap = nullptr;
	switch (typeOfTexture)
	{
	case textureType::BACKGROUND_TEXTURE:
		selectedMap = &BackgroundTextures;
		break;
	case textureType::OBJECT_TEXTURE:
		selectedMap = &ObjectTextures;
		break;
	default:
		break;
	}
	if (selectedMap)
	{	
		for (auto& [name, texture] : *selectedMap)
		{
			if (name == textureName)
			{
				return texture;
			}
		}
	}
	static Texture2D missingTexture{};
	return missingTexture;
}
Font& LoadingTextures::getFont()
{
	return font;
}