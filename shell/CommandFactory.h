#include "Command.h"

#pragma once

class CommandFactory{
public:
	static Command *createCommand(const wstring &cmd);
};