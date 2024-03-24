#include "warp.h"

#include <iostream>

#include "../../types.h"

#include "../funkMapGen.h"

bool ScannerWarp::skipLevel(int level)
{
	return level != 15;
}

bool ScannerWarp::levelMatches(std::optional<uint32_t> levelSeed)
{
	if (nSolidTable[dPiece[POI.x][POI.y]])
		return false;

	std::cout << sgGameInitInfo.dwSeed << std::endl;

	return true;
}
