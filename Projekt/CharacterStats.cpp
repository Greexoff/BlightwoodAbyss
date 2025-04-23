module CharacterStats;

using namespace std;
namespace fs = filesystem;
CharactersData::CharactersData()
{
    DataPath = fs::current_path() / "DataBase";
    LoadCharacterStats();
    LoadEnemyStats();
}
CharactersData& CharactersData::getInstance()
{
    static CharactersData instance;
    return instance;
}

void CharactersData::LoadCharacterStats() {
    fs::path wholePath;
    wholePath = DataPath / "CharacterStats.txt";
    ifstream file(wholePath);
    if (!file.is_open())
    { 
       return;
    }

    regex r(R"((\w+):\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+))");
    string line;
    smatch match;

    while (getline(file, line)) {
        if (regex_match(line, match, r)&& match[1] != "Character") {
            characterStats stats;
            stats.playerHealth = stoi(match[2]);
            stats.maxPlayerHealth = stoi(match[3]);
            stats.playerSpeed = stof(match[4]);
            stats.playerDamage = stof(match[5]);
            stats.tearSpeed = stof(match[6]);
            stats.tearRate = stof(match[7]);
            stats.imageScale = stof(match[8]);
            CharacterStats[match[1]] = stats;
        }
    }
}
void CharactersData::LoadEnemyStats() {
    fs::path wholePath;
    wholePath = DataPath / "EnemyStats.txt";
    ifstream file(wholePath);
    if (!file.is_open())
    {
        return;
    }
    regex r(R"((\w+):\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+))");
    string line;
    smatch match;

    while (getline(file, line)) {
        if (regex_match(line, match, r) && match[1] != "Enemy") {
            enemyStats stats;
            stats.enemyHealth = stoi(match[2]);
            stats.maxEnemyHealth = stoi(match[3]);
            stats.enemySpeed = stof(match[4]);
            stats.enemyAttackSpeed = stof(match[5]);
            stats.enemyScore = stof(match[6]);
            stats.imageScale = stof(match[7]);
            EnemyStats[match[1]] = stats;
        }
    }
}
map<string, characterStats> CharactersData::getCharacterStats()
{
    return CharacterStats;
}
map<string, enemyStats> CharactersData::getEnemyStats()
{
    return EnemyStats;
}
