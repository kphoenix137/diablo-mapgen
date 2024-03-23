#pragma once

#include <string>

#include "engine.h"

enum class Scanners {
	None,
	Path,
	Puzzler,
	Warp,
	Pattern,
	GameSeed,
};

struct Configuration {
	uint32_t startSeed = 0;
	uint32_t seedCount = 1;
	std::string seedFile;
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
extern Point POI;

extern char Path[MAX_PATH_LENGTH];

extern Configuration Config;
