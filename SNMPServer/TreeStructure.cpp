#include "pch.h"
#include "TreeStructure.h"

#pragma region TreeNode

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
}

#pragma endregion


#pragma region Tree
Tree::Tree()
{
	root = 0;
}

Tree::~Tree()
{
}

void Tree::addRoot(string pName, int pOID)
{
	TreeNode *newRoot = new TreeNode;

	newRoot->name = pName;
	newRoot->OID = pOID;

	if (root == 0)
	{
		root = newRoot;
	}

	else
	{
		//istnieje juz korzen
	}
}

void Tree::addNode(string pName, int pOID, TreeNode *parent)
{
	TreeNode *children = new TreeNode;

	children->name = pName;
	children->OID = pOID;

	parent->next.push_back(children);
}
#pragma endregion