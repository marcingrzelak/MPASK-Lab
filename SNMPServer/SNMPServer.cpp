#include "pch.h"
#include"FileHandler.h"

int main()
{
	FileHandler file("MIBS/RFC-1213-MIB.txt");

	file.FileRead();

	system("PAUSE");
	return(0);
}