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
			// ; void __fastcall AFGPlayerController::EnterChatMessage(AFGPlayerController *this, FString *inMessage)
			AFGPlayerControllerEnterChatMessageGlobal,

			/// AFGCharacterBase

			// ; void __fastcall AFGCharacterBase::BeginPlay(AFGCharacterBase *this)
			AFGCharacterBaseBeginPlay,
			// ; void __fastcall AFGCharacterBase::execGetHealthComponent(struct UObject *, struct FFrame *, void *const )
			AFGCharacterBaseGetHealthComponent,

			/// UFGHealthComponent

			// ; void __fastcall UFGHealthComponent::TakeDamage(UFGHealthComponent *this, AActor *damagedActor, float damageAmount, UDamageType *damageType, AController *instigatedBy, AActor *damageCauser)
			UFGHealthComponentTakeDamage,

			/// UPlayerInput

			// ; bool __fastcall UPlayerInput::InputKey(UPlayerInput *this, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
			UPlayerInputInputKey,

			/// FText

			// ; FString *__fastcall FText::ToString(FText *this)
			FTextToString,
			// ; FString *__fastcall FText::ToString(FText *this)
			FTextToStringGlobal,

			/// BuildSettings

			// ; const wchar_t *__fastcall BuildSettings::GetBuildVersion()
			BuildSettingsGetBuildVersion,
			// ; const wchar_t *__fastcall BuildSettings::GetBuildVersion()
			BuildSettingsGetBuildVersionGlobal,
			END
		};

		void RegisterHooks();
		void CheckVersion(std::string str);
		void HookEvent(Event event, const char* eventCall, PVOID hook);
	private:
		const char* _gameModule = "FactoryGame-Win64-Shipping.exe";
		const long long _modLoaderModule = 0x180000000;
	};
}