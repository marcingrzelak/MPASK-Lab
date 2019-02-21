#include "pch.h"
#include "Length.h"
#include "Strings.h"
#include "Exceptions.h"

Length::Length()
{
	firstOctet = 0;
}


Length::~Length()
{
}

void Length::setDefinedForm(unsigned long long pLength)
{
	if (pLength < 128)
	{
		firstOctet |= pLength;
	}
	else if (pLength > 128)
	{
		bits = floor(log2(pLength) + 1);
		K = ceil(bits / 8.0);
		uint8_t octet = 0;
		if (K == 127)
		{
			throw eLengthKEqual127();
		}
		else if (K > 127)
		{
			throw eLengthKGraterThen127();
		}
		else
		{
			firstOctet |= 0x80;
			firstOctet |= K;

			for (int i = K; i > 0; i--)
			{
				octet = (pLength >> (8 * (i - 1))) & 0xff;
				longOctet.push_back(octet);
			}
		}
	}
	else
	{
		throw eLengthEqual128();
	}
}

void Length::setUndefinedForm(unsigned long long pLength)
{
	firstOctet |= 0x80;
}

unsigned long long Length::getLength(vector<uint8_t> pOctets, int &index, bool &undefinedFlag)
{
	if (pOctets.at(index) >= 0x80) //pierwszy bit = '1' forma dluga lub nieokreslona
	{
		K = pOctets.at(index) & 0x1f;
		if (K > 0) //forma dluga
		{
			index++;
			undefinedFlag = true;
			unsigned long long lengthLong = 0;
			for (size_t i = index; i < index + K; i++)
			{
				lengthLong <<= 8;
				lengthLong |= (pOctets.at(i));
			}
			index += K;
			return lengthLong;
		}
		else
		{
			index++;
			undefinedFlag = false;
			return 0;
		}

	}
	else
	{
		uint8_t lengthShort = pOctets.at(index) & 0x7f;
		undefinedFlag = false;
		index++;
		return lengthShort;
	}
}
