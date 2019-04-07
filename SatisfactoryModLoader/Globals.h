#pragma once

#include <string>
#include <vector>
#include <map>
#include "HookLoader.h"

namespace SML {
	class Globals {
	public:
		static const char* modLoaderVersion;
		static const char* targetVersion;
		static bool showConsoleDebug;
		static bool supressConsoleErrors;
		std::map<HookLoader::Event, PVOID> functions;
	};

	extern Globals globals;
}