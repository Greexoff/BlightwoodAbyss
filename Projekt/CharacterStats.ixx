export module CharacterStats;

import <string>;
import <map>;
import <regex>;
import <fstream>;
import <filesystem>;
using namespace std;
namespace fs = filesystem;

export struct characterStats {
    int playerHealth;
    int maxPlayerHealth;
    float playerSpeed;
    float playerDamage;
    float tearSpeed;
    float tearRate;
    float imageScale;
};
export struct enemyStats
{
    float enemyHealth;
    float maxEnemyHealth;
    float enemySpeed;
    float enemyAttackSpeed;
    float enemyScore;
    float imageScale;
};
 
export class CharactersData
{
public:
    static CharactersData& getInstance();
    map<string, characterStats> getCharacterStats();
    map<string, enemyStats> getEnemyStats();
private:
    CharactersData();
    CharactersData(const CharactersData&) = delete;
    void operator=(const CharactersData&) = delete;
    fs::path DataPath;
    map<string, characterStats> CharacterStats;
    map<string, enemyStats> EnemyStats;
    void LoadCharacterStats();
    void LoadEnemyStats();
};