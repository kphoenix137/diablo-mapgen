//#include "types.h"
//#include "enums.h"
//#include "level.h"
//#include "drlg_l1.h"
//#include "drlg_l2.h"
//#include "drlg_l3.h"
//#include "drlg_l4.h"
//
//#include "engine.h"
//#include "gendung.h"
#include "all.h"
#include "level.h"
#include <iostream>
#include <iomanip>



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

void whatleveltype(int l)
{
	if (l == 0)
	{
		leveltype = DTYPE_TOWN;
		FillSolidBlockTbls();
		LoadLvlGFX();
	}
	if (l == 1)
	{
		leveltype = DTYPE_CATHEDRAL;
		FillSolidBlockTbls();
		LoadLvlGFX();
	}
	if (l == 5)
	{
		leveltype = DTYPE_CATACOMBS;
		FillSolidBlockTbls();
		LoadLvlGFX();
	}
	if (l == 9)
	{
		leveltype = DTYPE_CAVES;
		FillSolidBlockTbls();
		LoadLvlGFX();
	}
	if (l == 13)
	{
		leveltype = DTYPE_HELL;
		FillSolidBlockTbls();
		LoadLvlGFX();
	}
}

void createSpecificDungeon(int i)
{
	if (leveltype == DTYPE_CATHEDRAL)
		CreateL5Dungeon(glSeedTbl[i], 0);
	else if (leveltype == DTYPE_CATACOMBS)
		CreateL2Dungeon(glSeedTbl[i], 0);
	else if (leveltype == DTYPE_CAVES)
		CreateL3Dungeon(glSeedTbl[i], 0);
	else if (leveltype == DTYPE_HELL)
		CreateL4Dungeon(glSeedTbl[i], 0);
}

/**
 * @brief GET MAIN SEED, GET ALL MAP SEEDS
 * @return nothing, but updates RNG seeds list glSeedTbl[i]
 */
void seedSelection(int s)
{
	int i;

	SetRndSeed(0);
	sgGameInitInfo.dwSeed = s;
	sgGameInitInfo.bDiff = gnDifficulty;

	gnDifficulty = sgGameInitInfo.bDiff;
	SetRndSeed(sgGameInitInfo.dwSeed);

	for (i = 0; i < NUMLEVELS; i++) {
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

int main(int argc, char **argv)
{
	int startSeed = 0;
	int seedCount = 1;
	bool quiet = false;
	bool printLevels = true;
	bool exportLevels = true;

	for (int seed = startSeed; seed < seedCount; seed++) {
		if (!quiet)
			std::cout << "processing seed " << seed << std::endl;

		seedSelection(seed);
		InitQuests();

		for (int level = 1; level < NUMLEVELS; level++) {
			currlevel = level;
			whatleveltype(level);

			createSpecificDungeon(level);
			if (printLevels && !quiet)
				printAsciiLevel();
			if (exportLevels)
				ExportDun();
		}
	}

	return 0;
}



//if (dungeon[bobx][boby] == 6) //CATH:15, CATA:3, HELL:6
