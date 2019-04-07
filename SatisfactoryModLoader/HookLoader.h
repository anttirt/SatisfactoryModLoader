#pragma once

#include <string>
#include <map>
#include <vector>

namespace SML {
	class HookLoader {
	public:
		enum Event {
			AFGCharacterPlayerBeginPlay,
			UFGItemDescriptorUFGItemDescriptor,
			END
		};

		void RegisterHooks();
		void HookEvent(Event event, const char* eventCall, PVOID hook);
	private:
		const char* _gameModule = "FactoryGame-Win64-Shipping.exe";
		const long long _modLoaderModule = 0x180000000;
	};
}