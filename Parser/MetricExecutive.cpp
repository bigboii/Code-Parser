/////////////////////////////////////////////////////////////////////////////////////////////
//  MetricExecutive.h - collects metrics for all the packages with names that match        //
//                     a specified pattern in a directory tree rooted at a specified path  //
//  ver 1.0                                                                                //
//  Application:   Project #2, Requirement #9                                              //
//  Author:        Young Kyu Kim, Syracuse University,                                     //
//                 (315) 443-3948, ykim127@syr.edu                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

#include "MetricExecutive.h"

//Constructor: update parser reference
MetricExec::MetricExec(std::string path)
{
	filemgr = new FileMgr(".."+path, ds);  //step outside once then move to specified folder
	specifiedPath = path;
	pattern = "";
}

//perform metric analysis of all files at specified path
//C:\Users\YQ\Desktop\CSE687_OOD\Project2\Project2HelperCode\TestFiles1 *.h *.cpp
void MetricExec::performMetricAnalysis()
{
	std::cout << "\nPattern: " << pattern << std::endl;	   //2. Add Patterns
	filemgr->addPattern(pattern);

	config = new ConfigParseToConsole();                   //3. Metric Analysis for Header files
	for (auto file : ds)
	{
		parser = config->BuildMetric();        //Create new instance of parser+repository

		//std::string fpath = FileSystem::Path::getFullFileSpec(file);
		std::string newfile = ".."+specifiedPath;
		newfile = newfile + "/" + file;
		try
		{
			if (parser)
			{
				if (!config->Attach(newfile))
				{
					std::cout << "\n  could not open file " << newfile << std::endl;
					continue;
				}
				else
				{
					std::cout << "\n Opening file " << newfile << std::endl;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return;
			}
			// now that parser is built, use it
			config->setFilename(newfile);                  //set root's filename filename 
			while (parser->next())                  //1. get One SemiExpression
			{
				parser->parse();                     //2. Parse SemiExpression, returns true?
			}
			delete parser;            //delete parser, and reset
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	std::cout << "\n";
}

//Set specified path
void MetricExec::setDirectory(std::string& path)
{
	specifiedPath = path;
}

//Update specified path
void MetricExec::changeDirectory(std::string& path)
{
	//Split Filename
	std::size_t found = path.find_last_of("/\\");    //find last slash
	std::string newDir = path.substr(0, found);
	std::string endDir = path.substr(found + 1);
	std::cout << "\n Modified Directory: " << newDir;
	std::cout << "\n File name: " << endDir;

	specifiedPath = path;
}

//Return specified path
std::string MetricExec::getPath()
{
	return specifiedPath;
}

//Get reference to file manager
FileMgr* MetricExec::getFileMgr()
{
	return filemgr;
}

//Get reference to DataStore
DataStore& MetricExec::getDataStore()
{
	return ds;
}

//get Reference to file manager.
void MetricExec::addPattern(std::string pat)
{
	filemgr->addPattern(pat);
}

//#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

//#define TEST_FINAL
#ifdef TEST_FINAL

int main(int argc, char* argv[])
{
	std::string testPath = "C:/Users/YQ/Desktop/CSE687_OOD/Project2/Project2HelperCode/TestFiles";
	std::string curDir = FileSystem::Directory::getCurrentDirectory();
	std::cout << "\n Current Directory is:  " << curDir << std::endl;
	
	//Split Filename
	std::size_t found = curDir.find_last_of("/\\");    //find last slash
	std::string newDir = curDir.substr(0, found);
	std::string endDir = curDir.substr(found+1);
	std::cout << "\n Modified Directory: " << newDir;
	std::cout << "\n File name: " << endDir;

	//Check if there are command line arguments
	std::cout << "\n argv[]s are: " << std::endl;
	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	std::string inputs[3];

	for (int i = 1; i < argc; ++i)
	{
		inputs[i-1] = argv[i];
		std::cout << inputs[i-1] << std::endl;
	}

	MetricExec* exec = new MetricExec(inputs[0]);

	//Perform Testing
	exec->addPattern("*.cpp");        //get cpp files
	exec->addPattern("*.h");          //get header files
	exec->getFileMgr()->search();     //search for files

	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";
	for (auto fs : exec->getDataStore())
	{
		std::cout << "\n  " << fs;
	}

	exec->performMetricAnalysis();

	return 0;
}
#endif