///////////////////////////////////////////////////////////////////////
//  ASTree.h - Contains facilities for creating Abstract syntax tree //
//             nodes and the Abstract Syntax Tree                    //
//  ver 1.0                                                          //
//  Application:   Project #2, Requirement #6                        //
//  Author:        Young Kyu Kim, Syracuse University,               //
//                 (315) 443-3948, ykim127@syr.edu                   //
///////////////////////////////////////////////////////////////////////

#include "ASTree.h"

std::string ASNode::show()
{
	std::ostringstream temp;
	temp << "(";
	temp << type;
	temp << ", ";
	temp << name;
	temp << ", ";
	temp << lineStart;
	temp << ")";
	return temp.str();
}

//Add child node to current element
void ASNode::addNewChild(std::string name, std::string type, int lineCount)
{
	ASNode* newNode = new ASNode();
	newNode->name = name;
	newNode->type = type;
	newNode->lineStart = lineCount;
	children.push_back(newNode);
}

//Add existing childe node to current element
void ASNode::addChild(ASNode* elem)
{
	children.push_back(elem);
}

void ASNode::setFilename(std::string name)
{
	filename = name;
}

//Constructor
ASTree::ASTree(ASNode* stacksRoot)
{
	root = stacksRoot;
}

ASNode* ASTree::getRoot()
{
	return root;
}

//----< recursive left-to-right Depth First Search >-------------------------
void ASTree::displayTree(ASNode* node, int n)
{
	std::string tabs = "";
	//1. Calculate # of tabs
	for (int i = 0; i < n; i++)
	{
		tabs += "\t";
	}

	//Calculate method size
	int size = 0;
	//calcMethodSize(node, size);

	//Calculate Complexity
	int complexity = 1;                 //an empty function always has a CC of 1
										//calcComplexity(node, complexity);

										//2. Display tree: recursive traversal, Depth First Search
	std::cout << "\n  " << tabs << "name: " << node->name << ", " << node->type << ", " << node->lineStart;
	for (auto & pChild : node->children)           //"auto &" to get address of ASNode*
		displayTree(pChild, n + 1);

}
