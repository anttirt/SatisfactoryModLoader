#include <stdafx.h>
#include <other/BaseMod.h>
#include <events/PlayerEvents.h>
#include <utility/Logger.h>

namespace SML {
	void PlayerEvents::AFGCharacterPlayerBeginPlayHook(void* player) {
		auto returns = Run(event::AFGCharacterPlayerBeginPlay, player);
		if (!returns.UseOriginalFunction) {
			return;
		}
		((void(WINAPI*)(void*))globals.functions[event::AFGCharacterPlayerBeginPlay])(player);
	}

	void PlayerEvents::AFGPlayerControllerBeginPlayHook(void* controller) {
		auto returns = Run(event::AFGPlayerControllerBeginPlay, controller);
		if (!returns.UseOriginalFunction) {
			return;
		}
		((void(WINAPI*)(void*))globals.functions[event::AFGPlayerControllerBeginPlay])(controller);
	}

	void PlayerEvents::AFGPlayerControllerEnterChatMessageHook(void* player, FString* message) {
		auto pointer = (void(WINAPI*)(void*, FString*))globals.functions[event::AFGPlayerControllerEnterChatMessage];

		char* chars = new char[message->length];

		for (size_t i = 0; i < message->length; i++) {
			chars[i] = message->data[i];
		}

		std::string str(chars);

		auto returns = Run(event::AFGPlayerControllerEnterChatMessage, player, str);
		if (!returns.UseOriginalFunction) {
			return;
		}

		pointer(player, message);
	}

	void PlayerEvents::AFGPlayerControllerEnterChatMessageGlobal(void* player, std::string message) {
		FString msg = message.c_str();
		auto pointer = (void(WINAPI*)(void*, FString*))globals.functions[event::AFGPlayerControllerEnterChatMessage];
		pointer(player, &msg);
	}

	void PlayerEvents::AFGCharacterBaseBeginPlayHook(void* characterBase) {
		auto returns = Run(event::AFGCharacterBaseBeginPlay, characterBase);
		if (!returns.UseOriginalFunction) {
			return;
		}
		auto pointer = (void(WINAPI*)(void*))globals.functions[event::AFGCharacterBaseBeginPlay];
		pointer(characterBase);
	}

	void PlayerEvents::AFGCharacterBaseGetHealthComponentHook(void* uObject, void* fFrame) {
		auto returns = Run(event::AFGCharacterBaseGetHealthComponent, uObject, fFrame);
		if (!returns.UseOriginalFunction) {
			return;
		}
		auto pointer = (void(WINAPI*)(void*, void*))globals.functions[event::AFGCharacterBaseGetHealthComponent];
		pointer(uObject, fFrame);
	}

	void PlayerEvents::UFGHealthComponentTakeDamageHook(void* healthComponent, void* damagedActor, float damageAmount, void* damageType, void* instigatedBy, void* damageCauser) {
		auto returns = Run(event::UFGHealthComponentTakeDamage, healthComponent, damagedActor, damageAmount, damageType, instigatedBy, damageCauser);
		if (!returns.UseOriginalFunction) {
			return;
		}
		auto pointer = (void(WINAPI*)(void*, void*, float, void*, void*, void*))globals.functions[event::UFGHealthComponentTakeDamage];
		pointer(healthComponent, damagedActor, damageAmount, damageType, instigatedBy, damageCauser);
	}

	bool PlayerEvents::UPlayerInputInputKeyHook(void* input, void* key, void* event, float amountDepressed, bool gamepad) {
		auto returns = Run(event::UPlayerInputInputKey, input, key, event, amountDepressed, gamepad);
		if (!returns.UseOriginalFunction) {
			return *(bool*)returns.ReturnValue;
		}
		auto pointer = (bool(WINAPI*)(void*, void*, void*, float, bool))globals.functions[event::UPlayerInputInputKey];
		return pointer(input, key, event, amountDepressed, gamepad);
	}

	void PlayerEvents::Setup(HookLoader* hookLoader) {
		SML::info("PlayerEvents::Setup");

		// AFGCharacterPlayer
		hookLoader->HookEvent(event::AFGCharacterPlayerBeginPlay, "AFGCharacterPlayer::BeginPlay", this->AFGCharacterPlayerBeginPlayHook);

		// AFGPlayerController
		hookLoader->HookEvent(event::AFGPlayerControllerBeginPlay, "AFGPlayerController::BeginPlay", this->AFGPlayerControllerBeginPlayHook);
		hookLoader->HookEvent(event::AFGPlayerControllerEnterChatMessage, "AFGPlayerController::EnterChatMessage", this->AFGPlayerControllerEnterChatMessageHook);
		hookLoader->HookEvent(event::AFGPlayerControllerEnterChatMessageGlobal, "", this->AFGPlayerControllerEnterChatMessageGlobal);

		// AFGCharacterBase
		hookLoader->HookEvent(event::AFGCharacterBaseBeginPlay, "AFGCharacterBase::BeginPlay", this->AFGCharacterBaseBeginPlayHook);
		//hookLoader->HookEvent(event::AFGCharacterBaseBeginPlay, "AFGCharacterBase::GetHealthComponent", this->AFGCharacterBaseGetHealthComponentHook);

		// UFGHealthComponent
		hookLoader->HookEvent(event::UFGHealthComponentTakeDamage, "UFGHealthComponent::TakeDamage", this->UFGHealthComponentTakeDamageHook);

		// UPlayerInput
		hookLoader->HookEvent(event::UPlayerInputInputKey, "UPlayerInput::InputKey", this->UPlayerInputInputKeyHook);
	}
}