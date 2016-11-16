/////////////////////////////////////////////////////////////////////
// FileManager - navigates through a directory tree                //
// ver 4.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include<iostream>

using namespace FileSystem;                      //needed to use Directory::getFiles()... etc

class FileMgr
{
public:
	//Find files in specified path (e.g.   c:/temp/)
	void findFiles(std::string path)
	{
		// Display contents of non-current directory
		std::cout << "\n  .h and .cpp files residing in " << path << " are:";

		headerFiles = Directory::getFiles(path, "*.h");              //collect header files
		cppFiles = Directory::getFiles(path, "*.cpp");               //collect cpp files
		//collect cppfiles

		std::cout << "\n  It contains header files:";
		for (size_t i = 0; i<headerFiles.size(); ++i)
			std::cout << "\n    " << headerFiles[i].c_str();

		std::cout << "\n  It contains cpp files:";
		if (cppFiles.size() > 0)
		{
			for (size_t i = 0; i<cppFiles.size(); ++i)
				std::cout << "\n    " << cppFiles[i].c_str();
		}
	}

	//Find directories in specifed path
	void findDirectories(std::string path)
	{
		std::cout << "\n  and contains directories:";
		currDirs = Directory::getDirectories(path);

		for (size_t i = 0; i < currDirs.size(); ++i)
			std::cout << "\n    " << currDirs[i].c_str();
		std::cout << "\n";
	}

	bool exists(std::string path)
	{
		return Directory::exists(path);
	}

	//return reference to headerFiles
	std::vector<std::string>& getHeaderFiles()
	{
		return headerFiles;
	}

	//return reference to cppFiles
	std::vector<std::string>& getCppFiles()
	{
		return cppFiles;
	}

	//return reference to directories
	std::vector<std::string>& getDirectories()
	{
		return currDirs;
	}

private:
	std::vector<std::string> headerFiles;       //list of header files at specified path
	std::vector<std::string> cppFiles;          //list of cpp files at specified path
	std::vector<std::string> currDirs;           //list of directories at specified path
};