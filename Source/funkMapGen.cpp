#include "funkMapGen.h"

#include <iostream>

#include "analyzer/path.h"
#include "analyzer/puzzler.h"
#include "analyzer/pattern.h"
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

Point Spawn;
Point StairsDown;

char Path[MAX_PATH_LENGTH];

Configuration Config;

namespace {

constexpr uint64_t ProgressInterval = 10 * 1000 * 1000;

BYTE previousLevelType = DTYPE_NONE;

void InitEngine()
{
	gnDifficulty = DIFF_NORMAL;
	leveltype = DTYPE_NONE;

	DRLG_PreLoadL2SP();
	DRLG_PreLoadDiabQuads();
}

void ShutDownEngine()
{
	DRLG_UnloadL2SP();
	DRLG_FreeDiabQuads();
}

void InitiateLevel(int level)
{
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
		leveltype = DTYPE_CATACOMBS;

	InitVision();

	if (leveltype == previousLevelType)
		return;
	previousLevelType = leveltype;

	LoadLvlGFX();
	FillSolidBlockTbls();
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

int CreateDungeon(bool breakOnSuccess, bool breakOnFailure)
{
	int levelSeed = -1;
	uint32_t lseed = glSeedTbl[currlevel];
	if (leveltype == DTYPE_CATHEDRAL)
		levelSeed = CreateL5Dungeon(lseed, 0, breakOnSuccess, breakOnFailure);
	else if (leveltype == DTYPE_CATACOMBS)
		levelSeed = CreateL2Dungeon(lseed, 0, breakOnSuccess, breakOnFailure);
	else if (leveltype == DTYPE_CAVES)
		levelSeed = CreateL3Dungeon(lseed, 0, breakOnSuccess, breakOnFailure);
	else if (leveltype == DTYPE_HELL)
		levelSeed = CreateL4Dungeon(lseed, 0, breakOnSuccess, breakOnFailure);

	return levelSeed;
}

void InitDungeonMonsters()
{
	InitLevelMonsters();
	SetRndSeed(glSeedTbl[currlevel]);
	GetLevelMTypes();
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
}

void CreateItemsFromObject(int oid)
{
	switch (object[oid]._otype) {
	case OBJ_CHEST1:
	case OBJ_CHEST2:
	case OBJ_CHEST3:
	case OBJ_TCHEST1:
	case OBJ_TCHEST2:
	case OBJ_TCHEST3:
		SetRndSeed(object[oid]._oRndSeed);
		if (setlevel) {
			for (int j = 0; j < object[oid]._oVar1; j++) {
				CreateRndItem(object[oid]._ox, object[oid]._oy, TRUE, TRUE, FALSE);
			}
		} else {
			for (int j = 0; j < object[oid]._oVar1; j++) {
				if (object[oid]._oVar2 != 0)
					CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
				else
					CreateRndUseful(0, object[oid]._ox, object[oid]._oy, TRUE);
			}
		}
		break;
	case OBJ_SARC:
		SetRndSeed(object[oid]._oRndSeed);
		if (object[oid]._oVar1 <= 2)
			CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
		break;
	case OBJ_DECAP:
		SetRndSeed(object[oid]._oRndSeed);
		CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
		break;
	case OBJ_BARREL:
		SetRndSeed(object[oid]._oRndSeed);
		if (object[oid]._oVar2 <= 1) {
			if (object[oid]._oVar3 == 0)
				CreateRndUseful(0, object[oid]._ox, object[oid]._oy, TRUE);
			else
				CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
		}
		break;
	case OBJ_SKELBOOK:
	case OBJ_BOOKSTAND:
		SetRndSeed(object[oid]._oRndSeed);
		if (random_(161, 5) != 0)
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_MISC, IMISC_SCROLL, TRUE, FALSE);
		else
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_MISC, IMISC_BOOK, TRUE, FALSE);
		break;
	case OBJ_BOOKCASEL:
	case OBJ_BOOKCASER:
		SetRndSeed(object[oid]._oRndSeed);
		CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_MISC, IMISC_BOOK, TRUE, FALSE);
		break;
	case OBJ_ARMORSTAND:
	case OBJ_WARARMOR: {
		SetRndSeed(object[oid]._oRndSeed);
		BOOL uniqueRnd = random_(0, 2);
		if (currlevel <= 5) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, TRUE, ITYPE_LARMOR, IMISC_NONE, TRUE, FALSE);
		} else if (currlevel >= 6 && currlevel <= 9) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, uniqueRnd, ITYPE_MARMOR, IMISC_NONE, TRUE, FALSE);
		} else if (currlevel >= 10 && currlevel <= 12) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_HARMOR, IMISC_NONE, TRUE, FALSE);
		} else if (currlevel >= 13 && currlevel <= 16) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, TRUE, ITYPE_HARMOR, IMISC_NONE, TRUE, FALSE);
		}
	} break;
	case OBJ_WARWEAP:
	case OBJ_WEAPONRACK: {
		SetRndSeed(object[oid]._oRndSeed);
		int weaponType;

		switch (random_(0, 4) + ITYPE_SWORD) {
		case ITYPE_SWORD:
			weaponType = ITYPE_SWORD;
			break;
		case ITYPE_AXE:
			weaponType = ITYPE_AXE;
			break;
		case ITYPE_BOW:
			weaponType = ITYPE_BOW;
			break;
		case ITYPE_MACE:
			weaponType = ITYPE_MACE;
			break;
		}

		if (leveltype > 1)
			CreateTypeItem(object[oid]._ox, object[oid]._oy, TRUE, weaponType, IMISC_NONE, TRUE, FALSE);
		else
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, weaponType, IMISC_NONE, TRUE, FALSE);
	} break;
	}
}

void DropAllItems()
{
	MonsterItems = numitems;
	for (int i = 0; i < nummonsters; i++) {
		int mid = monstactive[i];
		if (monster[mid].MType->mtype == MT_GOLEM)
			continue;
		SetRndSeed(monster[mid]._mRndSeed);
		SpawnItem(mid, monster[mid]._mx, monster[mid]._my, TRUE);
	}

	ObjectItems = numitems;
	for (int i = 0; i < nobjects; i++) {
		int oid = objectactive[i];
		CreateItemsFromObject(oid);
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
	std::cout << "                   path: Search for the shortst stairs walk path" << std::endl;
	std::cout << "                   pattern: Search a set tile pattern" << std::endl;
	std::cout << "                   gameseed: Search for GameSeeds with LevelSeed" << std::endl;
	std::cout << "--start <#>    The seed to start from" << std::endl;
	std::cout << "--count <#>    The number of seeds to process" << std::endl;
	std::cout << "--quality <#>  Number of levels that must be good [default: 6]" << std::endl;
	std::cout << "--quiet        Do print status messages" << std::endl;
	std::cout << "--verbose      Print out details about seeds" << std::endl;
}

void ParseArguments(int argc, char **argv)
{
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
			} else if (scanner == "pattern") {
				Config.scanner = Scanners::Pattern;
			} else if (scanner == "gameseed") {
				Config.scanner = Scanners::GameSeed;
			} else {
				std::cerr << "Unknown scanner: " << scanner << std::endl;
				exit(255);
			}
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
			Config.seedCount = std::stoll(argv[i]);
		} else if (arg == "--quality") {
			i++;
			if (argc <= i) {
				std::cerr << "Missing value for --quality" << std::endl;
				exit(255);
			}
			Config.quality = std::stoll(argv[i]);
		} else if (arg == "--verbose") {
			Config.verbose = true;
		} else {
			std::cerr << "Unknown argument: " << arg << std::endl;
			exit(255);
		}
	}
}

}

int main(int argc, char **argv)
{
	ParseArguments(argc, argv);
	InitEngine();

	int yseconds = micros();
	uint32_t prevseed = Config.startSeed;
	for (uint32_t seed = Config.startSeed; seed < Config.startSeed + Config.seedCount; seed++) {
		int elapsed = micros() - yseconds;
		if (!Config.quiet && elapsed >= ProgressInterval) {
			int pct = 100 * (seed - Config.startSeed) / Config.seedCount;
			int speed = ((seed - prevseed) / 10);
			int eta = (Config.seedCount - (seed - Config.startSeed)) / speed;
			std::cerr << "Progress: " << pct << "% eta: " << eta << "s (" << speed << "seed/s)" << std::endl;
			yseconds += elapsed;
			prevseed = seed;
		}

		SetGameSeed(seed);
		InitQuests();

		int startLevel = 1;
		int maxLevels = NUMLEVELS;
		bool breakOnSuccess = false;
		bool breakOnFailure = false;

		if (Config.scanner != Scanners::Path && Config.scanner != Scanners::None) {
			startLevel = 9;
			maxLevels = startLevel + 1;
		}

		if (Config.scanner == Scanners::Path) {
			if (ShortPathSeedSkip())
				continue;
		} else if (Config.scanner == Scanners::Pattern) {
			glSeedTbl[9] = seed;
			breakOnFailure = true;
		} else if (Config.scanner == Scanners::GameSeed) {
			breakOnSuccess = true;
		}

		if (Config.verbose)
			std::cerr << "Game Seeds: " << seed << std::endl;

		for (int level = startLevel; level < maxLevels; level++) {
			InitiateLevel(level);

			if (Config.scanner == Scanners::GameSeed) {
				InitDungeonMonsters();
				bool hasLavaLoards = false;
				for (int i = 0; i < nummtypes && !hasLavaLoards; i++)
					hasLavaLoards = Monsters[i].mtype == MT_WMAGMA;
				if (!hasLavaLoards)
					break;
			}

			int levelSeed = CreateDungeon(breakOnSuccess, breakOnFailure);
			if (Config.scanner != Scanners::Pattern && Config.scanner != Scanners::GameSeed) {
				InitTriggers();
				CreateDungeonContent();

				// Analyze
				FindStairCordinates();
			}

			if (Config.verbose && oobwrite)
				std::cerr << "Game Seed: " << sgGameInitInfo.dwSeed << " OOB write detected" << std::endl;

			if (Config.scanner == Scanners::Path) {
				if (!ShortPathSearch())
					break;
			} else if (Config.scanner == Scanners::Puzzler) {
				DropAllItems();
				if (!SearchForPuzzler())
					break;
			} else if (Config.scanner == Scanners::Pattern) {
				if (levelSeed == -1 || !MatchPattern())
					break;
				std::cout << "Level Seed: " << (uint32_t)levelSeed << std::endl;
			} else if (Config.scanner == Scanners::GameSeed) {
				if (levelSeed != Config.quality)
					break;
				std::cout << "Game Seed: " << seed << std::endl;
			}

			if (Config.asciiLevels)
				printAsciiLevel(Spawn, StairsDown, Path);
			if (Config.exportLevels)
				ExportDun(seed);
		}
	}

	ShutDownEngine();

	return 0;
}
