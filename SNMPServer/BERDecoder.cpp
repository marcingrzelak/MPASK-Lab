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

void BERDecoder::getIdentifier(int &pIndex)
{
	classValue = identifier.getClass(octets.at(pIndex));
	complexityValue = identifier.getComplexity(octets.at(pIndex));
	tagValue = identifier.getTag(octets, pIndex);
}

void BERDecoder::getLength(int &pIndex)
{
	lengthValue = length.getLength(octets, pIndex, undefinedFlag);
}

void BERDecoder::getValue(int &pIndex)
{
	values = value.getValue(octets, pIndex, lengthValue);

	if (complexityValue == IDENTIFIER_COMPLEXITY_PRIMITIVE)
	{
		if (tagValue == INTEGER_TAG_NUMBER)
		{
			stringstream data;
			for (size_t i = 0; i < values.size(); i++)
			{
				data << setfill('0') << setw(2) << hex << static_cast<int>(values.at(i));
			}
			long dataINT = stol(data.str(), nullptr, 16);

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
			int i = 0, objectIdentifierLongOctet = 0;
			bool longOctet = false;
			vector<string> objectIdentifierSubidentifiers;
			string str;

			while (i < values.size())
			{
				int objectIdentifierOctet = 0;
				stringstream ss;

				if (values.at(i) >= 0x80 && i > 0) //pierwszy bit = '1'
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
				i++;
			}

			for (size_t i = 0; i < objectIdentifierSubidentifiers.size(); i++)
			{
				if (i == 0)
				{
					stringstream ss;
					string firstSubidentifier, secondSubidentifier;
					int first = floor(stoi(objectIdentifierSubidentifiers.at(i)) / 40);
					int second = stoi(objectIdentifierSubidentifiers.at(i)) % 40;

					ss << first;
					firstSubidentifier = ss.str();
					ss.str("");
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
	else
	{
		stringstream data;

		for (size_t i = 0; i < values.size(); i++)
		{
			data << setfill('0') << setw(2) << hex << static_cast<int>(values.at(i));
			if (i < values.size() - 1)
			{
				data << " ";
			}
		}

		dataValue = data.str();
	}
}

void BERDecoder::decode(string &pValue, int pIndex, TreeBER &pTree, TreeNodeBER *pParentNode)
{
	getVectorOfBytes(pValue);
	getIdentifier(pIndex);
	getLength(pIndex);
	getValue(pIndex);

	if ((tagValue == SEQUENCE_TAG_NUMBER && complexityValue == IDENTIFIER_COMPLEXITY_CONSTRUCTED) || (complexityValue == IDENTIFIER_COMPLEXITY_CONSTRUCTED && classValue == IDENTIFIER_CLASS_CONTEXT_SPECIFIC && (tagValue == GET_REQUEST_TAG_NUMBER || tagValue == GET_RESPONSE_TAG_NUMBER || tagValue == SET_REQUEST_TAG_NUMBER || tagValue == GET_NEXT_REQUEST_TAG_NUMBER)))
	{
		TreeNodeBER* node;
		int tmpIndex = 0;
		if (pTree.root == nullptr)
		{
			pTree.addRoot(classValue, classConstructedValue, complexityValue, dataValue, tagValue, tagConstructedValue, lengthValue);
			currentParent = pTree.root;
		}
		else
		{
			node = pTree.addNode(pParentNode, classValue, classConstructedValue, complexityValue, dataValue, tagValue, tagConstructedValue, lengthValue);
			currentParent = node;
		}
		BERDecoder recursion;
		recursion.decode(dataValue, tmpIndex, pTree, currentParent);
		currentParent = pParentNode;
	}
	else if (complexityValue == IDENTIFIER_COMPLEXITY_CONSTRUCTED)
	{
		TreeNodeBER* node;
		int tmpIndex = 0;
		if (pTree.root == nullptr)
		{
			pTree.addRoot(classValue, classConstructedValue, complexityValue, dataValue, tagValue, tagConstructedValue, lengthValue);
			currentParent = pTree.root;
		}
		else
		{
			node = pTree.addNode(pParentNode, classValue, classConstructedValue, complexityValue, dataValue, tagValue, tagConstructedValue, lengthValue);
			currentParent = node;
		}
		BERDecoder recursion;
		recursion.decode(dataValue, tmpIndex, pTree, currentParent);
	}
	else
	{
		if (pTree.root == nullptr)
		{
			pTree.addRoot(classValue, classConstructedValue, complexityValue, dataValue, tagValue, tagConstructedValue, lengthValue);
			currentParent = pTree.root;
		}
		else
		{
			pTree.addNode(pParentNode, classValue, classConstructedValue, complexityValue, dataValue, tagValue, tagConstructedValue, lengthValue);
			currentParent = pParentNode;
		}
	}

	if (pIndex < octets.size())
	{
		BERDecoder recursion;
		recursion.decode(pValue, pIndex, pTree, currentParent);
	}
}