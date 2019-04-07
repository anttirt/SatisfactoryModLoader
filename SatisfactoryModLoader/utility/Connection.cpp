#include <stdafx.h>
#include "Connection.h"
#include "Dispatcher.h"

namespace SML {
	Connection::Connection()
	{
	}

	unsigned int Connection::id() const {
		return _id;
	}

	Connection::Connection(Dispatcher* dispatcher, unsigned int id)
		: _dispatcher(dispatcher)
		, _id(id) {

	}
}