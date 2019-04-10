#pragma once

#include <string>
#include <map>
#include <other/CommandParser.h>
#include "Internals.h"

namespace SML {
	struct Command {
		bool Empty;
		std::string Name;
		void* Action;

		void Invoke(void* player, SML::CommandParser::CommandData command) {
			auto pointer = (void(__stdcall*)(void*, SML::CommandParser::CommandData))Action;
			pointer(player, command);
		}
	};

	class CommandSystem {
	public:
		void RegisterCommand(std::string name, void* action) {
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