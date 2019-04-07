#pragma once

#include <vector>
#include <string>

namespace SML {
	class Mod {
	public:
		std::string Name;
		std::string Version;
		std::string Description;
		std::string Authors;
		std::vector<std::string> Dependencies;
		std::string FileName;
	};
}