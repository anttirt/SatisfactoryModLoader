#pragma once

#include <functional>
#include <map>
#include <vector>
#include "HookLoader.h"

namespace SML {
	class Connection;
	class Dispatcher;

	template<typename ...Args>
	using EventFunc = bool(Args...);

	class Dispatcher {
	public:
		template<typename ...Args>
		void subscribe(HookLoader::Event descriptor, EventFunc<Args...>* slot) {
			auto id = _nextID;
			SlotHandle handle = { id, slot };

			_observers[descriptor].push_back(handle);
			_nextID++;
		}

		template<typename ...Args>
		bool post(HookLoader::Event type, Args ...args) const {
			if (_observers.find(type) == _observers.end()) {
				return true;
			}

			bool run = true;

			auto observers = _observers.at(type);
			for (auto observer : observers) {
				auto slot = *(EventFunc<Args...>*)observer.slot;
				if (!slot((args)...)) {
					run = false;
				}
			}

			return run;
		}

	private:
		unsigned int _nextID = 0;

		struct SlotHandle {
			unsigned int id;
			void* slot;
		};

		std::map<HookLoader::Event, std::vector<SlotHandle>> _observers;
	};
}