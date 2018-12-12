#include "pch.h"
#include "BERCoder.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "Regex.h"
#include "CheckValue.h"


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

string BERCoder::encode(string pValue, int pType, int pTypeID, long long pByteCount, string pKeyword, string pVisibility, vector<string> pSequenceValues, vector<int> pSequenceTypes, vector<int> pSequenceTypeID, vector<long long> pSequenceBytesCount, vector<string> pSequenceKeywords, vector<string> pSequenceVisibilities)
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
	if (pType != SEQUENCE_TAG_NUMBER)
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
			if (i>0)
			{
				allSequenceValuesEncoded += " ";
			}
			allSequenceValuesEncoded += encode(pSequenceValues.at(i), pSequenceTypes.at(i), pSequenceTypeID.at(i), pSequenceBytesCount.at(i), pSequenceKeywords.at(i), pSequenceVisibilities.at(i), pSequenceValues, pSequenceTypes, pSequenceTypeID, pSequenceBytesCount, pSequenceKeywords, pSequenceVisibilities);
		}
		string allSequenceValuesEncodedString = allSequenceValuesEncoded;
		allSequenceValuesEncodedString.erase(remove_if(allSequenceValuesEncodedString.begin(), allSequenceValuesEncodedString.end(), isspace), allSequenceValuesEncodedString.end());
		
		//todo prechodzenie wartosci z ostatniego elementu sekwencji
		clearIdentifier();
		clearLength();
		clearValue();

		this->setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_CONSTRUCTED, SEQUENCE_TAG_NUMBER);
		this->setLength(allSequenceValuesEncodedString.size() / 2);

		encodedValue = concatAllValues(false);
		return encodedValue + " " + allSequenceValuesEncoded;
	}
	return "";
}