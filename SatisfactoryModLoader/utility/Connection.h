#pragma once

namespace SML {
	class Dispatcher;

	class Connection
	{
	public:
		Connection();

		unsigned int id() const;

	private:
		Connection(Dispatcher* dispatcher, unsigned int id);

		Dispatcher* _dispatcher = nullptr;

		unsigned int _id = 0;

		friend class Dispatcher;
	};
}