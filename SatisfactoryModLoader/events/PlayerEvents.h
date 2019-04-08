#pragma once

#include <Windows.h>

class FString;
class Globals;

namespace SML {
	class HookLoader;

	class PlayerEvents {
	public:
		// ; void __fastcall AFGPlayerController::BeginPlay(AFGPlayerController *this)
		static void AFGPlayerControllerBeginPlayHook(void* controller);

		// ; void __fastcall AFGCharacterPlayer::BeginPlay(AFGCharacterPlayer *this)
		static void AFGCharacterPlayerBeginPlayHook(void* player);

		// ; void __fastcall AFGPlayerController::EnterChatMessage(AFGPlayerController *this, FString *inMessage)
		static void AFGPlayerControllerEnterChatMessageHook(void* player, FString* message);

		// ; void __fastcall AFGPlayerController::EnterChatMessage(AFGPlayerController *this, FString *inMessage)
		static void AFGPlayerControllerEnterChatMessageGlobal(void* player, std::string message);

		// ; void __fastcall AFGCharacterBase::BeginPlay(AFGCharacterBase *this)
		static void AFGCharacterBaseBeginPlayHook(void* characterBase);

		// ; void __fastcall AFGCharacterBase::execGetHealthComponent(struct UObject *, struct FFrame *, void *const )
		static void AFGCharacterBaseGetHealthComponentHook(void* uObject, void* fFrame);

		// ; void __fastcall UFGHealthComponent::TakeDamage(UFGHealthComponent *this, AActor *damagedActor, float damageAmount, UDamageType *damageType, AController *instigatedBy, AActor *damageCauser)
		static void UFGHealthComponentTakeDamageHook(void* healthComponent, void* damagedActor, float damageAmount, void* damageType, void* instigatedBy, void* damageCauser);

		// ; bool __fastcall UPlayerInput::InputKey(UPlayerInput *this, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
		static bool UPlayerInputInputKeyHook(void* input, void* key, void* event, float amountDepressed, bool gamepad);

		void Setup(HookLoader* hookLoader);
	};
}