#pragma once

#include <iostream>
#include <fstream>
#include "external/json.hpp"

using json = nlohmann::json;

namespace SML {
	class Configuration {
	public:
		Configuration(const char* name);
		~Configuration();

		template<typename T>
		void set(const char* name, T value) {
			_data[name] = value;
		}

		template<typename T>
		T get(const char* name, T defaultValue) {
			if (_data.contains(name)) {
				return _data[name];
			}
			else {
				return defaultValue;
			}
		}

		void save();
		void load();
		bool exists();

	private:
		std::string _defaultPath = "config\\";
		std::string _name;
		std::ifstream _fileInput;
		std::ofstream _fileOutput;
		std::string _contents;
		json _data;
	};
}