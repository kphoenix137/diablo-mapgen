#include <iostream>

#include "../../types.h"

#include "../funkMapGen.h"
#include "../quests.h"

bool SkipQuest()
{
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
