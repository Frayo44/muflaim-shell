#include "stdafx.h"

#pragma once
using namespace std;
class Command{
protected:
	const wstring command;
	Command(const wstring &cmd) : command(cmd) {
	}
public:
	virtual void execute(wistream &in, wostream &out)=0;
};