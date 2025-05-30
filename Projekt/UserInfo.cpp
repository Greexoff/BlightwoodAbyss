#include "UserInfo.h"

bool stob(string value)
{
	int tmpInt = stoi(value);
	if (tmpInt == 1)
	{
		return true;
	}
	return false;
}

UserInfo::UserInfo()
{
	DataBase_path = fs::current_path() / "database" / "DataBase.txt";
	username = "";
	password = "";

}
UserInfo::~UserInfo()
{

}
UserInfo& UserInfo::GetInstance()
{
	static UserInfo instance;
	return instance;
}

string UserInfo::getUsername()
{
	return username;
}
void UserInfo::setUsername(string username)
{
	this->username = username;
}
string UserInfo::getPassword()
{
	return password;
}
void UserInfo::setPassword(string password)
{
	this->password = password;
}
void UserInfo::addUserItems(string itemName, bool isItemUnlocked)
{
	UserItems.insert({ itemName,isItemUnlocked });
}
bool UserInfo::getUserItemValue(string itemName)
{
	for (auto [Name, Value] : UserItems)
	{
		if (Name == itemName)
		{
			return Value;
		}
	}
	return false;
}
map<string, bool> UserInfo::getUserItems()
{
	return UserItems;
}
void UserInfo::updateUserItems(string itemName, bool value)
{
	for (auto& [trinketName, trinketValue] : UserItems)
	{
		if (trinketName == itemName)
		{
			trinketValue = value;
		}
	}
}



