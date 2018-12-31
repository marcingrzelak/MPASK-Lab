#include "pch.h"
#include "BERDecoder.h"
#include "TreeStructure.h"


BERDecoder::BERDecoder()
{
}


BERDecoder::~BERDecoder()
{
}

void BERDecoder::getVectorOfBytes(string pValue)
{
	stringstream test(pValue);
	string segment;
	uint8_t oneByte;

	while (std::getline(test, segment, ' '))
	{
		oneByte = stoul(segment, nullptr, 16);
		octets.push_back(oneByte);
	}
}

void BERDecoder::getIdentifier(int &index)
{
	classValue = identifier.getClass(octets.at(index));
	complexityValue = identifier.getComplexity(octets.at(index));
	tagValue = identifier.getTag(octets, index);
}

void BERDecoder::getLength(int &index)
{
	lengthValue = length.getLength(octets, index, undefinedFlag);
}

void BERDecoder::getValue(int & index)
{
	values = value.getValue(octets, index, lengthValue);

	if (tagValue == INTEGER_TAG_NUMBER)
	{
		stringstream data;
		for (size_t i = 0; i < values.size(); i++)
		{
			data << hex << values.at(i);
		}
		int dataINT = stoi(data.str(), nullptr, 16);

		stringstream ss;
		ss << dataINT;
		dataValue = ss.str();
	}
	else if (tagValue == OCTET_STRING_TAG_NUMBER)
	{
		for (size_t i = 0; i < values.size(); i++)
		{
			dataValue += (char)values.at(i);
		}
	}
	else if (tagValue == NULL_TAG_NUMBER)
	{
		dataValue = "";
	}
	else if (tagValue == OBJECT_IDENTIFIER_TAG_NUMBER)
	{
		int objectIdentifierLongOctet = 0, objectIdentifierOctet = 0, i = 0;
		bool longOctet = false;
		vector<string> objectIdentifierSubidentifiers;
		stringstream ss;
		string str;

		while (i < values.size())
		{
			if (values.at(i) >= 0x80) //pierwszy bit = '1'
			{
				longOctet = true;
				objectIdentifierLongOctet <<= 7;
				objectIdentifierLongOctet |= (values.at(i) & 0x7f);
			}
			else
			{
				if (longOctet == true)
				{
					longOctet = false;
					objectIdentifierLongOctet <<= 7;
					objectIdentifierLongOctet |= (values.at(i) & 0x7f);

					ss << objectIdentifierLongOctet;
					str = ss.str();
				}
				else
				{
					objectIdentifierOctet |= (values.at(i) & 0x7f);

					ss << objectIdentifierOctet;
					str = ss.str();
				}

				objectIdentifierSubidentifiers.push_back(str);
			}
		}

		for (size_t i = 0; i < objectIdentifierSubidentifiers.size; i++)
		{
			if (i = 0)
			{
				string firstSubidentifier, secondSubidentifier;
				int first = floor(stoi(objectIdentifierSubidentifiers.at(i)) / 40);
				int second = stoi(objectIdentifierSubidentifiers.at(i)) % 40;

				ss << first;
				firstSubidentifier = ss.str();

				ss << second;
				secondSubidentifier = ss.str();

				dataValue += firstSubidentifier + "." + secondSubidentifier + ".";
			}
			else
			{
				dataValue += objectIdentifierSubidentifiers.at(i);
				if (i < objectIdentifierSubidentifiers.size() - 1)
				{
					dataValue += ".";
				}
			}			
		}
	}
}

void BERDecoder::decode(string pValue, int &index, TreeBER &pTree)
{
	getVectorOfBytes(pValue);
	getIdentifier(index);

	if (tagValue == SEQUENCE_TAG_NUMBER && complexityValue == IDENTIFIER_COMPLEXITY_CONSTRUCTED)
	{

	}
	else
	{
		if (tagValue == INTEGER_TAG_NUMBER)
		{
			if (pTree.root == nullptr)
			{
				pTree.addRoot(classValue, complexityValue, dataValue, tagValue, lengthValue);
			}
		}
		else if (tagValue == OCTET_STRING_TAG_NUMBER)
		{

		}
		else if (tagValue == NULL_TAG_NUMBER)
		{

		}
		else if (tagValue == OBJECT_IDENTIFIER_TAG_NUMBER)
		{

		}
	}

	getLength(index);
	getValue(index);


}
