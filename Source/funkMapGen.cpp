#include <algorithm>
#include <iostream>
#include <iomanip>

#include "../types.h"

#include "drlg_l1.h"
#include "drlg_l2.h"
#include "drlg_l3.h"
#include "drlg_l4.h"
#include "engine.h"
#include "gendung.h"
#include "level.h"
#include "quests.h"
#include "trigs.h"
#include "path.h"

typedef struct Point {
	int x;
	int y;

	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point &other) const
	{
		return !(*this == other);
	}
} Point;

static int InitLevelType(int l)
{
	if (l == 0)
		return DTYPE_TOWN;
	if (l >= 1 && l <= 4)
		return DTYPE_CATHEDRAL;
	if (l >= 5 && l <= 8)
		return DTYPE_CATACOMBS;
	if (l >= 9 && l <= 12)
		return DTYPE_CAVES;
	return DTYPE_HELL;
}

void whatleveltype()
{
	switch (currlevel) {
	case 1:
		leveltype = DTYPE_CATHEDRAL;
		break;
	case 5:
		leveltype = DTYPE_CATACOMBS;
		break;
	case 9:
		leveltype = DTYPE_CAVES;
		break;
	case 13:
		leveltype = DTYPE_HELL;
		break;
	default:
		return;
	}

	LoadLvlGFX();
	FillSolidBlockTbls();
}

Point StairsUp;
Point StairsDown;

void InitStairCordinates()
{
	if (currlevel >= 1 && currlevel <= 4)
		InitL1Triggers();
	if (currlevel >= 5 && currlevel <= 8)
		InitL2Triggers();
	if (currlevel >= 9 && currlevel <= 12)
		InitL3Triggers();
	if (currlevel >= 13 && currlevel <= 16)
		InitL4Triggers();

	StairsUp = { -1, -1 };
	StairsDown = { -1, -1 };

	for (int i = 0; i < numtrigs; i++) {
		if (trigs[i]._tmsg == WM_DIABNEXTLVL) {
			StairsDown = { trigs[i]._tx, trigs[i]._ty };
		} else if (trigs[i]._tmsg == WM_DIABPREVLVL) {
			StairsUp = { trigs[i]._tx, trigs[i]._ty };
		}
	}
}

BOOL SolidLoc(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAXDUNX || y >= MAXDUNY) {
		return FALSE;
	}

	return nSolidTable[dPiece[x][y]];
}

BOOL PosOkPlayer(int pnum, int x, int y)
{
	if (SolidLoc(x, y) || dPiece[x][y] == 0) {
		return FALSE;
	}

	// if (dObject[x][y] != 0) {
	//  char bv;
	//	if (dObject[x][y] > 0) {
	//		bv = dObject[x][y] - 1;
	//	} else {
	//		bv = -(dObject[x][y] + 1);
	//	}
	//	if (object[bv]._oSolidFlag) {
	//		return FALSE;
	//	}
	// }

	return TRUE;
}

int PathLength()
{
	char path[MAX_PATH_LENGTH];

	return FindPath(PosOkPlayer, 0, StairsUp.x, StairsUp.y, StairsDown.x, StairsDown.y, path);
}

int CalcStairsChebyshevDistance()
{
	if (StairsUp == Point { -1, -1 } || StairsDown == Point { -1, -1 }) {
		return -1;
	}

	int horizontal = std::max(StairsUp.x, StairsDown.x) - std::min(StairsUp.x, StairsDown.x);
	int vertical = std::max(StairsUp.y, StairsDown.y) - std::min(StairsUp.y, StairsDown.y);

	return std::max(horizontal, vertical);
}

void printAsciiCoords()
{
	std::cout << "Ent: (" << StairsUp.x << ", " << StairsUp.y << ") ";
	std::cout << "Ext: (" << StairsDown.x << ", " << StairsDown.y << ") " << std::endl;
}

bool IsGoodLevel()
{
	const int maxDistance = 10;

	int cDistance = CalcStairsChebyshevDistance();
	if (cDistance != -1 && cDistance > maxDistance)
		return false;

	int stairsPath = PathLength();
	if (stairsPath != 0 && stairsPath < maxDistance)
		return false;

	return true;
}

void createSpecificDungeon()
{
	uint32_t lseed = glSeedTbl[currlevel];
	if (leveltype == DTYPE_CATHEDRAL)
		CreateL5Dungeon(lseed, 0);
	else if (leveltype == DTYPE_CATACOMBS)
		CreateL2Dungeon(lseed, 0);
	else if (leveltype == DTYPE_CAVES)
		CreateL3Dungeon(lseed, 0);
	else if (leveltype == DTYPE_HELL)
		CreateL4Dungeon(lseed, 0);
}

/**
 * @brief GET MAIN SEED, GET ALL MAP SEEDS
 * @return nothing, but updates RNG seeds list glSeedTbl[i]
 */
void seedSelection(int seed)
{
	SetRndSeed(0);
	sgGameInitInfo.dwSeed = seed;
	sgGameInitInfo.bDiff = gnDifficulty;

	gnDifficulty = sgGameInitInfo.bDiff;
	SetRndSeed(sgGameInitInfo.dwSeed);

	for (int i = 0; i < NUMLEVELS; i++) {
		glSeedTbl[i] = GetRndSeed();
		gnLevelTypeTbl[i] = InitLevelType(i);
	}
}

void printAsciiLevel()
{
	for (int boby = 16; boby < MAXDUNY - 17; boby++) {
		for (int bobx = 16; bobx < MAXDUNX - 17; bobx++) {
			if (nSolidTable[dPiece[bobx][boby]])
				std::cout << "#";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
}

void printHelp()
{
	std::cout << "--help         Print this message and exit" << std::endl;
	std::cout << "--quiet        Do not print to console" << std::endl;
	std::cout << "--export       Export levels as .dun files" << std::endl;
	std::cout << "--start <#>    The seed to start from" << std::endl;
	std::cout << "--count <#>    The number of seeds to process" << std::endl;
	std::cout << "--quality <#>  Number of levels that must be good (default 4)" << std::endl;
	std::cout << "--verbose      Print out details about rejected seeds" << std::endl;
}

int main(int argc, char **argv)
{
	int startSeed = 0;
	int seedCount = 1;
	bool quiet = false;
	bool exportLevels = false;
	int quality = 4;
	bool verbose = false;

	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--help") {
			printHelp();
			return 0;
		} else if (arg == "--quiet") {
			quiet = true;
		} else if (arg == "--export") {
			exportLevels = true;
		} else if (arg == "--start" && argc >= i + 1) {
			startSeed = std::stoi(argv[i + 1]);
		} else if (arg == "--count" && argc >= i + 1) {
			seedCount = std::stoi(argv[i + 1]);
		} else if (arg == "--quality" && argc >= i + 1) {
			quality = std::stoi(argv[i + 1]);
		} else if (arg == "--verbose") {
			verbose = true;
		}
	}

	for (int seed = startSeed; seed < startSeed + seedCount; seed++) {
		if (!quiet)
			std::cout << "processing seed " << seed << std::endl;

		seedSelection(seed);
		InitQuests();
		if (quests[Q_LTBANNER]._qactive != QUEST_NOTAVAIL) {
			if (verbose)
				std::cout << "Game Seed: " << sgGameInitInfo.dwSeed << " thrown out: Sign Quest" << std::endl;
			continue;
		}
		if (quests[Q_WARLORD]._qactive != QUEST_NOTAVAIL) {
			if (verbose)
				std::cout << "Game Seed: " << sgGameInitInfo.dwSeed << " thrown out: Warlord" << std::endl;
			continue;
		}
		for (int level = 1; level < NUMLEVELS; level++) {
			currlevel = level;
			whatleveltype();
			createSpecificDungeon();
			InitStairCordinates();

			if (!IsGoodLevel()) {
				if (level > quality || verbose) {
					std::cout << "Game Seed: " << sgGameInitInfo.dwSeed << " quality: ";
					for (int p = 0; p < level - 1; p++) {
						std::cout << "+";
					}
					std::cout << " (" << (level - 1) << ")" << std::endl;
					break;
				}
				break;
			}
			if (!quiet)
				printAsciiLevel();
			if (!quiet || verbose) {
				printAsciiCoords();
				std::cout << std::endl;
			}
			if (exportLevels)
				ExportDun();
		}
	}

	return 0;
}
