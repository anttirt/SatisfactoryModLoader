#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iterator>

namespace SML {
	namespace CommandParser {
		struct CommandData {
			std::string Command;
			std::vector<std::string> Args;

			std::string get_string(int arg) {
				return Args[arg];
			}

			int get_int(int arg) {
				return std::stoi(Args[arg]);
			}

			float get_float(int arg) {
				return std::stof(Args[arg]);
			}
		};

		static CommandData Parse(std::string message) {
			std::istringstream iss(message);
			std::vector<std::string> results(
				std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>());

			std::string command = results[0];
			std::string commandSplit = command.substr(1, command.length() - 1);
			results.erase(results.begin());
			return CommandData{
				commandSplit,
				results
			};
		}
	}
}