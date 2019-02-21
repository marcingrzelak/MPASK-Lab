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
		coder.clearIdentifier();
		coder.clearLength();
		coder.clearValue();
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

void PDUPackage::analyzePacket(string packet, bool printTree)
{
	BERDecoder decoder;
	TreeBER BERTree;

	int index = 0;
	decoder.decode(packet, index, BERTree, nullptr);
	if (printTree)
	{
		BERTree.root->printTree("", true);
	}

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
	BERCoder encoder;
	CheckValue checkValue;
	string oid;
	analyzePacket(packet, true);

	map<string, string>::iterator itr;

	int i = 0;
	for (itr = varBindList.begin(); itr != varBindList.end(); ++itr)
	{
		TreeNode *node;
		if (itr->first.back() == '0')
		{
			oid = itr->first.substr(0, itr->first.size() - 2);
		}
		else
		{
			oid = itr->first;
		}

		node = OIDTree.findOID(oid, OIDTree.root);

		if (node != nullptr)//znaleziono oid
		{
			string value;
			if (stoi(packetType) == GET_REQUEST_TAG_NUMBER)
			{
				value = getValue(oid, node->syntax);

				string encodedNode;
				try
				{
					encodedNode = encoder.treeNodeEncoding(oid, value, OIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
				}
				catch (Exceptions &)
				{
					errorIndex = i;
					errorStatus = PDU_ERR_BAD_VALUE_CODE;
					while (itr != varBindList.end())
					{
						itr->second = encoder.nullEncode();
						itr++;
					}
					return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
				}
				if (encodedNode != "")
				{
					itr->second = encodedNode;
				}
			}

			else if (stoi(packetType) == GET_NEXT_REQUEST_TAG_NUMBER)
			{
				string getNextOID;
				try
				{
					getNextOID = OIDTree.findNextNode(oid, OIDTree.root);
				}
				catch (Exceptions &e)
				{
					e.message();
					throw ePDU();
				}

				TreeNode *getNextNode;
				getNextNode = OIDTree.findOID(getNextOID, OIDTree.root);
				value = getValue(getNextOID, getNextNode->syntax);

				string encodedNode;
				try
				{
					encodedNode = encoder.treeNodeEncoding(getNextOID, value, OIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
				}
				catch (Exceptions &)
				{
					errorIndex = i;
					errorStatus = PDU_ERR_BAD_VALUE_CODE;
					while (itr != varBindList.end())
					{
						itr->second = encoder.nullEncode();
						itr++;
					}
					return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
				}
				if (encodedNode != "")
				{
					varBindListGetNext.insert(pair<string, string>(getNextOID, encodedNode));
				}
			}

			else if (stoi(packetType) == SET_REQUEST_TAG_NUMBER)
			{
				if (node->access == READ_ONLY)
				{
					errorIndex = i;
					errorStatus = PDU_ERR_READ_ONLY_CODE;
					while (itr != varBindList.end())
					{
						itr->second = encoder.nullEncode();
						itr++;
					}
					return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
				}
				else
				{
					string test;
					try
					{
						test = encoder.treeNodeEncoding(oid, itr->second, OIDTree, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
					}
					catch (Exceptions &)
					{
						errorIndex = i;
						errorStatus = PDU_ERR_BAD_VALUE_CODE;
						while (itr != varBindList.end())
						{
							itr->second = encoder.nullEncode();
							itr++;
						}
						return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
					}
					if (test != "")//mozna zakodowac nowa wartosc - poprawny typ i rozmiar
					{
						try
						{
							setValue(oid, itr->second);
						}
						catch (Exceptions &e)
						{
							errorIndex = i;
							errorStatus = PDU_ERR_GEN_ERR_CODE;
							while (itr != varBindList.end())
							{
								itr->second = encoder.nullEncode();
								itr++;
							}
							e.message();
							return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
						}

						checkValue.setValueParameters(itr->second);
						if (checkValue.isValueNumber)
						{
							itr->second = (encoder.encode(itr->second, INTEGER_TAG_NUMBER, 0, checkValue.byteCount, "", "", checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities));
						}
						else if (checkValue.isObjectIdentifier)
						{
							itr->second = (encoder.encode(itr->second, OBJECT_IDENTIFIER_TAG_NUMBER, 0, checkValue.byteCount, "", "", checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities));
						}
						else
						{
							itr->second = (encoder.encode(itr->second, OCTET_STRING_TAG_NUMBER, 0, checkValue.byteCount, "", "", checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities));
						}
					}
				}
			}
		}
		else
		{
			//error brak liscia o podanym oid
			errorIndex = i;
			errorStatus = PDU_ERR_NO_SUCH_NAME_CODE;
			while (itr != varBindList.end())
			{
				itr->second = encoder.nullEncode();
				itr++;
			}
			return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
		}
		i++;
	}
	if (stoi(packetType) == GET_NEXT_REQUEST_TAG_NUMBER)
	{
		return generatePacket(varBindListGetNext, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
	}
	else
	{
		return generatePacket(varBindList, GET_RESPONSE_TAG_NUMBER, requestID, errorStatus, errorIndex, community);
	}
}

void PDUPackage::printResponse(int errIndex, int errStatus)
{
	map<string, string>::iterator itr;
	itr = varBindList.begin();
	int i = 0;

	if (errStatus == PDU_ERR_NO_ERROR_CODE)
	{
		while (itr != varBindList.end())
		{
			cout << itr->first << " = " << itr->second << endl;
			itr++;
		}
	}
	else
	{
		while (itr != varBindList.end() && i < errIndex)
		{
			if (itr->second != "")
			{
				cout << itr->first << " = " << itr->second << endl;
			}
			itr++;
			i++;
		}
	}

	cout << endl;
}

string PDUPackage::getValue(string oid, string syntax)
{
	string value;
	if (oid == SYS_NAME_OID)
	{
		constexpr auto INFO_BUFFER_SIZE = 32767;
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;

		GetComputerName(infoBuf, &bufCharCount);
		wstring test(&infoBuf[0]);
		string test2(test.begin(), test.end());
		value = test2;
	}
	else if (oid == SYS_UP_TIME_OID)
	{
		auto uptime = chrono::milliseconds(GetTickCount64());
		value = to_string(uptime.count());
	}
	else
	{
		if (syntax.find(IDENTIFIER_TYPE_INTEGER) != string::npos)
		{
			value = "120";
		}
		else if (syntax.find(IDENTIFIER_TYPE_OCTET_STRING) != string::npos)
		{
			value = "test";
		}
		else if (syntax.find(IDENTIFIER_TYPE_NULL) != string::npos)
		{
			value = "";
		}
		else if (syntax.find(IDENTIFIER_TYPE_OBJECT_IDENTIFIER) != string::npos)
		{
			value = "1.3.6.1.4.1.311.1.1.3.1.1";
		}
		else
		{
			value = "inne";
		}
	}
	return value;
}

void PDUPackage::setValue(string oid, string value)
{
	if (oid == SYS_NAME_OID)
	{
		wstring stemp = wstring(value.begin(), value.end());
		LPCWSTR sw = stemp.c_str();
		bool bSuccess = SetComputerName(sw);
		if (bSuccess == 0)
		{
			throw eSetValueNotChanged();
		}
		else
		{
			return;
		}
	}
	else
	{
		throw eSetValueNotImplemented();
	}
}
