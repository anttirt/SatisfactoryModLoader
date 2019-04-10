#include <stdafx.h>

#include "HookLoader.h"
#include "events/PlayerEvents.h"
#include "utility/Logger.h"
#include "Globals.h"

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace SML {
	void CheckVersion(std::string str) {
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
}