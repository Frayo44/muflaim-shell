#include "Command.h"
#pragma once
class DirCommand : public Command
{
public:
	virtual void execute(wistream &in, wostream &out);

	DirCommand(const wstring &cmd) : Command(cmd) {}
};