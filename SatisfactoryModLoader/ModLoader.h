#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include "other/Mod.h"

namespace SML {
	class ModLoader {
	public:
		void CacheMods();
		std::vector<Mod> VerifyMods();
	private:
		std::vector<std::experimental::filesystem::v1::path> _cachedFiles;
	};
}