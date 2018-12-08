#include "pch.h"
#include "BERCoder.h"
#include "TreeStructure.h"
#include "DataStructures.h"
#include "Identifier.h"
#include "Length.h"
#include "Value.h"
#include "Regex.h"


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

string BERCoder::encode(string pValue)
{
	/*
	clearIdentifier();
	clearLength();
	clearValue();

	string encodedValue;
	int checkTypeStatus, checkSizeStatus;

	checkTypeStatus = checkValueType(pValue, pNode, pVDataType, pVIndex, pVChoice, pVSequence, pVObjectTypeSize);
	if (checkTypeStatus == 0)
	{
		checkSizeStatus = checkValueSize(pValue, pNode, pVObjectTypeSize, pVDataType);
		if (checkSizeStatus == 0)
		{
			//kodowanie BER
			//null
			if (type == 4)
			{
				setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, IDENTIFIER_TYPE_NULL);
				setLength(0);
				encodedValue = concatAllValues(isValueNumber);
				return encodedValue;
			}
			//data type
			for (unsigned int i = 0; i < pVDataType.size(); i++)
			{
				if (pNode->syntax.find(pVDataType.at(i).name) != string::npos)
				{
					if (pVDataType.at(i).keyword.size() == 0) //kodowanie uniwersalne
					{
						setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).type);
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
						return encodedValue;
					}
					else if (pVDataType.at(i).keyword == DATATYPE_KEYWORD_IMPLICIT)
					{
						if (pVDataType.at(i).visibility.size() == 0)//brak widocznosci klasy
						{
							setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).typeID);
						}
						else
						{
							setIdentifier(pVDataType.at(i).visibility, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).typeID);
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
						return encodedValue;
					}
					else if (pVDataType.at(i).keyword == DATATYPE_KEYWORD_EXPLICIT)
					{
						if (pVDataType.at(i).visibility.size() == 0)//brak widocznosci klasy
						{
							setIdentifier(IDENTIFIER_CLASS_CONTEXT_SPECIFIC, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pVDataType.at(i).typeID);
						}
						else
						{
							setIdentifier(pVDataType.at(i).visibility, IDENTIFIER_COMPLEXITY_CONSTRUCTED, pVDataType.at(i).typeID);
						}

						BERCoder temp;

						temp.setIdentifier(IDENTIFIER_CLASS_UNIVERSAL, IDENTIFIER_COMPLEXITY_PRIMITIVE, pVDataType.at(i).type);
						temp.setLength(byteCount);
						if (isValueNumber)
						{
							temp.setValue(pValueINT, byteCount);
						}
						else
						{
							temp.setValue(pValue);
						}
						string explicitValue = temp.concatAllValues(isValueNumber);
						isValueNumber = false;
						string explicitString = explicitValue;

						explicitString.erase(remove_if(explicitString.begin(), explicitString.end(), isspace), explicitString.end());
						unsigned int explicitLength = explicitString.size() / 2;

						setLength(explicitLength);

						encodedValue = concatAllValues(isValueNumber);
						return encodedValue + " " + explicitValue;
					}
				}
			}
			//sequence
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