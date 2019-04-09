#pragma once

#include "ModReturns.h"
#include "HookLoader.h"
#include "Globals.h"
#include "Internals.h"

using event = SML::HookLoader::Event;

namespace SML {
	class BaseEvent {
	public:
		virtual void Setup(HookLoader* hookLoader) {}

	protected:
		template<typename ...Args>
		static ModReturns Run(HookLoader::Event event, Args ...args) {
			ModReturns returns;
			returns.UseOriginalFunction = true;
			for (BaseMod* mod : globals.mods) {
				mod->Run(&returns, event, (args)...);
			}
			return returns;
		}
	};
}