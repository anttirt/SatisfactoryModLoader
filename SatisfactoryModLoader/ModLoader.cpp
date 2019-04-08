#include <stdafx.h>
#include <iostream>
#include <utility/Reflection.h>
#include <utility/Logger.h>
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
				SML::info("Caching ", path.string());
				_cachedFiles.push_back(path);
			}
		}
	}

	// run Setup() on each mod
	void ModLoader::SetupMods() {
		for(BaseMod* mod : globals.mods) {
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
				SML::error("Invalid: ", path.string());
				FreeLibrary(dll);
				continue;
			}
			BaseMod* mod = ((BaseMod*(WINAPI*)())func)();

			// check if the mod is already loaded
			bool isDuplicate = false;
			for (auto existingMod : globals.mods) {
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
			globals.mods.push_back(mod);

			SML::info("Loaded [", mod->Name(), "@", mod->Version(), "]");
		}
	}

	void ModLoader::VerifyDependencies() {
		std::vector<const char*> names;
		for (BaseMod* mod : globals.mods) {
			names.push_back(mod->Name());
		}
		for (BaseMod* mod : globals.mods) {
			std::string name(mod->Name());
			for (std::string dep : mod->Dependencies()) {
				info("Parsing dependency ", dep);
				if (dep.substr(0, 1) == "*") {
					auto it = std::find(names.begin(), names.end(), dep.substr(1));
					if (it == names.end()) {
						warning(name, " is missing optional dependency ", dep.substr(1));
					}
				}
				else {
					auto it = std::find(names.begin(), names.end(), dep);
					if (it == names.end()) {
						std::string msg = "Mod " + name + " is missing dependency " + dep + "!\nPlease install " + dep + " or remove " + name + ".\nPress Ok to exit.";
						MessageBoxA(NULL, msg.c_str(), "SatisfactoryModLoader Fatal Error", MB_ICONERROR);
						abort();
					}
				}
			}
		}
	}
}