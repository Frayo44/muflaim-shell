#include "stdafx.h"
#include "CommandFactory.h"
#include "Split.h"
#include "DirCommand.h"
#include "MkDirCommand.h"

Command *CommandFactory::createCommand(const wstring &cmd){
	
	vector<wstring> result = split(cmd, wstring(_T(" ")));
	if (result.size() < 2)
	{
		throw   "Unexpected input";
	}
	wstring command = result[0];

	if (command == _T("dir"))
	{
		return new DirCommand(cmd);
	}
	else if (command == _T("mkdir"))
	{		
		return new MkDirCommand(cmd);
	}
}