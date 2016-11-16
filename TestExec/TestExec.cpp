#include<iostream>
#include<string>
#include <fstream>
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/ConfigureParser.h"
#include "../Parser/Parser.h"
#include "../DataStore/DataStore.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/MetricExecutive.h"
//#include "../DataStore/DataStore.h"

//namespace { char dummy; };

#define NoEmptyFile()   namespace { char NoEmptyFileDummy##__LINE__; }
using namespace Scanner;          //allows use of Tokenizer and SemiExpression

//Display Introductory message for requirements
void displayRequirement(std::string str)
{
	std::string msg = " Demonstrating Requirement ";
	msg.append(str);
	std::cout << "\n --------------------------------------\n";
	std::cout << msg << std::endl;
	std::cout << " --------------------------------------\n";
}

//Display Content of File
void displayInput(std::string fileSpec)
{
	std::string str = "\n Input: ";
	str.append(fileSpec);
	str.append("\n --------------------");
	std::cout << str << std::endl;
	
	std::cout << "displayInput(): "<< fileSpec << std::endl;
	
	std::ifstream MyFile(fileSpec);
	std::string text = "";
	
	while (!MyFile.eof())
	{
		getline(MyFile, text);
		std::cout << "" << text << "\n";
	}
	
	MyFile.close();	
}

//Demonstrate Project#1, Tokenizer and SemiExpression
void demoReq3456()
{
	std::string msg = " Demonstrating Requirement #3, #4, #5, and #6";
	std::cout << "\n ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + " << msg << std::endl;
	std::cout << " ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " For this implementation, I will be using Dr. Fawcett's Project 1 solutions";
	std::cout << "   - In this demo, a Parser will analyze and detect beginning and end of all scopes, as well as functions, which will demonstrate requirement #4 and #5\n";
	std::cout << "   - In addition, it will generate an abstract syntax tree at the end, which will demonstrate requirement #6\n";
	std::cout << "   - Implementing a parser will require the use of Tokenizer and SemiExpression; therefore requirement #3 will be naturally demoed." << std::endl;

	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();       //configure initializes parser with Toker, SemiExpression, and adds Rules and Actions

	std::string testfile = "../../TestFiles1/test2.h";
	displayInput(testfile);

	std::cout << "\n Output : ";
	std::cout << "\n ------------ " << std::endl;
	try
	{
		if (pParser)
		{
			if (!configure.Attach(testfile))
			{
				std::cout << "\n  could not open file " << testfile << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}
		// now that parser is built, use it

		while (pParser->next())                  //1. get One SemiExpression
			pParser->parse();                     //2. Parse SemiExpression, returns true?
		std::cout << "\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
}

//Demonstrate Requirement #7
void demoReq7()
{
	std::string msg = " Demonstrating Requirement #7, FileMgr";
	std::cout << "\n +++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + " << msg << std::endl;
	std::cout << " +++++++++++++++++++++++++++++++++++++++ " << std::endl;

	DataStore ds;
	std::string testdir = "../../TestFiles1/";
	FileMgr fm(testdir, ds);

	std::cout << "\n Search Directory:   " << "../TestFiles1/"<< std::endl;
	
	//Search Files without patterns
	std::cout << "\n\n Searching Files without patterns";
	fm.search();
	/*
	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";
	for (auto fs : ds)
	{
	std::cout << "\n  " << fs;
	}
	*/

	//Search Files with patterns
	DataStore ds2;
	FileMgr fm2(testdir, ds2);
	fm2.addPattern("*.h");
	fm2.addPattern("*.c*");
	std::cout << "\n\n  Searching Files with patterns *.h *.cpp";
	fm2.search();
	/*
	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";

	for (auto fs2 : ds2)
	{
		std::cout << "\n  " << fs2;
	}
	*/
	std::cout << "\n\n";
}

//Demonstrate Requirement #8 part 2
void demoReq8_1()
{
	std::string msg = " Demonstrating Requirement #8, MetricAnalyzer";
	std::cout << "\n +++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + " << msg << std::endl;
	std::cout << " +++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;

	std::cout << "Similar to the Abstract Syntax Tree constructed in requirement #6, the Metric Analyzer will display data in a tree format as well\n";
	ConfigParseToConsole configure;
	Parser* pParser = configure.BuildMetric();       //configure initializes parser with Toker, SemiExpression, and adds Rules and Actions

	std::string testfile = "../../TestFiles1/test2.h";
	displayInput(testfile);
	configure.setFilename(testfile);                  //set root's filename filename 
	std::cout << "\n Output : ";
	std::cout << "\n ------------ " << std::endl;
	try
	{
		if (pParser)
		{
			if (!configure.Attach(testfile))
			{
				std::cout << "\n  could not open file " << testfile << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}
		// now that parser is built, use it

		while (pParser->next())                  //1. get One SemiExpression
			pParser->parse();                     //2. Parse SemiExpression, returns true?
		std::cout << "\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}

	//delete pParser;
}

//Demonstrate Requirement #8 part 2
void demoReq8_2()
{
	std::cout << "\n  +++ Opening files in a different directory +++" << std::endl;

	ConfigParseToConsole configure;
	Parser* pParser = configure.BuildMetric();       //configure initializes parser with Toker, SemiExpression, and adds Rules and Actions

	std::string testfile = "../../TestFiles2/test5.h";
	displayInput(testfile);
	configure.setFilename(testfile);                  //set root's filename filename 
	std::cout << "\n Output : ";
	std::cout << "\n ------------ " << std::endl;
	try
	{
		if (pParser)
		{
			if (!configure.Attach(testfile))
			{
				std::cout << "\n  could not open file " << testfile << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}
		// now that parser is built, use it

		while (pParser->next())                  //1. get One SemiExpression
			pParser->parse();                     //2. Parse SemiExpression, returns true?
		std::cout << "\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}

	//delete pParser;
}

//Demonstrate Requirement #9
void demoReq9(int argc, char* argv[])
{
	std::string msg = " Demonstrating Requirement #9, MetricExecutive";
	std::cout << "\n ++++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + " << msg << std::endl;
	std::cout << " ++++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << "In this demo, the specified path \"/TestFiles1\" is being used. To change paths, change the first command line argument to, e.g., \"/TestFiles2\"\n";
	std::cout << "\n 1. Verifying Command line Arguments " << std::endl;

	std::vector<std::string> inputs(5);                //1. Collect command line arguments
	std::cout << "\n CommandLine arguments, argv[], are: " << std::endl;
	if (argc < 2)                               	//Check if there are command line arguments
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return;
	}
	for (int i = 1; i < argc; ++i)                  //save cmd args into input[]
	{
		inputs[i - 1] = argv[i];
		std::cout << inputs[i - 1] << std::endl;
	}

	std::cout << "\n 2. Setting Patterns " << std::endl;   //2. Set Patterns
	MetricExec* exec = new MetricExec(inputs[0]);
	for (int i = 1; i < argc-1; i++)
	{	
		std::cout << "- " << inputs[i] << std::endl;
		exec->addPattern(inputs[i]);
	}
	
	//3. Perform Testing
	std::cout << "\n 3. Searching for files" << std::endl;
	exec->getFileMgr()->search();     //search for files

	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";
	/*
	for (auto fs : exec->getDataStore())
	{
		std::cout << "\n";
		std::string newfile = ".." + exec->getPath();
		newfile = newfile + "/" + fs;
		std::cout << "looping" << std::endl;
		//displayInput(newfile);
	}
	*/
	std::cout << "\n 4. Performing Metric Analysis" << std::endl;
	exec->performMetricAnalysis();
}
#define TEST_FINAL
#ifdef TEST_FINAL
int main(int argc, char* argv[])
{
	std::string directory = "../Tokenizer/alphanumeric.cpp";
	std::string parserArg = "../TestFiles/test2.h";
	std::string metricArg = "C:/Users/YQ/Desktop/CSE687 OOD/Project2/Project2HelperCode/TestFiles *.h *.cpp";

	demoReq3456();
	demoReq7();
	demoReq8_1();
	demoReq8_2();
	demoReq9(argc, argv);

	std::cout << " --- END OF PROJECT 2 DEMO! ---" << std::endl;
	std::cout << " --- Have an amazing Spring Break! ---" << std::endl;
	return 0;
}
#endif
