#pragma once

#include <utility/Connection.h>
#include <utility/Dispatcher.h>
#include "HookLoader.h"
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

		virtual void PreSetup(Globals* globals) {}

		template<typename ...Args>
		void Run(HookLoader::Event event, Args ...args) {
			_dispatcher.post(event, (args)...);
		}

	protected:
		SML::Globals* _globals;
		SML::Dispatcher _dispatcher;

		virtual void Setup() {}
	};
}