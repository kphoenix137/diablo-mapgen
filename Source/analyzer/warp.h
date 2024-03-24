#pragma once

#include "../funkMapGen.h"

class ScannerWarp : public Scanner {
public:
	bool skipLevel(int level) override;
	bool levelMatches(int levelSeed) override;
};
