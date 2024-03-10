#pragma once

#include "engine.h"

enum class Scanners {
	None,
	Path,
	Puzzler,
};

struct Configuration {
	uint32_t startSeed = 0;
	uint32_t seedCount = 1;
	Scanners scanner = Scanners::None;
	bool quiet = false;
	bool asciiLevels = false;
	bool exportLevels = false;
	int quality = 6;
	bool verbose = false;
};

extern int MonsterItems;
extern int ObjectItems;

extern Point Spawn;
extern Point StairsDown;

extern char Path[MAX_PATH_LENGTH];

extern Configuration Config;
