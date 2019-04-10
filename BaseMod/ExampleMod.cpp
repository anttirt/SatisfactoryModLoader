#include <stdafx.h>
#include <utility/Configuration.h>
#include <other/CommandSystem.h>
#include <events/PlayerEvents.h>
#include <other/CommandSystem.h>
#include <other/CommandParser.h>
#include "ExampleMod.h"
#include "ModReturns.h"
#include "Internals.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef GetMessage

// if you want to access the Global values
ExampleMod* mod;
SML::CommandSystem commandSystem;
SML::Configuration config("ExampleMod");

SML::PlayerCharacter* tmpPlayer;
void* tmpPlayerController;

void GetMessage(SML::ModReturns* returns, SML::AFGPlayerController* player, SML::FString* message_) {
	std::string message = message_->get_string();
	SML::mod_info(mod->Name(), "Message recieved: ", message);

	SML::CommandParser::CommandData data = SML::CommandParser::Parse(message);
	SML::Command command = commandSystem.get_command(data.Command);
	if (!command.Empty) {
		SML::mod_info(mod->Name(), "Invoking Command: ", data.Command);
		command.Invoke(player, data);
		// cancel the original function
		returns->UseOriginalFunction = false;
	}
	else {
		SML::mod_info(mod->Name(), "Invalid Command: ", data.Command);
	}
}

void GetPlayer(SML::ModReturns* returns, SML::AFGCharacterPlayer* player) {
	SML::mod_info(mod->Name(), "Set Player ", player);
	tmpPlayer = new SML::PlayerCharacter(player);
}

void GetController(SML::ModReturns* returns, SML::AFGPlayerController* controller) {
	SML::mod_info(mod->Name(), "Set Controller ", controller);
	tmpPlayerController = controller;
}

bool GetInput(SML::ModReturns* returns, SML::UPlayerInput* input, SML::FKey key, SML::EInputEvent event, float amountDepressed, bool gamepad) {
	if (GetAsyncKeyState('K')) {
		SML::mod_info(mod->Name(), "K pressed");
	}
	return false;
}

/// commands

void HealPlayer(SML::AFGPlayerController* player, SML::CommandParser::CommandData data) {
	if (data.Args.capacity() < 1) {
		return;
	}

	float arg0 = data.get_float(0);
	*tmpPlayer->healthComponent->currentHealth += arg0;

	float health = *tmpPlayer->healthComponent->currentHealth;

	std::string str(
		"Set the character's health to " + std::to_string((int)health) + " [" + std::to_string((int)arg0) + "]");
	SML::FString msg{ str.c_str() };
	SML::Call<&SML::AFGPlayerController::EnterChatMessage>(player, &msg);
}

void CheckHealth(SML::AFGPlayerController* player, SML::CommandParser::CommandData data) {
	float health = *tmpPlayer->healthComponent->currentHealth;

	std::string str("The character's health is " + std::to_string((int)health));
	SML::FString msg{ str.c_str() };
	SML::Call<&SML::AFGPlayerController::EnterChatMessage>(player, &msg);
}

void SetItem(SML::AFGPlayerController* player, SML::CommandParser::CommandData data) {
	int slot = data.get_int(0);
	int amount = data.get_int(1);

	SML::ItemStack stack = tmpPlayer->inventory->get_item(slot);
	*stack.amount = amount;
	SML::Item item = stack.item;
	SML::ItemDescriptor* descriptor = item.descriptor;
	SML::mod_info(mod->Name(), "Item: ", item.pointer);
	SML::mod_info(mod->Name(), "Amount: ", stack.amount);
}

void ExampleMod::Setup() {
	mod = this;

	SML::mod_info(mod->Name(), "ExampleMod setup");

	if (!config.exists()) {
		config.set("SampleInt", 5);
		config.save();
	}

	config.load();

	SML::Subscribe<&SML::AFGCharacterPlayer::BeginPlay>(GetPlayer);
	SML::Subscribe<&SML::AFGPlayerController::BeginPlay>(GetController);
	SML::Subscribe<&SML::AFGPlayerController::EnterChatMessage>(GetMessage);
	SML::Subscribe<&SML::UPlayerInput::InputKey>(GetInput);

	commandSystem.RegisterCommand("heal", HealPlayer);
	commandSystem.RegisterCommand("checkHealth", CheckHealth);
	commandSystem.RegisterCommand("setItem", SetItem);
}

void ExampleMod::Cleanup() {

}