/*
	fileTree.cpp : Contains all definitions and functions
	for the file tree class
*/

#include "day7.h"

TreeNode::TreeNode(string name, TreeNode* parentPointer=nullptr)
{
	this->name = name;
	this->parentNode = parentPointer;
}

TreeNode::TreeNode(const TreeNode& obj)
{
	this->name = obj.name;
	this->parentNode = obj.parentNode;
	this->filesInNode = obj.filesInNode;
	this->childNodes = obj.childNodes;
	
	for(int i=0;i<this->childNodes.size();i++)
	{
		this->childNodes.at(i).parentNode = this;
	}
}

TreeNode& TreeNode::operator=(const TreeNode& obj)
{
	if (this != &obj)
	{
		this->name = obj.name;
		this->parentNode = obj.parentNode;
		this->filesInNode = obj.filesInNode;
		this->childNodes = obj.childNodes;

		for (int i = 0; i < this->childNodes.size(); i++)
		{
			this->childNodes.at(i).parentNode = this;
		}
	}
	return *this;
}

void TreeNode::addFile(string filename, int filesize)
{
	this->filesInNode.push_back(make_tuple(filename, filesize));
}

void TreeNode::addNode(TreeNode& childNode)
{
	this->childNodes.push_back(childNode);
}

int TreeNode::getTotalFolderSize()
{
	int size = 0;
	for (auto& [file, filesize] : filesInNode)
	{
		size += filesize;
	}
	for (TreeNode childNode : childNodes)
	{
		size += childNode.getTotalFolderSize();
	}
	return size;
}

void TreeNode::writeFolderContents(int depth)
{
	cout << string(depth * 2, ' ') << "- " << name << " (dir)\n";
	for (auto& [filename, filesize] : filesInNode)
	{
		cout << string((depth+1) * 2, ' ') << "- " << filename << " (file, size=" << filesize <<")\n";
	}
	for (TreeNode childNode : this->childNodes)
	{
		childNode.writeFolderContents(depth + 1);
	}
}
