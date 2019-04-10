#pragma once

#include <string>
#include <vector>
#include <map>
#include "HookLoader.h"

namespace SML {
	class BaseMod;

	class Globals {
	public:
		static const char* modLoaderVersion;
		static const char* targetVersion;
		static bool showConsoleDebug;
		static bool supressConsoleErrors;
		std::vector<BaseMod*> mods;
	};

	extern Globals globals;
}