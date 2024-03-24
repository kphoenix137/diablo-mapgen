#include "pattern.h"

#include <cstdint>
#include <iostream>

#include "../../types.h"

#include "../engine.h"
#include "../gendung.h"

#define TEMPLATEX 8
#define TEMPLATEY 8

const uint8_t GROOBO9[TEMPLATEX][TEMPLATEY] = {
	// clang-format off
    {   0,  0,  0,  0,  8,  0,  0,  0 },
    {   0,  8,  8, 51, 50,  8,  8, 89 },
    {   8,  8, 11, 48, 49,  3, 11, 88 },
    {   8,  8, 47,106,  7, 13, 12,106 },
    {   8,  8, 46,107,  7,106,108,106 },
    {   0,  8,  1, 14,106,106,  5, 14 },
    {   0,  0,  8,  1, 14,106,  4,  0 },
    {   0,  0,  0,  0, 12,  0,  0,  0 },
	// clang-format on
};

bool ScannerPattern::breakOnFailure()
{
	return true;
}

bool ScannerPattern::skipSeed()
{
	glSeedTbl[9] = sgGameInitInfo.dwSeed;

	return false;
}

bool ScannerPattern::skipLevel(int level)
{
	return level != 9;
}

bool ScannerPattern::levelMatches(int levelSeed)
{
	bool foundStairs = false;
	int sx = 0;
	int sy = 0;
	for (int y = 0; y < DMAXX && !foundStairs; y++) {
		for (int x = 0; x < DMAXY; x++) {
			if (dungeon[x][y] == 51) {
				sx = x - 3;
				sy = y - 1;
				foundStairs = true;
				break;
			}
		}
	}

	if (sx < 0 || sy < 0 || sx >= DMAXX - TEMPLATEX || sx >= DMAXX - TEMPLATEX)
		return false;

	bool found = true;
	for (int column = 0; column < TEMPLATEX; column++) {
		for (int row = 0; row < TEMPLATEY; row++) {
			if (GROOBO9[row][column] == 0)
				continue;
			if (dungeon[sx + column][sy + row] != GROOBO9[row][column]) {
				found = false;
				break;
			}
		}
		if (!found)
			return false;
	}

	std::cout << "Level Seed: " << (uint32_t)levelSeed << std::endl;

	return true;
}
