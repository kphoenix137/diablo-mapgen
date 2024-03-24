#include "path.h"

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

int TotalTickLenth;

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
	constexpr int ticksToTeleport = 12;
	constexpr int ticksToWalkATile = 8;
	int tickLenth = 0;

	if (currlevel < 9) {
		int cDistance = CalcStairsChebyshevDistance();
		if (cDistance != -1 && cDistance > MAX_PATH_LENGTH)
			return false;

		int stairsPath = PathLength();
		if (stairsPath == 0)
			return false;

		tickLenth = stairsPath * ticksToWalkATile;
	} else if (currlevel == 9) {
		/** @todo Calculate walk to Puzzler and TP to exit */
		int cDistance = CalcStairsChebyshevDistance();
		if (cDistance != -1 && cDistance > MAX_PATH_LENGTH)
			return false;

		int stairsPath = PathLength();
		if (stairsPath == 0)
			return false;

		tickLenth = stairsPath * ticksToWalkATile;
	} else if (currlevel == 15) {
		/** @todo Try warp, or teleport to staff + walk in town then teleport to Laz, and then exit */
		if (IsVisibleSpawn() || IsVisiblePrevious()) {
			tickLenth = ticksToTeleport;
		} else {
			int cDistance = CalcStairsChebyshevDistance();
			if (cDistance == -1)
				return false;
			/** @todo Take teleport limits in to considerations instead of just estimating 5 steps */
			tickLenth = cDistance * ticksToTeleport / 5;
		}
	} else {
		if (IsVisibleSpawn() || IsVisiblePrevious()) {
			tickLenth = ticksToTeleport;
		} else {
			int cDistance = CalcStairsChebyshevDistance();
			if (cDistance == -1)
				return false;
			/** @todo Take teleport limits in to considerations instead of just estimating 5 steps */
			tickLenth = cDistance * ticksToTeleport / 5;
		}
	}

	TotalTickLenth += tickLenth;

	return true;
}

bool Ended;

bool ScannerPath::skipSeed()
{
	TotalTickLenth = 0;
	Ended = false;

	return false;
}

bool ScannerPath::skipLevel(int level)
{
	return Ended;
}

bool ScannerPath::levelMatches(int levelSeed)
{
	std::memset(Path, 0, sizeof(Path));
	if (!IsGoodLevel()) {
		Ended = true;
	}

	StairsDownPrevious = StairsDown;

	int level = currlevel;
	if (level == Config.quality - 1) {
		std::cout << sgGameInitInfo.dwSeed << " (level " << (level + 1) << " in " << ((float)TotalTickLenth / 50) << " sec)" << std::endl;
		Ended = true;
	}

	return true;
}
