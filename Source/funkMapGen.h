#pragma once

#include <string>

#include "engine.h"

enum class Scanners {
	None,
	Path,
	Quest,
	Puzzler,
	Warp,
	Pattern,
	GameSeed,
};

class Scanner {
public:
	virtual void init() {};

	virtual DungeonMode getDungeonMode()
	{
		return DungeonMode::Full;
	};

	virtual bool skipSeed()
	{
		return false;
	};

	virtual bool skipLevel(int level)
	{
		return false;
	};

	virtual bool levelMatches(std::optional<uint32_t> levelSeed)
	{
		return true;
	};

	virtual ~Scanner()
	{
	}
};

struct Configuration {
	uint32_t startSeed = 0;
	uint32_t seedCount = 1;
	std::string seedFile;
	Scanners scanner = Scanners::None;
	bool quiet = false;
	bool asciiLevels = false;
	bool exportLevels = false;
	uint32_t etc = 420;
	bool verbose = false;
};

extern int MonsterItems;
extern int ObjectItems;

extern Point Spawn;
extern Point StairsDown;
extern Point POI;

extern char Path[MAX_PATH_LENGTH];

extern Configuration Config;

void InitDungeonMonsters();
