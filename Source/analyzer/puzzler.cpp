#include <iostream>

#include "../funkMapGen.h"
#include "../items.h"
#include "../monster.h"
#include "../objects.h"

bool SearchForPuzzler()
{
	bool foundPuzzler = false;
	for (int i = 0; i < numitems; i++) {
		int ii = itemactive[i];
		foundPuzzler |= item[ii]._iMagical == ITEM_QUALITY_UNIQUE && item[ii]._iUid == 60;
	}
	if (!foundPuzzler)
		return false;

	if (Config.verbose) {
		std::cout << "Monster Count: " << nummonsters << std::endl;
		for (int i = 0; i < nummonsters; i++) {
			std::cout << "Monster " << i << ": " << monster[monstactive[i]].mName << " (" << monster[monstactive[i]]._mRndSeed << ")" << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Object Count: " << nobjects << std::endl;
		for (int i = 0; i < nobjects; i++) {
			int oid = objectactive[i];
			char objstr[50];
			GetObjectStr(oid, objstr);
			std::cout << "Object " << i << ": " << objstr << " (" << object[oid]._oRndSeed << ")" << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Item Count: " << numitems << std::endl;
		for (int i = 0; i < numitems; i++) {
			std::string prefix = "";
			if (i >= ObjectItems)
				prefix = "Object ";
			else if (i >= MonsterItems)
				prefix = "Monster ";
			std::cout << prefix << "Item " << i << ": " << item[itemactive[i]]._iIName << " (" << item[itemactive[i]]._iSeed << ")" << std::endl;
		}
	}

	return true;
}
