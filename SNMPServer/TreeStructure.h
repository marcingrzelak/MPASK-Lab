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
	TreeNode* findOID(string pOID, TreeNode* node);
};

class TreeNodeBER
{
public:
	TreeNodeBER();
	~TreeNodeBER();

	string classValue, classConstructedValue, complexity, value;
	unsigned int tagValue, tagConstructedValue;
	unsigned long long lengthValue;
	vector <TreeNodeBER*> next;

	void printTree(string indent, bool last);
};

class TreeBER
{
public:
	TreeBER();
	~TreeBER();

	TreeNodeBER *root;

	void addRoot(string pClassValue, string pClassConstructedValue, string pComplexity, string pValue, unsigned int pTagValue, unsigned int pTagConstructedValue, unsigned long long pLengthValue);
	TreeNodeBER* addNode(TreeNodeBER * parent, string pClassValue, string pClassConstructedValue, string pComplexity, string pValue, unsigned int pTagValue, unsigned int pTagConstructedValue, unsigned long long pLengthValue);
};