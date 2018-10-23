#include "pch.h"
#include "TreeStructure.h"

#pragma region TreeNode

TreeNode::TreeNode()
{
	next.push_back(0);
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
	TreeNode *children = new TreeNode;

	children->name = pName;
	children->OID = pOID;

	if (root == 0)
	{
		root = children;
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