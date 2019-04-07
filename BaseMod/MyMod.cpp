#include <stdafx.h>
#include <utility/Configuration.h>
#include "MyMod.h"

bool ItemDescriptor(void* item) {
	std::cout << "Items" << std::endl;
	return true;
}

void MyMod::Setup() {
	_dispatcher.subscribe(SML::HookLoader::Event::UFGItemDescriptorUFGItemDescriptor, ItemDescriptor);
}