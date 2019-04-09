#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>

namespace SML {
	static std::ofstream logFile;

	enum ConsoleColor {
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkMagenta,
		DarkYellow,
		DarkWhite,
		Grey,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White
	};

	static void set_color(ConsoleColor color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// sets the console color
		SetConsoleTextAttribute(hConsole, color + 1);
	}

	static void OpenLog() {
		logFile.open("SML.log", std::ios_base::app);
	}

	static void RenderText() {}
	static void RenderHeader(const char* header, ConsoleColor color) {
		const int length = 8;

		std::string headerS(header);
		std::string split;
		if (headerS.length() > length - 2) {
			split = headerS.substr(0, length - 2);
		}
		else {
			split = headerS;
		}

		split.erase(remove_if(split.begin(), split.end(), isspace), split.end());

		set_color(color);

		std::string line = '[' + split + ']';
		for (int i = line.length(); i < length; i++) {
			line += ' ';
		}

		std::cout << line;

		OpenLog();
		logFile << '[' << header << "] ";
		logFile.close();

		set_color(ConsoleColor::Grey);
	}

	template<typename First, typename ...Args>
	static void RenderText(First&& arg0, Args&& ...args) {
		std::cout << std::forward<First>(arg0);

		OpenLog();
		logFile << std::forward<First>(arg0);
		logFile.close();

		RenderText(std::forward<Args>(args)...);

		if (sizeof...(args) == 0) {
			std::cout << std::endl;

			OpenLog();
			logFile << std::endl;
			logFile.close();
		}
	}

	template<typename First, typename ...Args>
	static void info(First&& arg0, Args&& ...args) {
		RenderHeader("SML", ConsoleColor::Cyan);
		set_color(ConsoleColor::Green);
		RenderText(arg0, args...);
		set_color(ConsoleColor::Grey);
	}

	template<typename First, typename ...Args>
	static void warning(First&& arg0, Args&& ...args) {
		RenderHeader("SML", ConsoleColor::Cyan);
		set_color(ConsoleColor::Yellow);
		RenderText(arg0, args...);
		set_color(ConsoleColor::Grey);
	}

	template<typename First, typename ...Args>
	static void error(First&& arg0, Args&& ...args) {
		RenderHeader("SML", ConsoleColor::Cyan);
		set_color(ConsoleColor::Red);
		RenderText(arg0, args...);
		set_color(ConsoleColor::Grey);
	}

	template<typename First, typename ...Args>
	static void mod_info(const char* name, First&& arg0, Args&& ...args) {
		RenderHeader(name, ConsoleColor::White);
		logFile << name;
		set_color(ConsoleColor::Green);
		RenderText(arg0, args...);
		set_color(ConsoleColor::Grey);
	}

	template<typename First, typename ...Args>
	static void mod_warning(const char* name, First&& arg0, Args&& ...args) {
		RenderHeader(name, ConsoleColor::White);
		set_color(ConsoleColor::Yellow);
		RenderText(arg0, args...);
		set_color(ConsoleColor::Grey);
	}

	template<typename First, typename ...Args>
	static void mod_error(const char* name, First&& arg0, Args&& ...args) {
		RenderHeader(name, ConsoleColor::White);
		set_color(ConsoleColor::Red);
		RenderText(arg0, args...);
		set_color(ConsoleColor::Grey);
	}
}