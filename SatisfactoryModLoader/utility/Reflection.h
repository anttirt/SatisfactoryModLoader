#pragma once
#include <Windows.h>
#include <string>

namespace SML {
	namespace Reflection {
		template<typename T>
		bool get_field_value(HMODULE module, const char* fieldName, T& output) {
			auto proc = GetProcAddress(module, fieldName);
			if (!proc) {
				return false;
			}

			output = *(T*)proc;
			return true;
		}
	}
}