#include <stdafx.h>
#include <iostream>
#include <utility/Reflection.h>
#include <other/BaseMod.h>
#include "ModLoader.h"
#include "Globals.h"

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

	// run Setup() on each mod
	void ModLoader::SetupMods() {
		for(BaseMod* mod : _mods) {
			mod->PreSetup(&globals);
		}
	}

	// verify all dlls to find valid mods
	void ModLoader::VerifyMods() {
		for (auto path : _cachedFiles) {
			HMODULE dll = LoadLibraryA(path.string().c_str());
			if (!dll) {
				continue;
			}

			auto func = ref::get_function(dll, "CreateMod");
			if (!func) {
				std::cout << "Invalid Values" << std::endl;
				FreeLibrary(dll);
				continue;
			}
			BaseMod* mod = ((BaseMod*(WINAPI*)())func)();

			// check if the mod is already loaded
			bool isDuplicate = false;
			for (auto existingMod : _mods) {
				if (existingMod->Name() == mod->Name()) {
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

			mod->FileName = fileName.c_str();
			std::cout << mod->FileName << std::endl;
			_mods.push_back(mod);

			std::cout << "Loaded [" << mod->Name() << "@" << mod->Version() << "]" << std::endl;
		}
	}
}