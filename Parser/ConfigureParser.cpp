/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.2                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Young Kyu Kim, Syracuse University,             //
//                 ykim127@syr.edu                                 //
//  Source:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
	Package

	Maintenance History:
	====================
	ver 2.2 : 12 March, 2016
	- Deleted all Rules and Actions except for BeginScope, HandlePush, EndOfScope, HandlePop
*/

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pPrintScope;                //Q's PrintScope
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pEndOfFile;                //Q's rule, detect end of file
  delete pShowASTree;               //Q's action, Print Tree
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);         //Originally, Constructor is Parser(ITokCollection* itok), but since pSemi is a derived class of ITokCollection, it works
    pRepo = new Repository(pToker);      

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
	pPrintScope = new PrintScope(pRepo);             //Q's PrintScope, initialization
    pBeginningOfScope->addAction(pHandlePush);
	//pBeginningOfScope->addAction(pPrintScope);       //Q's PrintScope, addAction
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();                  //updates lineEnd of a scope node
    pHandlePop = new HandlePop(pRepo, false);
	pPrintEndOfScope = new PrintEndOfScope(pRepo);   //Q's Print End of Scope
    pEndOfScope->addAction(pHandlePop);
	pEndOfScope->addAction(pPrintEndOfScope);

	//Q's final Action
	pEndOfFile = new EndOfFile(pRepo);               //rule to check if end of file
	pShowASTree = new ShowASTree(pRepo, false);             //action to display abstract syntax tree       
	pEndOfFile->addAction(pShowASTree);
    pParser->addRule(pEndOfScope);
	pParser->addRule(pEndOfFile);
	
    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

//Similar to Build above, but displays Metric Analysis in the end instead of AST
Parser* ConfigParseToConsole::BuildMetric()
{
	try
	{
		// add Parser's main parts

		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);         //Originally, Constructor is Parser(ITokCollection* itok), but since pSemi is a derived class of ITokCollection, it works
		pRepo = new Repository(pToker);

		// configure to manage scope
		// these must come first - they return true on match
		// so rule checking continues

		pBeginningOfScope = new BeginningOfScope();
		pHandlePush = new HandlePush(pRepo);
		//pPrintScope = new PrintScope(pRepo);             //Q's PrintScope, initialization
		pBeginningOfScope->addAction(pHandlePush);
		//pBeginningOfScope->addAction(pPrintScope);       //Q's PrintScope, addAction
		pParser->addRule(pBeginningOfScope);
		pEndOfScope = new EndOfScope();                  //updates lineEnd of a scope node
		pHandlePop = new HandlePop(pRepo, true);
		//pPrintEndOfScope = new PrintEndOfScope(pRepo);   //Q's Print End of Scope
		pEndOfScope->addAction(pHandlePop);
		//pEndOfScope->addAction(pPrintEndOfScope);

		//Q's final Action
		pEndOfFile = new EndOfFile(pRepo);               //rule to check if end of file
		pShowASTree = new ShowASTree(pRepo, true);             //action to display abstract syntax tree       
		pEndOfFile->addAction(pShowASTree);
		pParser->addRule(pEndOfScope);
		pParser->addRule(pEndOfFile);

		return pParser;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}

//Set filename of rootnode
void ConfigParseToConsole::setFilename(std::string name)
{
	pRepo->getTree()->getRoot()->setFilename(name);
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
