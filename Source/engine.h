#pragma once
#include "../types.h"
#include "gendung.h"

#include <string>
#define assert_fail(exp) ((void)(exp))

struct Point {
	int x;
	int y;

	bool operator==(const Point &other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point &other) const
	{
		return !(*this == other);
	}
};

const int myplr = 0;
extern BYTE gbMaxPlayers;
extern BOOL leveldebug;
extern BOOL light4flag;
extern DWORD glSeedTbl[NUMLEVELS];
extern _gamedata sgGameInitInfo;
extern int gnDifficulty;
extern PlayerStruct plr[MAX_PLRS];
extern bool zoomflag;
extern int questdebug;
extern bool oobread;
extern bool oobwrite;

/**
 * Get time stamp in microseconds.
 */
uint64_t micros();

BOOL delta_quest_inited(int i);
int random_(BYTE idx, int v);
[[noreturn]] void app_fatal(const char *dummystring);
void SetAutomapView(int nXPos, int nYPos);
void SetRndSeed(int s);
int GetRndSeed();
int GetRndState();

inline int GetdPiece(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAXDUNX || y >= MAXDUNY) {
		oobread = true;
		return 0;
	}
	return dPiece[x][y];
}

inline BYTE GetDungeon(int x, int y)
{
	if (x < 0 || y < 0 || x >= DMAXX || y >= DMAXY) {
		oobread = true;
		return 0;
	}
	return dungeon[x][y];
}

inline void SetDungeon(int x, int y, BYTE value)
{
	if (x < 0 || y < 0 || x >= DMAXX || y >= DMAXY) {
		oobwrite = true;
		return;
	}
	dungeon[x][y] = value;
}

BYTE *DiabloAllocPtr(DWORD dwBytes);
void mem_free_dbg(void *p);
BYTE *LoadFileInMem(std::string pszName, DWORD *pdwFileLen);
void LoadLvlGFX();

void SetMapObjects(BYTE *pMap, int startx, int starty);

void SetMapMonsters(BYTE *pMap, int startx, int starty);
