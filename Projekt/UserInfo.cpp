#include "UserInfo.h"

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


