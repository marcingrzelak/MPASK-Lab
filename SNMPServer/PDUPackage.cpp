#include "pch.h"
#include "PDUPackage.h"
#include "Strings.h"
#include "BERCoder.h"
#include "BERDecoder.h"
#include "CheckValue.h"
#include "TreeStructure.h"
#include "Exceptions.h"


PDUPackage::PDUPackage()
{
}

PDUPackage::~PDUPackage()
{
}

string PDUPackage::generatePacket(map<string, string> pVarBindList, int tag, int requestID, int errorStatus, int errorIndex, string pCommunity)
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
		if (itr != pVarBindList.begin()) //dodanie spacji pomiedzy elem. varbindlist
		{
			varBindEncoded += " ";
		}
		checkValue.setValueParameters(itr->first);
		addDataToVector(OBJECT_IDENTIFIER_TAG_NUMBER, 0, itr->first, "", "", checkValue.byteCount);
		if (itr->second == "")
		{
			addDataToVector(SEQUENCE_TAG_NUMBER, 0, coder.nullEncode(), "", "", 0);
		}
		else
		{
			addDataToVector(SEQUENCE_TAG_NUMBER, 0, itr->second, "", "", 0);//w itr->second mamy juz zakodowana wartosc z drzewa
		}

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
	addDataToVector(INTEGER_TAG_NUMBER, 0, to_string(errorStatus), "", "", checkValue.byteCount);

	//error index
	checkValue.setValueParameters(to_string(errorIndex));
	addDataToVector(INTEGER_TAG_NUMBER, 0, to_string(errorIndex), "", "", checkValue.byteCount);

	//varbind list
	addDataToVector(SEQUENCE_TAG_NUMBER, 0, varBindListEncoded, "", "", 0);

	if (tag == GET_RESPONSE_TAG_NUMBER)
	{
		PDUEncoded = coder.encode("", GET_RESPONSE_MY_TAG, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	}
	else if (tag == GET_REQUEST_TAG_NUMBER)
	{
		PDUEncoded = coder.encode("", GET_REQUEST_MY_TAG, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	}
	else if (tag == GET_NEXT_REQUEST_TAG_NUMBER)
	{
		PDUEncoded = coder.encode("", GET_NEXT_REQUEST_MY_TAG, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	}
	else if (tag == SET_REQUEST_TAG_NUMBER)
	{
		PDUEncoded = coder.encode("", SET_REQUEST_MY_TAG, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
	}
	else
	{
		throw eWrongTagGeneratePDU();
	}

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
	BERTree.root->printTree("", true);

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

string PDUPackage::packetHandler(string packet, Tree &OIDTree, vector<DataType>& pVDataType, vector<Index>& pVIndex, vector<Choice>& pVChoice, vector<Sequence>& pVSequence, vector<ObjectTypeSize>& pVObjectTypeSize)
{
	BERCoder coder;
	CheckValue checkValue;
	string st;
	analyzePacket(packet);

	map<string, string>::iterator itr;

	int i = 0;
	for (itr = varBindList.begin(); itr != varBindList.end(); ++itr)
	{

		TreeNode *node;
		if (itr->first.back() == '0')
		{
			st = itr->first.substr(0, itr->first.size() - 2);
		}
		else
		{
			st = itr->first;
		}

		node = OIDTree.findOID(st, OIDTree.root);

		if (node != nullptr)//znaleziono oid
		{
			if (stoi(packetType) == GET_REQUEST_TAG_NUMBER)
			{
				//pobranie wartosci z drzewa todo
				string value = "1";
				string encodedNode = coder.treeNodeEncoding(st, value, OIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
				if (encodedNode != "")
				{
					itr->second = encodedNode;
				}
			}

			if (stoi(packetType) == SET_REQUEST_TAG_NUMBER)
			{
				if (node->access == READ_ONLY)
				{
					errorIndex = i;
					errorStatus = PDU_ERR_READ_ONLY;
					return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
				}
				else
				{
					string test = coder.treeNodeEncoding(st, itr->second, OIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
					if (test != "")//mozna zakodowac nowa wartosc - poprawny typ i rozmiar
					{
						//ustawienie wartosci w drzewie todo
						itr->second = coder.nullEncode();
					}
					else
					{
						errorIndex = i;
						errorStatus = PDU_ERR_BAD_VALUE;
						return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
					}
				}
			}

			if (stoi(packetType) == GET_NEXT_REQUEST_TAG_NUMBER)
			{

			}
		}
		else
		{
			//error brak liscia o podanym oid
			errorIndex = i;
			errorStatus = PDU_ERR_NO_SUCH_NAME;
			return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
		}
		i++;
	}
	return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
}
