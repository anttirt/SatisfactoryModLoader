#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <other/BaseMod.h>

namespace SML {
	class ModLoader {
	public:
		void CacheMods();
		void SetupMods();
		void VerifyMods();
		void VerifyDependencies();
	private:
		std::vector<std::experimental::filesystem::v1::path> _cachedFiles;
	};
}