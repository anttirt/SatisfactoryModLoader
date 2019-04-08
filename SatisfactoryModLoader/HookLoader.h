#pragma once

#include <string>
#include <map>
#include <vector>

namespace SML {
	class ModLoader;

	class HookLoader {
	public:
		enum Event {
			/// AFGCharacterPlayer

			// ; void __fastcall AFGCharacterPlayer::BeginPlay(AFGCharacterPlayer *this)
			AFGCharacterPlayerBeginPlay,

			/// AFGPlayerController

			// ; void __fastcall AFGPlayerController::BeginPlay(AFGPlayerController *this)
			AFGPlayerControllerBeginPlay,
			// ; void __fastcall AFGPlayerController::EnterChatMessage(AFGPlayerController *this, FString *inMessage)
			AFGPlayerControllerEnterChatMessage,
			// ; void __fastcall AFGPlayerController::EnterChatMessage(AFGPlayerController *this, FString *inMessage) - GLOBAL
			AFGPlayerControllerEnterChatMessageGlobal,

			/// UFGHealthComponent

			// ; void __fastcall UFGHealthComponent::TakeDamage(UFGHealthComponent *this, AActor *damagedActor, float damageAmount, UDamageType *damageType, AController *instigatedBy, AActor *damageCauser)
			UFGHealthComponentTakeDamage,

			/// UPlayerInput

			// ; bool __fastcall UPlayerInput::InputKey(UPlayerInput *this, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
			UPlayerInputInputKey,
			END
		};

		void RegisterHooks();
		void HookEvent(Event event, const char* eventCall, PVOID hook);
	private:
		const char* _gameModule = "FactoryGame-Win64-Shipping.exe";
		const long long _modLoaderModule = 0x180000000;
	};
}