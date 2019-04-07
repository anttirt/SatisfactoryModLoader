#include <stdafx.h>
#include <Main.h>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "utility/Configuration.h"
#include "Globals.h"
#include "ModLoader.h"
#include "HookLoader.h"

namespace SML {
	void Entry() {
		// run logic once
		std::ifstream waitFileInput("wait");
		if (!waitFileInput.good()) {
			std::ofstream waitFileOutput("wait");
			waitFileOutput.close();
			waitFileInput.close();
			return;
		}
		waitFileInput.close();

		int code = std::remove("wait");
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

		Globals::showConsoleDebug = config.get("ShowDebug", false);
		Globals::supressConsoleErrors = config.get("SupressErrors", false);

		// verify folders
		struct stat info;

		if (stat("configs", &info) != 0) {
			CreateDirectoryA("configs", NULL);
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
		modLoader.CacheMods();
		Globals::modList = modLoader.VerifyMods();

		// load hooks
		HookLoader hookLoader;
		hookLoader.RegisterHooks();
	}
}