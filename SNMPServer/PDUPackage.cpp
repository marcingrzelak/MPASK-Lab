#include "pch.h"
#include "PDUPackage.h"
#include "Strings.h"
#include "BERCoder.h"
#include "CheckValue.h"


PDUPackage::PDUPackage()
{
}


PDUPackage::~PDUPackage()
{
}

string PDUPackage::GeneratePacket(string pPacketType, map<string, string> pVarBindList, int &requestID, int &errorStatus, int &errorIndex, string community)
{
	//GetRequest
	//client->serwer

	//GetResponse
	//serwer->client

	//SetRequest
	//client->serwer

	//GetNextRequest
	//client->serwer


	BERCoder coder;
	CheckValue checkValue;
	string varBindEncoded, varBindListEncoded, PDUEncoded, messageEncoded;

	map<string, string>::iterator itr;

	if (pPacketType == PACKET_TYPE_GET_REQUEST)
	{
		for (itr = pVarBindList.begin(); itr != pVarBindList.end(); ++itr)
		{
			checkValue.setValueParameters(itr->first);
			int cVTreturned = checkValue.checkValueType(itr->first, IDENTIFIER_TYPE_OBJECT_IDENTIFIER);
			if (cVTreturned == 0)//typ ok
			{
				int cVSreturned = checkValue.checkValueSize();

				if (cVSreturned == 0)//rozmiar ok
				{
					addDataToVector(OBJECT_IDENTIFIER_TAG_NUMBER, 0, itr->first, "", "", checkValue.byteCount);
				}
				else
				{
					//error
				}
			}

			checkValue.setValueParameters(itr->second);
			cVTreturned = checkValue.checkValueType(itr->second, IDENTIFIER_TYPE_NULL);
			if (cVTreturned == 0)//typ ok
			{
				int cVSreturned = checkValue.checkValueSize();

				if (cVSreturned == 0)//rozmiar ok
				{
					addDataToVector(NULL_TAG_NUMBER, 0, itr->second, "", "", checkValue.byteCount);
				}
				else
				{
					//error
				}
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
		addDataToVector(INTEGER_TAG_NUMBER, 0, "0", "", "", 1);

		//error index
		addDataToVector(INTEGER_TAG_NUMBER, 0, "0", "", "", 1);

		//varbind list
		addDataToVector(SEQUENCE_TAG_NUMBER, 0, varBindListEncoded, "", "", 0);

		PDUEncoded = coder.encode("", SEQUENCE_TAG_NUMBER, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
		clearVectors();

		//version
		addDataToVector(INTEGER_TAG_NUMBER, 0, "0", "", "", 1);

		//community string
		checkValue.setValueParameters(community);
		addDataToVector(OCTET_STRING_TAG_NUMBER, 0, community, "", "", checkValue.byteCount);

		//pdu
		addDataToVector(SEQUENCE_TAG_NUMBER, 0, PDUEncoded, "", "", 0);

		messageEncoded = coder.encode("", SEQUENCE_TAG_NUMBER, 0, 0, "", "", sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
		clearVectors();

		return messageEncoded;
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


//varBindList.insert(pair<string, string>("1", "40"));
