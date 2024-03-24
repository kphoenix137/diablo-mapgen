#pragma once

#include "../funkMapGen.h"

class ScannerPath : public Scanner {
public:
	bool skipSeed() override;
	bool skipLevel(int level) override;
	bool levelMatches(int levelSeed) override;
};
