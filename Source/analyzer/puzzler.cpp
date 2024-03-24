#include <iostream>

#include "../funkMapGen.h"
#include "../items.h"
#include "../monster.h"
#include "../objects.h"

bool SearchForPuzzler()
{
	POI = { -1, -1 };
	for (int i = 0; i < numitems; i++) {
		int ii = itemactive[i];
		if (item[ii]._iMagical == ITEM_QUALITY_UNIQUE && item[ii]._iUid == 60) {
			POI = { item[ii]._ix, item[ii]._iy };
			break;
		}
	}
	if (POI == Point { -1, -1 })
		return false;

	if (Config.verbose) {
		std::cerr << "Monster Count: " << nummonsters << std::endl;
		for (int i = 0; i < nummonsters; i++) {
			std::cerr << "Monster " << i << ": " << monster[monstactive[i]].mName << " (" << monster[monstactive[i]]._mRndSeed << ")" << std::endl;
		}
		std::cerr << std::endl;
		std::cerr << "Object Count: " << nobjects << std::endl;
		for (int i = 0; i < nobjects; i++) {
			int oid = objectactive[i];
			char objstr[50];
			GetObjectStr(oid, objstr);
			std::cerr << "Object " << i << ": " << objstr << " (" << object[oid]._oRndSeed << ")" << std::endl;
		}
		std::cerr << std::endl;
		std::cerr << "Item Count: " << numitems << std::endl;
		for (int i = 0; i < numitems; i++) {
			std::string prefix = "";
			if (i >= ObjectItems)
				prefix = "Object ";
			else if (i >= MonsterItems)
				prefix = "Monster ";
			std::cerr << prefix << "Item " << i << ": " << item[itemactive[i]]._iIName << " (" << item[itemactive[i]]._iSeed << ")" << std::endl;
		}
	}

	return true;
}
