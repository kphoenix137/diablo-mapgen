#include <iostream>

#include "../../types.h"

#include "../funkMapGen.h"
#include "../objects.h"
#include "../path.h"
#include "../quests.h"

#define MAXVIEWX 21
#define MAXVIEWY 21
bool isVisible[MAXVIEWY][MAXVIEWX] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //	-y
	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, // -x	origin(10,10)	+x
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //	+y
};

Point StairsDownPrevious;

BOOL PosOkPlayer(int pnum, int x, int y)
{
	if (x < 0 || y < 0 || x >= MAXDUNX || y >= MAXDUNY)
		return FALSE;
	int tileId = dPiece[x][y];
	if (tileId == 0)
		return FALSE;
	if (nSolidTable[tileId])
		return FALSE;

	if (dObject[x][y] != 0) {
		char bv;
		if (dObject[x][y] > 0) {
			bv = dObject[x][y] - 1;
		} else {
			bv = -(dObject[x][y] + 1);
		}
		if (object[bv]._oSolidFlag) {
			return FALSE;
		}
	}

	return TRUE;
}

int PathLength()
{
	std::memset(Path, 0, sizeof(Path));

	return FindPath(PosOkPlayer, 0, Spawn.x, Spawn.y, StairsDown.x, StairsDown.y, Path);
}

bool IsVisibleSpawn()
{
	if (Spawn == Point { -1, -1 } || StairsDown == Point { -1, -1 }) {
		return false;
	}

	int horizontal = StairsDown.x - Spawn.x + 10;
	int vertical = StairsDown.y - Spawn.y + 10;

	if (horizontal < 0 || horizontal > MAXVIEWX)
		return false;
	if (vertical < 0 || vertical > MAXVIEWY)
		return false;
	return isVisible[vertical][horizontal];
}

bool IsVisiblePrevious()
{
	if (StairsDownPrevious == Point { -1, -1 } || StairsDown == Point { -1, -1 }) {
		return false;
	}

	int horizontal = StairsDown.x - StairsDownPrevious.x + 10;
	int vertical = StairsDown.y - StairsDownPrevious.y + 10;

	if (horizontal < 0 || horizontal > MAXVIEWX)
		return false;
	if (vertical < 0 || vertical > MAXVIEWY)
		return false;
	return isVisible[vertical][horizontal];
}

int lengthPathToDlvl9 = 0;

int CalcStairsChebyshevDistance()
{
	if (Spawn == Point { -1, -1 } || StairsDown == Point { -1, -1 }) {
		return -1;
	}

	int horizontal = std::max(Spawn.x, StairsDown.x) - std::min(Spawn.x, StairsDown.x);
	int vertical = std::max(Spawn.y, StairsDown.y) - std::min(Spawn.y, StairsDown.y);

	return std::max(horizontal, vertical);
}

bool IsGoodLevel()
{
	int maxDistance = 25;
	int lengthMaxPathToDlvl9 = 100;

	if (leveltype == DTYPE_CATACOMBS || leveltype == DTYPE_CATHEDRAL) {
		int cDistance = CalcStairsChebyshevDistance();
		StairsDownPrevious = StairsDown;

		if (cDistance != -1 && cDistance > maxDistance)
			return false;

		int stairsPath = PathLength();
		lengthPathToDlvl9 = lengthPathToDlvl9 + stairsPath;

		if (lengthPathToDlvl9 > lengthMaxPathToDlvl9)
			return false;
		if (stairsPath == 0 || stairsPath > maxDistance)
			return false;
	} else //(leveltype == DTYPE_CAVES || leveltype == DTYPE_HELL)
	{
		maxDistance = 25;

		bool isStairsVisibile = IsVisibleSpawn() || IsVisiblePrevious();
		StairsDownPrevious = StairsDown;

		if (currlevel != 9 && isStairsVisibile)
			return true;

		int cDistance = CalcStairsChebyshevDistance(); // CONDITIONALLY REWORK REQUIRED: WITH TELEPORT, DLVL 10+ DEPENDING ON DESTINATION VECTOR, MOVEMENT DISTANCE WILL BE SHORTER OR LONGER

		if (cDistance != -1 && cDistance > maxDistance)
			return false;

		int stairsPath = PathLength(); // CONDITIONALLY OBSOLETED: WITH TELEPORT, DLVL 10+ WILL NOT USE THIS PATHING. ONLY USED FOR PATHING ON DLVL9

		if (stairsPath == 0 || stairsPath > maxDistance)
			return false;
	}
	return true;
}

bool ShortPathSeedSkip()
{
	lengthPathToDlvl9 = 0;

	if (quests[Q_LTBANNER]._qactive != QUEST_NOTAVAIL) {
		if (Config.verbose)
			std::cerr << "Game Seed: " << sgGameInitInfo.dwSeed << " thrown out: Sign Quest" << std::endl;
		return true;
	}
	if (quests[Q_WARLORD]._qactive != QUEST_NOTAVAIL) {
		if (Config.verbose)
			std::cerr << "Game Seed: " << sgGameInitInfo.dwSeed << " thrown out: Warlord" << std::endl;
		return true;
	}

	return false;
}

bool ShortPathSearch()
{
	int level = currlevel;
	if (level == Config.quality) {
		std::cout << sgGameInitInfo.dwSeed << " (level " << (level - 1) << " in " << lengthPathToDlvl9 << " steps)" << std::endl;
		return false;
	}

	return IsGoodLevel();
}
