#include "pch.h"
#include "TreeStructure.h"

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

	for (unsigned int i = 0; i < next.size(); i++)
	{
		next.at(i)->printTree(indent, i == next.size() - 1);
	}
}



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
		for (unsigned int i = 0; i < node->next.size(); i++)
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
		for (unsigned int i = 0; i < node->next.size(); i++)
		{
			TreeNode* result = findNode(pName, node->next.at(i));
			if (result != nullptr) {
				return result;
			}
		}
	}
	return nullptr;
}

string Tree::findNodeWord(string pName, TreeNode * node, string OID)
{
	if (node->name == pName)
	{
		OID.append(to_string(node->OID));
		return OID;
	}
	else
	{
		for (unsigned int i = 0; i < node->next.size(); i++)
		{
			string result = findNodeWord(pName, node->next.at(i), OID);
			if (result != "")
			{
				OID.append(result);
				OID.insert(0, ".");
				OID.insert(0, to_string(node->OID));
				return OID;
			}
		}
	}
	return OID;
}

TreeNode * Tree::findOID(string pOID, TreeNode * node)
{
	if (pOID.find(".") == string::npos)
	{
		return node;
	}
	stringstream streamOID(pOID);
	string singleOID;
	vector<int> OIDlist;
	TreeNode* newNode = node;

	while (getline(streamOID, singleOID, '.'))
	{
		OIDlist.push_back(stoi(singleOID));
	}

	if (OIDlist.at(0) == 1)
	{
		for (unsigned int i = 1; i < OIDlist.size(); i++)
		{
			int counter = newNode->next.size();;
			for (int j = 0; j < counter; j++)
			{
				if (newNode->next.at(j)->OID == OIDlist.at(i))
				{
					newNode = newNode->next.at(j);
					j = counter;
				}
			}
			//return nullptr;
		}
	}

	return newNode;
}
