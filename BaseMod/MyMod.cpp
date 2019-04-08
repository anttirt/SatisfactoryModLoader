#include <stdafx.h>
#include <utility/Configuration.h>
#include <events/PlayerEvents.h>
#include <external/FString.h>
#include <other/CommandSystem.h>
#include <other/CommandParser.h>
#include "MyMod.h"
#include "ModReturns.h"
#include "Internals.h"

using event = SML::HookLoader::Event;

// if you want to access the Global values
MyMod* myMod;
SML::CommandSystem commandSystem;

SML::PlayerCharacter* tmpPlayer;
void* tmpPlayerController;

void GetMessage(SML::ModReturns* returns, void* player, std::string message) {
	SML::mod_info(myMod->Name(), "Message recieved: ", message);

	SML::mod_info(myMod->Name(), tmpPlayer->pointer);

	SML::CommandParser::CommandData data = SML::CommandParser::Parse(message);
	SML::Command command = commandSystem.get_command(data.Command);
	if (!command.Empty) {
		command.Invoke(player, data);
		// cancel the original function
		returns->UseOriginalFunction = false;
	}
}

void GetPlayer(SML::ModReturns* returns, void* player) {
	SML::mod_info(myMod->Name(), "Set Player ", player);
	tmpPlayer = new SML::PlayerCharacter(player);
}

void GetController(SML::ModReturns* returns, void* controller) {
	SML::mod_info(myMod->Name(), "Set Controller ", controller);
	tmpPlayerController = controller;
}

void HealPlayer(void* player, SML::CommandParser::CommandData data) {
	if (data.Args.capacity() < 1) {
		return;
	}

	float arg0 = data.get_float(0);
	*tmpPlayer->healthComponent->currentHealth += arg0;

	float health = *tmpPlayer->healthComponent->currentHealth;

	std::string str(
		"Set the character's health to " + std::to_string((int)health) + " [" + std::to_string((int)arg0) + "]");
	auto pointer = myMod->GlobalsReference->functions[event::AFGPlayerControllerEnterChatMessageGlobal];
	((void(__stdcall*)(void*, std::string))pointer)(player, str);
}

void CheckHealth(void* player, SML::CommandParser::CommandData data) {
	// 1C164E0F100
	SML::mod_info(myMod->Name(), "Player ", player);
	intptr_t address = (intptr_t)player;
	intptr_t offset = 0x15C;
	long long newAddress = address + offset;
	void* p = (void*)newAddress;
	SML::mod_info(myMod->Name(), "Health: ", p, " Value (float): ", *(float*)p, " Value (int): ", *(int*)p);
}

void MyMod::Setup() {
	myMod = this;
	_dispatcher.subscribe(SML::HookLoader::Event::AFGCharacterPlayerBeginPlay, GetPlayer);
	_dispatcher.subscribe(SML::HookLoader::Event::AFGPlayerControllerBeginPlay, GetController);
	_dispatcher.subscribe(SML::HookLoader::Event::AFGPlayerControllerEnterChatMessage, GetMessage);

	commandSystem.RegisterCommand("heal", HealPlayer);
	commandSystem.RegisterCommand("checkHealth", CheckHealth);
}