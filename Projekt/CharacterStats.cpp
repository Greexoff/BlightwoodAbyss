module CharacterStats;

using namespace std;
namespace fs = std::filesystem;

static map<string, characterStats> allStats;
static bool loaded = false;

void CharacterData::LoadStatsOnce() {
    if (loaded)
    {
        return;
    }

    fs::path path = fs::current_path() / "database" / "CharacterStats.txt";
    ifstream file(path);
    if (!file.is_open()) return;

    regex r(R"((\w+):\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+)\s+([\d\.]+))");
    string line;
    smatch match;

    while (getline(file, line)) {
        if (regex_match(line, match, r)) {
            characterStats stats;
            stats.playerHealth = match[2];
            stats.maxPlayerHealth = match[3];
            stats.playerSpeed = match[4];
            stats.playerDamage = match[5];
            stats.tearSpeed = match[6];
            stats.tearRate = match[7];
            stats.imageScale = match[8];

            allStats[match[1]] = stats;
        }
    }
    loaded = true;
}

const map<string, characterStats>& CharacterData::GetAllStats() {
    return allStats;
}