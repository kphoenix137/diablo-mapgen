#include "level.h"

#include <cstdint>
#include <stdio.h>

#include "../types.h"
#include "gendung.h"

inline void WriteLE16(FILE *out, uint16_t val)
{
	fwrite(&val, 1, 2, out);
}

void ExportDun()
{
	FILE *dunFile = fopen("levelName.dun", "wb");

	WriteLE16(dunFile, DMAXX);
	WriteLE16(dunFile, DMAXY);

	/** Tiles. */
	for (int y = 0; y < DMAXY; y++) {
		for (int x = 0; x < DMAXX; x++) {
			WriteLE16(dunFile, dungeon[x][y]);
		}
	}

	/** Padding */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			WriteLE16(dunFile, 0);
		}
	}

	/** Monsters */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			uint16_t monsterId = 0;
			// if (dMonster[x][y] > 0) {
			//	for (int i = 0; i < 157; i++) {
			//		if (MonstConvTbl[i] == Monsters[std::abs(dMonster[x][y]) - 1].type().type) {
			//			monsterId = i + 1;
			//			break;
			//		}
			//	}
			// }
			WriteLE16(dunFile, monsterId);
		}
	}

	/** Objects */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			uint16_t objectId = 0;
			// Object *object = FindObjectAtPosition({ x, y }, false);
			// if (object != nullptr) {
			//	for (int i = 0; i < 147; i++) {
			//		if (ObjTypeConv[i] == object->_otype) {
			//			objectId = i;
			//			break;
			//		}
			//	}
			// }
			WriteLE16(dunFile, objectId);
		}
	}

	/** Transparency */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			WriteLE16(dunFile, dTransVal[x][y]);
		}
	}
	fclose(dunFile);
}
