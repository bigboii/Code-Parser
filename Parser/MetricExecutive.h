#ifndef METRICEXEC_H
#define METRICEXEC_H

/////////////////////////////////////////////////////////////////////////////////////////////
//  MetricExecutive.h - collects metrics for all the packages with names that match        //
//                     a specified pattern in a directory tree rooted at a specified path  //
//  ver 1.0                                                                                //
//  Application:   Project #2, Requirement #9                                              //
//  Author:        Young Kyu Kim, Syracuse University,                                     //
//                 (315) 443-3948, ykim127@syr.edu                                         //
/////////////////////////////////////////////////////////////////////////////////////////////
/*
	Package Operation:
	==================
	MetricExec acts as an entry point to the Parser-based Code Analyzer. It generates Abstract Syntax Trees 
	for all files with names that match a specified pattern at a specified directory. Then it performs 
	a metric analysis and prints its results. Arguments for path and patterns will be defined in command line arguments.

	Build Process:
	--------------
	Required Files: MetricExecutive.h, ASTree.h, Parser.h, ConfigureParser.h, FileMgr.h, MetricExecutive.cpp

	Maintenance History:
	====================
	ver 1.0 : 12 March, 2016
*/

//Entry point to proj

//It collects "path" and patterns as a command argument
//pattern = *.cpp, *.h    handles if i want to test single files or multiple files

//Collect vector of fully qualifed file names
#include "../FileMgr/FileMgr.h"
#include "ASTree.h"
#include "Parser.h"
#include "ConfigureParser.h"

class MetricExec
{
public:
	MetricExec(std::string path);
	void performMetricAnalysis();
	void setDirectory(std::string& path);
	void changeDirectory(std::string& path);
	std::string getPath();
	FileMgr* getFileMgr();
	DataStore& getDataStore();
	void addPattern(std::string pat);      //setPattern

private:
	Parser* parser;                         //reference to parser declared outside, e.g., TestExec
	ConfigParseToConsole* config;
	FileMgr* filemgr;                        //finds files and directories at a specified path
	DataStore ds;
	std::string pattern;

	std::vector<ASNode*> trees;             //holds root nodes of abstract syntax trees for each files
	std::string specifiedPath;                //specified path
};

#endif