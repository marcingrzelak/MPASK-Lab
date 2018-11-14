#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"

int main()
{
	Tree OIDTree;

	//----------------------------------------------------------------------------------
		//import OBJECT IDENTIFIER - nowe OIDy
	rgx = Rgx.OBJECT_IDENTIFIER();
	sregex_iterator testIterator(mibFile.begin(), mibFile.end(), rgx);
	while (testIterator != endIterator)
	{
		if (string((*testIterator)[sObjectIdentifier.iParent]).size() != 0) //skladnia: directory OBJECT IDENTIFIER ::= { internet 1 }
		{
			sObjectIdentifier.name = (*testIterator)[sObjectIdentifier.iName];
			sObjectIdentifier.parent = (*testIterator)[sObjectIdentifier.iParent];
			sObjectIdentifier.oid = stoi((*testIterator)[sObjectIdentifier.iOid]);

			TreeNode* parent = OIDTree.findNode(sObjectIdentifier.parent, OIDTree.root);
			OIDTree.addNode(sObjectIdentifier.name, sObjectIdentifier.oid, parent);
		}
		else // skladnia: internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }
		{
			sObjectIdentifierExtended.name = (*testIterator)[sObjectIdentifierExtended.iName];
			sObjectIdentifierExtended.parent = (*testIterator)[sObjectIdentifierExtended.iParent];
			sObjectIdentifierExtended.parent2 = (*testIterator)[sObjectIdentifierExtended.iParent2];
			sObjectIdentifierExtended.oidParent2 = stoi((*testIterator)[sObjectIdentifierExtended.iOidParent2]);
			sObjectIdentifierExtended.parent3 = (*testIterator)[sObjectIdentifierExtended.iParent3];
			sObjectIdentifierExtended.oidParent3 = stoi((*testIterator)[sObjectIdentifierExtended.iOidParent3]);
			sObjectIdentifierExtended.oid = stoi((*testIterator)[sObjectIdentifierExtended.iOid]);

			TreeNode* node = OIDTree.findNode(sObjectIdentifierExtended.parent, OIDTree.root);
			TreeNode* node2 = OIDTree.findNodeSpecificOID(sObjectIdentifierExtended.parent2, sObjectIdentifierExtended.oidParent2, node);
			TreeNode* node3 = OIDTree.findNodeSpecificOID(sObjectIdentifierExtended.parent3, sObjectIdentifierExtended.oidParent3, node2);

			OIDTree.addNode(sObjectIdentifierExtended.name, sObjectIdentifierExtended.oid, node3);

		}
		++testIterator;
	}

	rgx = Rgx.OBJECT_TYPE();
	sregex_iterator test2Iterator(mibFile.begin(), mibFile.end(), rgx);
	while (test2Iterator != endIterator)
	{
		sObjectType.name = (*test2Iterator)[sObjectType.iName];
		sObjectType.syntax = (*test2Iterator)[sObjectType.iSyntax];
		sObjectType.access = (*test2Iterator)[sObjectType.iAccess];
		sObjectType.status = (*test2Iterator)[sObjectType.iStatus];
		sObjectType.description = (*test2Iterator)[sObjectType.iDescription];
		sObjectType.parent = (*test2Iterator)[sObjectType.iParent];
		sObjectType.oid = stoi((*test2Iterator)[sObjectType.iOid]);

		TreeNode* parent = OIDTree.findNode(sObjectType.parent, OIDTree.root);
		OIDTree.addNodeObject(sObjectType.name, sObjectType.oid, parent, sObjectType.syntax, sObjectType.access, sObjectType.description);
	}

	OIDTree.root->printTree("", true);
	return(0);
}