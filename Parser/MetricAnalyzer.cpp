////////////////////////////////////////////////////////////////////////////////////////////
//  MetricAnalyzer.h - evaluating and displaying the size and complexity of all functions //
//                     in each of a set of specified packages.                            //
//  ver 1.0                                                                               //
//  Application:   Project #2, Requirement #8                                             //
//  Author:        Young Kyu Kim, Syracuse University,                                    //
//                 (315) 443-3948, ykim127@syr.edu                                        //
////////////////////////////////////////////////////////////////////////////////////////////
#include "MetricAnalyzer.h"

//Print Tree recursively (l->r) using Depth First Search
void MetricAnalyzer::printTree(ASNode* node, int n)                      //n: level of tree (0 would be root)
{
	std::string tabs = "";
	//1. Calculate # of tabs
	for (int i = 0; i < n; i++)
	{
		tabs += "\t";
	}

	//Calculate method size
	size_t size = 0;
	calcMethodSize(node, size);

	//Calculate Complexity
	int complexity = 1;                 //an empty function always has a CC of 1
	calcComplexity(node, complexity);

	//2. Display tree: recursive traversal, Depth First Search
	if (n == 0)
	{
		std::cout << "\n Rootfile: " << getTree()->getRoot()->filename << std::endl;
		std::cout << "\n  " << tabs << "(name: " << node->name << ", type: " << node->type << ", lineStart: " << node->lineStart << ", lineEnd: " << node->lineEnd << ", size: " << size << ", CC: " << complexity << ")";
	}
	else
	{
		std::cout << "\n  " << tabs << "(name: " << node->name << ", type: " << node->type << ", lineStart: " << node->lineStart << ", lineEnd: " << node->lineEnd << ", size: " << size << ", CC: " << complexity << ")";
	}
	for (auto & pChild : node->children)           //"auto &" to get address of ASNode*
		printTree(pChild, n + 1);
}

//Calculate Method Size, recursively
void MetricAnalyzer::calcMethodSize(ASNode* node, size_t &n)
{
	n = node->lineEnd - node->lineStart;
}

//Calculate Complexities, recursively
void MetricAnalyzer::calcComplexity(ASNode* node, int &n)
{
	for (auto & pChild : node->children)           //"auto &" to get address of ASNode*
	{
		if (pChild != NULL)
			n++;                                       //increment complexity by 1
		calcComplexity(pChild, n);
	}
}

//Returns pointer to Abstract Syntax Tree
ASTree* MetricAnalyzer::getTree()
{
	return tree;
}