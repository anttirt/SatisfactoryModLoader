#include <stdafx.h>
#include <Main.h>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <utility/Configuration.h>
#include <utility/Logger.h>
#include <other/CommandParser.h>
#include <other/CommandSystem.h>
#include "Globals.h"
#include "ModLoader.h"
#include "HookLoader.h"

namespace SML {
	void HealPerson(void* player, float amount) {
		SML::info("Healing for ", amount, " health");
	}

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
		SML::info("Caching dlls");
		modLoader.CacheMods();
		SML::info("Verifying mods");
		modLoader.VerifyMods();
		SML::info("Verifying dependencies");
		modLoader.VerifyDependencies();

		// load hooks
		HookLoader hookLoader;
		SML::info("Registering hooks");
		hookLoader.RegisterHooks();

		SML::info("Setting up mods");
		modLoader.SetupMods();

		// test commands
		CommandSystem system;
		system.RegisterCommand("heal", HealPerson);

		CommandParser::CommandData data = CommandParser::Parse("!heal 75");
		SML::info("Command: ", data.Command);
		for (auto s : data.Args) {
			SML::info("Arg: ", s);
		}

		Command cmd = system.get_command(data.Command);
		cmd.Invoke(nullptr, data.get_float(0));
	}
}