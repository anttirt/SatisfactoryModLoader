#pragma once

#include "HookLoader.h"

#pragma warning (push)
#pragma warning (disable: 4667)

namespace SML {
	// the functions in this header are never defined; they only define the hookable API

	struct FString;

	class UObject {

	};

	class AFGCharacterBase : public UObject {
	public:
		void BeginPlay();
		void GetHealthComponent(struct FFrame*, void* const);
	};

	DEFINE_METHOD(AFGCharacterBase::BeginPlay);

	// manually override name
	template <>
	struct HookName<&AFGCharacterBase::GetHealthComponent> {
		static constexpr const char Name[] = "AFGCharacterBase::execGetHealthComponent";
	};

	EXPORT_METHOD(AFGCharacterBase::GetHealthComponent);

	class AFGCharacterPlayer : public AFGCharacterBase
	{
	public:
		void BeginPlay();
	};

	DEFINE_METHOD(AFGCharacterPlayer::BeginPlay);

	class AFGPlayerController {
	public:
		void BeginPlay();
		void EnterChatMessage(FString *inMessage);
	};

	DEFINE_METHOD(AFGPlayerController::BeginPlay);
	DEFINE_METHOD(AFGPlayerController::EnterChatMessage);

	class UFGHealthComponent {
	public:
		void TakeDamage(class AActor* damagedActor, float damageAmount, class UDamageType *damageType, class AController *instigatedBy, class AActor *damageCauser);
	};

	DEFINE_METHOD(UFGHealthComponent::TakeDamage);

	struct FKey { void* data; };
	struct EInputEvent { void* data; };

	class UPlayerInput {
	public:
		bool InputKey(FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad);
	};

	DEFINE_METHOD(UPlayerInput::InputKey);
}

#pragma warning (pop)
