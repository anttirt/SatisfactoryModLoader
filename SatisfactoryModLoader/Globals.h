#pragma once

#include <string>
#include <vector>
#include <map>
#include "other/Mod.h"
#include "HookLoader.h"

namespace SML {
	namespace Globals {
		static const char* modLoaderVersion = "v0.1 beta";
		static const std::string targetVersion = "96463";
		static bool showConsoleDebug;
		static bool supressConsoleErrors;
		static std::vector<Mod> modList;
		static std::map<HookLoader::Event, PVOID> functions;
	}
}