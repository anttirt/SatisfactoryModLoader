#include <stdafx.h>
#include <other/CommandSystem.h>

namespace SML {
	void CommandSystem::RegisterCommand(std::string name, PVOID action) {
		_commands.insert(std::pair<std::string, Command>(name, Command{ name, action }));
	}

	Command CommandSystem::get_command(std::string name) {
		if (_commands.find(name) == _commands.end()) {
			return Command{};
		}

		return _commands[name];
	}
}