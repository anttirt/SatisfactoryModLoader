#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include <other/CommandParser.h>

namespace SML {
	struct Command {
		std::string Name;
		PVOID Action;

		template<typename ...Args>
		void Invoke(void* player, Args ...args) {
			auto pointer = (void(WINAPI*)(void*, Args...))Action;
			pointer(player, args...);
		}
	};

	class CommandSystem {
	public:
		void RegisterCommand(std::string name, PVOID action);
		Command get_command(std::string name);

	private:
		std::map<std::string, Command> _commands;
	};
}