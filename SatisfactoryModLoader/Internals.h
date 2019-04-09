#pragma once

#include <string>

namespace SML {
	template <class T>
	T * OffsetPointer(T * ptr, size_t offset) {
		return (T *)(*(size_t*)(reinterpret_cast<unsigned char*>(ptr) + offset));
	}

	template <class T>
	T * Offset(T * ptr, size_t offset) {
		return (T *)(reinterpret_cast<unsigned char*>(ptr) + offset);
	}

	struct FString {
		int16_t* data;
		int32_t length;

		FString();
		FString(const char* message);

		std::string get_string();
	private:
		void set_message(const char* message);
	};

	class PlayerCharacter {
	public:
		void* pointer;

		class HealthComponent* healthComponent;
		class InventoryComponent* inventory;

		float* deathTimestamp;
		float* maxDPSFromRadiation;
		float* healthGenerationThreshold;
		float* healthGenerationAmount;
		float* healthGenerationInterval;
		float* healthGenerationWaitTime;
		float* healthGenerationTimer;
		float* lastDamageTakenTime;

		PlayerCharacter(void* pointer);
	};

	class HealthComponent {
	public:
		void* pointer;

		float* maxHealth;
		float* currentHealth;
		float* respawnHealthFactor;

		HealthComponent(void* pointer);
	};

	class InventoryComponent {
	public:
		void* pointer;

		int* defaultSize;
		int* adjustedSize;

		void* itemStacks;

		InventoryComponent(void* pointer);

		class ItemStack get_item(int index);
	};

	class ItemStack {
	public:
		void* pointer;

		class Item* item;

		int* amount;

		ItemStack(void* pointer);
	};

	class Item { // TODO
	public:
		void* pointer;

		class ItemDescriptor* descriptor;

		Item(void* pointer);
	};

	enum EStackSize
	{
		SS_ONE = 0,
		SS_SMALL = 1,
		SS_MEDIUM = 2,
		SS_BIG = 3,
		SS_HUGE = 4,
		SS_LAST_ENUM = 5,
	};

	enum EResourceForm
	{
		RF_INVALID = 0,
		RF_SOLID = 1,
		RF_LIQUID = 2,
		RF_GAS = 3,
		RF_HEAT = 4,
		RF_LAST_ENUM = 5,
	};

	class ItemDescriptor {
	public:
		void* pointer;

		bool* useDisplayNameAndDescription;

		void* displayName;
		void* description;

		EStackSize* stackSize;

		float* energyValue;
		float* radioactiveDecay;

		EResourceForm* form;

		ItemDescriptor(void* pointer);
	};
}