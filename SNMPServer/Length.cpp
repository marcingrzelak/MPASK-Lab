#include "pch.h"
#include "Length.h"
#include "Strings.h"

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
		bits = ceil(log2(pLength));
		K = ceil(bits / 8.0);
		uint8_t octet = 0;
		if (K == 127)
		{
			//error
		}
		else if (K > 127)
		{
			//error
		}
		else
		{
			firstOctet |= 0x80;
			firstOctet |= K;

			for (int i = K; i > 0; i--)
			{
				octet = (pLength >> (8 * (i-1))) & 0xff;
				longOctet.push_back(octet);
			}
		}			
	}
	else
	{
		//error
	}
}

void Length::setUndefinedForm(unsigned long long pLength)
{
	firstOctet |= 0x80;
}
