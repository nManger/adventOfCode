// day7.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <cctype>

using namespace std;

class TreeNode
{
public:
	string name;
	TreeNode *parentNode = NULL;
	vector<tuple<string, int>> filesInNode;
	vector<TreeNode> childNodes;

	TreeNode() {  }; //default constructor
	TreeNode(string, TreeNode*);
	TreeNode(const TreeNode&); //copy constructor

	TreeNode& operator=(const TreeNode&);
	void addFile(string , int );
	void addNode(TreeNode&);
	int getTotalFolderSize();
	void writeFolderContents(int);
};


