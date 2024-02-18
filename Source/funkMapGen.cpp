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

int main()
{
	for (int q = 0; q < 1; q++)
	{
		seedSelection(q);
		InitQuests();

		int abc = 1;

		for (int i = abc; i < NUMLEVELS; i++)
		{
			currlevel = i;
			whatleveltype(i);

			createSpecificDungeon(i);

			for (int boby = 0; boby < MAXDUNY; boby++)
			{
				for (int bobx = 0; bobx < MAXDUNX; bobx++)
				{
					if (nSolidTable[dPiece[bobx][boby]])
						std::cout << "#";
					else
						std::cout << " ";
					//std::cout << (0 + (dPiece[bobx][boby] % 10));
					dPiece[bobx][boby];
					//std::cout << min(max(dPiece[bobx][boby]-8,0),9);
				}
				std::cout << std::endl;
			}
			std::cout << "--------------------------" << std::endl;
		}
		ExportDun();
	}
	return 0;
}



//if (dungeon[bobx][boby] == 6) //CATH:15, CATA:3, HELL:6
