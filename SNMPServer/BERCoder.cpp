#include "pch.h"
#include "BERCoder.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "Regex.h"
#include "CheckValue.h"
#include "Exceptions.h"

BERCoder::BERCoder()
{
}


BERCoder::~BERCoder()
{
}

void BERCoder::setIdentifier(string pClass, string pComplexity, string pType)
{
	identifier.setClass(pClass);
	identifier.setComplexity(pComplexity);
	identifier.setTagFromType(pType);
}

void BERCoder::setIdentifier(string pClass, string pComplexity, unsigned int pTag)
{
	identifier.setClass(pClass);
	identifier.setComplexity(pComplexity);
	identifier.setTag(pTag);
}

void BERCoder::clearIdentifier()
{
	identifier.firstOctet = 0;
	identifier.longOctet.clear();
}

void BERCoder::setLength(unsigned long long pLength)
{
	length.setDefinedForm(pLength);
}

void BERCoder::clearLength()
{
	length.firstOctet = 0;
	length.longOctet.clear();
}

void BERCoder::setValue(string pValue)
{
	value.setValue(pValue);
}

void BERCoder::setValue(long long pValue, int pLength)
{
	value.setValue(pValue, pLength);
}

unsigned long long BERCoder::setValue(vector<string> pObjectIdentifierSubidentifiers)
{
	value.setValue(pObjectIdentifierSubidentifiers);
	return value.octet.size();
}

void BERCoder::clearValue()
{
	value.octet.clear();
	value.octetWord.clear();
}

string BERCoder::concatAllValues(bool pIsValueNumber)
{
	stringstream concatedValue;

	concatedValue << setfill('0') << setw(2) << hex << static_cast<int>(identifier.firstOctet) << " ";
	for (size_t i = 0; i < identifier.longOctet.size(); i++)
	{
		concatedValue << setfill('0') << setw(2) << hex << static_cast<int>(identifier.longOctet.at(i)) << " ";
	}

	concatedValue << setfill('0') << setw(2) << hex << static_cast<int>(length.firstOctet);
	for (size_t i = 0; i < length.longOctet.size(); i++)
	{
		concatedValue << " " << setfill('0') << setw(2) << hex << static_cast<int>(length.longOctet.at(i));
	}

	if (pIsValueNumber)
	{
		for (size_t i = 0; i < value.octet.size(); i++)
		{
			concatedValue << " " << setfill('0') << setw(2) << hex << static_cast<int>(value.octet.at(i));
		}
	}
	else
	{
		for (size_t i = 0; i < value.octetWord.size(); i++)
		{
			concatedValue << " " << setfill('0') << setw(2) << hex << static_cast<int>(value.octetWord.at(i));
		}
	}
	return concatedValue.str();
}

string BERCoder::nullEncode()
{
	setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, IDENTIFIER_TYPE_NULL);
	setLength(0);
	string returnString = concatAllValues(false);
	return returnString;
}

string BERCoder::encode(string pValue, int pType, int pTypeID, unsigned long long pByteCount, string pKeyword, string pVisibility, vector<string> pSequenceValues, vector<int> pSequenceTypes, vector<int> pSequenceTypeID, vector<unsigned long long> pSequenceBytesCount, vector<string> pSequenceKeywords, vector<string> pSequenceVisibilities)
{
	clearIdentifier();
	clearLength();
	clearValue();

	string typeName, encodedValue;
	vector<string> pObjectIdentifierSubidentifiers;
	long long valueINT;

	if (pType == INTEGER_TAG_NUMBER)
	{
		valueINT = stoll(pValue);
		typeName = IDENTIFIER_TYPE_INTEGER;
	}

	else if (pType == OBJECT_IDENTIFIER_TAG_NUMBER)
	{
		stringstream test(pValue);
		string segment;

		while (std::getline(test, segment, '.'))
		{
			pObjectIdentifierSubidentifiers.push_back(segment);
		}
		typeName = IDENTIFIER_TYPE_OBJECT_IDENTIFIER;
	}

	else if (pType == OCTET_STRING_TAG_NUMBER)
	{
		typeName = IDENTIFIER_TYPE_OCTET_STRING;
	}

	else if (pType == NULL_TAG_NUMBER)
	{
		encodedValue = nullEncode();
		return encodedValue;
	}

	//kodowanie BER
	if (pType != SEQUENCE_TAG_NUMBER && pType != GET_REQUEST_MY_TAG && pType != GET_RESPONSE_MY_TAG && pType != SET_REQUEST_MY_TAG && pType != GET_NEXT_REQUEST_MY_TAG)
	{
		if (pKeyword.size() == 0) //kodowanie uniwersalne
		{
			this->setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, typeName);
			if (!(pType == OBJECT_IDENTIFIER_TAG_NUMBER))
			{
				this->setLength(pByteCount);
			}

			if (pType == INTEGER_TAG_NUMBER)
			{
				this->setValue(valueINT, pByteCount);
			}
			else if (pType == OBJECT_IDENTIFIER_TAG_NUMBER)
			{
				unsigned long long tmp = this->setValue(pObjectIdentifierSubidentifiers);
				this->setLength(tmp);
			}
			else
			{
				this->setValue(pValue);
			}
			encodedValue = concatAllValues(pType == INTEGER_TAG_NUMBER || pType == OBJECT_IDENTIFIER_TAG_NUMBER);
			return encodedValue;
		}
		else if (pKeyword == DATATYPE_KEYWORD_IMPLICIT)
		{
			if (pVisibility.size() == 0)//brak widocznosci klasy
			{
				this->setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_PRIMITIVE, pTypeID);
			}
			else
			{
				this->setIdentifier(pVisibility, IDENTIFIER_COMPLEXITY_PRIMITIVE, pTypeID);
			}

			if (!(pType == OBJECT_IDENTIFIER_TAG_NUMBER))
			{
				this->setLength(pByteCount);
			}

			if (pType == INTEGER_TAG_NUMBER)
			{
				this->setValue(valueINT, pByteCount);
			}
			else if (pType == OBJECT_IDENTIFIER_TAG_NUMBER)
			{
				unsigned long long tmp = this->setValue(pObjectIdentifierSubidentifiers);
				this->setLength(tmp);
			}
			else
			{
				this->setValue(pValue);
			}
			encodedValue = concatAllValues(pType == INTEGER_TAG_NUMBER || pType == OBJECT_IDENTIFIER_TAG_NUMBER);
			return encodedValue;
		}
		else if (pKeyword == DATATYPE_KEYWORD_EXPLICIT)
		{
			if (pVisibility.size() == 0)//brak widocznosci klasy
			{
				this->setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pTypeID);
			}
			else
			{
				this->setIdentifier(pVisibility, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pTypeID);
			}

			BERCoder temp;

			temp.setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, typeName);
			if (!(pType == OBJECT_IDENTIFIER_TAG_NUMBER))
			{
				temp.setLength(pByteCount);
			}
			if (pType == INTEGER_TAG_NUMBER)
			{
				temp.setValue(valueINT, pByteCount);
			}
			else if (pType == OBJECT_IDENTIFIER_TAG_NUMBER)
			{
				unsigned long long tmp = temp.setValue(pObjectIdentifierSubidentifiers);
				temp.setLength(tmp);
			}
			else
			{
				temp.setValue(pValue);
			}
			string explicitValue = temp.concatAllValues(pType == INTEGER_TAG_NUMBER || pType == OBJECT_IDENTIFIER_TAG_NUMBER);
			string explicitString = explicitValue;

			explicitString.erase(remove_if(explicitString.begin(), explicitString.end(), isspace), explicitString.end());
			unsigned int explicitLength = explicitString.size() / 2;

			this->setLength(explicitLength);

			encodedValue = concatAllValues(false);
			return encodedValue + " " + explicitValue;
		}
	}
	else
	{
		string allSequenceValuesEncoded = "";

		for (size_t i = 0; i < pSequenceValues.size(); i++)
		{
			if (i > 0)
			{
				allSequenceValuesEncoded += " ";
			}

			if (pSequenceTypes.at(i) != SEQUENCE_TAG_NUMBER && pSequenceTypes.at(i) != GET_REQUEST_MY_TAG && pSequenceTypes.at(i) != GET_RESPONSE_MY_TAG && pSequenceTypes.at(i) != SET_REQUEST_MY_TAG && pSequenceTypes.at(i) != GET_NEXT_REQUEST_MY_TAG)
			{
				allSequenceValuesEncoded += encode(pSequenceValues.at(i), pSequenceTypes.at(i), pSequenceTypeID.at(i), pSequenceBytesCount.at(i), pSequenceKeywords.at(i), pSequenceVisibilities.at(i), pSequenceValues, pSequenceTypes, pSequenceTypeID, pSequenceBytesCount, pSequenceKeywords, pSequenceVisibilities);
			}
			else
			{
				allSequenceValuesEncoded += pSequenceValues.at(i);
			}
		}
		string allSequenceValuesEncodedString = allSequenceValuesEncoded;
		allSequenceValuesEncodedString.erase(remove_if(allSequenceValuesEncodedString.begin(), allSequenceValuesEncodedString.end(), isspace), allSequenceValuesEncodedString.end());

		//todo przechodzenie wartosci z ostatniego elementu sekwencji
		clearIdentifier();
		clearLength();
		clearValue();

		if (pType == SEQUENCE_TAG_NUMBER)
		{
			this->setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_CONSTRUCTED, SEQUENCE_TAG_NUMBER);
		}
		else if (pType == GET_REQUEST_MY_TAG)
		{
			this->setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, GET_REQUEST_TAG_NUMBER);
		}
		else if (pType == GET_RESPONSE_MY_TAG)
		{
			this->setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, GET_RESPONSE_TAG_NUMBER);
		}
		else if (pType == SET_REQUEST_MY_TAG)
		{
			this->setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, SET_REQUEST_TAG_NUMBER);
		}
		else if (pType == GET_NEXT_REQUEST_MY_TAG)
		{
			this->setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, GET_NEXT_REQUEST_TAG_NUMBER);
		}
		this->setLength(allSequenceValuesEncodedString.size() / 2);

		encodedValue = concatAllValues(false);
		return encodedValue + " " + allSequenceValuesEncoded;
	}
	return "";
}

string BERCoder::treeNodeEncoding(string nodeNameOrOID, string valueToEncode, Tree &pOIDTree, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize)
{
	string keyword = "", visibility = "";
	CheckValue checkValue;
	int encodingType = 0, type = 0, typeID = 0, encodingDataType = 0;
	bool askForValues = false;
	unsigned long long byteCount = 0;
	TreeNode* node;

	if (nodeNameOrOID.length() == 0)
	{
		askForValues = true;
	}

	if (askForValues)
	{
		cout << endl << "Podaj nazwe lub OID liscia:" << endl;
		cin >> nodeNameOrOID;
	}

	try
	{
		node = pOIDTree.findOID(nodeNameOrOID, pOIDTree.root);
	}
	catch (Exceptions &e)
	{
		e.message();
		throw eEncoder();
	}

	if (node == pOIDTree.root)
	{
		node = pOIDTree.findNode(nodeNameOrOID, pOIDTree.root);
	}
	if (node != nullptr && node != pOIDTree.root)
	{
		if (node->syntax != IDENTIFIER_TYPE_NULL && askForValues)
		{
			cout << "Podaj wartosc do zakodowania:" << endl;
			cin >> valueToEncode;
		}

		int result = checkValue.checkValue(valueToEncode, node, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
		if (result == 0)
		{
			if (checkValue.typeDataType != 0)
			{
				typeID = pVDataType.at(checkValue.indexDataType).typeID;
				byteCount = checkValue.byteCount;
				keyword = pVDataType.at(checkValue.indexDataType).keyword;
				visibility = pVDataType.at(checkValue.indexDataType).visibility;
				type = checkValue.typeDataType;
			}
			else if (checkValue.type != 0)
			{
				typeID = NULL;
				byteCount = checkValue.byteCount;
				keyword = "";
				visibility = "";
				type = checkValue.type;
			}
			else
			{
				typeID = NULL;
				byteCount = NULL;
				keyword = "";
				visibility = "";
				type = SEQUENCE_TAG_NUMBER;
			}

			string encodedValue = encode(valueToEncode, type, typeID, byteCount, keyword, visibility, checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities);
			return encodedValue;
		}
		else if (result == -1)
		{
			throw eEncodingWrongType();
		}
		else if (result == -2)
		{
			throw eEncodingWrongSize();
		}
		else
		{
			throw eEncoder();
		}
	}
	else
	{
		throw eNodeNotFound();
	}
}

string BERCoder::anyValueEncoding(string encodedValue, bool isSequence)
{
	int encodingDataType = 0, type = 0, typeID = 0;
	unsigned long long byteCount = 0;
	string valueToEncode = "", syntax = "", keyword = "", visibility = "";
	CheckValue checkValue;

	if (!isSequence)
	{
		cout << "Wybierz typ danych do zakodowania" << endl << "2 - INTEGER" << endl << "4 - OCTET STRING" << endl << "5 - NULL" << endl << "6 - OBJECT IDENTIFIER" << endl << "16 - SEQUENCE" << endl;
		cin >> encodingDataType;
		if (encodingDataType != INTEGER_TAG_NUMBER && encodingDataType != OCTET_STRING_TAG_NUMBER && encodingDataType != NULL_TAG_NUMBER && encodingDataType != OBJECT_IDENTIFIER_TAG_NUMBER && encodingDataType != SEQUENCE_TAG_NUMBER)
		{
			throw eAnyValueEncodingWrongType();
		}
	}

	if (encodingDataType != SEQUENCE_TAG_NUMBER && !isSequence)
	{
		if (encodingDataType != NULL_TAG_NUMBER)
		{
			cout << "Podaj dane do zakodowania" << endl;
			cin >> valueToEncode;
		}

		switch (encodingDataType)
		{
		case INTEGER_TAG_NUMBER:
			type = INTEGER_TAG_NUMBER;
			syntax = IDENTIFIER_TYPE_INTEGER;
			break;
		case OCTET_STRING_TAG_NUMBER:
			type = OCTET_STRING_TAG_NUMBER;
			syntax = IDENTIFIER_TYPE_OCTET_STRING;
			break;
		case NULL_TAG_NUMBER:
			type = NULL_TAG_NUMBER;
			syntax = IDENTIFIER_TYPE_NULL;
			break;
		case OBJECT_IDENTIFIER_TAG_NUMBER:
			type = OBJECT_IDENTIFIER_TAG_NUMBER;
			syntax = IDENTIFIER_TYPE_OBJECT_IDENTIFIER;
			break;
		default:
			type = 0;
			syntax = "";
			break;
		}

		checkValue.setValueParameters(valueToEncode);
		int cVTreturned = checkValue.checkValueType(valueToEncode, syntax);
		if (cVTreturned == 0)//typ ok
		{
			int cVSreturned = checkValue.checkValueSize();

			if (cVSreturned == 0)//rozmiar ok
			{
				byteCount = checkValue.byteCount;

				encodedValue += encode(valueToEncode, type, typeID, byteCount, keyword, visibility, checkValue.sequenceValues, checkValue.sequenceDefaultTypes, checkValue.sequenceTypeID, checkValue.sequenceBytesCount, checkValue.sequenceKeywords, checkValue.sequenceVisibilities);
				return encodedValue;
			}
			else
			{
				throw eEncodingWrongSize();
			}
		}
		else
		{
			throw eEncodingWrongType();
		}
	}

	else
	{
		int sequenceSize = 0, sequenceDataType;
		vector<int> sequenceDataTypes, sequenceTypeIds;
		vector<string> sequenceDataValues, sequenceKeywords, sequenceVisibilities;
		vector<unsigned long long> sequenceDataSizes;

		cout << "Podaj rozmiar sekwencji" << endl;
		cin >> sequenceSize;
		cout << "Podaj typy danych do zakodowania" << endl << "2 - INTEGER" << endl << "4 - OCTET STRING" << endl << "5 - NULL" << endl << "6 - OBJECT IDENTIFIER" << endl << "16 - SEQUENCE" << endl;
		for (size_t i = 0; i < sequenceSize; i++)
		{
			cin >> sequenceDataType;
			if (sequenceDataType != INTEGER_TAG_NUMBER && sequenceDataType != OCTET_STRING_TAG_NUMBER && sequenceDataType != NULL_TAG_NUMBER && sequenceDataType != OBJECT_IDENTIFIER_TAG_NUMBER && sequenceDataType != SEQUENCE_TAG_NUMBER)
			{
				throw eAnyValueEncodingWrongType();
			}
			sequenceDataTypes.push_back(sequenceDataType);
		}

		if (sequenceSize > 1 && syntax != IDENTIFIER_TYPE_NULL)
		{
			cout << "Podaj dane do zakodowania" << endl;
		}

		for (size_t i = 0; i < sequenceSize; i++)
		{
			if (sequenceDataTypes.at(i) == SEQUENCE_TAG_NUMBER)
			{
				string encodedSequence = anyValueEncoding(encodedValue, true);
				sequenceDataValues.push_back(encodedSequence);
				sequenceTypeIds.push_back(0);
				sequenceDataSizes.push_back(0);
				sequenceKeywords.push_back("");
				sequenceVisibilities.push_back("");
			}
			else
			{
				switch (sequenceDataTypes.at(i))
				{
				case INTEGER_TAG_NUMBER:
					syntax = IDENTIFIER_TYPE_INTEGER;
					break;
				case OCTET_STRING_TAG_NUMBER:
					syntax = IDENTIFIER_TYPE_OCTET_STRING;
					break;
				case NULL_TAG_NUMBER:
					syntax = IDENTIFIER_TYPE_NULL;
					break;
				case OBJECT_IDENTIFIER_TAG_NUMBER:
					syntax = IDENTIFIER_TYPE_OBJECT_IDENTIFIER;
					break;
				default:
					syntax = "";
					break;
				}

				if (syntax != IDENTIFIER_TYPE_NULL)
				{
					cin >> valueToEncode;
				}

				checkValue.setValueParameters(valueToEncode);
				int cVTreturned = checkValue.checkValueType(valueToEncode, syntax);
				if (cVTreturned == 0)//typ ok
				{
					int cVSreturned = checkValue.checkValueSize();

					if (cVSreturned == 0)//rozmiar ok
					{
						byteCount = checkValue.byteCount;
						sequenceDataValues.push_back(valueToEncode);
						sequenceTypeIds.push_back(typeID);
						sequenceDataSizes.push_back(byteCount);
						sequenceKeywords.push_back(keyword);
						sequenceVisibilities.push_back(visibility);
					}
					else
					{
						throw eEncodingWrongSize();
					}
				}
				else
				{
					throw eEncodingWrongType();
				}
			}
		}

		encodedValue += encode("", SEQUENCE_TAG_NUMBER, typeID, byteCount, keyword, visibility, sequenceDataValues, sequenceDataTypes, sequenceTypeIds, sequenceDataSizes, sequenceKeywords, sequenceVisibilities);
		return encodedValue;
	}
}