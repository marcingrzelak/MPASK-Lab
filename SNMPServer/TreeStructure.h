#pragma once

class TreeNode
{
public:
	TreeNode();
	~TreeNode();

	string name, syntax, access, description;
	int OID;
	vector <TreeNode*> next;
	void printTree(string indent, bool last);
};

class Tree
{
public:
	Tree();
	~Tree();

	TreeNode *root;
	void addRoot(string pName, int pOID);
	void addNode(string pName, int pOID, TreeNode *parent);
	void addNodeObject(string pName, int pOID, TreeNode *parent, string pSyntax, string pAccess, string pDescription);
	TreeNode* findNode(string pName, TreeNode* node);
	TreeNode* findNodeSpecificOID(string pName, int pOID, TreeNode* node);
	string findNodeWord(string pName, TreeNode* node, string OID);
	TreeNode* findNodeOID(string pOID, TreeNode* node);
};
