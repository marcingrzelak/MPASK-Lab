#include "pch.h"
#include "TreeStructure.h"

#pragma region TreeNode

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
}

void TreeNode::printTree(string indent, bool last)
{
	cout << (indent);
	if (last)
	{
		cout << ("\\-");
		indent += "  ";
	}
	else
	{
		cout << ("|-");
		indent += "| ";
	}
	cout << name << endl;

	for (int i = 0; i < next.size(); i++)
	{
		next.at(i)->printTree(indent, i == next.size() - 1);
	}
}

#pragma endregion


#pragma region Tree
Tree::Tree()
{
	addRoot("iso", 1);
	addNode("org", 3, root);
	TreeNode* node = findNode("org", root);
	addNode("dod", 6, node);
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
void Tree::addNodeObject(string pName, int pOID, TreeNode *parent, string pSyntax, string pAccess, string pDescription)
{
	TreeNode *children = new TreeNode;

	children->name = pName;
	children->OID = pOID;
	children->syntax = pSyntax;
	children->access = pAccess;
	children->description = pDescription;

	parent->next.push_back(children);
}
TreeNode * Tree::findNode(string pName, TreeNode* node)
{
	if (node->name == pName)
	{
		return node;
	}
	else
	{
		for (int i = 0; i < node->next.size(); i++)
		{
			TreeNode* result = findNode(pName, node->next.at(i));
			if (result != nullptr) {
				return result;
			}
		}
	}
	return nullptr;
}
TreeNode * Tree::findNodeSpecificOID(string pName, int pOID, TreeNode * node)
{
	if (node->name == pName)
	{
		if (node->OID == pOID)
		{
			return node;
		}
	}
	else
	{
		for (int i = 0; i < node->next.size(); i++)
		{
			TreeNode* result = findNode(pName, node->next.at(i));
			if (result != nullptr) {
				return result;
			}
		}
	}
	return nullptr;
}
#pragma endregion