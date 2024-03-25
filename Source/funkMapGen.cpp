#include "funkMapGen.h"

#include <fstream>
#include <optional>
#include <iostream>
#include <limits>
#include <vector>

#include "analyzer/gameseed.h"
#include "analyzer/path.h"
#include "analyzer/pattern.h"
#include "analyzer/puzzler.h"
#include "analyzer/quest.h"
#include "analyzer/warp.h"
#include "drlg_l1.h"
#include "drlg_l2.h"
#include "drlg_l3.h"
#include "drlg_l4.h"
#include "items.h"
#include "level.h"
#include "lighting.h"
#include "monster.h"
#include "objects.h"
#include "quests.h"
#include "themes.h"
#include "trigs.h"

int MonsterItems;
int ObjectItems;

Point Spawn = { -1, -1 };
Point StairsDown = { -1, -1 };
Point POI = { -1, -1 };

char Path[MAX_PATH_LENGTH];

Configuration Config;

namespace {

constexpr uint64_t ProgressInterval = 10 * 1000 * 1000;

BYTE previousLevelType = DTYPE_NONE;
Scanner *scanner;

void InitEngine()
{
	gnDifficulty = DIFF_NORMAL;
	leveltype = DTYPE_NONE;

	DRLG_PreLoadL2SP();
	DRLG_PreLoadDiabQuads();

	if (Config.scanner == Scanners::None) {
		scanner = new Scanner();
	} else if (Config.scanner == Scanners::Path) {
		scanner = new ScannerPath();
	} else if (Config.scanner == Scanners::Quest) {
		scanner = new ScannerQuest();
	} else if (Config.scanner == Scanners::Puzzler) {
		scanner = new ScannerPuzzler();
	} else if (Config.scanner == Scanners::Warp) {
		scanner = new ScannerWarp();
	} else if (Config.scanner == Scanners::Pattern) {
		scanner = new ScannerPattern();
	} else if (Config.scanner == Scanners::GameSeed) {
		scanner = new ScannerGameSeed();
	}
}

void ShutDownEngine()
{
	DRLG_UnloadL2SP();
	DRLG_FreeDiabQuads();
	delete scanner;
}

void InitiateLevel(int level)
{
	POI = { -1, -1 };
	currlevel = level;

	oobread = false;
	oobwrite = false;

	if (level > 12)
		leveltype = DTYPE_HELL;
	else if (level > 8)
		leveltype = DTYPE_CAVES;
	else if (level > 4)
		leveltype = DTYPE_CATACOMBS;
	else if (level > 0)
		leveltype = DTYPE_CATHEDRAL;

	InitVision();

	if (leveltype == previousLevelType)
		return;
	previousLevelType = leveltype;

	LoadLvlGFX();
	FillSolidBlockTbls();

	// Mark doors as non solid
	if (leveltype == DTYPE_CATHEDRAL) {
		nSolidTable[44] = FALSE;
		nSolidTable[46] = FALSE;
		nSolidTable[51] = FALSE;
		nSolidTable[56] = FALSE;
		nSolidTable[214] = FALSE;
		nSolidTable[270] = FALSE;
	} else if (leveltype == DTYPE_CATACOMBS) {
		nSolidTable[55] = FALSE;
		nSolidTable[58] = FALSE;
		nSolidTable[538] = FALSE;
		nSolidTable[540] = FALSE;
	} else if (leveltype == DTYPE_CAVES) {
		nSolidTable[531] = FALSE;
		nSolidTable[534] = FALSE;
	}
}

void InitTriggers()
{
	if (leveltype == DTYPE_CATHEDRAL)
		InitL1Triggers();
	else if (leveltype == DTYPE_CATACOMBS)
		InitL2Triggers();
	else if (leveltype == DTYPE_CAVES)
		InitL3Triggers();
	else if (leveltype == DTYPE_HELL)
		InitL4Triggers();
	Freeupstairs();
}

void FindStairCordinates()
{
	Spawn = { -1, -1 };
	StairsDown = { -1, -1 };

	for (int i = 0; i < numtrigs; i++) {
		if (trigs[i]._tmsg == WM_DIABNEXTLVL) {
			StairsDown = { trigs[i]._tx, trigs[i]._ty };
		} else if (trigs[i]._tmsg == WM_DIABPREVLVL) {
			if (leveltype == DTYPE_CATHEDRAL)
				Spawn = { trigs[i]._tx + 1, trigs[i]._ty + 2 };
			else if (leveltype == DTYPE_CATACOMBS)
				Spawn = { trigs[i]._tx + 1, trigs[i]._ty + 1 };
			else if (leveltype == DTYPE_CAVES)
				Spawn = { trigs[i]._tx, trigs[i]._ty + 1 };
			else if (leveltype == DTYPE_HELL)
				Spawn = { trigs[i]._tx + 1, trigs[i]._ty };
		}
	}
}

void CreateDungeonContent()
{
	InitDungeonMonsters();

	InitThemes();
	SetRndSeed(glSeedTbl[currlevel]);
	HoldThemeRooms();
	GetRndSeed();

	InitMonsters();
	GetRndSeed();

	InitObjects();

	InitItems();
	CreateThemeRooms();
}

std::optional<uint32_t> CreateDungeon(DungeonMode mode)
{
	uint32_t lseed = glSeedTbl[currlevel];
	std::optional<uint32_t> levelSeed = std::nullopt;
	if (leveltype == DTYPE_CATHEDRAL)
		levelSeed = CreateL5Dungeon(lseed, 0, mode);
	if (leveltype == DTYPE_CATACOMBS)
		levelSeed = CreateL2Dungeon(lseed, 0, mode);
	if (leveltype == DTYPE_CAVES)
		levelSeed = CreateL3Dungeon(lseed, 0, mode);
	if (leveltype == DTYPE_HELL)
		levelSeed = CreateL4Dungeon(lseed, 0, mode);

	if (mode == DungeonMode::Full) {
		InitTriggers();
		CreateDungeonContent();

		if (currlevel == 15) {
			// Locate Lazarus warp point
			Point point = { quests[Q_BETRAYER]._qtx, quests[Q_BETRAYER]._qty };
			if (!nSolidTable[dPiece[point.x][point.y]])
				POI = point;
		}

		FindStairCordinates();
	}

	if (Config.verbose && oobwrite)
		std::cerr << "Game Seed: " << sgGameInitInfo.dwSeed << " OOB write detected" << std::endl;

	return levelSeed;
}

/**
 * @brief GET MAIN SEED, GET ALL MAP SEEDS
 * @return nothing, but updates RNG seeds list glSeedTbl[i]
 */
void SetGameSeed(uint32_t seed)
{
	sgGameInitInfo.dwSeed = seed;
	SetRndSeed(sgGameInitInfo.dwSeed);

	for (int i = 0; i < NUMLEVELS; i++) {
		glSeedTbl[i] = GetRndSeed();
	}

	InitQuests();
	memset(UniqueItemFlag, 0, sizeof(UniqueItemFlag));
}

std::vector<uint32_t> SeedsFromFile = {};

void readFromFile()
{
	if (Config.seedFile.empty())
		return;

	std::ifstream file(Config.seedFile);
	if (!file.is_open()) {
		std::cerr << "Unable to read seeds file: " << Config.seedFile << std::endl;
		exit(255);
	}

	if (!Config.quiet)
		std::cerr << "Loading seeds from: " << Config.seedFile << std::endl;

	std::string line;
	while (std::getline(file, line))
		SeedsFromFile.push_back(std::stoll(line.substr(0, line.find(' '))));

	Config.seedCount = std::min(Config.seedCount, uint32_t(SeedsFromFile.size() - Config.startSeed));

	if (file.is_open())
		file.close();
}

int ProgressseedMicros;
uint32_t ProgressseedIndex = 0;

void printProgress(uint32_t seedIndex, uint32_t seed)
{
	if (Config.verbose)
		std::cerr << "Processing: " << seed << std::endl;

	int elapsed = micros() - ProgressseedMicros;
	if (!Config.quiet && elapsed >= ProgressInterval) {
		int pct = 100 * seedIndex / Config.seedCount;
		int speed = ((seedIndex - ProgressseedIndex) / 10);
		int eta = (Config.seedCount - seedIndex) / speed;
		std::cerr << "Progress: " << pct << "% eta: " << eta << "s (" << speed << "seed/s)" << std::endl;
		ProgressseedMicros += elapsed;
		ProgressseedIndex = seedIndex;
	}
}

void printHelp()
{
	std::cout << "--help         Print this message and exit" << std::endl;
	std::cout << "--ascii        Print ASCII version of levels" << std::endl;
	std::cout << "--export       Export levels as .dun files" << std::endl;
	std::cout << "--scanner <#>  How to analyze levels [default: none]" << std::endl;
	std::cout << "                   none: No analyzing" << std::endl;
	std::cout << "                   puzzler: Search for Naj's Puzzler on level 9" << std::endl;
	std::cout << "                   path: Estimate the time to complete the game" << std::endl;
	std::cout << "                   warp: Find seeds with a warp on level 15" << std::endl;
	std::cout << "                   pattern: Search a set tile pattern" << std::endl;
	std::cout << "                   gameseed: Search for GameSeeds with LevelSeed" << std::endl;
	std::cout << "--start <#>    The seed to start from" << std::endl;
	std::cout << "--count <#>    The number of seeds to process" << std::endl;
	std::cout << "--seeds <#>    A file to read seeds from" << std::endl;
	std::cout << "--etc <#>      Max number of sec to complete the game [default: 420]" << std::endl;
	std::cout << "--quiet        Do print status messages" << std::endl;
	std::cout << "--verbose      Print out details about seeds" << std::endl;
}

void ParseArguments(int argc, char **argv)
{
	bool fromFile = false;
	bool hasCount = false;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--help") {
			printHelp();
			exit(0);
		} else if (arg == "--quiet") {
			Config.quiet = true;
		} else if (arg == "--ascii") {
			Config.asciiLevels = true;
		} else if (arg == "--export") {
			Config.exportLevels = true;
		} else if (arg == "--scanner") {
			i++;
			if (argc <= i) {
				std::cerr << "Missing value for --scanner" << std::endl;
				exit(255);
			}
			std::string scanner = argv[i];
			if (scanner == "none") {
				Config.scanner = Scanners::None;
			} else if (scanner == "puzzler") {
				Config.scanner = Scanners::Puzzler;
			} else if (scanner == "path") {
				Config.scanner = Scanners::Path;
			} else if (scanner == "quest") {
				Config.scanner = Scanners::Quest;
			} else if (scanner == "warp") {
				Config.scanner = Scanners::Warp;
			} else if (scanner == "pattern") {
				Config.scanner = Scanners::Pattern;
			} else if (scanner == "gameseed") {
				Config.scanner = Scanners::GameSeed;
			} else {
				std::cerr << "Unknown scanner: " << scanner << std::endl;
				exit(255);
			}
		} else if (arg == "--seeds") {
			i++;
			if (argc <= i) {
				std::cerr << "Missing filename for --seeds" << std::endl;
				exit(255);
			}
			fromFile = true;
			Config.seedFile = argv[i];
		} else if (arg == "--start") {
			i++;
			if (argc <= i) {
				std::cerr << "Missing value for --start" << std::endl;
				exit(255);
			}
			Config.startSeed = std::stoll(argv[i]);
		} else if (arg == "--count") {
			i++;
			if (argc <= i) {
				std::cerr << "Missing value for --count" << std::endl;
				exit(255);
			}
			hasCount = true;
			Config.seedCount = std::stoll(argv[i]);
		} else if (arg == "--etc") {
			i++;
			if (argc <= i) {
				std::cerr << "Missing value for --etc" << std::endl;
				exit(255);
			}
			Config.etc = std::stoll(argv[i]);
		} else if (arg == "--verbose") {
			Config.verbose = true;
		} else {
			std::cerr << "Unknown argument: " << arg << std::endl;
			exit(255);
		}
	}

	if (fromFile && !hasCount) {
		Config.seedCount = std::numeric_limits<uint32_t>::max();
	}
}

}

void InitDungeonMonsters()
{
	InitLevelMonsters();
	SetRndSeed(glSeedTbl[currlevel]);
	GetLevelMTypes();
}

int main(int argc, char **argv)
{
	ParseArguments(argc, argv);
	InitEngine();
	readFromFile();

	ProgressseedMicros = micros();
	for (uint32_t seedIndex = 0; seedIndex < Config.seedCount; seedIndex++) {
		uint32_t seed = seedIndex + Config.startSeed;
		if (!SeedsFromFile.empty()) {
			seed = SeedsFromFile[seed];
		}
		printProgress(seedIndex, seed);

		SetGameSeed(seed);
		if (scanner->skipSeed())
			continue;

		for (int level = 1; level < NUMLEVELS; level++) {
			if (scanner->skipLevel(level))
				continue;

			InitiateLevel(level);
			std::optional<uint32_t> levelSeed = CreateDungeon(scanner->getDungeonMode());
			if (!scanner->levelMatches(levelSeed))
				continue;

			if (Config.asciiLevels)
				printAsciiLevel();
			if (Config.exportLevels)
				ExportDun(seed);
		}
	}

	ShutDownEngine();

	return 0;
}
