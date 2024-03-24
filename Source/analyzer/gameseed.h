#pragma once

#include "../funkMapGen.h"

class ScannerGameSeed : public Scanner {
public:
	bool breakOnFailure() override;
	bool skipLevel(int level) override;
	bool levelMatches(int levelSeed) override;
};
