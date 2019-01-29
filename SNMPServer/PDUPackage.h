#pragma once
#include "Strings.h"
#include "TreeStructure.h"
#include "DataStructures.h"

class PDUPackage
{
public:
	PDUPackage();
	~PDUPackage();

	string generateResponsePacket(map<string, string> pVarBindList, int & requestID, int & errorStatus, int & errorIndex, string pCommunity);
	void analyzePacket(string packet);
	void clearVectors();
	void addDataToVector(int dataType, int typeId, string dataValue, string keyword, string visibility, unsigned long long dataSize);
	string packetHandler(string packet, Tree &OIDTree, vector<DataType>& pVDataType, vector<Index>& pVIndex, vector<Choice>& pVChoice, vector<Sequence>& pVSequence, vector<ObjectTypeSize>& pVObjectTypeSize);

	int requestID, errorStatus, errorIndex;
	string packetType, community;
	map<string, string> varBindList;

	vector<int> sequenceDataTypes, sequenceTypeIds;
	vector<string> sequenceDataValues, sequenceKeywords, sequenceVisibilities;
	vector<unsigned long long> sequenceDataSizes;

};

