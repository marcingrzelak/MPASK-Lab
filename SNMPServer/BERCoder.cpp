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
			setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, typeName);
			if (!(pType == OBJECT_IDENTIFIER_TAG_NUMBER))
			{
				setLength(pByteCount);
			}

			if (pType == INTEGER_TAG_NUMBER)
			{
				setValue(valueINT, pByteCount);
			}
			else if (pType == OBJECT_IDENTIFIER_TAG_NUMBER)
			{
				unsigned long long tmp = setValue(pObjectIdentifierSubidentifiers);
				setLength(tmp);
			}
			else
			{
				setValue(pValue);
			}
			encodedValue = concatAllValues(pType == INTEGER_TAG_NUMBER);
			return encodedValue;
		}
		else if (pKeyword == DATATYPE_KEYWORD_IMPLICIT)
		{
			if (pVisibility.size() == 0)//brak widocznosci klasy
			{
				setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_PRIMITIVE, pTypeID);
			}
			else
			{
				setIdentifier(pVisibility, IDENTIFIER_COMPLEXITY_PRIMITIVE, pTypeID);
			}

			if (!(pType == OBJECT_IDENTIFIER_TAG_NUMBER))
			{
				setLength(pByteCount);
			}

			if (pType == INTEGER_TAG_NUMBER)
			{
				setValue(valueINT, pByteCount);
			}
			else if (pType == OBJECT_IDENTIFIER_TAG_NUMBER)
			{
				unsigned long long tmp = setValue(pObjectIdentifierSubidentifiers);
				setLength(tmp);
			}
			else
			{
				setValue(pValue);
			}
			encodedValue = concatAllValues(pType == INTEGER_TAG_NUMBER);
			return encodedValue;
		}
		else if (pKeyword == DATATYPE_KEYWORD_EXPLICIT)
		{
			if (pVisibility.size() == 0)//brak widocznosci klasy
			{
				setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pTypeID);
			}
			else
			{
				setIdentifier(pVisibility, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pTypeID);
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
			string explicitValue = temp.concatAllValues(pType == INTEGER_TAG_NUMBER);
			string explicitString = explicitValue;

			explicitString.erase(remove_if(explicitString.begin(), explicitString.end(), isspace), explicitString.end());
			unsigned int explicitLength = explicitString.size() / 2;

			setLength(explicitLength);

			encodedValue = concatAllValues(false);
			return encodedValue + " " + explicitValue;
		}
	}
	else
	{
		setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_CONSTRUCTED, SEQUENCE_TAG_NUMBER);
		string allSequenceValuesEncoded = "";

		for (size_t i = 0; i < pSequenceValues.size(); i++)
		{	
			allSequenceValuesEncoded += encode(pSequenceValues.at(i), pSequenceTypes.at(i), pSequenceTypeID.at(i), pSequenceBytesCount.at(i), pSequenceKeywords.at(i), pSequenceVisibilities.at(i), pSequenceValues, pSequenceTypes, pSequenceTypeID, pSequenceBytesCount, pSequenceKeywords, pSequenceVisibilities);
		}
		setValue(allSequenceValuesEncoded);
		allSequenceValuesEncoded.erase(remove_if(allSequenceValuesEncoded.begin(), allSequenceValuesEncoded.end(), isspace), allSequenceValuesEncoded.end());
		setLength(allSequenceValuesEncoded.size() / 2);
	}

	//sequence
	/*
	for (size_t i = 0; i < pVSequence.size(); i++)
	{
		if (pNode->syntax.find(pVSequence.at(i).name) != string::npos)
		{
			stringstream test(pValue);
			string segment;
			vector<string> seglist;

			while (std::getline(test, segment, ','))
			{
				seglist.push_back(segment);
			}
			for (size_t j = 0; j < pVSequence.at(i).type.size(); j++)
			{
				for (size_t k = 0; k < pVDataType.size(); k++)
				{
					if (pVSequence.at(i).type.at(j).find(pVDataType.at(k).name) != string::npos)
					{
						if (pVDataType.at(k).keyword.size() == 0) //kodowanie uniwersalne
						{
							setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(k).type);
							setLength(byteCount);
							if (isValueNumber)
							{
								setValue(pValueINT, byteCount);
							}
							else
							{
								setValue(pValue);
							}
							encodedValue = concatAllValues(isValueNumber);
							encodedValue.str()
						}
						else if (pVDataType.at(k).keyword == DATATYPE_KEYWORD_IMPLICIT)
						{
							if (pVDataType.at(k).visibility.size() == 0)//brak widocznosci klasy
							{
								setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(k).typeID);
							}
							else
							{
								setIdentifier(pVDataType.at(k).visibility, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(k).typeID);
							}

							setLength(byteCount);
							if (isValueNumber)
							{
								setValue(pValueINT, byteCount);
							}
							else
							{
								setValue(pValue);
							}
							encodedValue = concatAllValues(isValueNumber);
							encodedValue.str()
						}
						else if (pVDataType.at(k).keyword == DATATYPE_KEYWORD_EXPLICIT)
						{
							if (pVDataType.at(k).visibility.size() == 0)//brak widocznosci klasy
							{
								setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pVDataType.at(k).typeID);
							}
							else
							{
								setIdentifier(pVDataType.at(k).visibility, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pVDataType.at(k).typeID);
							}

							BERCoder temp;

							temp.setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(k).type);
							temp.setLength(byteCount);
							if (isValueNumber)
							{
								temp.setValue(pValueINT, byteCount);
							}
							else
							{
								temp.setValue(pValue);
							}
							stringstream explicitValue = temp.concatAllValues(isValueNumber);
							isValueNumber = false;
							string explicitString = explicitValue.str();

							explicitString.erase(remove_if(explicitString.begin(), explicitString.end(), isspace), explicitString.end());
							unsigned int explicitLength = explicitString.size() / 2;

							setLength(explicitLength);

							encodedValue = concatAllValues(isValueNumber);
							encodedValue.str() + " " + explicitValue.str()
						}
					}
				}
				BERCoder tmp;
				tmp.setIdentifier
			}

		}
	}
}
		else
		{
		//todo blad rozmiaru
		}
	}
	else
	{
	//todo blad typu
	}
	*/
	return "test";
}