#pragma once

#include <cstdint>
#include <string>
#include <optional>

#include "../analyzer/scannerName.h"

struct Configuration {
	static Configuration ParseArguments(int argc, char **argv);

	uint32_t startSeed = 0;
	uint32_t seedCount = 1;
	std::string seedFile;
	Scanners scanner = Scanners::None;
	bool quiet = false;
	bool asciiLevels = false;
	bool exportLevels = false;
	std::optional<uint32_t> target = std::nullopt;
	std::optional<uint8_t> prefix = std::nullopt;
	std::optional<uint8_t> suffix = std::nullopt;
	std::optional<uint32_t> prefixval = std::nullopt;
	std::optional<uint32_t> suffixval = std::nullopt;
	std::optional<uint32_t> uid = std::nullopt;
	bool verbose = false;
};
