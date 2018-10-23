#pragma once

class TreeNode
{
public:
	TreeNode();
	~TreeNode();

	string name;
	int OID;
	vector <TreeNode*> next;
};

class Tree
{
public:
	Tree();
	~Tree();

	TreeNode *root;
	void addRoot(string pName, int pOID);
	void addNode(string pName, int pOID, TreeNode *parent);
};
