#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.2                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.2 : 12 March, 2016
  - Updated Beginning of scope rule and Handle Push to acommodate Abstract Syntax Tree
  - Updated End of Scope rule and Handle Pop to accommodate Abstract Syntax Tree
  - Added Metric Analyzer to Repository
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "ASTree.h"
#include "MetricAnalyzer.h"
//#include "../AbstractSyntaxTree/AbstractSyntaxTree.h"


///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
 * ToDo:
 * - add AST Node class
 * - provide field to hold root of AST
 * - provide interface to access AST
 * - provide helper functions to build AST, perhaps in Tree class
 */
class Repository               // application specific
{
private:
  ScopeStack<ASNode*> stack;   //Scope Stack
  Scanner::Toker* p_Toker;
  ASTree* tree;                //Q: Tree that holds scope nodes
  MetricAnalyzer* mAnalyzer;   //Q: Analyzer for metric analysis

public:
  Repository(Scanner::Toker* pToker)      //Constructor
  {
    p_Toker = pToker;

	//Initialize Root node for AST
	ASNode * root = new ASNode();          //Global Name Space
	root->name = "Global";
	root->type = "namespace";
	root->lineStart = 0;

	stack.push(root);                       //Push root node into ScopeStack
	tree = new ASTree(root);                //update tree's root
	mAnalyzer = new MetricAnalyzer(tree);   //Analyzer has reference to tree
  }
  ScopeStack<ASNode*>& scopeStack()             //returns reference to scopeStack
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());   //fawcett's version: return toker's linecount
  }

  //Q's functions
  ASTree* getTree() { return tree; };
  //void setFilename(std::string name)
  //{
//	  getTree()->getRoot()->setFilename(name);
  //}

  //Perform Metric Analysis and Print tree
  void analyzeTree()
  {
	  mAnalyzer->printTree(mAnalyzer->getTree()->getRoot(), 0);
  }

};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;                                  //Points to repository located in ConfigureParser
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}

	//check if function
	bool isFunct(const std::string& tok)
	{
		const static std::string keys[] = { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	//check if loop
	bool isLoop(const std::string& tok)
	{
		const static std::string keys[] = { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	//check if conditional
	bool isConditional(const std::string& tok)
	{
		const static std::string keys[] = { "if", "else if", "else", "switch"};
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	//check if class, INCOMPLETE
	bool isClass(const std::string& tok)
	{
		if (tok == "class")
			return true;
		else
			return false;
	}

	//check if struct
	bool isStruct(const std::string& tok)
	{
		if (tok == "struct")
			return true;
		else
			return false;
	}

	//check if exception
	bool isException(const std::string& tok)
	{
		const static std::string keys[] = { "try", "catch", "finally", "throw" };
		for (int i = 0; i<4; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	//check if namespace
	bool isNamespace(const std::string& tok)
	{
		if (tok == "namespace")
			return true;
		else
			return false;
	}

	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;               //get reference to semi expression

		//1. Create Elem
		ASNode* elem = new ASNode();                       //*******(HEAP)    elem.type = "unknown";                               //initialize type	 
		std::string name = (*pTc)[pTc->find("(") - 1];     //calculate scope name
		elem->name = name;                          //initialize name
		elem->lineStart = p_Repos->lineCount();            //initialize line count

		if (tc[tc.length() - 1] == "{")                    //verify beginning of scope
		{
			size_t len = tc.find("(");
			if (len < tc.length())
			{
				//1. Determine scope type
				size_t len = tc.find("(");          //Find index of '('
				if (!isFunct(tc[len - 1]))          // tc[len - 1] is the name before '('
					elem->type = "function";
				if (isLoop(tc[len - 1]))
					elem->type = "loop";
				if (isConditional(tc[len - 1]))
					elem->type = "conditional";
				//if (isClass(pTc))
				//	elem->type = "class";
				if (isStruct(tc[len - 1]))
					elem->type = "struct";
				if (isException(tc[len - 1]))
					elem->type = "exception";
				if (isNamespace(tc[len - 1]))
					elem->type = "namespace";
			}
		}

		//2. Make Child: Get current top, and add child
		p_Repos->scopeStack().top()->addChild(elem);

		//3. Add to scopestack : add element to top element of scope stack
		p_Repos->scopeStack().push(elem);                    //get reference to Repository's stack, and push the element
	}
};

///////////////////////////////////////////////////////////////
// Q's New Action: print scope info

class PrintScope : public IAction
{
	Repository* p_Repos;
public:
	PrintScope(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i<tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		// show cleaned semiExp
		if (p_Repos->scopeStack().top()->type != "function")
			std::cout << "\n  ScopeDef: " << se.show();
		else
			std::cout << "\n  ScopeDef; Type: "<< p_Repos->scopeStack().top()->type << se.show();

		//Show state of tree
		//std::cout << "\n*** Start: Abstract Syntax Tree ***";
		//p_Repos->getTree()->DFS_r_ltr(p_Repos->getTree()->getRoot(), 0);
		//std::cout << "\n*** END: Abstract Syntax Tree ***" << std::endl;
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope: Here we update a scope's size

class HandlePop : public IAction
{
  Repository* p_Repos;
  bool isMetric;

public:
  HandlePop(Repository* pRepos, bool isprint)
  {
    p_Repos = pRepos;
	isMetric = isprint;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(p_Repos->scopeStack().size() == 0)
      return;

	p_Repos->scopeStack().top()->lineEnd = p_Repos->lineCount() + 1; //update lineEnd before we pop
    ASNode* elem = p_Repos->scopeStack().pop();          //save copy of currest scope node after popping
	//std::cout << "Popping " << elem->name << std::endl;
	//elem->lineEnd = p_Repos->lineCount();
	if (isMetric == false)
	{
		if (elem->type == "function")
		{
			//std::cout << "\nHandlePop";
			//std::cout << "\n--popping at line count = " << p_Repos->lineCount();
			std::cout << "\n Popping Function " << elem->name << ", lines = " << p_Repos->lineCount()/* - elem->lineEnd*/;
			std::cout << "\n";
		}
		else
		{
			std::cout << "\n  End of " << elem->name << ", lines = " << p_Repos->lineCount()/* - elem->lineEnd*/;
			std::cout << "\n";
		}
	}
  }
};

///////////////////////////////////////////////////////////////
// Q's New Action: action to print scopestack at end of scope

class PrintEndOfScope : public IAction
{
	Repository* p_Repos;
public:
	PrintEndOfScope(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i<tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		// show cleaned semiExp
		std::cout << "\n  End of ScopeDef: " << se.show();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // push function scope, Calculate name
    std::string name = (*pTc)[pTc->find("(") - 1];

	//Q's Implementation
	//1. Create Elem
	//std::cout << "\n--BeginningOfScope rule";
	ASNode* elem = new ASNode();                       //*******(HEAP)
	elem->type = "function";
	elem->name = name;                                 //initialize name
	elem->lineStart = p_Repos->lineCount();            //initialize line count

	//2. Assign child to current scope: Get current top, and add child
	p_Repos->scopeStack().top()->addChild(elem);

	//3. Add to scopestack : add element to top element of scope stack
	p_Repos->scopeStack().push(elem);                    //get reference to Repository's stack, and push the element
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    size_t len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

///////////////////////////////////////////////////////////////
// Q's New + Last rule : Last rule to display ASTree

class EndOfFile : public IRule
{
	Repository* p_Repos;                                  //Points to repository located in ConfigureParser
public:
	EndOfFile(Repository* pRepos)
	{
		p_Repos = pRepos;
	}

	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "Checking EndOfFile, length = " << p_Repos->scopeStack().size() <<std::endl;
		if (p_Repos->scopeStack().size() == 1)        //if we find last scope
		{
			doActions(pTc);
			return true;
		}
		//doActions(pTc);
		return true;
	}
};


///////////////////////////////////////////////////////////////
// Q's New Action : action to show abstract syntax tree

class ShowASTree : public IAction
{
	Repository* p_Repos;                                  //Points to repository located in ConfigureParser
	bool isMetric;

public:
	ShowASTree(Repository* pRepos, bool ismetric)
	{
		p_Repos = pRepos;
		isMetric = ismetric;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;

		//Show state of tree
		if (isMetric == false)
		{
			std::cout << "\n*** Start: Abstract Syntax Tree ***";
			p_Repos->getTree()->displayTree(p_Repos->getTree()->getRoot(), 0);
			//p_Repos->analyzeTree();
			std::cout << "\n*** END: Abstract Syntax Tree ***" << std::endl;
		}
		else
		{
			std::cout << "\n*** Start: Metric Analysis ***";
			//p_Repos->getTree()->displayTree(p_Repos->getTree()->getRoot(), 0);
			p_Repos->analyzeTree();
			std::cout << "\n*** END: Metric Analysis ***" << std::endl;
		}
	}
};



#endif
