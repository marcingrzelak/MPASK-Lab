#include "pch.h"
#include "PDUPackage.h"
#include "Strings.h"
#include "BERCoder.h"
#include "BERDecoder.h"
#include "CheckValue.h"
#include "TreeStructure.h"


PDUPackage::PDUPackage()
{
}


PDUPackage::~PDUPackage()
{
}

string PDUPackage::generateResponsePacket(map<string, string> pVarBindList, int &requestID, int &errorStatus, int &errorIndex, string pCommunity)
{
	//GetResponse
	//serwer->client
	//pVarBindList
	//1)oid
	//2)zakodowana wartosc liscia o id oid

	BERCoder coder;
	CheckValue checkValue;
	string varBindEncoded, varBindListEncoded, PDUEncoded, messageEncoded;

	map<string, string>::iterator itr;

	for (itr = pVarBindList.begin(); itr != pVarBindList.end(); ++itr)
	{
		checkValue.setValueParameters(itr->first);
		addDataToVector(OBJECT_IDENTIFIER_TAG_NUMBER, 0, itr->first, "", "", checkValue.byteCount);
		addDataToVector(SEQUENCE_TAG_NUMBER, 0, itr->second, "", "", 0);//zakodowana juz wartosc z drzewa

		varBindEncoded += coder.encode("", SEQUENCE_TAG_NUMBER, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
		clearVectors();
	}

	clearVectors();
	addDataToVector(SEQUENCE_TAG_NUMBER, 0, varBindEncoded, "", "", 0);
	varBindListEncoded = coder.encode("", SEQUENCE_TAG_NUMBER, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	clearVectors();

	//request ID
	checkValue.setValueParameters(to_string(requestID));
	addDataToVector(INTEGER_TAG_NUMBER, 0, to_string(requestID), "", "", checkValue.byteCount);

	//error status
	checkValue.setValueParameters(to_string(errorStatus));
	addDataToVector(INTEGER_TAG_NUMBER, 0, "0", "", "", checkValue.byteCount);

	//error index
	checkValue.setValueParameters(to_string(errorIndex));
	addDataToVector(INTEGER_TAG_NUMBER, 0, "0", "", "", checkValue.byteCount);

	//varbind list
	addDataToVector(SEQUENCE_TAG_NUMBER, 0, varBindListEncoded, "", "", 0);

	PDUEncoded = coder.encode("", GET_RESPONSE_MY_TAG, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	clearVectors();

	//version
	addDataToVector(INTEGER_TAG_NUMBER, 0, "0", "", "", 1);

	//community string
	checkValue.setValueParameters(pCommunity);
	addDataToVector(OCTET_STRING_TAG_NUMBER, 0, pCommunity, "", "", checkValue.byteCount);

	//pdu
	addDataToVector(SEQUENCE_TAG_NUMBER, 0, PDUEncoded, "", "", 0);

	messageEncoded = coder.encode("", SEQUENCE_TAG_NUMBER, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	clearVectors();

	return messageEncoded;
}

void PDUPackage::analyzePacket(string packet)
{
	BERDecoder decoder;
	TreeBER BERTree;

	int index = 0;
	decoder.decode(packet, index, BERTree, nullptr);
	//BERTree.root->printTree("", true);

	community = BERTree.root->next[1]->value;
	requestID = stoi(BERTree.root->next[2]->next[0]->value);
	errorStatus = stoi(BERTree.root->next[2]->next[1]->value);
	errorIndex = stoi(BERTree.root->next[2]->next[2]->value);
	packetType = to_string(BERTree.root->next[2]->tagValue);
	for (size_t i = 0; i < BERTree.root->next[2]->next[3]->next.size(); i++)
	{
		varBindList.insert(pair<string, string>(BERTree.root->next[2]->next[3]->next[i]->next[0]->value, BERTree.root->next[2]->next[3]->next[i]->next[1]->value));
	}
}

void PDUPackage::clearVectors()
{
	sequenceDataTypes.clear();
	sequenceTypeIds.clear();
	sequenceDataValues.clear();
	sequenceKeywords.clear();
	sequenceVisibilities.clear();
	sequenceDataSizes.clear();
}

void PDUPackage::addDataToVector(int dataType, int typeId, string dataValue, string keyword, string visibility, unsigned long long dataSize)
{
	sequenceDataTypes.push_back(dataType);
	sequenceTypeIds.push_back(typeId);
	sequenceDataValues.push_back(dataValue);
	sequenceKeywords.push_back(keyword);
	sequenceVisibilities.push_back(visibility);
	sequenceDataSizes.push_back(dataSize);
}

int PDUPackage::packetHandler(string packet)
{
	analyzePacket(packet);

	map<string, string>::iterator itr;


	for (itr = varBindList.begin(); itr != varBindList.end(); ++itr)
	{
		
	}


	return 0;
}
