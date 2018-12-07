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

void BERCoder::checkIsNumber(string pValue)
{
	try
	{
		isValueNumber = true;
		pValueINT = stoll(pValue);
	}
	catch (const std::exception&)
	{
		isValueNumber = false;
	}
}

void BERCoder::lengthCalc(string pValue)
{
	if (isValueNumber)
	{
		bitCount = floor(log2(abs(pValueINT)) + 1) + 1;
		byteCount = ceil(bitCount / 8.0);
	}
	else
	{
		byteCount = pValue.size();
	}
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

int BERCoder::checkValueType(string pValue, TreeNode* pNode, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize)
{
	type = defaultTypeCheck(pNode->syntax, isValueNumber);
	if (type < 0)//blad typu danych
	{
		return -1;
	}
	else if (type > 0)//podstawowy typ danych
	{
		return 0;
	}
	else if (type == 0)//niepodstawowy typ danych
	{
		//sprawdzamy czy obiekt nie jest typu data type
		typeDataType = dataTypeCheck(pNode->syntax, pVDataType);
		if (typeDataType < 0)//blad typu danych
		{
			return -1;
		}
		else if (typeDataType > 0)//podstawowy typ danych
		{
			return 0;
		}
		else if (typeDataType == 0)//obiekt nie jest typu data type
		{
			return -3;//obiekt nie ma zadnego typu
		}
	}
	return -2;
}

short BERCoder::defaultTypeCheck(string pValue, bool &isValueNumber)
{
	//sprawdzamy czy podany obiekt ma jeden z domyslnych typow
	smatch result;
	regex_search(pValue, result, Regex::dataTypeEncoder());

	if (result[0].matched)
	{
		if (result[1].matched)//integer
		{
			if (!isValueNumber)
			{
				return -1;
			}
			else
			{
				return INTEGER_TAG_NUMBER;
			}
		}
		else if (result[2].matched)//octet string
		{
			isValueNumber = false;
			return OCTET_STRING_TAG_NUMBER;
		}
		else if (result[3].matched)//object identifier
		{
			//todo sprawdzenie poprawnosci
			isValueNumber = false;
			return OBJECT_IDENTIFIER_TAG_NUMBER;
		}
		else if (result[4].matched)//null
		{
			if (byteCount > 0)
			{
				return -2;
			}
			else
			{
				return NULL_TAG_NUMBER;
			}
		}
	}
	return 0;
}

short BERCoder::dataTypeCheck(string pValue, vector<DataType> &pVDataType)
{
	indexDataType = -1;
	//sprawdzamy czy podany obiekt ma typ zawarty w data type
	for (size_t i = 0; i < pVDataType.size(); i++)
	{
		if (pValue.find(pVDataType.at(i).name) != string::npos)
		{
			indexDataType = i;
			BERCoder tmp;
			short returnedType;
			returnedType = tmp.defaultTypeCheck(pVDataType.at(i).type, isValueNumber);

			if (returnedType <= 0)//blad typu danych
			{
				return -1;
			}
			else//podstawowy typ danych
			{
				return returnedType;
			}
		}
	}
	return 0;//typ nie jest zawarty w data type
}

int BERCoder::checkValueSize(string pValue, TreeNode *pNode, vector<ObjectTypeSize> &pVObjectTypeSize, vector<DataType> &pVDataType)
{
	short oTSCreturned = objectTypeSizeCheck(pVObjectTypeSize, pNode->name);
	if (oTSCreturned == 0)//rozmiar ok
	{
		return 0;
	}
	else if (oTSCreturned > 0)//brak ograniczen w objectTypeSize
	{
		if (typeDataType == 0)//podstawowy typ, todo dodac potem spradzanie typeSequence, typeIndex itp
		{
			//todo sprawdzamy domyslne ograniczenia rozmiaru dla typu podstawowego (zmienna type)
			return 0;
		}
		else if (typeDataType > 0)//typ w data type
		{
			//sprawdzamy ograniczenia zawarte w data type
			short dTSCretuened = dataTypeSizeCheck(pVDataType);
			if (dTSCretuened == 0)//rozmiar ok
			{
				return 0;
			}
			else if (dTSCretuened > 0)//brak ograniczen w DataType
			{
				//todo sprawdzamy domyslne ograniczenia rozmiaru dla typu podstawowego (zmienna typeDataType)
				return 0;
			}
			else if (dTSCretuened < 0)
			{
				return -1;
			}
		}
		//todo kolejne else if do sprawdzania sekwencji indexow itp
	}
	else if (oTSCreturned < 0)//blad
	{
		return -1;
	}
}

short BERCoder::objectTypeSizeCheck(vector<ObjectTypeSize> pVObjectTypeSize, string pName)
{
	//dla danej nazwy obiektu np. sysName
	//sprawdzamy ograniczenia rozmiarow podane w deklaracji obiektu
	//te ograniczenia maja wyzszy priorytet niz ograniczenia podane w deklaracji typu
	//0 rozmiar ok
	//<0 rozmiar poza zakresem lub brak definicji dopuszczalnego rozmiaru
	//>0 nie znaleziono ograniczen
	for (size_t i = 0; i < pVObjectTypeSize.size(); i++)
	{
		if (pVObjectTypeSize.at(i).name == pName)
		{
			checkSize(pVObjectTypeSize.at(i).size, pVObjectTypeSize.at(i).sizeMin, pVObjectTypeSize.at(i).sizeMax);
		}
	}
	return 1;
}

short BERCoder::dataTypeSizeCheck(vector<DataType> &pVDataType)
{
	//nie trzeba sprawdzac indexDataType bo zawsze bedzie >= 0 w tym miejscu
	short returnedValue = checkSize(pVDataType.at(indexDataType).size, pVDataType.at(indexDataType).sizeMax, pVDataType.at(indexDataType).sizeMax);
	return returnedValue;
}

short BERCoder::checkSize(int pSize, long long pSizeMin, long long pSizeMax)
{
	if (pSize != -1)
	{
		if (byteCount > pSize)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else if (pSizeMin != -1 && pSizeMax != -1)
	{
		//todo zmienic regexa brak rozroznienia SIZE(x..y) od (x..y)
		//todo co chodzi ze sprawdzeniem isValueNumber
		if (isValueNumber)
		{
			if (byteCount < pSizeMin || byteCount > pSizeMax)
			{
				return -2;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			//todo jezeli inna wartosc niz integer moze miec ograniczenia rozmiaru x..y to zmienic
			return 0;
		}

	}
	else
	{
		//brak ograniczen
		return -3;
	}
	return 1;
}

string BERCoder::encode(TreeNode* pNode, string pValue, vector<DataType> &pVDataType, vector <Index> &pVIndex, vector<Choice> &pVChoice, vector<Sequence> &pVSequence, vector<ObjectTypeSize> &pVObjectTypeSize)
{
	clearIdentifier();
	clearLength();
	clearValue();

	string encodedValue;
	int checkTypeStatus, checkSizeStatus;

	checkIsNumber(pValue);
	lengthCalc(pValue);

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
			}*/
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
}