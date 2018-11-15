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
	for (int i = 0; i < pVDataType.size(); i++)
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
