#pragma once

#include <utility/Connection.h>
#include <utility/Dispatcher.h>
#include "HookLoader.h"
#include "ModReturns.h"
#include "Globals.h"

#define GLOBAL extern "C" __declspec(dllexport)

namespace SML {
	class BaseMod {
	public:
		const char* FileName;

		virtual const char* Name() { return ""; };
		virtual const char* Version() { return ""; };
		virtual const char* Description() { return ""; };
		virtual const char* Authors() { return ""; };
		virtual const std::vector<const char*> Dependencies() { return std::vector<const char*>{}; }

		virtual void PreSetup(Globals* globals) {}

		template<typename ...Args>
		void Run(ModReturns* returns, HookLoader::Event event, Args ...args) {
			return _dispatcher.post(returns, event, (args)...);
		}

	protected:
		SML::Dispatcher _dispatcher;

		virtual void Setup() {}
	};
}