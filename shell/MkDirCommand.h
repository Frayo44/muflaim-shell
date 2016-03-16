#include "Command.h"
#pragma once
class MkDirCommand : public Command
{
public:
	virtual void execute(wistream &in, wostream &out);

	MkDirCommand(const wstring &cmd) : Command(cmd) {}
};