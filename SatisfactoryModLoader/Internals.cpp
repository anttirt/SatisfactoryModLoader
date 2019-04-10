#include <stdafx.h>
#include "Internals.h"

#include <cstdint>

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace SML {
	FString::FString() {}

	FString::FString(const char* message) {
		this->set_message(message);
	}

	void FString::set_message(const char* message) {
		length = strlen(message) + 1;
		int16_t *chars = new int16_t[length + 1];
		for (size_t i = 0; i < length; i++) {
			chars[i] = message[i];
		}
		chars[length] = '\0';
		data = chars;
	}

	std::string FString::get_string() {
		char* chars = new char[this->length];

		for (size_t i = 0; i < this->length; i++) {
			chars[i] = this->data[i];
		}

		return std::string(chars);
	}

	PlayerCharacter::PlayerCharacter(void* pointer) {
		this->pointer = pointer;

		this->healthComponent = new HealthComponent(OffsetPointer(pointer, 0x07a0));
		this->inventory = new InventoryComponent(OffsetPointer(pointer, 0xa48));

		this->deathTimestamp = (float*)Offset(pointer, 0x07d4);
		this->maxDPSFromRadiation = (float*)Offset(pointer, 0x0a78);
		this->healthGenerationThreshold = (float*)Offset(pointer, 0x0ac0);
		this->healthGenerationAmount = (float*)Offset(pointer, 0x0ac4);
		this->healthGenerationInterval = (float*)Offset(pointer, 0x0ac8);
		this->healthGenerationWaitTime = (float*)Offset(pointer, 0x0acc);
		this->healthGenerationTimer = (float*)Offset(pointer, 0x0ad0);
		this->lastDamageTakenTime = (float*)Offset(pointer, 0x0ad4);
	}

	HealthComponent::HealthComponent(void* pointer) {
		this->pointer = pointer;

		this->maxHealth = (float*)Offset(pointer, 0x0158);
		this->currentHealth = (float*)Offset(pointer, 0x015c);
		this->respawnHealthFactor = (float*)Offset(pointer, 0x0160);
	}

	InventoryComponent::InventoryComponent(void* pointer) {
		this->pointer = pointer;

		this->defaultSize = (int*)Offset(pointer, 0x0148);
		this->adjustedSize = (int*)Offset(pointer, 0x013c);

		this->itemStacks = OffsetPointer(pointer, 0x0148);
	}

	ItemStack InventoryComponent::get_item(int index) {
		/*if (index < 0)
			throw new std::exception("index < 0");

		if (index > (*this->defaultSize + *this->adjustedSize - 1))
			throw new std::exception("index > invSize");*/

		ItemStack itemStack(Offset(this->itemStacks, 0x20 * index));

		return itemStack;
	}

	ItemStack::ItemStack(void* pointer) {
		this->pointer = pointer;

		this->item = new Item(OffsetPointer(pointer, 0x0));
		this->amount = (int*)Offset(pointer, 0x18);
	}

	Item::Item(void* pointer) {
		this->pointer = pointer;

		this->descriptor = new ItemDescriptor(OffsetPointer(pointer, 0x0));
	}

	ItemDescriptor::ItemDescriptor(void* pointer) {
		this->pointer = pointer;

		this->useDisplayNameAndDescription = (bool*)Offset(pointer, 0x0028);

		this->displayName = Offset(pointer, 0x0030);
		this->description = Offset(pointer, 0x0048);

		this->stackSize = (EStackSize*)Offset(pointer, 0x0060);

		this->energyValue = (float*)Offset(pointer, 0x0064);
		this->radioactiveDecay = (float*)Offset(pointer, 0x0068);

		this->form = (EResourceForm*)Offset(pointer, 0x006c);
	}
}