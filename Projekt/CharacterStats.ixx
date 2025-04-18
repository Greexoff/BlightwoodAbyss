export module CharacterStats;

import <string>;
import <map>;
import <regex>;
import <fstream>;
import <filesystem>;
using namespace std;

export struct characterStats {
    int playerHealth;
    int maxPlayerHealth;
    float playerSpeed;
    float playerDamage;
    float tearSpeed;
    float tearRate;
    float imageScale;
};

export namespace CharacterData {
    void LoadStatsOnce();
    const map<string, characterStats>& GetAllStats();
}