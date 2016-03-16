#include "stdafx.h"
#include "MkDirCommand.h"
#include "Split.h"

void MkDirCommand::execute(wistream &in, wostream &out){
	vector<wstring> result = split(command, wstring(_T(" ")));
	if (result.size() < 2)
	{
		throw "Too few arguments";
	}
	wstring path = result[1];
	const TCHAR *newPath = path.c_str();
	if (_wmkdir(newPath) == 0)
	{
		cout << "dir created";
	}
	else {
		char buffer[80];
		if (EEXIST != errno){
			cout << strerror_s(buffer, sizeof(buffer)) << endl;
		}
	}
}