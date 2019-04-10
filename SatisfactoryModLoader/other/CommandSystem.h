#pragma once

#include <string>
#include <map>
#include <other/CommandParser.h>
#include "Internals.h"

namespace SML {
	typedef void __stdcall ActionFunc(class AFGPlayerController* player, CommandParser::CommandData data);

	struct Command {
		bool Empty;
		std::string Name;
		ActionFunc* Action;

		void Invoke(class AFGPlayerController* player, SML::CommandParser::CommandData command) {
			Action(player, command);
		}
	};

	class CommandSystem {
	public:
		void RegisterCommand(std::string name, ActionFunc* action) {
			_commands.insert(std::pair<std::string, Command>(name, Command{ false, name, action }));
		}

		Command get_command(std::string name) {
			if (_commands.find(name) == _commands.end()) {
				return Command{ true };
			}

			return _commands[name];
		}

	private:
		std::map<std::string, Command> _commands;
	};
}