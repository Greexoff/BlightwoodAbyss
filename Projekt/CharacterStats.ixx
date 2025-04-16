export module CharacterStats;

import <string>;
import <map>;
import <regex>;
import <fstream>;
import <filesystem>;
using namespace std;

export struct characterStats {
    string playerHealth;
    string maxPlayerHealth;
    string playerSpeed;
    string playerDamage;
    string tearSpeed;
    string tearRate;
    string imageScale;
};

export namespace CharacterData {
    void LoadStatsOnce();
    const map<string, characterStats>& GetAllStats();
}