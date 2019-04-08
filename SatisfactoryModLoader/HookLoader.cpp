#include <stdafx.h>
#include <detours.h>
#include <iostream>
#include <events/PlayerEvents.h>
#include "Globals.h"
#include "HookLoader.h"
#include "ModLoader.h"

namespace SML {
	// get original functions and hook them
	void HookLoader::RegisterHooks() {
		PlayerEvents playerEvents;
		playerEvents.Setup(*this);
	}

	// hook event from keypair at index
	void HookLoader::HookEvent(Event event, const char* eventCall, PVOID hook) {
		if ((eventCall != NULL) && (eventCall[0] == '\0')) {
			globals.functions.insert(std::pair<Event, PVOID>(event, hook));
			return;
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		auto function = DetourFindFunction(_gameModule, eventCall);
		if (!function) {
			return;
		}

		DetourAttach(&(PVOID&)function, hook);
		DetourTransactionCommit();

		std::cout << "Hooked " << eventCall << std::endl;

		globals.functions.insert(std::pair<Event, PVOID>(event, function));
	}
}