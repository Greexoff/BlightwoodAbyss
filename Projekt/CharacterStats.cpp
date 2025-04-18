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
            stats.playerHealth = stoi(match[2]);
            stats.maxPlayerHealth = stoi(match[3]);
            stats.playerSpeed = stof(match[4]);
            stats.playerDamage = stof(match[5]);
            stats.tearSpeed = stof(match[6]);
            stats.tearRate = stof(match[7]);
            stats.imageScale = stof(match[8]);

            allStats[match[1]] = stats;
        }
    }
    loaded = true;
}

const map<string, characterStats>& CharacterData::GetAllStats() {
    return allStats;
}