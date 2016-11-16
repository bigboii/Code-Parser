#ifndef ASTREE_H
#define ASTREE_H
// Above statements very IMPORTANT!!!
///////////////////////////////////////////////////////////////////////
//  ASTree.h - Contains facilities for creating Abstract syntax tree //
//             nodes and the Abstract Syntax Tree                    //
//  ver 1.0                                                          //
//  Application:   Project #2, Requirement #6                        //
//  Author:        Young Kyu Kim, Syracuse University,               //
//                 (315) 443-3948, ykim127@syr.edu                   //
///////////////////////////////////////////////////////////////////////
/*
	Package Operation:
	==================
	This package acts as the facility for building an abstract syntax tree that provides an 
	interface for adding scope nodes to the tree and an methods to analyze the contents of the tree.

	Build Process:
	--------------
	Required Files: ASTree.h, ASTree.cpp
	
	Maintenance History:
	====================
	ver 1.0 : 12 March, 2016
	- Moved "struct element" from repository and moved it here as "struct ASNode"
	- Implemented Abstract Syntax tree that uses ASNodes
*/

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
* - chanage lineCount to two fields: lineCountStart and lineCountEnd
* - Piazza: struct Element needs to be modified to become a ASTNode!!!
*/

#include <string>
#include<vector>
#include<sstream>
#include<iostream>
#include <memory>

struct ASNode
{
	std::string type;
	std::string name;
	size_t lineStart;                      //beginning of scope
	size_t lineEnd;                        //end of scope 
	std::vector<ASNode*> children;         //list of children
	std::string filename;                  //If root node, stores filename
	std::string show();

	//Add child node to current element
	void addNewChild(std::string name, std::string type, int lineCount);

	//Add existing childe node to current element
	void addChild(ASNode* elem);

	void setFilename(std::string name);
};

//TREE CLASS
class ASTree
{
public:
	ASTree(ASNode* stacksRoot);                   //Constructor
	ASNode* getRoot();
	
	//----< recursive left-to-right Depth First Search >-------------------------
	void displayTree(ASNode* node, int n);
	
private:
	ASNode* root;              //root of tree
};

#endif