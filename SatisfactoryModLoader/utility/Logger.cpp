#include <stdafx.h>
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace SML {
	void set_color(ConsoleColor color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// sets the console color
		SetConsoleTextAttribute(hConsole, color + 1);
	}
}