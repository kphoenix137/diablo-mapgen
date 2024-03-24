#pragma once

#include "../funkMapGen.h"

class ScannerPattern : public Scanner {
public:
	bool breakOnFailure() override;
	bool skipSeed() override;
	bool skipLevel(int level) override;
	bool levelMatches(int levelSeed) override;
};
