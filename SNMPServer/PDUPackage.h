#pragma once
#include "Strings.h"

class PDUPackage
{
public:
	PDUPackage();
	~PDUPackage();

	string GeneratePacket(string pPacketType, map<string, string> pVarBindList, int & requestID, int & errorStatus, int & errorIndex, string community);
	void clearVectors();
	void addDataToVector(int dataType, int typeId, string dataValue, string keyword, string visibility, unsigned long long dataSize);

	//int requestID, errorStatus, errorIndex;
	//string packetType;
	//map<string, string> varBindList;

	vector<int> sequenceDataTypes, sequenceTypeIds;
	vector<string> sequenceDataValues, sequenceKeywords, sequenceVisibilities;
	vector<unsigned long long> sequenceDataSizes;

};

