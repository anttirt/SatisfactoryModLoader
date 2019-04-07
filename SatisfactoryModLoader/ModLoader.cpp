#include <stdafx.h>
#include <iostream>
#include "utility/Reflection.h"
#include "ModLoader.h"

namespace fs = std::experimental::filesystem;
namespace ref = SML::Reflection;

namespace SML {
	// cache all dll files from the /mods/ folder
	void ModLoader::CacheMods() {
		_cachedFiles.clear();

		for (const auto& entry : fs::directory_iterator("mods")) {
			auto path = entry.path();

			if (path.extension().string() == ".dll") {
				std::cout << "Located: " << path.string() << std::endl;
				_cachedFiles.push_back(path);
			}
		}
	}

	// verify all dlls to find valid mods
	std::vector<Mod> ModLoader::VerifyMods() {
		std::vector<Mod> mods;
		for (auto path : _cachedFiles) {
			HMODULE dll = LoadLibraryA(path.string().c_str());
			if (!dll) {
				continue;
			}

			Mod mod;

			// check if the mod is valid
			if (!ref::get_field_value(dll, "ModName", mod.Name) ||
				!ref::get_field_value(dll, "ModVersion", mod.Version) || 
				!ref::get_field_value(dll, "ModDescription", mod.Description) || 
				!ref::get_field_value(dll, "ModAuthors", mod.Authors) /*|| 
				!ref::get_field_value(dll, "ModDependencies", mod.Dependencies)*/) {
				std::cout << "Invalid Values" << std::endl;
				FreeLibrary(dll);
				continue;
			}

			// check if the mod is already loaded
			bool isDuplicate = false;
			for (Mod existingMod : mods) {
				if (existingMod.Name == mod.Name) {
					FreeLibrary(dll);
					isDuplicate = true;
					break;
				}
			}

			if (isDuplicate) {
				continue;
			}

			std::string fileNameBase = path.filename().string();
			std::string fileName = fileNameBase.substr(0, fileNameBase.find_last_of('.'));

			mod.FileName = fileName;
			std::cout << mod.FileName << std::endl;
			mods.push_back(mod);

			std::cout << "Loaded [" << mod.FileName << "@" << mod.Version << "]" << std::endl;
		}

		return mods;
	}
}