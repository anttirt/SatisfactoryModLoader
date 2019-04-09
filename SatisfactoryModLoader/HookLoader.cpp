#include <stdafx.h>
#include <detours.h>
#include <iostream>
#include <events/PlayerEvents.h>
#include <events/UtilityEvents.h>
#include <utility/Logger.h>
#include <string>
#include "Globals.h"
#include "HookLoader.h"
#include "ModLoader.h"

namespace SML {
	// get original functions and hook them
	void HookLoader::RegisterHooks() {
		PlayerEvents playerEvents;
		playerEvents.Setup(this);

		UtilityEvents utilityEvents;
		utilityEvents.Setup(this);
	}

	void HookLoader::CheckVersion(std::string str) {
		std::string version = str.substr(str.length() - 5, str.length());
		if (version == globals.targetVersion) {
			SML::info("Version check passed!");
		}
		else {
			SML::error("FATAL: Version check failed");
			if (!globals.supressConsoleErrors) {
				std::string msg("The version of SML (");
				msg += globals.targetVersion;
				msg += ") that you are using is not compatible with your version of Satisfactory (";
				msg += version;
				msg += ")!\nIf SML is not available for the latest version of Satisfactory, please yell at SuperCoder or Nomnom to compile one.\nPress Ok to continue at your own discresion or cancel to stop the program.";
				int ret = MessageBoxA(NULL, msg.c_str(), "SatisfactoryModLoader Fatal Error", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONERROR);
				if (ret == IDCANCEL) {
					exit(1);
				}
			}
			else {
				SML::warning("SupressErrors set to true, continuing...");
			}
		}
	}

	// hook event from keypair at index
	void HookLoader::HookEvent(Event event, const char* eventCall, PVOID hook) {
		if ((eventCall != NULL) && (eventCall[0] == '\0')) {
			SML::info("Inserted Event::", event);
			globals.functions.insert(std::pair<Event, PVOID>(event, hook));
			return;
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		auto function = DetourFindFunction(_gameModule, eventCall);
		if (!function) {
			SML::error("Cannot find ", eventCall);
			return;
		}

		DetourAttach(&(PVOID&)function, hook);
		DetourTransactionCommit();

		SML::info("Hooked ", eventCall);

		globals.functions.insert(std::pair<Event, PVOID>(event, function));
	}
}