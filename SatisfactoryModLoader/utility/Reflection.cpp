#include "stdafx.h"
#include "Reflection.h"

namespace SML {
	namespace Reflection {
		PVOID get_function(HMODULE module, const char* functionName) {
			return GetProcAddress(module, functionName);
		}
	}
}