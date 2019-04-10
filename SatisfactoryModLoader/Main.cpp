#include <stdafx.h>
#include <Main.h>
#include <utility/Configuration.h>
#include <utility/Logger.h>
#include <events/UtilityEvents.h>
#include "Globals.h"
#include "ModLoader.h"
#include "HookLoader.h"

#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <conio.h>

namespace SML {
	void Entry() {
		// run logic once
		const char* waitFile = "wait";
		std::ifstream waitFileInput(waitFile);
		if (!waitFileInput.good()) {
			std::ofstream waitFileOutput(waitFile);
			waitFileOutput.close();
			waitFileInput.close();
			return;
		}
		waitFileInput.close();

		int code = std::remove(waitFile);
		if (code != 0) {
			// error in removing tmp file
			return;
		}

		// load config
		Configuration config("..\\config");
		if (!config.exists()) {
			config.set("ShowConsole", true);
			config.set("ShowDebug", false);
			config.set("SupressErrors", false);
			config.save();
		}
		config.load();

		globals.showConsoleDebug = config.get("ShowDebug", false);
		globals.supressConsoleErrors = config.get("SupressErrors", false);

		// verify folders
		struct stat info;

		if (stat("config", &info) != 0) {
			CreateDirectoryA("config", NULL);
		}

		if (stat("mods", &info) != 0) {
			CreateDirectoryA("mods", NULL);
		}

		if (config.get("ShowConsole", true)) {
			// display console
			AllocConsole();
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			FILE* fp;
			freopen_s(&fp, "CONOIN$", "r", stdin);
			freopen_s(&fp, "CONOUT$", "w", stdout);
			freopen_s(&fp, "CONOUT$", "w", stderr);
		}

		// load mods
		ModLoader modLoader;
		SML::info("Caching dlls");
		modLoader.CacheMods();
		SML::info("Verifying mods");
		modLoader.VerifyMods();
		SML::info("Verifying dependencies");
		modLoader.VerifyDependencies();

		// load hooks
		SML::info("Setting up mods");
		modLoader.SetupMods();

		SML::info("Mod setup complete");
		if(config.get("ShowConsole", true)) {
			_getch();
		}
	}

	void Cleanup() {
		for (BaseMod* mod : globals.mods) {
			mod->Cleanup();
		}

		SML::logFile.close();
	}
}