#include "pattern.h"

#include <cstdint>
#include <iostream>

#include "../../types.h"

#include "../engine.h"
#include "../gendung.h"
#include "../quests.h"

#define TEMPLATEX 8
#define TEMPLATEY 9

const uint8_t GROOBO1[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0,  0,  0,  0,  4,  0,  0 },
    {   0,  0, 66,204,  0,  0,  1,129,  0 },
    {   4, 63, 64, 65,  2,  0, 16,130,128 },
    {   1,129, 67, 68, 62, 57, 58,  0,135 },
    {   1,130,128,  0, 61, 59, 60,129,104 },
    {   0,  0,  0,  0,  0,  0,  0,130,128 },
    {   0,  0,  0,  0,  0,  0,140,140,140 },
    {   0,  0,  0,  0,  0,  0, 10,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO2[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0, 66,204,  0,  0,  0,  0,  0 },
    {   4, 63, 64, 65,108,  0,  0,  0,  0 },
    { 111,106, 67, 68, 62, 57, 58,  0,  0 },
    { 107,  0,  0,  0, 61, 59, 60,  0,  0 },
    {   1,129,  0,  0,  0,  0,  0,134,  0 },
    {   0,130,128,140,  0,  0,  0,  0,  0 },
    {   0,  0,  4,  2,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO3[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0, 66,204,  0,  0,  0,  0,  0 },
    {   4, 63, 64, 65,  2,  0,  0,  0, 16 },
    { 111,  0, 67, 68, 62, 57, 58,  0,  0 },
    {   1,129,106,  0, 61, 59, 60,  0,  0 },
    {   0,130,128,145,  0,  0,145,  0,  0 },
    {   0,104,129,142, 15,  0,142, 15,  0 },
    {   0,  0,130,128,  0,104,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO4[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0, 66,  0,  0,  0,  0,122,  0 },
    {   0, 63, 64, 65,  0,  0,  0,  0,  0 },
    {   0,  0, 67, 68, 62, 57, 58,  0,  0 },
    {   0,  0,  0,  0, 61, 59, 60,  0,  0 },
    { 107,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO5[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   1,  3,  3,  3,  3,  3,  0,  0,  0 },
    {  81,  3, 72, 77,  3, 48, 71,  0,  0 },
    {   1,  3, 76,  3,  3, 50, 78,  3,  0 },
    {   1,  0,114,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0, 13, 11, 11, 11, 11 },
    {   0,  0,  0,  0, 10, 12, 12, 12, 12 },
    {   0,  0,  0,  0,  0, 12, 12, 12, 12 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO6[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0, 90,  0,  0,  0,  0,  0, 91,  0 },
    { 133,135, 72, 77,  0, 48, 71, 89,  0 },
    { 132,134, 76,  0,  0, 50, 78,  0, 90 },
    {  82,  0,  0,  0, 89,  0,120,122,  0 },
    {   0,  0, 90,  0,  0, 88,121,123,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO7[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   1,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  82,  0, 72, 77,  0, 48, 71,  0,  0 },
    {   1,  0, 76,  0,  0, 50, 78,  0,  0 },
    {  80,  0,  0,  0,106,  0,107,  0,  0 },
    {   0,  0,  0,128,130,  0,  0,  0,  0 },
    {   0,  0,  0,129,131,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO8[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   1, 89,  0,  0,  0,  0,  0,  0,  0 },
    {  82,  0, 72, 77,  0, 48, 71,  0,  8 },
    {   1,  0, 76,  0,  0, 50, 78,  0, 80 },
    {  81,  0,  0,  0,  0,  0,  0, 48,  0 },
    {   0,  0, 87,  2,  0,  7,  0, 50,  9 },
    {   0,  0,  1,  0,  0,  4,  0,  0,  0 },
    {   0,  0,  0,  0,  0, 81,  0,  0,  0 },
    {   0,  0,  0,  0,  0, 83,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO9[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0,  0,  8,  0,  0,  0,  0 },
    {   0,  8,  8, 51, 50,  8,  8, 89,  0 },
    {   8,  8, 11, 48, 49,  3, 11, 88,  0 },
    {   8,  8, 47,106,  7, 13, 12,106,  0 },
    {   8,  8, 46,107,  7,106,108,106,  0 },
    {   0,  8,  1, 14,106,106,  5, 14,  0 },
    {   0,  0,  8,  1, 14,106,  4,  0,  0 },
    {   0,  0,  0,  0, 12,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO10[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0, 51, 50,  0, 92,  0,  0 },
    {   0,  0, 11, 48, 49,  3, 93,  3, 11 },
    {   0,  0, 23, 15, 20, 13, 12, 13, 12 },
    {   0,  0,  1, 14, 17,107,107,106,  0 },
    {   0,  0, 11, 12, 21, 16, 44,  0,  0 },
    {   0,  0, 47, 19, 20,106,  7, 29,  0 },
    {   0,  0, 46, 17, 17,108, 29, 34,  0 },
	// clang-format on
};

const uint8_t GROOBO11[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  3,  8,  8,  8,  8,  0,  0,  0 },
    {   7, 13,  3, 51, 50,  8,  8,  0,  0 },
    {   7,  7, 13, 48, 49,  3,  8,  0,  0 },
    {   7,  7,  7,107,  7,  4,  8,  0,  0 },
    { 102, 14,  7,  7,  5,  6,  8,  0,  0 },
    {  97, 47,  7,  7,  4, 11,  0,  0,  0 },
    {   8, 46,106,106, 13, 12,  0,  0,  0 },
    {   8,130,134,136,146,136,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO12[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0,  8,  8,  8,  8, 47,107 },
    {   0, 11,  3, 51, 50,  8,  8, 46,  7 },
    {  87, 12, 13, 48, 49,  3,  8,130,136 },
    {   1, 14,106,106,  7,  4,  8,  1, 14 },
    {   0, 12,106,  7,  5,  6,  8,  8,  1 },
    {   0,  0,106,107, 13,  3,  8,  0,  0 },
    {   0,  0,  0,  0,  0,  4,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO13[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0, 50, 48, 10,  0,  0,  0 },
    {   0, 36, 38, 35, 47, 81, 49,  0,  6 },
    {   0, 37, 34, 33, 32,  6,  6,  6,  0 },
    {  12,  6, 97, 31,  0,  7, 51,  6, 44 },
    {   0, 50,  6,  6,  0,  0,  6,  6, 46 },
    {   0,  0,  0,  0,  0,  0,  0, 50,  6 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO14[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0, 51,  6,  6,  0,  0,  0,  0,  0 },
    {   6,  0, 45, 41, 97,  0,  0,  0,  0 },
    {   6, 44, 43, 40,  6,  0,  0,  0,  0 },
    {   6, 46, 42, 39,  6,  0,  0,  0,  0 },
    {  49,  6,  6,  6,  6,  0,  0,  0,  0 },
    {   0, 36, 38, 35,  6,  0,  0,  0,  0 },
    {   0, 37, 34, 33, 32,  0,  0,  0,  0 },
    {   0, 50,  6, 31, 50,  0,  0,  0,  0 },
	// clang-format on
};

const uint8_t GROOBO15[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0, 36, 38, 35,  6, 51, 48,  0 },
    {   0,  0, 37, 34, 33, 32,  6, 47,  4 },
    {   0,  0, 49,  6, 31,  6,  6,  6,  0 },
    {   0,  0,  6, 49,  6,  6, 50,  6,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {   0,  9,  2,  0,  0,  0, 70,  2,  0 },
    {   0,  0, 50,  6, 49, 95,  6, 47, 15 },
	// clang-format on
};

const uint8_t GROOBO16[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0, 96,  6,  6,  0,119,117 },
    {   0, 36, 38, 35, 51,  6,  0,116,  9 },
    {   0, 37, 34, 33, 32,  6,  0, 15, 12 },
    {   0, 96,  6, 31,  6,  6,  0,  0,  0 },
    {   0,  6, 51,  6,  6,  6,  0,  0,  0 },
    {   0,  0, 50, 51,  6,  6,  0,  0,  0 },
    {   0,  0,  6, 97,  6,  6,  0,119, 16 },
    {   0,  0,  0,  0,  0,  0,  0,116,125 },
	// clang-format on
};

DungeonMode ScannerPattern::getDungeonMode()
{
	return DungeonMode::BreakOnFailure;
}

bool ScannerPattern::skipSeed()
{
	// Church
	glSeedTbl[1] = sgGameInitInfo.dwSeed; // Matches level seed 1691869883, game seed 1230144506
	quests[Q_BUTCHER]._qactive = QUEST_INIT;
	quests[Q_PWATER]._qactive = QUEST_INIT;
	glSeedTbl[2] = sgGameInitInfo.dwSeed;         // Matches level seed 3227087147
	quests[Q_SKELKING]._qactive = QUEST_NOTAVAIL; // QUEST_INIT;
	glSeedTbl[3] = sgGameInitInfo.dwSeed;
	quests[Q_LTBANNER]._qactive = QUEST_NOTAVAIL;
	glSeedTbl[4] = sgGameInitInfo.dwSeed;

	// Catacombs
	quests[Q_BLOOD]._qactive = QUEST_INIT;
	glSeedTbl[5] = sgGameInitInfo.dwSeed;       // Matches level seed 3155785492
	quests[Q_SCHAMB]._qactive = QUEST_NOTAVAIL; // QUEST_INIT;
	glSeedTbl[6] = sgGameInitInfo.dwSeed;
	quests[Q_BLIND]._qactive = QUEST_NOTAVAIL; // QUEST_INIT;
	glSeedTbl[7] = sgGameInitInfo.dwSeed;
	glSeedTbl[8] = sgGameInitInfo.dwSeed;

	// Caves
	glSeedTbl[9] = sgGameInitInfo.dwSeed; // Matches level seed 3916317768, game seed 2728353700 or 3284159136
	quests[Q_ANVIL]._qactive = QUEST_INIT;
	glSeedTbl[10] = sgGameInitInfo.dwSeed; // Matches level seed 618024415
	glSeedTbl[11] = sgGameInitInfo.dwSeed; // Matches level seed 2376709555, dungeon seed 651290160
	glSeedTbl[12] = sgGameInitInfo.dwSeed; // Matches level seed 1903656652

	// Hell
	quests[Q_WARLORD]._qactive = QUEST_NOTAVAIL;
	glSeedTbl[13] = sgGameInitInfo.dwSeed; // Matches level seed 4250794344
	glSeedTbl[14] = sgGameInitInfo.dwSeed; // Matches level seed 1005627431
	glSeedTbl[15] = sgGameInitInfo.dwSeed; // Matches level seed 2844841604, game seed 1342549707
	glSeedTbl[16] = sgGameInitInfo.dwSeed; // Matches level seed 277866386

	return false;
}

bool ScannerPattern::skipLevel(int level)
{
	return level == 3 || level == 4 || level == 6 || level == 7 || level == 8; // Pattern are still not correct
}

bool ScannerPattern::levelMatches(std::optional<uint32_t> levelSeed)
{
	if (levelSeed == std::nullopt)
		return false;

	int stairTile;
	const uint8_t(*pattern)[TEMPLATEX][TEMPLATEY];
	if (currlevel == 1) {
		stairTile = 204;
		pattern = &GROOBO1;
	} else if (currlevel == 2) {
		stairTile = 65;
		pattern = &GROOBO2;
	} else if (currlevel == 3) {
		stairTile = 65;
		pattern = &GROOBO3;
	} else if (currlevel == 4) {
		stairTile = 65;
		pattern = &GROOBO4;
	} else if (currlevel == 5) {
		stairTile = 77;
		pattern = &GROOBO5;
	} else if (currlevel == 6) {
		stairTile = 77;
		pattern = &GROOBO6;
	} else if (currlevel == 7) {
		stairTile = 77;
		pattern = &GROOBO7;
	} else if (currlevel == 8) {
		stairTile = 77;
		pattern = &GROOBO8;
	} else if (currlevel == 9) {
		stairTile = 51;
		pattern = &GROOBO9;
	} else if (currlevel == 10) {
		stairTile = 51;
		pattern = &GROOBO10;
	} else if (currlevel == 11) {
		stairTile = 51;
		pattern = &GROOBO11;
	} else if (currlevel == 12) {
		stairTile = 51;
		pattern = &GROOBO12;
	} else if (currlevel == 13) {
		stairTile = 35;
		pattern = &GROOBO13;
	} else if (currlevel == 14) {
		stairTile = 41;
		pattern = &GROOBO14;
	} else if (currlevel == 15) {
		stairTile = 34;
		pattern = &GROOBO15;
	} else if (currlevel == 16) {
		stairTile = 35;
		pattern = &GROOBO16;
	}

	bool foundStairs = false;
	int sx = -1;
	int sy = -1;
	for (int y = 0; y < DMAXX && !foundStairs; y++) {
		for (int x = 0; x < DMAXY; x++) {
			if (dungeon[x][y] == stairTile) {
				sx = x - 3;
				sy = y - 1;
				foundStairs = true;
				break;
			}
		}
	}

	if (!foundStairs) {
		if (Config.verbose)
			std::cerr << "Pattern: Failed to locate the stairs on level seed " << *levelSeed << std::endl;
		return false;
	}

	bool found = true;
	int misses = 0;
	for (int column = 0; column < TEMPLATEY; column++) {
		for (int row = 0; row < TEMPLATEX; row++) {
			if ((*pattern)[row][column] == 0)
				continue;
			int x = sx + column;
			int y = sy + row;
			if (x < 0 || y < 0 || x >= DMAXX || y >= DMAXX || dungeon[x][y] != (*pattern)[row][column]) {
				misses++;
				if (misses > 1) {
					found = false;
					break;
				}
			}
		}
		if (!found)
			return false;
	}

	std::cout << "Level Seed for dlvl " << (int)currlevel << ": " << *levelSeed << std::endl;

	return true;
}
