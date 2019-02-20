#pragma once
#include "Strings.h"
#include "TreeStructure.h"
#include "DataStructures.h"

class PDUPackage
{
public:
	PDUPackage();
	~PDUPackage();

	string generatePacket(map<string, string> pVarBindList, int tag, int requestID, int errorStatus, int errorIndex, string pCommunity);
	void analyzePacket(string packet, bool printTree);
	void clearVectors();
	void addDataToVector(int dataType, int typeId, string dataValue, string keyword, string visibility, unsigned long long dataSize);
	string packetHandler(string packet, Tree &OIDTree, vector<DataType>& pVDataType, vector<Index>& pVIndex, vector<Choice>& pVChoice, vector<Sequence>& pVSequence, vector<ObjectTypeSize>& pVObjectTypeSize);
	void printResponse();

	int requestID, errorStatus, errorIndex;
	string packetType, community;
	map<string, string> varBindList, varBindListGetNext;

	vector<int> sequenceDataTypes, sequenceTypeIds;
	vector<string> sequenceDataValues, sequenceKeywords, sequenceVisibilities;
	vector<unsigned long long> sequenceDataSizes;

};

