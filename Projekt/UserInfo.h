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
namespace fs = filesystem;

class UserInfo
{
public:
	static UserInfo& GetInstance();
	string getUsername();
	void setUsername(string username);
	string getPassword();
	void setPassword(string password);
	~UserInfo();
private:
	UserInfo();
	UserInfo(const UserInfo&) = delete;
	void operator=(const UserInfo&) = delete;
	fs::path DataBase_path;
	string username;
	string password;

};