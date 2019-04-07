#pragma once

#include <Windows.h>
#include "Globals.h"

namespace SML {
	namespace PlayerEvents {
		// ; void __fastcall AFGCharacterPlayer::BeginPlay(AFGCharacterPlayer *this)
		void AFGCharacterPlayerBeginPlayHook(void* player) {
			((void(WINAPI*)(void*))globals.functions[HookLoader::Event::AFGCharacterPlayerBeginPlay])(player);
		}

		// ; void __fastcall UFGItemDescriptor::UFGItemDescriptor(UFGItemDescriptor *this)
		void UFGItemDescriptorUFGItemDescriptorHook(void* item) {
			((void(WINAPI*)(void*))globals.functions[HookLoader::Event::UFGItemDescriptorUFGItemDescriptor])(item);
		}

		void Setup(HookLoader& hookLoader) {
			hookLoader.HookEvent(HookLoader::Event::AFGCharacterPlayerBeginPlay, "AFGCharacterPlayer::BeginPlay", AFGCharacterPlayerBeginPlayHook);
			hookLoader.HookEvent(HookLoader::Event::UFGItemDescriptorUFGItemDescriptor, "UFGItemDescriptor::UFGItemDescriptor", UFGItemDescriptorUFGItemDescriptorHook);
		}
	}
}