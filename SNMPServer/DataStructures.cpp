#include "pch.h"
#include "DataStructures.h"


DataStructures::DataStructures()
{
}


DataStructures::~DataStructures()
{
}

void DataType::showDataTypes(vector<DataType> pVDataType)
{
	cout << "Nowe typy danych:" << endl;
	for (unsigned int i = 0; i < pVDataType.size(); i++)
	{
		if (pVDataType.at(i).name.length() > 0)
			cout << pVDataType.at(i).name << endl;

		if (pVDataType.at(i).visibility.length() > 0)
			cout << pVDataType.at(i).visibility << endl;

		if (pVDataType.at(i).typeID.length() > 0)
			cout << pVDataType.at(i).typeID << endl;

		if (pVDataType.at(i).keyword.length() > 0)
			cout << pVDataType.at(i).keyword << endl;

		if (pVDataType.at(i).type.length() > 0)
			cout << pVDataType.at(i).type << endl;

		if (pVDataType.at(i).size != -1)
			cout << pVDataType.at(i).size << endl;

		if (pVDataType.at(i).sizeMin != -1)
			cout << pVDataType.at(i).sizeMin << endl;

		if (pVDataType.at(i).sizeMax != -1)
			cout << pVDataType.at(i).sizeMax << endl;
		cout << endl;
	}
}

void Sequence::showSequences(vector<Sequence> pVSequence)
{
	cout << "Nowe sekwencje:" << endl;
	for (unsigned int i = 0; i < pVSequence.size(); i++)
	{
		cout << pVSequence.at(i).name << endl;
		for (unsigned int j = 0; j < pVSequence.at(i).type.size(); j++)
		{
			cout << pVSequence.at(i).typeName.at(j) << "\t\t" << pVSequence.at(i).type.at(j) << endl;
		}
		cout << endl << endl;
	}

}
