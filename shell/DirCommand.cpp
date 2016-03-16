#include "stdafx.h"
#include "DirCommand.h"
#include "Split.h"

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

static shared_ptr<vector<FileInfo>> getFilesUnderDirectory(const wstring & path)
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

void DirCommand::execute(wistream &in, wostream &out){
	vector<wstring> result = split(command, wstring(_T(" ")));
	if (result.size() < 2)
	{
		throw "Too few arguments";
	}
	wstring path = result[1];
	auto files = getFilesUnderDirectory(path);
	for (auto it = files->begin(); it != files->end(); ++it)
	{
		if (it->is_dir())
		{
			out << "DIR " << it->get_file_name() << endl;
		}
		else
		{
			out << it->get_size().QuadPart << "  "
				<< it->get_file_name() << endl;
		}
	}
}
