#include <stdafx.h>
#include <utility/Configuration.h>
#include <events/PlayerEvents.h>
#include <external/FString.h>
#include "MyMod.h"
#include "ModReturns.h"

using event = SML::HookLoader::Event;

// if you want to access the Global values
MyMod* myMod;

void GetMessage(SML::ModReturns* returns, void* player, std::string message) {
	std::cout << message << std::endl;
	// string -> const char* -> FString
	FString msg = "TestMessage";
	// get pointer
	auto pointer = myMod->GlobalsReference->functions[event::AFGPlayerControllerEnterChatMessageGlobal];
	((void(__stdcall*)(void*, FString*))pointer)(player, &msg);
	// cancel the original function
	returns->UseOriginalFunction = false;
}

void MyMod::Setup() {
	myMod = this;
	_dispatcher.subscribe(SML::HookLoader::Event::AFGPlayerControllerEnterChatMessage, GetMessage);
}