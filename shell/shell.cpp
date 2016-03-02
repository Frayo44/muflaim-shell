// shell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <memory>

// Getting a command, parsing it and executing it

using namespace std;


class FileInfo
{
private:
	wstring file_name;
	bool directory;
	LARGE_INTEGER size;
public:
	FileInfo(const wstring& file_name) :
		file_name(file_name), directory(true){}

	FileInfo(const wstring& file_name,
		const LARGE_INTEGER& size) :
		file_name(file_name), directory(false), size(size){}

	inline const wstring& get_file_name() const
	{
		return file_name;
	}

	inline bool is_dir() const
	{
		return directory;
	}

	inline const LARGE_INTEGER& get_size() const
	{
		return size;
	}




};

template<typename T>
vector<T>
split(const T & str, const T & delimiters) {
	vector<T> v;
	T::size_type start = 0;
	auto pos = str.find_first_of(delimiters, start);
	while (pos != T::npos) {
		if (pos != start) // ignore empty tokens
			v.emplace_back(str, start, pos - start);
		start = pos + 1;
		pos = str.find_first_of(delimiters, start);
	}
	if (start < str.length()) // ignore trailing delimiter
		v.emplace_back(str, start, str.length() - start); // add what's left of the string
	return v;
}

shared_ptr<vector<FileInfo>> getFilesUnderDirectory(const wstring & path)
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	shared_ptr<vector<FileInfo>> results(new vector<FileInfo>());


	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(path.c_str(), MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		throw "Directory path is too long.";
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, path.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		throw "Error in FindFirstFile";
	}

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			results->push_back(FileInfo(ffd.cFileName));
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			results->push_back(FileInfo(ffd.cFileName, filesize));

			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
		
		
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		throw "Error in FindFirstFile";
	}

	FindClose(hFind);
	return results;
}

int _tmain(int argc, _TCHAR* argv[])
{
	wstring str;
	getline(wcin, str);
	wcout << str;
	vector<wstring> result = split(str, wstring(_T(" ")));
	if (result.size() < 2)
	{
		wcout << "Error";
		return 1;
	}

	wstring command = result[0];

	if (command == _T("dir"))
	{
		wstring path = result[1];
		auto files = getFilesUnderDirectory(path);
		for (auto it = files->begin(); it != files->end(); ++it)
		{
			if (it->is_dir())
			{
				wcout << "DIR "<< it->get_file_name() << endl;
			}
			else
			{
				wcout << it->get_size().QuadPart << "  " 
					<< it->get_file_name() << endl;
			}
			
		}
	}

	return 0;
}


