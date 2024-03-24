#include "gameseed.h"

#include <iostream>

#include "../../types.h"

#include "../funkMapGen.h"
#include "../monster.h"

bool ScannerGameSeed::breakOnFailure()
{
	return true;
}

bool ScannerGameSeed::skipLevel(int level)
{
	if (level != 9)
		return true;

	InitDungeonMonsters();

	bool hasLavaLoards = false;
	for (int i = 0; i < nummtypes && !hasLavaLoards; i++)
		hasLavaLoards = Monsters[i].mtype == MT_WMAGMA;

	return !hasLavaLoards;
}

bool ScannerGameSeed::levelMatches(int levelSeed)
{
	if (levelSeed == -1)
		return false;

	std::cout << sgGameInitInfo.dwSeed << std::endl;

	return levelSeed == Config.quality;
}
