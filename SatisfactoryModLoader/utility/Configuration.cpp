#include <stdafx.h>
#include <string>
#include "utility/Configuration.h"

namespace SML {
	Configuration::Configuration(const char* name) {
		_name = std::string(_defaultPath);
		_name.append(name);
		_name.append(".cfg");
	}

	Configuration::~Configuration() {
		_fileInput.close();
		_fileOutput.close();
	}

	void Configuration::save() {
		_fileOutput.open(_name);

		try {
			if (_fileOutput.is_open()) {
				_fileOutput << _data.dump(4);
			}
		}
		catch (...) {}

		_fileOutput.close();
	}

	void Configuration::load() {
		_fileInput.open(_name);
		_contents = "";

		std::string line;
		try {
			while (getline(_fileInput, line)) {
				_contents += line;
			}

			_data = json::parse(_contents);
		}
		catch (...) {}

		_fileInput.close();
	}

	bool Configuration::exists() {
		std::ifstream config(_name);
		bool good = config.good();
		config.close();
		return good;
	}
}