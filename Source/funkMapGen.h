#pragma once

#include "engine.h"

enum class Scanners {
	None,
	Path,
	Puzzler,
	Pattern,
	GameSeed,
};

struct Configuration {
	uint32_t startSeed = 0;
	uint32_t seedCount = 1;
	Scanners scanner = Scanners::None;
	bool quiet = false;
	bool asciiLevels = false;
	bool exportLevels = false;
	uint32_t quality = 6;
	bool verbose = false;
};

extern int MonsterItems;
extern int ObjectItems;

extern Point Spawn;
extern Point StairsDown;

extern char Path[MAX_PATH_LENGTH];

extern Configuration Config;
