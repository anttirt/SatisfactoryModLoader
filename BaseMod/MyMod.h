#pragma once

#include <other/BaseMod.h>
#include <utility/Connection.h>
#include <utility/Dispatcher.h>

class MyMod : SML::BaseMod {
	const char* Name() { return "Base Mod"; };
	const char* Version() { return "0.1"; };
	const char* Description() { return "A basic mod created to showcase SML's functionality."; };
	const char* Authors() { return "Nomnom, SuperCoder79"; };

	void PreSetup(SML::Globals* globals) {
		std::cout << "Setting up " << Name() << std::endl;
		_globals = globals;

		Setup();
	}

protected:
	void Setup();
};

GLOBAL MyMod* CreateMod() {
	return new MyMod;
}